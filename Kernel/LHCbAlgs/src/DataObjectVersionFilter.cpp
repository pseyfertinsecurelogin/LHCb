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
#include "DataObjectVersionFilter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DataObjectVersionFilter
//
// 2009-11-06 : Chris Jones
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DataObjectVersionFilter )

//=============================================================================
// Main execution
//=============================================================================
StatusCode DataObjectVersionFilter::execute()
{
  bool OK = true;

  try
  {
    const DataObject * data = m_loc.getIfExists();
    if ( data )
    {
      const auto ver = (unsigned int)data->version();
      if ( msgLevel(MSG::DEBUG) )
        debug() << "version = " << ver << endmsg;
      OK = ( ver <= m_maxV.value() && ver >= m_minV.value() );
    }
    else
    {
      OK = false;
    }
  }
  catch ( const GaudiException & )
  {
    OK = false;
  }

  setFilterPassed(OK);

  return StatusCode::SUCCESS;
}

//=============================================================================
