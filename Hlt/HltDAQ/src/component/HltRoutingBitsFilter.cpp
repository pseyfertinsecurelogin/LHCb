// Include files
#include <vector>

// from Gaudi
#include "GaudiAlg/FilterPredicate.h"
#include "GaudiAlg/FunctionalUtilities.h"
#include "Event/RawEvent.h"
#include "Event/RawBank.h"

class HltRoutingBitsFilter : public Gaudi::Functional::FilterPredicate<bool(const LHCb::RawEvent&)>  {
public:
  HltRoutingBitsFilter( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override;                      ///< Algorithm initialisation
  bool operator()(const LHCb::RawEvent&) const override; ///< Algorithm execution
private:
  std::vector<unsigned int> m_r,m_v;
  bool m_passOnError;
};

//-----------------------------------------------------------------------------
// Implementation file for class : HltRoutingBitsFilter
//
// 2008-07-29 : Gerhard Raven
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( HltRoutingBitsFilter )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltRoutingBitsFilter::HltRoutingBitsFilter( const std::string& name,
                                        ISvcLocator* pSvcLocator)
: FilterPredicate( name , pSvcLocator,
                   KeyValue{ "RawEventLocations",
                              Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Trigger,
                                                                      LHCb::RawEventLocation::Copied,
                                                                      LHCb::RawEventLocation::Default )
                           } )
{
  declareProperty("VetoMask", m_v = std::vector<unsigned int>(3, 0x0));
  declareProperty("RequireMask", m_r = std::vector<unsigned int>(3, 0xFFFF));
  declareProperty("PassOnError", m_passOnError = true );
}

//=============================================================================
// Initialisation
//=============================================================================
StatusCode HltRoutingBitsFilter::initialize() {
  StatusCode sc = FilterPredicate::initialize(); // must be executed first
  if (m_v.size()!=3) {
    return Error("Property VetoMask should contain exactly 3 unsigned integers");
  }
  if (m_r.size()!=3) {
    return Error("Property RequireMask should contain exactly 3 unsigned integers");
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
bool HltRoutingBitsFilter::operator()(const LHCb::RawEvent& rawEvent) const {

  const auto& banks = rawEvent.banks(LHCb::RawBank::HltRoutingBits);
  if (banks.size()!=1) {
    counter("#unexpected number of HltRoutingBits rawbanks")++;
    Error("Unexpected # of HltRoutingBits rawbanks",
                 StatusCode::SUCCESS,0).ignore();
    return m_passOnError;
  }
  if (banks.front()->size()!=3*sizeof(unsigned int)) {
    counter("#unexpected HltRoutingBits rawbank size")++;
    Error("Unexpected HltRoutingBits rawbank size",
                 StatusCode::FAILURE,0).ignore();
    return m_passOnError;
  }
  const unsigned int *data = banks.front()->data();

  bool veto = false;
  bool req  = false;
  for (unsigned i=0;i<3 && !veto; ++i) {
        veto = veto || ( data[i] & m_v[i] );
        req  = req  || ( data[i] & m_r[i] );
  }
  bool accept = ( req & !veto );
  counter("#accept") += accept;

  return accept;
}
