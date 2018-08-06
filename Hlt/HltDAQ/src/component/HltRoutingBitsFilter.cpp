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

  Gaudi::Property<std::vector<unsigned int>> m_r{ this, "RequireMask",{ ~0u,~0u,~0u } },
                                             m_v{ this, "VetoMask",   {  0u, 0u, 0u } };
  Gaudi::Property<bool> m_passOnError { this, "PassOnError", true };
};

//-----------------------------------------------------------------------------
// Implementation file for class : HltRoutingBitsFilter
//
// 2008-07-29 : Gerhard Raven
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltRoutingBitsFilter )

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
  if (banks[0]->size()!=3*sizeof(unsigned int)) {
    counter("#unexpected HltRoutingBits rawbank size")++;
    Error("Unexpected HltRoutingBits rawbank size",
                 StatusCode::FAILURE,0).ignore();
    return m_passOnError;
  }
  const unsigned int *data = banks[0]->data();

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
