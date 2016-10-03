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
      virtual Parameter* clone() const ;
      /// the major method 
      virtual result_type operator()( /* argument */ ) const ;
      /// optional: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    protected:
      // ======================================================================
      void getParams () const ;
      // ======================================================================
    private: 
      // ======================================================================
      /// map-like property ? 
      mutable bool            m_map_d = false   ;  // map-like property? 
      mutable bool            m_map_f = false   ;  // map-like property? 
      mutable bool            m_map_i = false   ;  // map-like property? 
      /// scalar   property ?
      mutable bool            m_scalar_d = false ;  // scalar   property? 
      mutable bool            m_scalar_f = false ;  // scalar   property? 
      mutable bool            m_scalar_i = false ;  // scalar   property? 
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
