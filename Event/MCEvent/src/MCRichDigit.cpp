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
