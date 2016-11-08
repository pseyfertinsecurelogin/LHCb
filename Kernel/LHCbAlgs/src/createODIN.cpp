#include "GaudiKernel/IEventTimeDecoder.h"
#include "GaudiAlg/Transformer.h"
#include "ODINCodec.h"
#include "Event/RawEvent.h"
#include "boost/optional.hpp"

namespace details {
    //@TODO/@FIXME: move this to a better place (IProperty?) so it can be re-used?
    template <typename T>
    boost::optional<T> getProperty(const IProperty& owner, const std::string& key) {
        std::string value; T val;
        return boost::make_optional(
                owner.getProperty( key, value ) && Gaudi::Parsers::parse( val, value ),
                val );
    }
}

/** Trivial algorithm to create DAQ/ODIN object from ODIN RawEvent bank
 *
 *  @author Marco Cattaneo
 *  @date   2008-01-15
 *  @author Marco Clemencic
 *  @date   2016-09-19
 */
struct createODIN final : Gaudi::Functional::Transformer<LHCb::ODIN(const LHCb::RawEvent&)>
{
  /// Standard constructor
  createODIN( const std::string& name, ISvcLocator* pSvcLocator ) :
    Transformer( name, pSvcLocator,
                 KeyValue("RawEvent",
                            Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Trigger, LHCb::RawEventLocation::Default ) ),
                 KeyValue("ODIN", LHCb::ODINLocation::Default)
               )
  {
  }

  LHCb::ODIN operator()(const LHCb::RawEvent& rawEvent) const override {
    const auto & odinBanks = rawEvent.banks(LHCb::RawBank::ODIN);
    Assert( !odinBanks.empty(), "no ODIN bank in raw event" );
    return LHCb::ODINCodec::decode(*odinBanks[0]);
  }
};

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( createODIN )
