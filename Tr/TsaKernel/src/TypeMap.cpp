
#include "TsaKernel/TypeMap.h"

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
using namespace boost::assign; // bring 'map_list_of()' into scope

Tsa::TypeMap::TypeMap(){
  // constructer

  m_mapping = map_list_of("B",Tsa::SpacePoint::B)("T",Tsa::SpacePoint::T)
                         ("L",Tsa::SpacePoint::L)("R",Tsa::SpacePoint::R)
                         ("OB",Tsa::SpacePoint::OB)("OT",Tsa::SpacePoint::OT);
}
