
// Gaudi
#include "GaudiAlg/FilterPredicate.h"

// STL
#include <string>
#include <math.h>

// from Boost
#include "boost/cstdint.hpp"
#include "boost/integer/integer_mask.hpp"
#include "boost/integer_traits.hpp"
using boost::uint32_t;
using boost::uint64_t;

// from LHCb core
#include "Event/ODIN.h"

class DeterministicPrescaler final : public Gaudi::Functional::FilterPredicate<bool(const LHCb::ODIN&)>
{
 public:
  DeterministicPrescaler( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  bool operator()(const LHCb::ODIN&) const override;

 private:

  /// fraction of input events to accept...
  double                  m_accFrac;

  /// integer representation of the above
  boost::uint32_t         m_acc{boost::integer_traits<uint32_t>::const_max};

  /// initial seed unique to this instance (computed from the name)
  boost::uint32_t         m_initial{0};

  // acahe pointer to counter, to avoid map look ups
  StatEntity*             m_counter = nullptr;

};

