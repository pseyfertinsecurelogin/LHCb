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
/** @file RichHistoID.cpp
 *
 *  Implementation file for RICH utility class : Rich::HistogramID
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   12/02/2010
 */
//-----------------------------------------------------------------------------

// local
#include "RichUtils/RichHistoID.h"

//-----------------------------------------------------------------------------

std::string Rich::HistogramID::fullid() const {
  std::string fid( "" );
  if ( rad() != Rich::InvalidRadiator ) {
    fid += Rich::text( rad() ) + "/";
  } else if ( det() != Rich::InvalidDetector ) {
    fid += Rich::text( det() ) + "/";
  }
  if ( side() != Rich::InvalidSide ) { fid += Rich::text( det(), side() ) + "/"; }
  if ( pid() != Rich::Unknown ) { fid += Rich::text( pid() ) + "/"; }
  return fid + id();
}

std::string Rich::HistogramID::fullTitle( const std::string& title ) const {
  std::string ftitle( "" );
  if ( rad() != Rich::InvalidRadiator ) {
    ftitle += Rich::text( rad() ) + " ";
  } else if ( det() != Rich::InvalidDetector ) {
    ftitle += Rich::text( det() ) + " ";
  }
  if ( side() != Rich::InvalidSide ) { ftitle += Rich::text( det(), side() ) + " "; }
  if ( pid() != Rich::Unknown ) { ftitle += Rich::text( pid() ) + " "; }
  return ftitle + title;
}
