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
// $Id: DataObjectVersionFilter.cpp,v 1.3 2009-11-10 09:27:51 jonrob Exp $
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
// Standard constructor, initializes variables
//=============================================================================
DataObjectVersionFilter::DataObjectVersionFilter( const std::string& name,
                                                  ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "MinVersion", m_minV = 0       );
  declareProperty( "MaxVersion", m_maxV = 9999999 );
  declareProperty( "DataObjectLocation", m_loc = "" );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode DataObjectVersionFilter::execute() 
{
  bool OK = true;

  try
  {
    const DataObject * data = getIfExists<DataObject>(m_loc);
    if ( data )
    {
      const auto ver = (unsigned int)data->version();
      if ( msgLevel(MSG::DEBUG) )
        debug() << "version = " << ver << endmsg;
      OK = ( ver <= m_maxV && ver >= m_minV );
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
