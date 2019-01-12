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
#ifndef LOKI_IGENHYBRIDTOOL_H
#define LOKI_IGENHYBRIDTOOL_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
// ============================================================================
// from Gaudi
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenTypes.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class IGenHybridTool LoKi/IGenHybridTool.h
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
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2004-06-29
   */
  struct GAUDI_API IGenHybridTool : extend_interfaces<IAlgTool>
  {
    // ========================================================================
    DeclareInterfaceID( IGenHybridTool , 5 , 0 ) ;
    // ========================================================================
    // predicates:
    // ========================================================================
    /// set the C++ predicate for HepMC::GenParticle
    virtual void set ( const LoKi::Types::GCuts&   cut ) = 0 ;
    /// set the C++ predicate for HepMC::GenVertex
    virtual void set ( const LoKi::Types::GVCuts&  cut ) = 0 ;
    // ========================================================================
    // functions
    // ========================================================================
    /// set the C++ function for HepMC::GenParticle
    virtual void set ( const LoKi::Types::GFunc&   cut ) = 0 ;
    /// set the C++ function for HepMC::GenVertex
    virtual void set ( const LoKi::Types::GVFunc&  cut ) = 0 ;
    // ========================================================================
    // maps
    // ========================================================================
    /// set the C++ function for HepMC::GenParticle
    virtual void set ( const LoKi::Types::GMaps&   cut ) = 0 ;
    /// set the C++ function for HepMC::GenVertex
    virtual void set ( const LoKi::Types::GVMaps&  cut ) = 0 ;
    // ========================================================================
    // pipes
    // ========================================================================
    /// set the C++ function for HepMC::GenParticle
    virtual void set ( const LoKi::Types::GPipes&   cut ) = 0 ;
    /// set the C++ function for HepMC::GenVertex
    virtual void set ( const LoKi::Types::GVPipes&  cut ) = 0 ;
    // ========================================================================
    // fun-vals
    // ========================================================================
    /// set the C++ function for HepMC::GenParticle
    virtual void set ( const LoKi::Types::GFunVals&   cut ) = 0 ;
    /// set the C++ function for HepMC::GenVertex
    virtual void set ( const LoKi::Types::GVFunVals&  cut ) = 0 ;
    // ========================================================================
    // cut-vals
    // ========================================================================
    /// set the C++ function for HepMC::GenParticle
    virtual void set ( const LoKi::Types::GCutVals&   cut ) = 0 ;
    // ========================================================================
    // sources
    // ========================================================================
    /// set the C++ function for HepMC::GenParticle
    virtual void set ( const LoKi::Types::GSources&   cut ) = 0 ;
    /// set the C++ function for HepMC::GenVertex
    virtual void set ( const LoKi::Types::GVSources&  cut ) = 0 ;
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_IGENHYBRIDTOOL_H
