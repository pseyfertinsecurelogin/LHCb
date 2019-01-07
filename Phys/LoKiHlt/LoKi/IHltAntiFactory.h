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
#ifndef LOKI_IHltANTIFACTORY_H
#define LOKI_IHltANTIFACTORY_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/OdinTypes.h"
#include "LoKi/L0Types.h"
#include "LoKi/HLTTypes.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class IHltAntiFactory
     *  The functor 'anti'-factory for creation of Hlt-related  LoKi functors
     *
     *  Helper interface for implementation of C++/Python "Hybrid" solution
     *
     *  This file is a part of LoKi project -
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
     *  contributions and advices from G.Raven, J.van Tilburg,
     *  A.Golutvin, P.Koppenburg have been used in the design.
     *
     *  @author Vanya BELYAEV Ivabn.Belyaev@nikhef.nl
     *  @date 2008-09-18
     */
    struct IHltAntiFactory : extend_interfaces<IAlgTool>
    {
      // ======================================================================
      DeclareInterfaceID( IHltAntiFactory , 2 , 0 ) ;
      // ======================================================================
      // L0
      // ======================================================================
      /// set the C++ function for LHCb::L0DUReport
      virtual void set ( const LoKi::Types::L0_Cuts& cut ) = 0 ;
      /// set the C++ function for LHCb::L0DUReport
      virtual void set ( const LoKi::Types::L0_Func& cut ) = 0 ;
      // ======================================================================
      // O1
      // ======================================================================
      /// set the C++ function for LHCb::ODIN
      virtual void set ( const LoKi::Types::ODIN_Cuts& cut ) = 0 ;
      /// set the C++ function for LHCb::ODIN
      virtual void set ( const LoKi::Types::ODIN_Func& cut ) = 0 ;
      // ======================================================================
      // HLT
      // ======================================================================
      /// set the C++ function for LHCb::HltDecReports
      virtual void set ( const LoKi::Types::HLT_Cuts& cut ) = 0 ;
      /// set the C++ function for LHCb::HltDecREports
      virtual void set ( const LoKi::Types::HLT_Func& cut ) = 0 ;
      // ======================================================================
    } ;
    // ========================================================================
  } // end of namespace LoKi::Hybrid
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_IHltANTIFACTORY_H
