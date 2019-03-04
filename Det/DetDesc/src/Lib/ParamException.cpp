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

// Include files
#include "GaudiKernel/StatusCode.h"

#include "DetDesc/ParamException.h"

//----------------------------------------------------------------------------

/// Constructor
ParamException::ParamException( const std::string& name, ExceptionType t )
    : GaudiException( "", "*ParamException*", StatusCode::FAILURE ), m_type( t ) {
  std::ostringstream msg;

  switch ( m_type ) {
  case BAD_KEY:
    msg << "Param `" << name << "' not found";
    break;
  case BAD_TYPE:
    msg << "Param `" << name << "' has a different type from the one requested";
    break;
  }

  m_message = msg.str();
}

//----------------------------------------------------------------------------
/// Constructor
ParamException::ParamException( const std::string& name, const std::type_info& req, const std::type_info& actual )
    : GaudiException( "", "*ParamException*", StatusCode::FAILURE ), m_type( BAD_TYPE ) {
  std::ostringstream msg;
  msg << "Param `" << name << "' has a different type from the one requested (requested = " << req.name()
      << ", actual = " << actual.name() << ")";
  m_message = msg.str();
}

//----------------------------------------------------------------------------
