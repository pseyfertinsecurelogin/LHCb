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
#include "GaudiKernel/Property.h"
#include "GaudiKernel/IJobOptionsSvc.h"

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/exception.hpp"

// local
#include "CondDBSQLiteCopyAccSvc.h"

// Factory implementation
DECLARE_COMPONENT( CondDBSQLiteCopyAccSvc )

//-----------------------------------------------------------------------------
// Implementation file for class : CondDBSQLiteCopyAccSvc
//
// 2007-03-22 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CondDBSQLiteCopyAccSvc::CondDBSQLiteCopyAccSvc( const std::string& name, ISvcLocator* svcloc ):
  CondDBAccessSvc(name,svcloc)
{
  declareProperty("OriginalFile",    m_source_path        = "" );
  declareProperty("DestinationFile", m_dest_path          = "" );
  declareProperty("DBName",          m_dbname             = "" );
  declareProperty("ForceCopy",       m_force_copy         = false );
  declareProperty("IgnoreCopyError", m_ignore_copy_error  = false );
}

//=============================================================================
// initialize
//=============================================================================
StatusCode CondDBSQLiteCopyAccSvc::initialize(){
  //before initializing the parent, I have to copy the file
  StatusCode sc = setProperties();
  if ( ! sc.isSuccess() ) {
    error() << "Failed to set properties" << endmsg;
    return sc;
  }

  // this should be done after getting the properties

  // preliminary checks on the options
  if ( m_source_path.empty() ) {
    error() << "You must provide the source file path via the option '"
        << name() << ".OriginalFile'" << endmsg;
    return StatusCode::FAILURE;
  }
  if ( m_dest_path.empty() ) {
    error() << "You must provide the destination file path via the option '"
        << name() << ".DestinationFile'" << endmsg;
    return StatusCode::FAILURE;
  }
  if ( m_dbname.empty() ) {
    error() << "You must provide the database name via the option '"
        << name() << ".DBName'" << endmsg;
    return StatusCode::FAILURE;
  }

  try {

    // if "force" mode is selected: remove the destination file if it exists
    if ( m_force_copy ) {
      bool file_existed = boost::filesystem::remove( m_dest_path );
      if ( file_existed ) {
        warning() << "Removed file '" << m_dest_path << "' to replace it" << endmsg;
      }
    }

    // copy the source file
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "Copying "
          << m_source_path << " -> "
          << m_dest_path << endmsg;
    boost::filesystem::copy_file(m_source_path,m_dest_path);

  }
  catch (boost::filesystem::filesystem_error &e){

    auto& log = (m_ignore_copy_error ? warning() : error() );
    log << "Problems occurred copying the file" << endmsg;
    log << e.what() << endmsg;
    if ( ! m_ignore_copy_error )
      return StatusCode::FAILURE;
  }

  // Set the connection string to be used (the one from the base class will be ignored).
  m_sqlite_connstring = "sqlite_file:" + m_dest_path + "/" + m_dbname;

  // Initialize the base class.
  return CondDBAccessSvc::initialize();
}

//=============================================================================
// Return the connection string used to connect to the database.
//=============================================================================
const std::string &CondDBSQLiteCopyAccSvc::connectionString() const {
  return m_sqlite_connstring;
}

//=============================================================================
