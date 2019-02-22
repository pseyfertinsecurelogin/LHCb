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
#pragma once

#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/Map.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartIF.h"

#include "DetDesc/RunChangeIncident.h"
#include "DetDesc/ValidDataObject.h"

#include "XmlTools/IXmlParserSvc.h"

#include <boost/format.hpp>

#include <array>
#include <fstream>
#include <mutex>
#include <openssl/sha.h>

/** @class RunChangeHandlerSvc RunChangeHandlerSvc.h
 *
 *  Service intercepting "RunChange" incidents to replace the path of the XML
 *  files according to the new run number.
 *
 *  @author Marco Clemencic
 *  @date   2008-07-24
 */
class RunChangeHandlerSvc : public extends<Service, IIncidentListener> {

public:
  /// Standard constructor
  using base_class::base_class;

  /// Initialize Service
  StatusCode initialize() override;

  /// Finalize Service
  StatusCode finalize() override;

  // ---- Implement IIncidentListener interface ----
  /// Handle RunChange incident.
  void handle( const Incident& inc ) override;

private:
  /// Helper function to retrieve a service and cache the pointer to it.
  template <class I>
  inline SmartIF<I>& getService( const std::string& name, SmartIF<I>& ptr ) const {
    if ( UNLIKELY( !ptr ) ) {
      ptr = serviceLocator()->service( name, true );
      if ( UNLIKELY( !ptr ) ) {
        throw GaudiException( "Service [" + name + "] not found", this->name(), StatusCode::FAILURE );
      }
    }
    return ptr;
  }

  /// Get pointer to the event data service.
  inline SmartIF<IDataProviderSvc>& eventSvc() const { return getService( "EventDataSvc", m_evtSvc ); }

  /// Get pointer to the detector data service.
  inline SmartIF<IDataProviderSvc>& detectorSvc() const { return getService( "DetectorDataSvc", m_detSvc ); }

  /// Get pointer to the incident service.
  inline SmartIF<IIncidentSvc>& incidentSvc() const { return getService( "IncidentSvc", m_incSvc ); }

  /// Get pointer to the detector data service.
  inline SmartIF<IUpdateManagerSvc>& updMgrSvc() const { return getService( "UpdateManagerSvc", m_ums ); }

  /// Get pointer to the detector data service.
  inline SmartIF<IEventProcessor>& evtProc() const { return getService( "ApplicationMgr", m_evtProc ); }

  /// Get pointer to the detector data service.
  inline SmartIF<IXmlParserSvc>& xmlParser() const { return getService( "XmlParserSvc", m_xmlParser ); }

public:
  /// Helper class to compute the cryptographic hash (SHA1) of files.
  /// The class caches the hash of files already processed, to avoid
  /// useless computations.
  struct FileHasher {
    using Hash_t = std::array<unsigned char, SHA_DIGEST_LENGTH>;

    /// Return the cryptographic hash of the content of the file
    /// passed as argument
    const Hash_t& operator()( const std::string& path ) const {
      // protect the cache from multiple accesses
      std::unique_lock<std::mutex> lock( m_mutex );
      auto                         h = m_cache.find( path );
      if ( h == m_cache.end() ) {
        h = m_cache.emplace( std::pair{path, Hash_t{0}} ).first;
        SHA_CTX c;
        SHA1_Init( &c );
        std::ifstream data( path, std::ios::binary );
        if ( !data.good() ) { throw std::runtime_error( "problems opening " + path ); }
        const std::streamsize         BUFFER_SIZE = 2 * 1024;
        std::array<char, BUFFER_SIZE> buff{0};
        while ( !data.eof() ) {
          data.read( buff.data(), buff.size() );
          SHA1_Update( &c, buff.data(), data.gcount() );
        }
        SHA1_Final( h->second.data(), &c );
      }
      return h->second;
    }

  private:
    mutable std::map<std::string, Hash_t> m_cache;
    mutable std::mutex                    m_mutex;
  };

