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
#include "ConfigTarFileAccessSvc.h"

#include "GaudiKernel/System.h"
#include "tar.h"


using ConfigTarFileAccessSvc_details::TarFile;

// Factory implementation
DECLARE_COMPONENT( ConfigTarFileAccessSvc )

//=============================================================================
// Finalization
//=============================================================================
StatusCode ConfigTarFileAccessSvc::finalize()
{
    m_file.reset( ); // close file if still open
    return ConfigArchiveAccessSvc::finalize();
}


IArchive* ConfigTarFileAccessSvc::file() const
{
    if ( !m_file ) {
        if ( m_mode != "ReadOnly" && m_mode != "ReadWrite" &&
             m_mode != "Truncate" ) {
            error() << "invalid mode: " << m_mode << endmsg;
            return nullptr;
        }
        std::ios::openmode mode = ( m_mode == "ReadWrite"
                                  ? std::ios::in | std::ios::out | std::ios::ate
                                  : ( m_mode == "Truncate"
                                    ? std::ios::in | std::ios::out | std::ios::trunc
                                    : std::ios::in ) );
        if ( m_name.empty() ) {
            std::string def( System::getEnv( "HLTTCKROOT" ) );
            if ( !def.empty() ) {
                def += "/config.tar";
            } else {
               throw GaudiException("Environment variable HLTTCKROOT not specified and no explicit "
                                    "filename given; cannot obtain location of config.tar.",
                                    name(), StatusCode::FAILURE);
            }
            m_name = def;
        }
        info() << " opening " << m_name.value() << " in mode " << m_mode.value() << endmsg;
        m_file = std::make_unique<TarFile>( m_name.value(), mode, m_compress );
        if ( !*m_file ) {
            error() << " Failed to open " << m_name.value() << " in mode " << m_mode.value()
                    << ": " << strerror( errno ) << endmsg;
            m_file.reset( );
        }
    }
    return m_file.get();
}

