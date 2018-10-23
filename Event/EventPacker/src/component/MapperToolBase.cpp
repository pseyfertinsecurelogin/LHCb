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
// Include files

// local
#include "MapperToolBase.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MapperToolBase
//
// 2012-03-26 : Chris Jones
//-----------------------------------------------------------------------------

//============================================================================

StatusCode MapperToolBase::finalize()
{
  m_jos.reset();
  return GaudiTool::finalize();
}

//=============================================================================

SmartIF<IJobOptionsSvc>& MapperToolBase::joSvc() const
{
  if ( UNLIKELY(!m_jos) ) { m_jos = service("JobOptionsSvc"); }
  return m_jos;
}

// ============================================================================

std::string MapperToolBase::streamName( const std::string & path ) const
{
  auto tmp = ( path.compare(0,7,"/Event/") ==  0 ? path.substr(7) : path );
  return tmp.substr(0,tmp.find_first_of( "/" ));
}

//=============================================================================
