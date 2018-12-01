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
#ifndef LOKI_MCVERTICES_H
#define LOKI_MCVERTICES_H 1
// ============================================================================
// Include files
// ============================================================================
// STL & STD
// ============================================================================
#include <string>
#include <math.h>
// ============================================================================
// Event
// ============================================================================
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/Operators.h"
#include "LoKi/Constants.h"
#include "LoKi/MCTypes.h"
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
  // ==========================================================================
  /** @namespace  LoKi::MCVertices MCVertices.h LoKi/MCVertices.h
   *
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2004-07-07
   */
  namespace MCVertices
  {
    // ========================================================================
    /** @class TypeOfMCVertex
     *  evaluator of MC verte type
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    struct GAUDI_API TypeOfMCVertex
      : LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
      // ======================================================================
      /// clone method (mandatory)
      TypeOfMCVertex* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class TimeOfFlight
     *  evaluator of MC vertex TOF
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    struct GAUDI_API TimeOfFlight : LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
      // ======================================================================
      /// clone method (mandatory)
      TimeOfFlight* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class VertexPositionX
     *  evaluator of x-position of MC vertex
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    struct GAUDI_API VertexPositionX : LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
      // ======================================================================
      /// clone method (mandatory)
      VertexPositionX* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class VertexPositionY
     *  evaluator of y-position of MC vertex
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    struct GAUDI_API VertexPositionY : LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
      // ======================================================================
      /// clone method (mandatory)
      VertexPositionY* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class VertexPositionZ
     *  evaluator of z-position of MC vertex
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    struct GAUDI_API VertexPositionZ
      : LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
      // ======================================================================
      /// clone method (mandatory)
      VertexPositionZ* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class VertexTime
     *  evaluator of "vertex time" (?)
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-06
     */
    struct GAUDI_API VertexTime : LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
      // ======================================================================
      /// clone method (mandatory)
      VertexTime* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Primary
     *  return true for 'primary' vertices
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-06
     */
    struct GAUDI_API Primary : LoKi::BasicFunctors<const LHCb::MCVertex*>::Predicate
    {
      // ======================================================================
      /// clone method (mandatory)
      Primary* clone() const  override;
      /// the only one essential method
      bool operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Decay
     *  return true for 'decay' vertices
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-06
     */
    struct GAUDI_API Decay : LoKi::BasicFunctors<const LHCb::MCVertex*>::Predicate
    {
      // ======================================================================
      /// clone method (mandatory)
      Decay* clone() const  override;
      /// the only one essential method
      bool operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCVertexDistance
     *  evaluator of distance from MC verted to 3D-point
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    class GAUDI_API MCVertexDistance
      : public LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
    public:
      // ======================================================================
      /** constructor from LoKi::Point3D
       *  @param point 3D-point
       */
      MCVertexDistance ( const LoKi::Point3D& point ) ;
      /** constructor from MCVertex
       *  @param point 3D-point
       */
      MCVertexDistance ( const LHCb::MCVertex*   point ) ;
      /// clone method (mandatory!)
      MCVertexDistance* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::Point3D m_point ;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCVFunAsMCFun
     *  simple adapter function which allows to use MC Veretx function as
     *  MC particle function
     *  Adapter makes use of MCParticle::originVertex function
     *  @see LHCb::MCParticle
     *  @see LHCb::MCVertex
     *  @see LoKi::Cuts::MCVXFUN
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    class GAUDI_API MCVFunAsMCFun
      : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function
    {
    public:
      // ======================================================================
      /// constructor from MC Vertex fuction
      MCVFunAsMCFun ( const LoKi::MCTypes::MCVFunc& func   ) ;
      /// clone method (mandatory!)
      MCVFunAsMCFun* clone() const  override;
      /// the only one essential method
      double    operator() ( const LHCb::MCParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    protected:
      // ======================================================================
      const LoKi::MCTypes::MCVFunc& func() const { return m_fun.func() ; }
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCVFun m_fun ;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCVFunction
     *  simple adapter function which allows to use MC Veretx function as
     *  MC particle function
     *  Adapter makes use of MCParticle::originVertex function
     *  @see LHCb::MCParticle
     *  @see LHCb::MCVertex
     *  @see LoKi::Cuts::MCVFUN
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    class GAUDI_API MCVFunction : public LoKi::MCVertices::MCVFunAsMCFun
    {
    public:
      // ======================================================================
      enum {
        Regular  =  0 ,
        Last     = -1 ,
        Origin   = -2 ,
        Primary  = -3
      } ;
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from MC Vertex fuction
       *  @param func vertex function tobe applied
       *  @param index vertex index :
       *         -1   - the last vertex  in the list
       *         -2   - the origin vertex
       *         -3   - the primary ertex
       */
      MCVFunction ( const LoKi::MCTypes::MCVFunc& func  ,
                    const int                     index ) ;
      /// constructor from MC Vertex fuction and vertex selection
      MCVFunction ( const LoKi::MCTypes::MCVFunc& func  ,
                    const LoKi::MCTypes::MCVCuts& cuts  ) ;
      /// constructor from MC Vertex fuction and vertex selection
      MCVFunction ( const LoKi::MCTypes::MCVCuts& cuts  ,
                    const LoKi::MCTypes::MCVFunc& func  ) ;
      /// clone method (mandatory!)
      MCVFunction* clone() const  override;
      /// the only one essential method
      double    operator() ( const LHCb::MCParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      bool                  m_case   ;
      int                   m_index  ;
      LoKi::MCTypes::MCVCut m_cut    ;
      // ======================================================================
    };
    // ========================================================================
    /** @class Key
     *  get the key of MC-vertex
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date   2011-02-14
     */
    struct GAUDI_API Key : LoKi::BasicFunctors<const LHCb::MCVertex*>::Function
    {
      // ======================================================================
      /// clone method (mandatory)
      Key* clone() const  override;
      /// the only one essential method
      double operator() ( const LHCb::MCVertex* v ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCVPFunAsMCFun
     *  simple adapter function which allows to use MC Veretx function as
     *  MC particle function
     *  Adapter makes use of MCParticle::primaryVertex function
     *  @see MCParticle
     *  @see MCVertex
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     */
    struct GAUDI_API MCVPFunAsMCFun : MCVFunAsMCFun
    {
      // ======================================================================
      /// constructor from MC Vertex fuction
      MCVPFunAsMCFun ( const LoKi::MCTypes::MCVFunc& func   ) ;
      /// clone method (mandatory!)
      MCVPFunAsMCFun* clone() const  override;
      /// the only one essential method
      double    operator() ( const LHCb::MCParticle* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    };
  } //                                        end of namespace LoKi::MCVertices
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCVERTICES_H
// ============================================================================
