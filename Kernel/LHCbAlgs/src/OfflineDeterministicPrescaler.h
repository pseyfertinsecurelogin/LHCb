#include <stdint.h>
#include <iostream>
#include <string>

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/PropertyFwd.h"
#include "GaudiKernel/StatusCode.h"
#include "boost/cstdint.hpp"
// from Boost
#include "boost/integer/integer_mask.hpp"
#include "boost/integer_traits.hpp"

class ISvcLocator;

using boost::uint32_t;
using boost::uint64_t;

class StatEntity;
namespace LHCb {
class RecHeader;
}  // namespace LHCb

class OfflineDeterministicPrescaler : public GaudiAlgorithm
{

public:

  OfflineDeterministicPrescaler( const std::string& name, ISvcLocator* pSvcLocator );
  ~OfflineDeterministicPrescaler( ) = default;

  StatusCode initialize() override;
  StatusCode execute() override;

public:

  inline void update(double newAccFrac)
  {
    m_accFrac = newAccFrac;
    update();
  }

protected:

  bool accept() const ;
  bool accept(const LHCb::RecHeader& header) const ;
  inline void update()
  {
    m_acc = ( m_accFrac<=0 ? 0
              : m_accFrac>=1 ? boost::integer_traits<uint32_t>::const_max
              : boost::uint32_t( m_accFrac*boost::integer_traits<uint32_t>::const_max )
              );
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "frac: " << m_accFrac << " acc: 0x" << std::hex << m_acc << endmsg;
  }

protected:

  double                  m_accFrac;      // fraction of input events to accept...
  boost::uint32_t         m_acc{boost::integer_traits<uint32_t>::const_max};  // integer representation of the accept rate

  StatEntity*             m_counter = nullptr;

private:

  boost::uint32_t         m_initial{0};      // initial seed unique to this instance (computed from the name)

  void update(Property&) ;

};

