// ============================================================================
#ifndef LOKI_BIFUNCTIONS_H
#define LOKI_BIFUNCTIONS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <functional>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/Holder.h"
// ============================================================================
/** @file LoKi/BiFunctions.h
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
 *  @date 2012-01-17
 */
namespace LoKi
{
  // ==========================================================================
  /** mimic 2-argument function
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-08
   */
  template <class TYPE1,class TYPE2,class TYPE3>
  class Functor<LoKi::Holder<TYPE1,TYPE2>, TYPE3>
    : virtual public LoKi::AuxFunBase
  {
  public:
    // ========================================================================
    /// parameters: argument
    typedef LoKi::Holder<TYPE1,TYPE2>  Type1 ;          // parameters: argument
    /// parameters: return value
    typedef TYPE3                      Type2 ;      // parameters: return value
    // ========================================================================
    /// the type of the argument
    typedef LoKi::Holder<TYPE1,TYPE2>                    Type                 ;
    // ========================================================================
    //     // STD (fake) signature
    // ========================================================================
    /// STL: the result value
    typedef TYPE3                                        result_type          ;
    // ========================================================================
    //     // the actual signature
    // ========================================================================
    /// the actual type for the argument
    typedef typename boost::call_traits<Type>::param_type     argument        ;
    /// the actual type for the first argument
    typedef typename boost::call_traits<TYPE1>::param_type    first_argument  ;
    /// teh actual type for the second argument
    typedef typename boost::call_traits<TYPE2>::param_type    second_argument ;
    // ========================================================================
  public:
    // ========================================================================
    /// the native interface with 1 argument:
    virtual result_type  operator () ( argument a ) const = 0 ;
    /// the only one essential method ("function")
    virtual result_type  evaluate    ( argument a ) const
    { return (*this)( a ) ; }
    /// the only one essential method ("function")
    virtual result_type  eval        ( argument a ) const
    { return (*this)( a ) ; }
    /// the only one essential method ("function")
    virtual result_type operator()( first_argument  a1, second_argument a2 ) const
    { return (*this)( Type ( a1 , a2 ) ) ; }
    /// the only one essential method ("function")
    virtual result_type evaluate( first_argument  a1, second_argument a2 ) const
    { return (*this)( a1 , a2 ) ; }
    /// the only one essential method ("function")
    virtual result_type eval( first_argument  a1 , second_argument a2 ) const
    { return (*this)( a1 , a2 ) ; }
    /// MANDATORY: clone method
    virtual  Functor* clone()              const = 0 ;
    /// virtual destructor
    virtual ~Functor() = default;
    // ========================================================================
  public:
    // ========================================================================
    /// protected default constructor
    Functor () = default;
    /// protected copy constructor
    Functor ( const Functor& fun ) = default;
    // ========================================================================
    /// assignement is disallowed
    Functor& operator=( const Functor& ) = delete;
    // ========================================================================
  } ;
  // ==========================================================================
  /// Specialization for 2-argument function
  template <class TYPE1,class TYPE2, class TYPE3>
  class FunctorFromFunctor<LoKi::Holder<TYPE1,TYPE2>,TYPE3>
    : public LoKi::Functor<LoKi::Holder<TYPE1,TYPE2>,TYPE3>
  {
  public:
    // ========================================================================
    /// the underlying type of functor
    typedef LoKi::Functor<LoKi::Holder<TYPE1,TYPE2>,TYPE3>            functor ;
    typedef LoKi::Functor<LoKi::Holder<TYPE1,TYPE2>,TYPE3>            Base    ;
    // ========================================================================
  protected:
    // ========================================================================
    /// own type
    typedef FunctorFromFunctor<LoKi::Holder<TYPE1,TYPE2>,TYPE3>          Self ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    FunctorFromFunctor ( const functor& right )
      : m_fun ( right.clone() )
    {}
    /// copy constructor (deep copy)
    FunctorFromFunctor ( const Self& right )
      : LoKi::AuxFunBase ( right )
      , Base             ( right )
      , m_fun( typeid( Self ) == typeid( right ) ? right.m_fun->clone ()
                                                 : right.clone() )
    {}
    // ========================================================================
  public:
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    virtual  FunctorFromFunctor* clone() const
    { return new FunctorFromFunctor ( *this ) ; }
    /// MANDATORY: the only one essential method
    virtual typename functor::result_type operator()
      ( typename functor::argument a ) const { return fun ( a ) ; }
    /// the only one essential method ("function")
    virtual typename functor::result_type operator ()
      ( typename functor::first_argument  a ,
        typename functor::second_argument b ) const { return fun ( a , b ) ; }
    /// OPTIONAL: the basic printout method, delegate to the underlying object
    virtual std::ostream& fillStream( std::ostream& s ) const
    { return  m_fun->fillStream( s ) ; };
    /// OPTIONAL: unique function ID, delegate to the underlying objects
    virtual std::size_t   id () const { return m_fun->id() ; }
    /// OPTIONAL: delegate the object type
    virtual std::string   objType () const { return m_fun -> objType() ; }
    virtual std::string   toCpp   () const { return m_fun -> toCpp  () ; }
    // ========================================================================
  public:
    // ========================================================================
    /// the assignement operator is enabled
    FunctorFromFunctor& operator= ( const FunctorFromFunctor& right )
    {
      if ( this == &right ) { return *this ; }                        // RETURN
      m_fun.reset( ( typeid( Self ) != typeid( right ) ? right.clone()
                                                       : right.m_fun->clone() ) );
      return *this ;                                                  // RETURN
    }
    /// the assignement operator is enabled
    FunctorFromFunctor& operator= ( const Base& right )
    {
      if ( this == &right ) { return *this ; }                        // RETURN
      m_fun.reset( right.clone() );                                   // CLONE!
      return *this ;                                                  // RETURN
    }
    // ========================================================================
  public:
    // ========================================================================
    /// evaluate unary  function
    inline typename functor::result_type fun
    ( typename functor::argument a ) const { return (*m_fun) ( a ) ; }
    /// evaluate binary function
    inline typename functor::result_type fun
    ( typename functor::first_argument  a ,
      typename functor::second_argument b ) const
    { return (*m_fun) ( a , b ) ; }
    /// accessor to the function itself
    inline const functor& func () const { return *m_fun ; }
    // ========================================================================
    /// default constructor is disallowed
    FunctorFromFunctor() = delete;        // the default constructor is private
    // ========================================================================
  private:
    // ========================================================================
    /// the underlaying function
    std::unique_ptr<const functor> m_fun ;           // the underlaying functor
    // ========================================================================
  } ;
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_BIFUNCTIONS_H
// ============================================================================
