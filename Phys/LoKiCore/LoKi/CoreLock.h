/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef LOKI_CORELOCK_H
#define LOKI_CORELOCK_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Interface.h"
#include "LoKi/ICoreAntiFactory.h"
#include "LoKi/Context.h"
// ============================================================================
/** @file LoKi/CoreLock.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class CoreLock  LoKi/coreLock.h
     *  Helper class (sentry) to connent LoKi::Hybrid::ICoreAntiFactory
     *  to CoreEngine
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-18
     */
    class CoreLock
    {
    public:
      // ======================================================================
      /** contructor : Lock
       *  @param  factory pointer to (anti)factory 
       *  @param  context the context 
       */
      explicit CoreLock  
      ( const LoKi::Hybrid::ICoreAntiFactory* factory , 
        const LoKi::Context&                  context ) ; 
      /// destrcutor : UnLock
      virtual ~CoreLock () ;                              // destructor : UnLock
      // ======================================================================
      /// no copy constructor
      CoreLock ( const CoreLock& ) = delete ;            // no copy constructor
      /// no assignement opeartor
      CoreLock& operator = ( const CoreLock& ) = delete ;// no assignement operator
      // ======================================================================
    private:
      // ======================================================================
      /// the tool itself
      LoKi::Interface<LoKi::Hybrid::ICoreAntiFactory> m_tool ;  // the tool
      // ======================================================================
    } ;
    // ========================================================================
 } //                                         The end of namespace LoKi::Hybrid
  // ==========================================================================
} //                                                  The end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_CORELOCK_H
// ============================================================================
