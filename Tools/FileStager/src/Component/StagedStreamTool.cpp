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
// $Id:
// Include files
#include <iostream>

// STL
#include <vector>

// boost
#include <boost/regex.hpp>

// from Gaudi
#include <GaudiKernel/SmartIF.h>
#include <GaudiKernel/ISvcLocator.h>
#include <GaudiKernel/DataStreamTool.h>
#include <GaudiKernel/EventSelectorDataStream.h>
#include <GaudiUtils/IFileCatalog.h>

// FileStager
#include <FileStager/IFileStagerSvc.h>

// local
#include "StagedStreamTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : StagedStreamTool
//
// 2006-09-21 : Andres Felipe Osorio Oliveros
//-----------------------------------------------------------------------------

using std::string;
using std::vector;

// Declaration of the Tool Factory
DECLARE_COMPONENT( StagedStreamTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
StagedStreamTool::StagedStreamTool( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
: DataStreamTool( type, name , parent )
{
   m_stagerSvc = 0;
   m_catalog = 0;

   m_regex.assign("(?:DATA(?:FILE)?)='((?:(?<type>PFN|LFN):)?(?<desc>(?:\\w+:)*/{0,2}[a-zA-Z0-9_/:?&\\.=\\-+]+))'");
   declareProperty( "CatalogSvc", m_catalogSvc = "Gaudi::MultiFileCatalog/FileCatalog" );
}

//=============================================================================
StatusCode StagedStreamTool::initialize()
{
   StatusCode status = DataStreamTool::initialize();
   if( !status.isSuccess() )  {
      fatal() << "Error. Cannot initialize base class." << endmsg;
      return status;
   }

   // Get a reference to the FileStagerSvc
   m_stagerSvc = serviceLocator()->service("FileStagerSvc");
   if( !m_stagerSvc.isValid() )  {
      fatal() << "Error retrieving FileStagerSvc." << endmsg;
      return StatusCode::FAILURE;
   }

   // Get a reference to the IFileCatalog
   m_catalog = serviceLocator()->service( m_catalogSvc );
   if( !m_catalog.isValid() )  {
      fatal() << "Error retrieving Catalog." << endmsg;
      return StatusCode::FAILURE;
   }

   return StatusCode::SUCCESS;
}

//=============================================================================
StatusCode StagedStreamTool::addStreams(const StreamSpecs & inputs) {

   StatusCode status = StatusCode::SUCCESS;

   for( const string& input: inputs ) {
      status = addStream( input );
      if ( !status.isSuccess() ) break;
   }

   vector< Descriptor > descriptors;
   for( const std::string& descriptor: inputs ) {
      try {
         Descriptor d = extractFilename( descriptor );
         descriptors.push_back( d );
      } catch ( const std::exception& ) {
         return StatusCode::FAILURE;
      }
   }

   vector< string > files;
   for( const Descriptor& descriptor: descriptors ) {
      if ( descriptor.type() == LFN ) {
         string file = descriptor.descriptor();
         string lfn = descriptor.descriptor().substr( 4 );
         const string& fid = m_catalog->lookupLFN( lfn );
         if ( !fid.empty() ) {
            // the lfn is in the catalog
            Gaudi::IFileCatalog::Files entries;
            m_catalog->getPFN( fid, entries );
            if ( !entries.empty() ) {
               // We'll stage the PFN
               file = entries[ 0 ].first;
            }
         }
         debug() << "Adding " << file << " to files to be staged "
                << " for lfn: " << lfn << endmsg;
         files.push_back( file );
      } else {
         // Try with whatever we have
         debug() << "Adding: " << descriptor.descriptor()
                << " to the list of files to be staged." << endmsg;
         files.push_back( descriptor.descriptor() );
      }
   }

   if ( status.isSuccess() ) {
      status = m_stagerSvc->addFiles( files );
   }

   return status;
}

//=============================================================================
StatusCode StagedStreamTool::clear()
{
   StatusCode sc = m_stagerSvc->clearFiles();
   if ( !sc.isSuccess() ) return sc;
   return DataStreamTool::clear();
}

//=============================================================================
StagedStreamTool::Descriptor
StagedStreamTool::extractFilename( const std::string& descriptor )
{

   boost::smatch match;
   boost::match_flag_type flags = boost::match_default;
   if ( boost::regex_search( descriptor.begin(), descriptor.end(), match, m_regex, flags ) ) {
      if ( match.size() >= 1 ) {
         if ( match["type"].matched && match["type"].str() == "LFN" ) {
            return Descriptor( LFN, match.str( 1 ) );
         } else {
            return Descriptor( PFN, match.str( 1 ) );
         }
      } else {
         error() << "could not extract filename from descriptor: "
                << descriptor << endmsg;
         throw std::exception();
      }
   } else {
      error() << "bad descriptor: " << descriptor << endmsg;
      throw std::exception();
   }
   // To get rid of warning, will never be reached.
   return Descriptor( NONE, "" );
}
