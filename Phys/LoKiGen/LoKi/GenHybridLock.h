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
// ============================================================================
#ifndef LOKI_GENHYBRIDLOCK_H
#define LOKI_GENHYBRIDLOCK_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Interface.h"
#include "LoKi/IGenHybridTool.h"
#include "LoKi/Context.h"
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
 *  @date 2007-10-31
 *
 */
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class GenLock  LoKi/GenHybridLock.h
     *  Helper class (sentry) to connent IGenHybridTool to GenEngine
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-09
     */
    class GenLock final
    {
    public:
      // ======================================================================
      /// constructor : Lock
      GenLock  ( const IGenHybridTool* factory ,   // contructor : Lock
                 const LoKi::Context&  context ) ;
      /// destructor : UnLock
      ~GenLock () ;               // destructor : UnLock
      // ======================================================================
      /// no copy constructor
      GenLock ( const GenLock& ) = delete;     // no copy constructor
      /// no assignement operator
      GenLock& operator = ( const GenLock& ) = delete; // no assignement operator
      // ======================================================================
    private:
      // ======================================================================
      /// the tool itself
      LoKi::Interface<LoKi::IGenHybridTool> m_tool ;  // the tool itself
      // ======================================================================
    } ;
    // ========================================================================
  } //                                            end of namespace LoKi::Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCHYBRIDLOCK_H
// ============================================================================
