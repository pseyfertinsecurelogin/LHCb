/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Event
#include "Event/CaloPosition.h"

//-----------------------------------------------------------------------------

std::ostream& LHCb::CaloPosition::fillStream(std::ostream& s) const
{
  s << "{ " << std::endl
    << "z :	" << (float)m_z << std::endl
    << "parameters : " << m_parameters << std::endl
    << "covariance :" << std::endl << m_covariance << std::endl
    << "center : " << m_center << std::endl
    << "spread :" << std::endl << m_spread << std::endl << " }";
  return s;
}
