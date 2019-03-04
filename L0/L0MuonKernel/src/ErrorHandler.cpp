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

// local
#include "L0MuonKernel/ErrorHandler.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ErrorHandler
//
// 2008-03-27 : Julien Cogan
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0Muon::ErrorHandler::ErrorHandler() {
  m_value   = 0;
  m_inError = false;
  m_counter = 0;
  m_fmt     = " %1d";
  m_name    = "error";
  m_mask    = 0xFFFF;
}
L0Muon::ErrorHandler::ErrorHandler( std::string fmt, int mask ) {
  m_value   = 0;
  m_inError = false;
  m_counter = 0;
  m_fmt     = fmt;
  m_mask    = mask;
  m_name    = "error";
}
L0Muon::ErrorHandler::ErrorHandler( std::string fmt, int mask, std::string name ) {
  m_value   = 0;
  m_inError = false;
  m_counter = 0;
  m_fmt     = fmt;
  m_mask    = mask;
  m_name    = name;
}
//=============================================================================
// Destructor
//=============================================================================
L0Muon::ErrorHandler::~ErrorHandler() {}

//=============================================================================

std::ostream& L0Muon::operator<<( std::ostream& os, const L0Muon::ErrorHandler& field ) {
  if ( field.inError() ) os << "\033[" << 31 << ";1m";
  os << boost::format( field.fmt() ) % field.value();
  if ( field.inError() ) os << "\033[0m";
  return os;
}
