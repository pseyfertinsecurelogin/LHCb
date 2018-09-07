// ============================================================================
#ifndef LOKI_HltLOCK_H 
#define LOKI_HltLOCK_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Interface.h"
#include "LoKi/IHltAntiFactory.h"
#include "LoKi/Context.h"
// ============================================================================
namespace LoKi
{
  namespace Hybrid
  {
    // ========================================================================
    /** @class HltLock  LoKi/HltLock.h 
     *  Helper class (sentry) to connent LoKi::Hybrid::IHltAntFactory 
     *  to HltEngine  
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-18
     */
    class HltLock 
    {
    public:
      // ======================================================================
      /// contructor : Lock 
      HltLock  ( const LoKi::Hybrid::IHltAntiFactory* factory , 
                 const LoKi::Context&                 context ) ; 
      /// destrcutor : UnLock
      virtual ~HltLock () ;                              // destructor : UnLock
      // ======================================================================
    private:
      // ======================================================================
      /// no copy constructor 
      HltLock ( const HltLock& ) ;                       // no copy constructor 
      /// no assignement opeartor 
      HltLock& operator = ( const HltLock& ) ;       // no assignement operator
      // ======================================================================
    private:
      // ======================================================================
      /// the tool itself 
      LoKi::Interface<LoKi::Hybrid::IHltAntiFactory> m_tool ;  // the tool
      // ======================================================================
    } ; 
    // ========================================================================
 } //                                         The end of namespace LoKi::Hybrid 
  // ==========================================================================  
} //                                                  The end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_HltLOCK_H
// ============================================================================
