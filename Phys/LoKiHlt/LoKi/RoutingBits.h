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
// ============================================================================
#ifndef LOKI_ROUTINGBITS_H
#define LOKI_ROUTINGBITS_H 1
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Interface.h"
// ============================================================================
// forward declarations
// ============================================================================
namespace LHCb {
  class RawEvent;
}
// ============================================================================
/** @file
 *  set of helper function to deal with "hlt-routing-bits"
 *  @see Hlt::firedRoutingBits
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2010-05-17
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace HLT {
    // ========================================================================
    /** @class RoutingBits
     *  simple class to check if the given routing bit is fired
     *  @see LoKi::Cuts::ROUTINGBITS
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-05-17
     */
    class RoutingBits : public LoKi::Functor<void, bool> {
    public:
      // ======================================================================
      typedef std::vector<unsigned int> Bits;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the bits
      RoutingBits( std::vector<unsigned int> bits );
      /// constructor from the bit
      explicit RoutingBits( unsigned int bit ) : RoutingBits( std::vector<unsigned int>{bit} ) {}
      /// constructor from the bits
      RoutingBits( unsigned int bit1, unsigned int bit2 ) : RoutingBits( std::vector<unsigned int>{bit1, bit2} ) {}
      /// constructor from the bits
      RoutingBits( unsigned int bit1, unsigned int bit2, unsigned int bit3 )
          : RoutingBits( std::vector<unsigned int>{bit1, bit2, bit3} ) {}
      /// constructor from the bits
      RoutingBits( unsigned int bit1, unsigned int bit2, unsigned int bit3, unsigned int bit4 )
          : RoutingBits( std::vector<unsigned int>{bit1, bit2, bit3, bit4} ) {}
      /// MANDATORY: clones method ("virtual contructor")
      RoutingBits* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()() const override;
      /// OPTIONAL : the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
      const Bits& bits() const { return m_bits; }
      // ======================================================================
    private:
      // ======================================================================
      /// get the fired bits
      std::size_t getFired() const; // get the fired bits
      // ======================================================================
    private:
      // ======================================================================
      /// the list of fired bits
      mutable Bits m_fired; // the list of fired bits
      /// the bits to be checked
      Bits m_bits; //  the bit to be checked
      // ======================================================================
    };
  } // namespace HLT
  // ==========================================================================
  namespace Cuts {
    // ========================================================================
    /** @typedef ROUTINBITS
     *  simple (void)-predicate to check the fired routing bits
     *  @see LoKi::HLT::RoutingBits
     *  @see LoKi::HLT::RoutingBit
     *  @see LoKi::Cuts::ROUTINGBIT
     *  @see Hlt::firedRoutingBits
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @date   2010-05-17
     */
    typedef LoKi::HLT::RoutingBits ROUTINGBITS;
    // ========================================================================
  } // namespace Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_ROUTINGBITS_H
