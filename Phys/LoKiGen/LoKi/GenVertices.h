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
#ifndef LOKI_GENVERTICES_H
#define LOKI_GENVERTICES_H 1
// ============================================================================
// Include files
// ============================================================================
// HepMC
// ============================================================================
#include "HepMC/GenVertex.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Kinematics.h"
#include "LoKi/Constants.h"
#include "LoKi/GenTypes.h"
// ============================================================================
/** @file LoKi/GenVertices.h
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
 *  @date 2006-02-08
 *
 */
namespace LoKi
{
  // ==========================================================================
  /** @namespace LoKi::GenVertices GenVertices.h LoKi/GenVertices.h
   *
   *  Collection of LoKi functions and predicates to manipulate with
   *  generator (HepMC) information
   *
   *  @author Vanya  BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-08
   */
  namespace GenVertices
  {
    // ========================================================================
    /** @class BarCode
     *  the most primitive function - it return the "barcode"
     *  of HepMC::GenVertex object
     *
     *  @see LoKi::Cuts::GVBAR
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API BarCode : LoKi::GenTypes::GVFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      BarCode* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class PositionX
     *
     *  the most primitive function - it return the position
     *  of HepMC::GenVertex object
     *
     *  @see LoKi::Cuts::GVX
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API PositionX : LoKi::GenTypes::GVFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      PositionX* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class PositionY
     *
     *  the most primitive function - it return the position
     *  of HepMC::GenVertex object
     *
     *  @see LoKi::Cuts::GVY
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API PositionY : LoKi::GenTypes::GVFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      PositionY* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class PositionZ
     *
     *  the most primitive function - it return the position
     *  of HepMC::GenVertex object
     *
     *  @see LoKi::Cuts::GVZ
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API PositionZ : LoKi::GenTypes::GVFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      PositionZ* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class PositionT
     *
     *  the most primitive function - it return the time component of
     *  the position for HepMC::GenVertex object
     *
     *  @see LoKi::Cuts::GVT
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    struct GAUDI_API PositionT : LoKi::GenTypes::GVFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      PositionT* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Rho
     *
     *  the most primitive function - it return the rho-position (cylindrical)
     *  HepMC::GenVertex object
     *
     *  @see LoKi::Cuts::GVRHO
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2012-02-13
     */
    struct GAUDI_API Rho : LoKi::GenTypes::GVFunc
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual" constructor")
      Rho* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* p ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class CountIF
     *  simple class which counts how many particles
     *  satisfy the certain criteria
     *
     *  @see LoKi::Cuts::GVCOUNT
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    class GAUDI_API CountIF : public LoKi::GenTypes::GVFunc
    {
    public:
      // ======================================================================
      /** constructor from the criteria and "range"
       *  @param cut the criteria
       *  @param range search region
       *  @see HepMC::IteratorRange
       */
      CountIF
      ( const LoKi::Types::GCuts& cut   ,
        HepMC::IteratorRange      range ) ;
      /** constructor from the criteria and "range"
       *  @param range search region
       *  @param cut the criteria
       *  @see HepMC::IteratorRange
       */
      CountIF
      ( HepMC::IteratorRange      range ,
        const LoKi::Types::GCuts& cut   ) ;
      /// MANDATORY: clone method ("virtual contructor")
      CountIF* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* v ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::Types::GCut    m_cut   ;
      HepMC::IteratorRange m_range ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class SumIF
     *  simple class which accumulates the function values
     *  for particles from the given range which
     *  satisfy the certain criteria
     *
     *  @see LoKi::Cuts::GVSUM
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    class GAUDI_API SumIF : public LoKi::GenTypes::GVFunc
    {
    public:
      // ======================================================================
      /** constructor from the function and the "range"
       *  @param fun function to be accuulated
       *  @param range search region
       *  @see HepMC::IteratorRange
       */
      SumIF ( const LoKi::Types::GFunc& fun   ,
              HepMC::IteratorRange      range ) ;
      /** constructor from the function and the "range"
       *  @param fun function to be accuulated
       *  @param range search region
       *  @see HepMC::IteratorRange
       */
      SumIF ( HepMC::IteratorRange      range ,
              const LoKi::Types::GFunc& fun   ) ;
      /** constructor from the function, "range" and criteria
       *  @param fun function to be accuulated
       *  @param range search region
       *  @param cut the criteria
       *  @see HepMC::IteratorRange
       */
      SumIF ( const LoKi::Types::GFunc& fun   ,
              HepMC::IteratorRange      range ,
              const LoKi::Types::GCuts& cut   ) ;
      /** constructor from the function, "range" and criteria
       *  @param fun function to be accuulated
       *  @param cut the criteria
       *  @param range search region
       *  @see HepMC::IteratorRange
       */
      SumIF ( const LoKi::Types::GFunc& fun   ,
              const LoKi::Types::GCuts& cut   ,
              HepMC::IteratorRange      range ) ;
      /** constructor from the function, "range" and criteria
       *  @param cut the criteria
       *  @param fun function to be accuulated
       *  @param range search region
       *  @see HepMC::IteratorRange
       */
      SumIF ( const LoKi::Types::GCuts& cut   ,
              const LoKi::Types::GFunc& fun   ,
              HepMC::IteratorRange      range ) ;
      /// MANDATORY: clone method ("virtual contructor")
      SumIF* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenVertex* v ) const  override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::Types::GFun    m_fun   ;
      LoKi::Types::GCut    m_cut   ;
      HepMC::IteratorRange m_range ;
      // ======================================================================
    } ;
    // ========================================================================
  } //                                       end of namespace LoKi::GenVertices
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENVERTICES_H
