// ============================================================================
#ifndef KERNEL_IINACCEPTANCE_H
#define KERNEL_IINACCEPTANCE_H 1

// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// forward declarations
// ============================================================================
namespace LHCb::Event
{
  inline namespace v1
  {
    class Track ;
  }
}
// ============================================================================

// ============================================================================
/** @class IInAcceptance IInAcceptance.h Kernel/IInAcceptance.h
 *
 *  An abstract interface for implementation "acceptance" tools.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
 *  @date   2005-07-22
 */
// ============================================================================
struct IInAcceptance : extend_interfaces<IAlgTool>
{
  DeclareInterfaceID( IInAcceptance, 2, 0 ) ;

  /** check the track is in acceptance of given "detector"
   *  @param track track to be checked
   *  @return true if the track is in acceptance
   */
  virtual bool inAcceptance ( const LHCb::Event::Track* track ) const = 0 ;

};

// ============================================================================
// The END
// ============================================================================
#endif // KERNEL_IINACCEPTANCE_H
// ============================================================================
