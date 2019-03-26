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
#include "ConfigZipFileAccessSvc.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "GaudiKernel/System.h"

#include "zip.h"

using namespace std;

using ConfigZipFileAccessSvc_details::ZipFile;

// Factory implementation
DECLARE_COMPONENT( ConfigZipFileAccessSvc )

IArchive* ConfigZipFileAccessSvc::file() const {
  if ( !m_file ) {
    if ( m_mode != "ReadOnly" && m_mode != "ReadWrite" && m_mode != "Truncate" ) {
      error() << "invalid mode: " << m_mode << endmsg;
      return nullptr;
    }
    ios::openmode mode =
        ( m_mode == "ReadWrite" ? ios::in | ios::out | ios::ate
                                : ( m_mode == "Truncate" ? ios::in | ios::out | ios::trunc : ios::in ) );
    if ( m_name.empty() ) {
      std::string def( System::getEnv( "HLTTCKROOT" ) );
      if ( !def.empty() ) {
        def += "/config.zip";
      } else {
        throw GaudiException( "Environment variable HLTTCKROOT not specified and no explicit "
                              "filename given; cannot obtain location of config.zip.",
                              name(), StatusCode::FAILURE );
      }
      m_name = def;
    }
    info() << " opening " << m_name.value() << " in mode " << m_mode.value() << endmsg;
    m_file = std::make_unique<ZipFile>( m_name.value(), mode );
    if ( !*m_file ) {
      error() << " Failed to open " << m_name.value() << " in mode " << m_mode.value() << ": " << strerror( errno )
              << endmsg;
      m_file.reset();
    }
  }
  return m_file.get();
}

//=============================================================================
// Finalization
//=============================================================================
StatusCode ConfigZipFileAccessSvc::finalize() {
  m_file.reset(); // close file if still open
  return ConfigArchiveAccessSvc::finalize();
}
