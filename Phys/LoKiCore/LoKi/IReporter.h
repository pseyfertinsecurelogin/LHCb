// $Id: IReporter.h,v 1.4 2006-11-25 19:12:55 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ , version $Revision: 1.4 $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.3  2006/05/02 14:29:09  ibelyaev
//  censored
//
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
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23 
 */
// ============================================================================

namespace LoKi
{
  /** @class IReporter IReporter.h LoKi/IReporter.h
   *  
   *
   *  @author Ivan Belyaev ivelyaev@physics.syr.edu
   *  @date   2003-01-16
   */
  class IReporter : public virtual   IAlgTool   , 
                    public virtual   IErrorTool
  {
  public:
    
    /** Retrieve interface ID
     *  mandatory method from IInterface 
     *  @see IInterface 
     *  @see InterfaceID 
     *  @return unique interface identifier 
     */
    static const InterfaceID& interfaceID() ; 
    
  protected: 
    
    // destructor 
    virtual ~IReporter() ; 
  };
  
} // end of namespace LoKi 


// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_IREPORTER_H
// ============================================================================
