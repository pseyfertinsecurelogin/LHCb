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
    Functor ( const Functor& fun ) = default;
    /// protected move constructor
    Functor ( Functor&& fun ) = default;
    // ========================================================================
  private:
    // ========================================================================
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
    // ========================================================================
    /// move constructor (avoid cloning)
    FunctorFromFunctor ( FunctorFromFunctor&& right ) = default;
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
  public:
    // ========================================================================
    /// the assignement operator is enabled
    FunctorFromFunctor& operator=( const FunctorFromFunctor& right ) 
    {
      AuxFunBase::operator=(right);
      m_fun.reset( right.m_fun->clone() );
      return *this;
    }
    /// move  assignement operator is enabled
    FunctorFromFunctor& operator= ( FunctorFromFunctor&& right ) = default;
    /// the assignement operator is enabled
    FunctorFromFunctor& operator= ( const Functor<TYPE,TYPE2>& right )
    {
      if ( this != &right ) {
          AuxFunBase::operator=(right);
          m_fun.reset( right.clone() );
      }
      return *this ;                                                  // RETURN
    }
    // ========================================================================
  public:
    // ========================================================================
    /// swap two functors //TODO: what about AuxFunBase contents?
    void swap ( FunctorFromFunctor& right ) { std::swap ( m_fun , right.m_fun ) ; }
    // ========================================================================
  public:
    // ========================================================================
    /// evaluate the function
    inline typename functor::result_type fun
    ( typename functor::argument a ) const { return (*m_fun) ( a ) ; }
    // accessor to the function
    inline const functor& func () const { return *m_fun ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the default constructor is disabled
    FunctorFromFunctor();                // the default constructor is disabled
    // ========================================================================
  private:
    // ========================================================================
    /// the actual underlaying function // TODO: consider std::shared_ptr...
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
    /// copy constructor
    Constant ( const Constant& right ) = default;
    // ========================================================================
  public:
    // ========================================================================
    /// assignement
    Constant& operator=( const Constant& right )
    { m_value = right.m_value ; return *this ; }
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
    /// no default constructor
    Constant() ;                                      // no default constructor
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
    // ========================================================================
  private:
    // ========================================================================
    // the assignement operator is disabled
    Functor& operator=( const Functor& );             // assignement is private
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
    // ========================================================================
    /// move constructor (avoid cloning)
    FunctorFromFunctor ( FunctorFromFunctor&& right ) = default;
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
    /// the assignement operator is enabled
    FunctorFromFunctor& operator= ( const FunctorFromFunctor& right )
    {
      if ( this == &right ) { return *this ; }                        // RETURN
      functor::operator=( right );
      m_fun.reset( right.m_fun -> clone() ) ;      // CLONE
      return *this ;                                                  // RETURN
    }
    /// move  assignement operator is enabled
    FunctorFromFunctor& operator= ( FunctorFromFunctor&& right ) = default;

    /// the assignement operator is enabled
    FunctorFromFunctor& operator= ( const Functor<void,TYPE2>& right )
    {
      if ( this == &right ) { return *this ; }                        // RETURN
      m_fun.reset( right.clone() ) ;               // CLONE!
      return *this ;                                                  // RETURN
    }
    // ========================================================================
  public:
    // ========================================================================
    /// swap two functors
    void swap ( FunctorFromFunctor& right ) { m_fun.swap( right.m_fun ) ; }
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
    /// default constructor is private (declaration needed for dict)
    FunctorFromFunctor();                 // the default constructor is private
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
    /// copy constructor
    Constant ( const Constant& right ) = default;
    /// destructor
#if (ROOT_VERSION_CODE >= ROOT_VERSION(5, 99, 0)) && defined(ROOT_5721_WORKAROUND)
    // workaround for https://sft.its.cern.ch/jira/browse/ROOT-5721
    virtual ~Constant() throw() = default;
#else
    virtual ~Constant() = default;
#endif
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
    /// default constructor is private
    Constant();                                       // no default constructor
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
  /// swap two functors
  template <class TYPE1,class TYPE2>
  inline void swap
  ( LoKi::FunctorFromFunctor<TYPE1,TYPE2>& a ,
    LoKi::FunctorFromFunctor<TYPE1,TYPE2>& b ) { a.swap ( b ) ; }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_FUNCTOR_H
// ============================================================================
