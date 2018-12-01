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
#pragma once

#include <string>
#include <vector>
#include <tuple>
#include "GaudiKernel/ParsersFactory.h"
#include "CFNodeType.h"

BOOST_FUSION_ADAPT_STRUCT( NodeDefinition,
                           (std::string, name)
                           (std::string, type)
                           (std::vector<std::string>, children)
                           (bool, ordered)
                         )

namespace Gaudi { namespace Parsers {
  template <typename Iterator, typename Skipper>
  struct NodeDefGrammar : qi::grammar<Iterator, NodeDefinition(), Skipper> {
    typedef NodeDefinition ResultT;
    NodeDefGrammar() : NodeDefGrammar::base_type( NodeDef_literal )
    {
      NodeDef_literal = '('
          >> gstring >> ','
          >> gstring >> ','
          >> gvector >> ','
          >> gbool >> ')';
    }
    qi::rule<Iterator, NodeDefinition(), Skipper> NodeDef_literal;
    StringGrammar<Iterator, Skipper> gstring;
    BoolGrammar<Iterator, Skipper> gbool;
    VectorGrammar<Iterator, std::vector<std::string>, Skipper> gvector;
  };
  REGISTER_GRAMMAR( NodeDefinition, NodeDefGrammar );
}}

namespace std
{
  ostream& operator<<( ostream& s, nodeType const & m );
  ostream& operator<<( ostream& s, NodeDefinition const & m );
}
