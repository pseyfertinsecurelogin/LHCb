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
#ifndef LOKI_PARAMFUNCTORS_H
#define LOKI_PARAMFUNCTORS_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Property.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/AuxFunBase.h"
#include "LoKi/Param.h"
#include "LoKi/Functors.h"
// ============================================================================
/* @file LoKi/ParamFunctors.h
 *
 * This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 * The package has been designed with the kind help from
 * Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 * contributions and advices from G.Raven, J.van Tilburg,
 * A.Golutvin, P.Koppenburg have been used in the design.
 *
 * @date 2014-02-02
 * @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *
 */
namespace LoKi
{
  // ==========================================================================
  namespace Parameters
  {
    // ========================================================================
    /** @class ParamBase LoKi/ParamFunctors.h
     *
     *
     *  @author Vanya Belyaev
     *  @date   2014-02-02
     */
    class GAUDI_API ParamBase :  public virtual LoKi::AuxFunBase
    {
    public:
      // ======================================================================
      /** Standard constructor from property
       *  @param property   the property
       */
      ParamBase ( const Property&    property ) ;
      /** constructor from Param-object
       *  @param property the property description
       */
      ParamBase ( const LoKi::Param& property ) ;
      /// virtual destructor
      virtual ~ParamBase () = default; // virtual destructor
      // ======================================================================
    protected:
      // ======================================================================
      /// get the property
      const Property*    property () const { return  m_property ; }
      /// get the parameters
      const LoKi::Param& param    () const { return  m_param    ; }
      // ======================================================================
    protected:
      // ======================================================================
      void getProp () const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the property as parameter
      LoKi::Param     m_param    ;  // the property as parameter
      /// the property
      mutable const Property* m_property ;  // the property itself
      // ======================================================================
    } ;
    // ========================================================================
    class GAUDI_API Parameter
      : public    LoKi::Functor<void,double>
      , protected ParamBase
    {
    public:
      // ======================================================================
      /// constructor from parameter
      explicit Parameter  ( const LoKi::Param& param ) ;
      /// constructor from parameter
      explicit Parameter  ( const std::string& param ) ;
      /// clone-method: "virtual constructor"
      Parameter* clone() const override;
      /// the major method
      double operator()( ) const override;
      /// optional: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    protected:
      // ======================================================================
      void getParams () const ;
      // ======================================================================
    private:
      // ======================================================================
      /// map-like property ?
      enum class prop_t { unknown, map_d, map_f, map_i, scalar_d, scalar_f, scalar_i };
      mutable prop_t          m_prop = prop_t::unknown;
      // ======================================================================
    };
    // ========================================================================
  } //                                    The end of namespace LoKi::Parameters
  // ==========================================================================
} //                                                  The end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_PARAMFUNCTORS_H
// ============================================================================