  /// Helper class to work with conditions data file path templates.
  struct PathTemplate {
    /// Construct an instance from a template file name.
    /// The file name may contain the special sequence `%d` or `%s` (only one occurrence).
    /// `%d` will be replaced by the run number, while `%s` by the string "_k_/_n_"
    ///  where _k_ is run number divided by 1000 and _n_ is the run number.
    PathTemplate( const std::string& f ) : fmt{f}, hash{0}, splitRunString{f.find( "%s" ) != std::string::npos} {
      if ( fmt.expected_args() > 1 )
        throw std::invalid_argument( "format string can have only 0 or 1 placeholders: \"" + f + "\"" );
      if ( splitRunString ) fmt.parse( ( fmt % "%d/%d" ).str() );
    }

    /// Check if the file changes when going to the requested run.
    /// After this call, the data member path will hold the new name.
    bool changed( unsigned long run, const FileHasher& hasher ) {
      if ( fmt.expected_args() ) {
        if ( splitRunString ) {
          fmt % ( run / 1000 ) % run;
        } else {
          fmt % run;
        }
      }
      path         = fmt.str();
      auto oldhash = hash;
      hash         = hasher( path );
      return hash != oldhash;
    }

    boost::format      fmt;
    std::string        path;
    FileHasher::Hash_t hash;
    bool               splitRunString;
  };

  /// Class to simplify handling of the objects to modify.
  struct CondData {
    CondData( IDataProviderSvc* pService, const std::string& fullPath, const std::string& pathTempl )
        : object{pService, fullPath}, dataFile{pathTempl} {}

    /// Check if the wrapped data object requires an update for
    /// the specified run.
    bool needsUpdate( unsigned long run, const FileHasher& hasher, bool force = false ) {
      // assert that the object can be used
      if ( !object || !object->registry() || !object->registry()->address() )
        throw std::runtime_error{"Cannot modify address for object at " + object.path()};

      if ( dataFile.changed( run, hasher ) || force ) {
        auto addr = object->registry()->address();
        // This is a bit of a hack, but it is the only way of replacing the
        // URL to use for an object.
        std::string* par = const_cast<std::string*>( addr->par() );
        par[0]           = dataFile.path;
        // flag the object as in need of an update
        object->forceUpdateMode();
        return true;
      }
      return false;
    }

    SmartDataPtr<ValidDataObject> object;
    PathTemplate                  dataFile;
  };

private:
  /// Flag for update all the registered objects.
  void update( unsigned long run );

  typedef GaudiUtils::Map<std::string, std::string> CondDescMap;
  Gaudi::Property<CondDescMap>                      m_condDesc{
      this, "Conditions", {}, "Map defining what to use to replace the location of the source XML files."};

  Gaudi::Property<bool> m_forceUpdate{this, "ForceUpdate", false, "Always invalidate files."};

  typedef std::vector<CondData> Conditions;
  /// List of objects to modify
  Conditions m_conditions;

  /// Current run number.
  unsigned long m_currentRun = 0;

  /// EventDataSvc, for ODIN
  mutable SmartIF<IDataProviderSvc> m_evtSvc;

  /// SetectorDataSvc, for the objects to invalidate
  mutable SmartIF<IDataProviderSvc> m_detSvc;

  /// Incident service, to register as listener
  mutable SmartIF<IIncidentSvc> m_incSvc;

  /// UpdateMangerSvc, to invalidate objects
  mutable SmartIF<IUpdateManagerSvc> m_ums;

  /// Pointer to the event processor in order to be able to stop the run if
  /// something goes wrong during the incident handling.
  mutable SmartIF<IEventProcessor> m_evtProc;

  /// Pointer to the XML parser service, used to clear the cache of parsed XML
  /// files when we get a RunChangeIncident without actual run change.
  mutable SmartIF<IXmlParserSvc> m_xmlParser;

  mutable std::mutex m_updateMutex;
};
