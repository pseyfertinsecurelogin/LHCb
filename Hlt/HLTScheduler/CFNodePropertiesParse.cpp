#include "CFNodePropertiesParse.h"
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
