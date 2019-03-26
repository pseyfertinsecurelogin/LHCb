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
#include "Event/VertexBase.h"

//-----------------------------------------------------------------------------
// Implementation file for class : VertexBase
//
// 2012-04-26 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::VertexBase::fillStream( std::ostream& s ) const {
  s << "{ "
    << "position  : " << m_position << std::endl
    << "covMatrix : " << std::endl
    << m_covMatrix << std::endl
    << "chi2      : " << m_chi2 << std::endl
    << "nDoF      : " << m_nDoF << std::endl
    << "extraInfo : [";
  for ( const auto& i : extraInfo() ) { s << " " << i.first << "=" << i.second; }
  return s << " ] }";
}

LHCb::VertexBase* LHCb::VertexBase::clone() const { return new VertexBase( *this ); }

bool LHCb::VertexBase::isPrimary() const { return false; }
