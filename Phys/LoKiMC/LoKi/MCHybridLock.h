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
#ifndef LOKI_MCHYBRIDLOCK_H
#define LOKI_MCHYBRIDLOCK_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Context.h"
#include "LoKi/IMCHybridTool.h"
#include "LoKi/Interface.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Hybrid {
    // ========================================================================
    /** @class MCHybridLock  MCHybriddLock.h LoKi/MCHybridLock.h
     *  Helper class (sentry) to connent IMCHybridTool to MCHybridEngine
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-09
     */
    class GAUDI_API MCLock {
    public:
      // ======================================================================
      /// constructor : Lock
      MCLock( const LoKi::IMCHybridTool* tool, //  constructor : Lock
              const LoKi::Context&       context );
      /// destrcutor : UnLock
      virtual ~MCLock(); //  destructor : UnLock
      // ======================================================================
      /// no copy constructor
      MCLock( const MCLock& ) = delete; // no copy constructor
      /// no assignement opeartor
      MCLock& operator=( const MCLock& ) = delete; // no assignement operator
      // ======================================================================
    private:
      // ======================================================================
      /// the tool itself
      LoKi::Interface<LoKi::IMCHybridTool> m_tool; // the tool itself
      // ======================================================================
    };
    // ========================================================================
  } // namespace Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCHYBRIDLOCK_H
