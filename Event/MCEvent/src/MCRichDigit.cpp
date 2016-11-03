
//-----------------------------------------------------------------------------
/** @file MCRichDigit.cpp
 *
 *  Implementation file for class : MCRichDigit
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    01/09/2006
 */
//-----------------------------------------------------------------------------

#include "Event/MCRichDigit.h"

std::ostream& LHCb::MCRichDigit::fillStream(std::ostream& s) const
{
  s << "{ " << "hits :";
  for ( const auto & H : m_hits )
  {
    s << " [ " << H.history() << " " << *H.mcRichHit() << " ]";
  }
  s << std::endl;
  s << "history : " << m_history << std::endl << " }";
  return s;
}
