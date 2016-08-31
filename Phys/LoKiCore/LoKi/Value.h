// ============================================================================
#ifndef LOKI_VALUE_H 
#define LOKI_VALUE_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKiCore 
// ============================================================================
#include "LoKi/Functor.h"
// ============================================================================
/** @file  LoKi/Value.h
 *  helper class to implement the adaptor 
 *  for void->TYPE2 to TYPE->TYPE2 functor
 *
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 *  @date   2014-02-01
 *
 *  This file is a part of LoKi project - 
 *  ``C++ ToolKit  for Smart and Friendly Physics Analysis''
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design 
 *  
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class Value LoKi/Value.h
   *  Simple adaptor from void->TYPE2 to TYPE->TYPE2 functor  
   *  
   *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
   *  @date   2014-02-01
   */
  template <class TYPE, class TYPE2> 
  class Value : public LoKi::Functor<TYPE,TYPE2>
  {
  public: 
    // ========================================================================
    /** Standard constructor from "void"-functor
     *  @param fun  void-functor 
     */
    explicit Value ( const LoKi::Functor<void,TYPE2>& fun ) 
      : LoKi::AuxFunBase         ( std::tie( fun ) )  
      , m_void ( fun )
    {}
    /// clone method (mandatory) 
    virtual Value* clone   () const { return new Value( *this ) ; }
    /// the only one essential method ("function")      
    virtual typename LoKi::Functor<TYPE,TYPE2>::result_type operator() 
      ( typename LoKi::Functor<TYPE,TYPE2>::argument /* a */ ) const 
    { return m_void.fun() ; } // bypass one virtual call 
    /// the basic printout method  
    virtual std::ostream& fillStream( std::ostream& s ) const 
    { return m_void.fillStream ( s ) ; }
    // ========================================================================   
  private:
    // ========================================================================
    /// the actual storage of void-functor 
    LoKi::FunctorFromFunctor<void,TYPE2>  m_void ;
    // ========================================================================   
  };
  // ==========================================================================
  /// specialization for void, essentially bypass all actions  
  template <class TYPE2> 
  class Value<void,TYPE2> : public LoKi::Functor<void,TYPE2>
  {
  public: 
    // ========================================================================
    /** Standard constructor from "void"-functor
     *  @param fun  void-functor 
     */
    Value ( const LoKi::Functor<void,TYPE2>& fun ) 
      : LoKi::AuxFunBase         ( std::tie ( fun ) )  
      , m_void ( fun )
    {}
    /// clone method (mandatory) 
    virtual Value* clone   () const { return new Value( *this ) ; }
    /// the only one essential method ("function")      
    virtual typename LoKi::Functor<void,TYPE2>::result_type operator()
    ( /* typename LoKi::Functor<void,TYPE2>::argument a */ ) const 
    { return m_void.fun () ; } // bypass one virtual call 
    /// the basic printout method  
    virtual std::ostream& fillStream( std::ostream& s ) const 
    { return m_void.fillStream ( s ) ; }
    // ========================================================================   
  private:
    // ========================================================================
    /// the actual storage of void-functor 
    LoKi::FunctorFromFunctor<void,TYPE2>  m_void ;
    // ========================================================================   
  };
  // ==========================================================================
} //                                                      end of namespace LoKi 
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_VALUE_H
// ============================================================================
