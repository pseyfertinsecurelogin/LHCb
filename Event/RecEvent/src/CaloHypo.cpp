// local
#include "Event/CaloHypo.h"

//-----------------------------------------------------------------------------

std::ostream& LHCb::CaloHypo::fillStream(std::ostream& s) const
{
  s << "{\n"
    << "hypothesis : " << m_hypothesis << '\n'
    << "lh : " << m_lh << '\n';
  if ( m_position ) {
    s << "position : " << *m_position;
  } else {
    s << "No position" ;
  }
  return s << "\n }";
}

