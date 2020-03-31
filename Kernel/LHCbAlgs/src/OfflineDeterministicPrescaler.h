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
#include "Event/RecHeader.h"
#include "GaudiAlg/FilterPredicate.h"
#include <cstdint>
#include <string>

class OfflineDeterministicPrescaler : public Gaudi::Functional::FilterPredicate<bool( const LHCb::RecHeader& )> {

public:
  OfflineDeterministicPrescaler( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  bool       operator()( const LHCb::RecHeader& ) const override;

  void update( double newAccFrac ) {
    m_accFrac = newAccFrac;
    update();
  }

protected:
  void update() {
    constexpr auto mx = boost::integer_traits<uint32_t>::const_max;
    m_acc             = ( m_accFrac <= 0 ? 0 : m_accFrac >= 1 ? mx : boost::uint32_t( m_accFrac * mx ) );
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << "frac: " << m_accFrac.value() << " acc: 0x" << std::hex << m_acc << endmsg;
  }

  Gaudi::Property<double> m_accFrac{this, "AcceptFraction", 1., [=]( Gaudi::Details::PropertyBase& ) {
                                      update();
                                      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
                                        debug() << "frac: " << m_accFrac.value() << " acc: 0x" << std::hex << m_acc
                                                << endmsg;
                                    }}; // fraction of input events to accept...
private:
  bool     accept( const LHCb::RecHeader& header ) const;
  uint32_t m_initial{0}; // initial seed unique to this instance (computed from the name)
  uint32_t m_acc{boost::integer_traits<uint32_t>::const_max}; // integer representation of the accept rate
  mutable Gaudi::Accumulators::BinomialCounter<> m_counter{this, "#accept"};
};
