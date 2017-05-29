// $Id: RawEventCopy.cpp,v 1.2 2009-05-25 08:56:21 cattanem Exp $
// Include files from Gaudi
#include <functional>
#include <string>

#include "Event/RawEvent.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartDataStorePtr.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "MDF/RawEventHelpers.h"

class ISvcLocator;

/*
 *    LHCb namespace declaration
 */
namespace LHCb  {

  /** @class RawEventCopy RawEventCopy.cpp
    *
    *  Creates and copies RawEvent data structure to new location
    *  in the data store.
    *
    *  @author Markus Frank
    *  @date   2005-10-13
    */
  class RawEventCopy : public Algorithm    {
    /// Property: source leaf for rawevent copy
    std::string m_source;
    /// Property: target leaf name for rawevent copy
    std::string m_destination;
    /// Property: shallow or deep copy
    bool m_deepCopy;

  public:
    /// Standard constructor
    RawEventCopy(const std::string& nam, ISvcLocator* pSvc)
    : Algorithm(nam,pSvc) {
      declareProperty("Destination",m_destination="/Event/DAQ/RawCopy");
      declareProperty("Source",     m_source=RawEventLocation::Default);
      declareProperty("DeepCopy",   m_deepCopy=false);
    }
    /// Destructor
    virtual ~RawEventCopy()  {}
    /// Main execution
    StatusCode execute() override {
      SmartDataPtr<RawEvent> raw(eventSvc(),m_source);
      if( raw ) {
        RawEvent *org = raw;
        RawEvent *res = 0;
        StatusCode sc;
        if (m_deepCopy) {
           sc = deepCopyRawEvent(org, res);
        } else {
           sc = cloneRawEvent(org, res);
        }
        if ( sc.isSuccess() )  {
          sc = eventSvc()->registerObject(m_destination, res);
          if ( !sc.isSuccess() )  {
            delete res;
          } else {
            return sc;
          }
        }
      }
      return StatusCode::FAILURE;
    }
  };
}

DECLARE_NAMESPACE_ALGORITHM_FACTORY(LHCb,RawEventCopy)
