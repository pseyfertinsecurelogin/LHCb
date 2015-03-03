// $Id: IReporter.h 133878 2012-01-18 19:33:38Z ibelyaev $
// ============================================================================
#ifndef LOKI_IREPORTER_H 
#define LOKI_IREPORTER_H 1
// ============================================================================
// Include files
// ============================================================================
// from GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// GaudiAlg 
// ============================================================================
#include "GaudiAlg/IErrorTool.h"
// ============================================================================
// forward declarations 
// ============================================================================
class GaudiException ;
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *   By usage of this code one clearly states the disagreement 
 *    with the smear campaign of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 * 
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23 
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class IReporter IReporter.h LoKi/IReporter.h
   *  
   *  @author Vanya BELYAEV ivelyaev@physics.syr.edu
   *  @date   2003-01-16
   */
  class IReporter 
    : public virtual   IAlgTool   
    , public virtual   IErrorTool
  {
  public: 
    // ========================================================================
    /** Retrieve interface ID
     *  mandatory method from IInterface 
     *  @see IInterface 
     *  @see InterfaceID 
     *  @return unique interface identifier 
     */
    static const InterfaceID& interfaceID() ;
    // ========================================================================
  protected:
    // ========================================================================
    // destructor 
    virtual ~IReporter() ; 
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_IREPORTER_H
// ============================================================================
