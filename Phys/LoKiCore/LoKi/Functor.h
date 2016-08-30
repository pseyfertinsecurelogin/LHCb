// ============================================================================
#ifndef LOKI_FUNCTOR_H
#define LOKI_FUNCTOR_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <functional>
#include <memory>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/AuxFunBase.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/call_traits.hpp"
// ============================================================================
// ROOT
// ============================================================================
#include "RVersion.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class Functor Functor.h LoKi/Functor.h
   *  The most generic LoKi-functor, needed for implementation
   *  of numerious brilliant ideas by Gerhard "The Great".
   *
   *  This file is a part of LoKi project -
   *  ``C++ ToolKit  for Smart and Friendly Physics Analysis''
   *
   *  The package has been designed with the kind help from
   *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
   *  contributions and advices from G.Raven, J.van Tilburg,
   *  A.Golutvin, P.Koppenburg have been used in the design
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-10-31
   */
  template <class TYPE,class TYPE2>
  class Functor : public virtual LoKi::AuxFunBase
  {
  public:
    /// =======================================================================
    /// parameters: argument
    typedef TYPE  Type1 ;                               // parameters: argument
    /// parameters: return value
    typedef TYPE2 Type2 ;                           // parameters: return value
    /// =======================================================================
  public:
    // ========================================================================
    /// the type of the argument
    typedef TYPE                                                Type          ;
    // ========================================================================
  public:  // STD (fake) signature
    // ========================================================================
    /// STL: the result value
    typedef TYPE2                                               result_type   ;
    /// STL: the basic argument type
    typedef TYPE                                                argument_type ;
    // ========================================================================
  public:  // the actual signature
    // ========================================================================
    /// type for the argument
    typedef typename boost::call_traits<const TYPE>::param_type argument      ;
    // ========================================================================
  public:
    // ========================================================================
    /// the only one essential method ("function")
    virtual result_type operator () ( argument    ) const = 0 ;
    /// the only one essential method ("function")
    virtual result_type evaluate    ( argument  a ) const
    { return (*this)( a ) ; }
    /// the only one essential method ("function")
    virtual result_type eval        ( argument  a ) const
    { return (*this)( a ) ; }
    /// clone method
    virtual  Functor* clone    ()                   const = 0 ;
    /// virtual destructor
    virtual ~Functor() = default;
    // ========================================================================
  protected:
    // ========================================================================
    /// protected default constructor
    Functor() : AuxFunBase( std::tie() ) {}
    /// protected copy constructor
    Functor ( const Functor& ) = default;
    /// protected move constructor
    Functor ( Functor&& ) = default;
    /// the assignement operator is disabled
    Functor& operator=( const Functor& ) = delete;        // the assignement is disabled
    Functor& operator=( Functor&& ) = delete;        // the assignement is disabled
    // ========================================================================
  };
  // ==========================================================================
  /** @class FunctorFromFunctor
   *  the most trivial implementation of LoKi::Functor interface,
   *  It just delegates all actions to the underlying functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-10-30
   */
  template <class TYPE,class TYPE2>
  class FunctorFromFunctor final : public LoKi::Functor<TYPE,TYPE2>
  {
  public:
    // ========================================================================
    /// the underlying type of functor
    typedef LoKi::Functor<TYPE,TYPE2>   functor ;               // functor type
    // ========================================================================
  protected:
    // ========================================================================
    /// own type
    typedef FunctorFromFunctor<TYPE,TYPE2> Self ;               // the own type
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    FunctorFromFunctor ( const Functor<TYPE,TYPE2>& right )
      : LoKi::AuxFunBase( right )
      , LoKi::Functor<TYPE,TYPE2> (right)
      , m_fun ( right.clone() )
    {}
    /// copy constructor (deep copy)
    FunctorFromFunctor ( const FunctorFromFunctor& right )
      : LoKi::AuxFunBase( right )
      , LoKi::Functor<TYPE,TYPE2> ( right )
      , m_fun ( right.m_fun->clone()  )
    {}
    /// move constructor (avoid cloning)
    FunctorFromFunctor ( FunctorFromFunctor&& right ) = default;
    // ========================================================================
    /// the assignement operator is enabled
    FunctorFromFunctor& operator=( FunctorFromFunctor rhs ) noexcept
    {
      swap(rhs,*this);
      return *this;
    }
    /// move  assignement operator is enabled
    FunctorFromFunctor& operator= ( FunctorFromFunctor&& ) = default;

    // ========================================================================
    friend void swap(FunctorFromFunctor& lhs, FunctorFromFunctor& rhs) noexcept
    {
        using std::swap;
        swap(static_cast<AuxFunBase&>(lhs),static_cast<AuxFunBase&>(rhs));
        swap(lhs.m_fun,rhs.m_fun);
    }
    // ========================================================================
  public:
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    FunctorFromFunctor* clone() const override
    { return new FunctorFromFunctor ( *this ) ; }
    /// MANDATORY: the only one essential method
    typename functor::result_type
    operator()( typename functor::argument a ) const override
    { return fun ( a ) ; }
    /// OPTIONAL: the basic printout method, delegate to the underlying object
    std::ostream& fillStream( std::ostream& s ) const override
    { return  m_fun->fillStream( s ) ; };
    /// OPTIONAL: unique function ID, delegate to the underlying objects
    std::size_t   id () const override { return m_fun->id() ; }
    /// OPTIONAL: delegate the object type
    std::string   objType () const override { return m_fun -> objType() ; }
    /// C++ printout: delegate
    std::string   toCpp   () const override { return m_fun -> toCpp  () ; }
    // ========================================================================
    /// evaluate the function
    inline typename functor::result_type fun
    ( typename functor::argument a ) const { return (*m_fun) ( a ) ; }
    // accessor to the function
    inline const functor& func () const { return *m_fun ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the actual underlaying function // TODO: consider std::shared_ptr... or small object optimization, or both...
    std::unique_ptr<const functor> m_fun ;           // the underlaying functor
    // ========================================================================
  };
  // ==========================================================================
  /** @class Constant
   *  The helper concrete implementation of the simplest
   *  function ("constant")
   *  @see LoKi::Functor
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-15
   */
  template <class TYPE,class TYPE2>
  class Constant final : public LoKi::Functor<TYPE,TYPE2>
  {
  public:
    // ========================================================================
    /// argument type
    typedef typename boost::call_traits<TYPE2>::param_type T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    Constant ( T2 value )
      : LoKi::AuxFunBase ( std::tie( value ) )
      , m_value ( value )
    {}
    // ========================================================================
  public:
    // ========================================================================
    /// assignement
    Constant& operator=( T2 right )
    { m_value = right         ; return *this ; }
    /// clone method (mandatory)
    Constant* clone   () const override { return new Constant( *this ) ; }
    /// the only one essential method ("function")
    typename LoKi::Functor<TYPE,TYPE2>::result_type operator()
      ( typename LoKi::Functor<TYPE,TYPE2>::argument ) const override { return m_value ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override ;
    // ========================================================================
  private:
    // ========================================================================
    /// the constant itself
    TYPE2 m_value ;                                      // the constant itself
    // ========================================================================
  } ;
  // ==========================================================================
  /// specialiation for void-argument
  template <class TYPE2>
  class Functor<void,TYPE2> : public virtual LoKi::AuxFunBase
  {
  public:
    /// =======================================================================
    /// parameters: argument
    typedef void  Type1 ;                               // parameters: argument
    /// parameters: return value
    typedef TYPE2 Type2 ;                           // parameters: return value
    /// =======================================================================
  private:
    // ========================================================================
    /// STD signature (fake base)
    typedef typename std::unary_function<void,TYPE2>                   Base_1 ;
    // ========================================================================
  public:
    // ========================================================================
    /// the type of the argument
    typedef void                                                Type          ;
    /// the result value
    typedef typename Base_1::result_type                        result_type   ;
    /// the basic argument type
    typedef void                                                argument_type ;
    /// type for the argument
    typedef void                                                argument      ;
    // ========================================================================
  public:
    // ========================================================================
    /// the only one essential method ("function")
    virtual result_type operator () ( /* argument */ ) const = 0 ;
    /// the only one essential method ("function")
    virtual result_type evaluate    ( /* argument */ ) const
    { return (*this) ( /* */ ) ; }
    /// the only one essential method ("function")
    virtual result_type eval        ( /* argument */ ) const
    { return (*this) ( /* */ ) ; }
    /// clone method
    virtual  Functor* clone    ()                   const = 0 ;
    /// virtual destructor
    virtual ~Functor() = default;
    // ========================================================================
  protected:
    // ========================================================================
    /// protected default constructor
    Functor() : AuxFunBase( std::tie() ) {}    // protected default constructor
    /// protected copy constructor
    Functor ( const Functor& fun ) = default;                 // protected copy
    /// protected move constructor
    Functor ( Functor&& fun ) = default;                 // protected copy
    // ========================================================================
    // the assignement operator is disabled
    Functor& operator=( const Functor& ) = delete;             // assignement is private
    Functor& operator=( Functor&& ) = delete;             // assignement is private
    // ========================================================================
  };
  // ==========================================================================
  /// the specialization for "void"-argument
  template <class TYPE2>
  class FunctorFromFunctor<void,TYPE2> final : public LoKi::Functor<void,TYPE2>
  {
  public:
    // ========================================================================
    /// the underlying type of functor
    typedef LoKi::Functor<void,TYPE2>   functor ;     // the underlying functor
    // ========================================================================
  protected:
    // ========================================================================
    /// own type
    typedef FunctorFromFunctor<void,TYPE2> Self ;               // the own type
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    FunctorFromFunctor ( const Functor<void,TYPE2>& right )
      : LoKi::AuxFunBase( right )
      , LoKi::Functor<void,TYPE2> ( right )
      , m_fun ( right.clone() )
    {}
    /// copy constructor (deep copy)
    FunctorFromFunctor ( const FunctorFromFunctor& right )
      : LoKi::AuxFunBase          ( right )
      , LoKi::Functor<void,TYPE2> ( right )
      , m_fun ( right.m_fun->clone()  )
    {}
    /// move constructor (avoid cloning)
    FunctorFromFunctor ( FunctorFromFunctor&& right ) = default;
    // ========================================================================
    /// the assignement operator is enabled
    FunctorFromFunctor& operator= ( FunctorFromFunctor rhs ) noexcept
    {
      swap(rhs,*this);
      return *this;
    }
    /// move  assignement operator is enabled
    FunctorFromFunctor& operator= ( FunctorFromFunctor&& right ) = default;

    // ========================================================================
    friend void swap(FunctorFromFunctor& lhs, FunctorFromFunctor& rhs) noexcept
    {
        using std::swap;
        swap(static_cast<AuxFunBase&>(lhs),static_cast<AuxFunBase&>(rhs));
        swap(lhs.m_fun,rhs.m_fun);
    }
    // ========================================================================
  public:
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    FunctorFromFunctor* clone() const override
    { return new FunctorFromFunctor ( *this ) ; }
    /// MANDATORY: the only one essential method
    typename functor::result_type operator()() const override
    { return fun ( ) ; }
    /// OPTIONAL: the basic printout method, delegate to the underlying object
    std::ostream& fillStream( std::ostream& s ) const override
    { return  m_fun->fillStream( s ) ; };
    /// OPTIONAL: unique function ID, delegate to the underlying objects
    std::size_t   id () const override { return m_fun->id() ; }
    /// OPTIONAL: delegate the object type
    std::string   objType () const override { return m_fun -> objType() ; }
    /// C++ printout: delegate
    std::string   toCpp   () const override { return m_fun -> toCpp  () ; }
    // ========================================================================
  public:
    // ========================================================================
    /// evaluate the function
    inline typename functor::result_type fun( ) const { return (*m_fun)( ) ; }
    // accessor to the function
    inline const functor& func () const { return *m_fun ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the underlaying function
    std::unique_ptr<const functor> m_fun ;           // the underlaying functor
    // ========================================================================
  } ;
  // ==========================================================================
  /// specialization for "void"
  template <class TYPE2>
  class Constant<void,TYPE2> final : public LoKi::Functor<void,TYPE2>
  {
  public:
    // ========================================================================
    /// argument type
    typedef typename boost::call_traits<TYPE2>::param_type T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    Constant ( T2 value )
      : LoKi::AuxFunBase ( std::tie( value ) )
      , m_value ( value )
    {}
    // ========================================================================
  public:
    // ========================================================================
    /// assignement
    Constant& operator=( const Constant& right ) = default;
    /// assignement
    Constant& operator=( T2 right )
    { m_value = right         ; return *this ; }
    /// clone method (mandatory)
    Constant* clone   () const override { return new Constant( *this ) ; }
    /// the only one essential method ("function")
    typename LoKi::Functor<void,TYPE2>::result_type operator()() const override
    { return m_value ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override;
    // ========================================================================
  private:
    // ========================================================================
    /// the constant itself
    TYPE2 m_value ;                                      // the constant itself
    // ========================================================================
  } ;
  // ==========================================================================
  // the generic printout method
  // ==========================================================================
  template <class TYPE,class TYPE2>
  inline std::ostream&
  Constant<TYPE,TYPE2>::fillStream( std::ostream& s ) const
  { return Gaudi::Utils::toStream ( this->m_value , s ) ; }
  // ==========================================================================
  // the basic printout method
  // ==========================================================================
  template <class TYPE2>
  inline std::ostream&
  Constant<void,TYPE2>::fillStream( std::ostream& s ) const
  { return  Gaudi::Utils::toStream ( this->m_value , s ) ; }
  // ==========================================================================

  namespace details {

      template <typename T, typename U>
      using decays_to = std::is_convertible<typename std::decay<T>::type*, U*>;

      // is Derived derived from Base<Args...>?
      template <template <typename ...> class Base, typename Derived>
      struct is_derived_from_template_helper {
          template <typename ... Args>
          static std::true_type  test(Base<Args...>*);
          static std::false_type test(void*);

          using type = decltype(test(std::declval<typename std::decay<Derived>::type*>()));
      };

      template <typename T>
      using is_functor = typename is_derived_from_template_helper<LoKi::Functor,T>::type;

      // If T derives from LoKi::Functor<TYPE,TYPE2> what is TYPE?
      // If T derives from LoKi::Functor<TYPE,TYPE2> what is TYPE2?
      template <typename T,
                typename = typename std::enable_if<is_functor<T>::value>::type>
      struct LF {
          using U = typename std::decay<T>::type;
          template <typename T1, typename T2> static T1 test1( LoKi::Functor<T1,T2>* );
          template <typename T1, typename T2> static T2 test2( LoKi::Functor<T1,T2>* );
          using type1 = decltype( test1(std::declval<U*>()) );
          using type2 = decltype( test2(std::declval<U*>()) );
      };

      // ( F1 = LoKi::Functor<TYPE,TYPE2> , F2 = LoKi::Functor<TYPE,TYPE2> ) -> TYPE
      // ( F1 = LoKi::Functor<TYPE,TYPE2> , F2 = LoKi::Functor<TYPE,TYPE2> ) -> TYPE2
      template <typename F1, typename F2,
                typename = typename std::enable_if<is_functor<F1>::value
                                               &&  is_functor<F2>::value
                                               // the following is not strictly neccessary anymore...
                                               &&  std::is_same< typename LF<F1>::type1, typename LF<F2>::type1 >::value
                                               &&  std::is_same< typename LF<F1>::type2, typename LF<F2>::type2 >::value
                                               >::type>
      struct LF2 {
          using type1 = typename std::common_type< typename LF<F1>::type1,
                                                   typename LF<F2>::type1 >::type;
          using type2 = typename std::common_type< typename LF<F1>::type2,
                                                   typename LF<F2>::type2 >::type;
      };
  }


  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_FUNCTOR_H
// ============================================================================
