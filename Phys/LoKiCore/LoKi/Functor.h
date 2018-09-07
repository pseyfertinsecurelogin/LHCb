// ============================================================================
#ifndef LOKI_FUNCTOR_H
#define LOKI_FUNCTOR_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
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
#define LOKI_REQUIRES(...) std::enable_if_t<(__VA_ARGS__),bool> = true
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class Functor Functor.h LoKi/Functor.h
   *  The most generic LoKi-functor, needed for implementation
   *  of numerious brilliant ideas by Gerhard.
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
    virtual TYPE2 operator() ( argument    ) const = 0 ;
    /// the only one essential method ("function")
    virtual TYPE2 evaluate( argument  a ) const { return (*this)( a ) ; }
    /// the only one essential method ("function")
    virtual TYPE2 eval( argument  a ) const { return (*this)( a ) ; }
    /// clone method
    virtual  Functor* clone() const = 0 ;
    /// virtual destructor
    virtual ~Functor() = default;
    // ========================================================================
  protected:
    // ========================================================================
    /// protected default constructor
    Functor() = default;
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
    // public:
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    FunctorFromFunctor* clone() const override
    { return new FunctorFromFunctor ( *this ) ; }
    /// MANDATORY: the only one essential method
    TYPE2
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
    auto fun( typename functor::argument a ) const { return (*m_fun) ( a ) ; }
    // accessor to the function
    inline const functor& func () const { return *m_fun ; }
    // ========================================================================
  private:
    // ========================================================================
    /** the actual underlying function 
     *  @todo TODO: consider std::shared_ptr... 
     *  or small object optimization, or both...
     */
    std::unique_ptr<functor> m_fun ;           // the underlying functor
    // ========================================================================
  };
  // ==========================================================================
  /// specialiation for void-argument
  template <class TYPE2>
  class Functor<void,TYPE2> : public virtual LoKi::AuxFunBase
  {
  public:
    /// =======================================================================
    /// the result value
    typedef TYPE2                                               result_type   ;
    /// the basic argument type
    typedef void                                                argument_type ;
    /// type for the argument
    typedef void                                                argument      ;
    // ========================================================================
  public:
    // ========================================================================
    /// the only one essential method ("function")
    virtual TYPE2  operator () (  ) const = 0 ;
    /// the only one essential method ("function")
    virtual TYPE2  evaluate    (  ) const
    { return (*this) (  ) ; }
    /// the only one essential method ("function")
    virtual TYPE2  eval        (  ) const
    { return (*this) ( ) ; }
    /// clone method
    virtual  Functor* clone    ()                   const = 0 ;
    /// virtual destructor
    virtual ~Functor() = default;
    // ========================================================================
  protected:
    // ========================================================================
    /// protected default constructor
    Functor() = default;                       // protected default constructor
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
    TYPE2 operator()() const override
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
    decltype(auto) fun( ) const { return (*m_fun)( ) ; }
    // accessor to the function
    const functor& func () const { return *m_fun ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the underlaying function
    std::unique_ptr<const functor> m_fun ;           // the underlying functor
    // ========================================================================
  } ;
  // ========================================================================
  namespace details {

      template <typename T, typename U>
      using decays_to = std::conditional_t<std::is_void<U>::value,
                                           typename std::is_void<T>::type,
                                           typename std::is_convertible<std::decay_t<T>*, U*>::type >;

      // is Derived derived from Base<Args...>?
      template <template <typename ...> class Base, typename Derived>
      struct is_derived_from_template_helper {
          template <typename ... Args>
          static std::true_type  test(Base<Args...>*);
          static std::false_type test(void*);

          using type = decltype(test(std::declval<std::decay_t<Derived>*>()));
      };

      template <typename T>
      using is_functor = typename is_derived_from_template_helper<LoKi::Functor,T>::type;

      // If T derives from LoKi::Functor<TYPE,TYPE2> what is TYPE?
      // If T derives from LoKi::Functor<TYPE,TYPE2> what is TYPE2?
      template <typename T, LOKI_REQUIRES(is_functor<T>::value)>
      struct LF_ {
          using U = std::decay_t<T>;
          template <typename T1, typename T2> static T1 test1( LoKi::Functor<T1,T2>* );
          template <typename T1, typename T2> static T2 test2( LoKi::Functor<T1,T2>* );
          using type1 = decltype( test1(std::declval<U*>()) );
          using type2 = decltype( test2(std::declval<U*>()) );
      };
      template <typename... Fs> using type1_t = std::common_type_t<typename LF_<Fs>::type1...>;
      template <typename... Fs> using type2_t = std::common_type_t<typename LF_<Fs>::type2...>;

      template <typename F> using result_t   = type2_t<F>;
      template <typename F> using argument_t = typename boost::call_traits<std::add_const_t<type1_t<F>>>::param_type;

      template <typename F, typename TYPE1, typename TYPE2>
      using require_signature =
          std::enable_if_t<   decays_to<type1_t<F>, TYPE1>::value
                           && decays_to<type2_t<F>, TYPE2>::value >;


      template <typename Arg, typename Res> struct sig_helper {
          using type = Res(Arg);
      };
      template <typename Res> struct sig_helper<void,Res> {
          using type = Res();
      };
      template <typename Arg, typename Res>
      using sig_t = typename sig_helper<Arg,Res>::type;

      template <typename ... Fs>
      using signature_of_t = sig_t< type1_t<Fs...>, type2_t<Fs...> >;
  }

  namespace V2 {
  namespace details {

    template <typename Arg>
    using Param_t = typename boost::call_traits<std::add_const_t<Arg>>::param_type;


    template <typename Signature> struct sig_helper;
    template <typename Result, typename... Args> struct sig_helper<Result(Args...)> {
        using result_of_t = Result;
    };

    template <typename Signature>
    using result_of_t = typename sig_helper<Signature>::result_of_t;

  }
  }

  // ========================================================================

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
      : LoKi::AuxFunBase ( std::tie(value) )
      , m_value ( std::move(value) )
    {}
    // ========================================================================
  public:
    // ========================================================================
    /// assignment
    template <typename Arg, LOKI_REQUIRES(std::is_assignable<TYPE2,Arg>::value)>
    Constant& operator=( const Arg& arg )
    { m_value = arg; return *this ; }
    Constant& operator=( TYPE2&& arg )
    { m_value = std::move(arg); return *this ; }
    /// clone method (mandatory)
    Constant* clone() const override { return new Constant( *this ) ; }
    /// the only one essential method ("function")
    TYPE2 operator()
      ( typename LoKi::Functor<TYPE,TYPE2>::argument ) const override { return m_value ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return  Gaudi::Utils::toStream ( this->m_value , s ) ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the constant itself
    TYPE2 m_value ;                                      // the constant itself
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
      , m_value ( std::move(value) )
    {}
    // ========================================================================
  public:
    // ========================================================================
    /// assignement
    template <typename Arg, LOKI_REQUIRES(std::is_assignable<TYPE2,Arg>::value)>
    Constant& operator=( const Arg& arg )
    { m_value = arg; return *this ; }
    Constant& operator=( TYPE2&& arg )
    { m_value = std::move(arg); return *this ; }
    /// clone method (mandatory)
    Constant* clone() const override { return new Constant( *this ) ; }
    /// the only one essential method ("function")
    TYPE2 operator()() const override
    { return m_value ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return Gaudi::Utils::toStream ( this->m_value , s ) ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the constant itself
    TYPE2 m_value ;                                      // the constant itself
    // ========================================================================
  } ;

  namespace V2 {
  namespace details {
    template <typename Signature>  struct functor_helper;

    template <typename Result, typename Arg> struct functor_helper<Result(Arg)> {
        using Functor = LoKi::Functor<Arg,Result>;
        using FunctorFromFunctor = LoKi::FunctorFromFunctor<Arg,Result>;
        using Constant = LoKi::Constant<Arg,Result>;
    };
    template <typename Result> struct functor_helper<Result()> {
        using Functor = LoKi::Functor<void,Result>;
        using FunctorFromFunctor = LoKi::FunctorFromFunctor<void,Result>;
        using Constant = LoKi::Constant<void,Result>;
    };
  }
  template <typename Signature>
  using Functor = typename details::functor_helper<Signature>::Functor;

  template <typename Signature>
  using FunctorFromFunctor = typename details::functor_helper<Signature>::FunctorFromFunctor;

  template <typename Signature>
  using Constant = typename details::functor_helper<Signature>::Constant;
  }

  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_FUNCTOR_H
// ============================================================================
