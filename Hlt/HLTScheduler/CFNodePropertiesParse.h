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
  ostream& operator<<( ostream& s, nodeType const & m )
  {
     Gaudi::Utils::toStream( nodeTypeNames.at(m), s );
     return s;
  }

  ostream& operator<<( ostream& s, NodeDefinition const & m )
  {
     s << "[";
     Gaudi::Utils::toStream( m.name, s ) << ", ";
     Gaudi::Utils::toStream( m.type, s ) << ", ";
     if (m.children.empty()) {
       s << "[]";
     }
     else {
       s << "[" << m.children.front();
       std::for_each(next(begin(m.children)), end(m.children), [&] (auto i) {s << ", " << i;});
       s << "]";
     }
     s << ", ";
     s << m.ordered;
     s << "]";
     return s;
  }
}
