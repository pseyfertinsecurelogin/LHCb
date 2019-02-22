/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "Kernel/ConfigTreeNode.h"
#include <regex>

#include "GaudiKernel/GaudiException.h"

using Gaudi::Math::MD5;

ConfigTreeNode::ConfigTreeNode( const LeafRef& leaf, const NodeRefs& nodes, std::string label )
    : m_nodes( nodes ), m_leaf( leaf ), m_label( std::move( label ) ) {
  if ( this->label().find( ':' ) != std::string::npos )
    throw GaudiException( "':' is not allowed in label", label, StatusCode::FAILURE );
}

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/stream_translator.hpp>
#include <boost/property_tree/xml_parser.hpp>
using boost::property_tree::ptree;
namespace {
  void read_custom( std::istream& is, ptree& top ) {
    bool                    parsing_nodes = false;
    static const std::regex leaf( "^Leaf: ([a-fA-F0-9]{32})$" ), label( "^Label: (.*)$" ), nodestart( "^Nodes: \\[$" ),
        node( "^ ?([a-fA-F0-9]{32})$" ), nodeend( "^\\]$" );
    std::string s;
    std::smatch what;
    auto&       nodes = top.put_child( ptree::path_type( "Nodes" ), ptree{} );
    while ( std::istream::traits_type::not_eof( is.peek() ) ) {
      getline( is, s );
      if ( s.empty() ) continue;
      if ( parsing_nodes ) {
        if ( std::regex_match( s, what, nodeend ) ) {
          parsing_nodes = false;
        } else if ( std::regex_match( s, what, node ) ) {
          nodes.push_back( std::pair{std::string{}, ptree{what[1].str()}} );
        } else {
          std::cerr << "ConfigTreeNode: read_custom: parsing error while looking for nodes!!! : [" << s << "]"
                    << std::endl;
          top = ptree{};
          return;
        }
      } else {
        if ( std::regex_match( s, what, leaf ) ) {
          top.put( "Leaf", what[1].str() );
        } else if ( std::regex_match( s, what, nodestart ) ) {
          parsing_nodes = true;
        } else if ( std::regex_match( s, what, label ) ) {
          top.put( "Label", what[1].str() );
        } else {
          std::cerr << "ConfigTreeNode: read_custom: parsing error!!! : [" << s << "]" << std::endl;
          top = ptree{};
          return;
        }
      }
    }
  }

  struct MD5Translator {
    typedef std::string                 internal_type;
    typedef ConfigTreeNode::digest_type external_type;
    boost::optional<external_type>      get_value( internal_type const& v ) {
      return external_type::createFromStringRep( v );
    }
    boost::optional<internal_type> put_value( external_type const& v ) { return v.str(); }
  };

} // namespace

namespace boost::property_tree {
  template <>
  struct translator_between<std::string, ConfigTreeNode::digest_type> {
    typedef MD5Translator type;
  };
} // namespace boost::property_tree

std::istream& ConfigTreeNode::read( std::istream& is ) {
  ptree top;
  switch ( is.peek() ) {
  case '{':
    read_json( is, top );
    break;
  case '<':
    read_xml( is, top );
    break;
  default:
    read_custom( is, top );
    break;
  }
  auto toNodes = []( auto nodes ) {
    NodeRefs nr;
    if ( nodes ) {
      nr.reserve( nodes->size() );
      std::transform( begin( *nodes ), end( *nodes ), std::back_inserter( nr ),
                      []( const ptree::value_type& i ) { return i.second.get_value<ConfigTreeNode::digest_type>(); } );
    }
    return nr;
  };
  *this = ConfigTreeNode{
      top.get<digest_type>( "Leaf", digest_type::createInvalid() ), // add default: digest_type::createInvalid() (top
                                                                    // level has no leaf)
      toNodes( top.get_child_optional( "Nodes" ) ),
      top.get<std::string>( "Label", std::string{} )}; // add default: empty string (most nodes have no label)
  return is;
}

std::string ConfigTreeNode::str() const {
  std::string out;
  out.reserve( 58 + label().size() + 34 * nodes().size() );
  out += "Label: ";
  out += label();
  out += '\n';
  out += "Leaf: ";
  out += leaf().str();
  out += '\n';
  out += "Nodes: [\n";
  std::for_each( begin( nodes() ), end( nodes() ), [&out]( const NodeRef& i ) {
    out += ' ';
    out += i.str();
    out += '\n';
  } );
  out += "]\n";
  return out;
}

std::ostream& ConfigTreeNode::print( std::ostream& os ) const {
  // note: advantage of json (or xml): in case of hash collision, can add an optional extra field...
  // ...but that only works if the json representation is used to compute the digest!!!
  ptree top;
  if ( !label().empty() ) top.put( "Label", label() );
  if ( leaf().valid() ) top.put( "Leaf", leaf().str() );
  if ( !nodes().empty() ) {
    std::transform( begin( nodes() ), end( nodes() ), std::back_inserter( top.put_child( "Nodes", ptree{} ) ),
                    []( const NodeRef& i ) {
                      return std::pair{std::string(), ptree{i.str()}};
                    } );
  }
  write_json( os, top, false );
  return os;
}
