#pragma once

#include "GaudiKernel/Service.h"
#include "GaudiKernel/Map.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartIF.h"

#include "DetDesc/ValidDataObject.h"
#include "DetDesc/RunChangeIncident.h"

#include "XmlTools/IXmlParserSvc.h"

#include <boost/format.hpp>

#include <array>
#include <fstream>
#include <openssl/sha.h>

/** @class RunChangeHandlerSvc RunChangeHandlerSvc.h
 *
 *  Service intercepting "RunChange" incidents to replace the path of the XML
 *  files according to the new run number.
 *
 *  @author Marco Clemencic
 *  @date   2008-07-24
 */
class RunChangeHandlerSvc: public extends<Service, IIncidentListener> {

public:

  /// Standard constructor
  using base_class::base_class;

  /// Initialize Service
  StatusCode initialize() override;

  /// Finalize Service
  StatusCode finalize() override;

  // ---- Implement IIncidentListener interface ----
  /// Handle RunChange incident.
  void handle(const Incident &inc) override;

private:

  /// Helper function to retrieve a service and cache the pointer to it.
  template <class I>
  inline SmartIF<I>& getService(const std::string &name, SmartIF<I> &ptr) const {
    if (UNLIKELY( !ptr )) {
      ptr = serviceLocator()->service(name, true);
      if(UNLIKELY( !ptr )) {
        throw GaudiException("Service ["+name+"] not found", this->name(),
            StatusCode::FAILURE);
      }
    }
    return ptr;
  }

  /// Get pointer to the event data service.
  inline SmartIF<IDataProviderSvc>& eventSvc() const {
    return getService("EventDataSvc", m_evtSvc);
  }

  /// Get pointer to the detector data service.
  inline SmartIF<IDataProviderSvc>& detectorSvc() const {
    return getService("DetectorDataSvc", m_detSvc);
  }

  /// Get pointer to the incident service.
  inline SmartIF<IIncidentSvc>& incidentSvc() const {
    return getService("IncidentSvc", m_incSvc);
  }

  /// Get pointer to the detector data service.
  inline SmartIF<IUpdateManagerSvc>& updMgrSvc() const {
    return getService("UpdateManagerSvc", m_ums);
  }

  /// Get pointer to the detector data service.
  inline SmartIF<IEventProcessor>& evtProc() const {
    return getService("ApplicationMgr", m_evtProc);
  }

  /// Get pointer to the detector data service.
  inline SmartIF<IXmlParserSvc>& xmlParser() const {
    return getService("XmlParserSvc", m_xmlParser);
  }

  /// Helper class to work with conditions data file path templates.
  struct PathTemplate {
    PathTemplate(const std::string& f): fmt{f}, hash{0} {}

    using Hash_t = std::array<unsigned char, SHA_DIGEST_LENGTH>;

    /// Check if the file changes when going to the requested run.
    /// After this call, the data member path will hold the new name.
    bool changed(unsigned long run) {
      if (fmt.expected_args()) {
        fmt % run;
      }
      path = fmt.str();
      Hash_t oldhash = hash;
      {
        SHA_CTX c;
        SHA1_Init(&c);
        std::ifstream data(path, std::ios::binary);
        const std::streamsize BUFFER_SIZE = 1024 * 1024;
        std::array<char, BUFFER_SIZE> buff{0};
        while (!data.eof()) {
          data.read(buff.data(), buff.size());
          SHA1_Update(&c, buff.data(), data.gcount());
        }
        SHA1_Final(hash.data(), &c);
      }

      return hash != oldhash;
    }

    boost::format fmt;
    std::string path;
    Hash_t hash;
  };

  /// Class to simplify handling of the objects to modify.
  struct CondData {
    CondData(IDataProviderSvc* pService,
        const std::string& fullPath,
        const std::string& pathTempl):
          object{pService, fullPath},
          dataFile{pathTempl} {}

    /// Check if the wrapped data object requires an update for
    /// the specified run.
    bool needsUpdate(unsigned long run) {
      // assert that the object can be used
      if ( ! object || ! object->registry() || ! object->registry()->address() )
        throw std::runtime_error{"Cannot modify address for object at " + object.path()};

      if (dataFile.changed(run)) {
        auto addr = object->registry()->address();
        // This is a bit of a hack, but it is the only way of replacing the
        // URL to use for an object.
        std::string* par = const_cast<std::string*>(addr->par());
        par[0] = dataFile.path;
        // flag the object as in need of an update
        object->forceUpdateMode();
        return true;
      }
      return false;
    }

    SmartDataPtr<ValidDataObject> object;
    PathTemplate dataFile;
  };

  /// Modify the object opaque address (flag for update).
  void update(CondData &cond);

  /// Flag for update all the registered objects.
  void update();

  typedef GaudiUtils::Map<std::string,std::string> CondDescMap;
  Gaudi::Property<CondDescMap> m_condDesc { this, "Conditions", {},
   "Map defining what to use to replace the location of the source XML files."};

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

};
