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
//====================================================================
//	LHCbOutputStreamAgent.cpp
//--------------------------------------------------------------------
//
//	Package    :  (The LHCb PersistencySvc service)
//
//  Description: Implementation of the LHCbOutputStream Agent
//
//	Author     : M.Frank
//  Created    : 13/1/99
//	Changes    : 
//
//====================================================================
#define PERSISTENCYSVC_OUTPUTSTREAMAGENT_CPP

// Framework includes
#include "GaudiKernel/IDataManagerSvc.h"
#include "LHCbOutputStream.h"
#include "LHCbOutputStreamAgent.h"

/// Standard Constructor
LHCbOutputStreamAgent::LHCbOutputStreamAgent(LHCbOutputStream* OutputStream)
: m_OutputStream(OutputStream)  
{
}

/// Standard Destructor
LHCbOutputStreamAgent::~LHCbOutputStreamAgent()  {
}

/// Analysis callback
bool LHCbOutputStreamAgent::analyse(IRegistry* pRegistry, int level)   {
  return m_OutputStream->collect(pRegistry, level);
}
