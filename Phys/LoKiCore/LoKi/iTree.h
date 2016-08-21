// $Id$
// ============================================================================
#ifndef LOKI_ITREE_H
#define LOKI_ITREE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <functional>
#include <string>
#include <vector>
#include <iosfwd>
#include <typeinfo>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/call_traits.hpp"
// ============================================================================
namespace LHCb { class IParticlePropertySvc ; }
// ============================================================================
namespace Decays
{
  // ==========================================================================
  /** @class iTree
   *  The abstract class which represents the sub-tree of the decay tree
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date   2008-04-12
   */
  class iTree
  {
  public:
    // ======================================================================
    /// check the validity of the tree
    virtual bool valid () const = 0 ;
    // ======================================================================
    /** validate the decay tree
     *  @param svc pointer to Particle Property Service
     *  @return StatusCode
     */
    virtual StatusCode validate
    ( const LHCb::IParticlePropertySvc* svc ) const = 0 ;
    // ======================================================================
    /** has marked elements in the tree ?
     *  method to check if some elements of the decay tree are marked
     *  @code
     *
     *  typedef iTree_<XXX> iTree ;
     *
     *  const iTree& tree = ... ;
     *  if ( tree.marked() ) { ... } /
     *  @endcode
     *  It is especially suited for usage with "collect"
     *
     *  @code
     *
     *  typedef iTree_<XXX> iTree ;
     *
     *  Collection collection  = ... ;
     *
     *  XXX B= ... ;
     *
     *  const iTree& tree = ... ;
     *
     *  if ( tree ( B ) && tree.marked() )
     *   {
     *     tree.collect ( collection ) ;
     *     tree.reset() ;
     *   }
     *
     *  @endcode
     */
    virtual bool marked() const = 0 ;
    // ========================================================================
    /// reset the collection cache
    virtual void reset() const = 0 ;
    // ======================================================================
    /// clone method ("virtual constructor")
    virtual  iTree* clone() const = 0 ;
    // ======================================================================
    /** printout of the stream
     *  @param s the reference to the output stream
     *  @return the reference to the output stream
     */
    virtual std::ostream& fillStream ( std::ostream& s ) const = 0 ;
    /// conversion to the string
    virtual std::string toString() const ;
    /// invalid tree?
    virtual bool operator!() const { return ! ( this -> valid() ) ; }
    // ======================================================================
    /// virtual destructor
    virtual ~iTree () = default;
    // ======================================================================
  } ;
  // ========================================================================
  /** @class iTree_ Decays/Tree.h
   *  The abstract class which represents the sub-tree of the decay tree
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date   2008-04-12
   */
  template <class PARTICLE>
  class iTree_ : public iTree
  {
  public:
    // ======================================================================
    /// the actual type of the argument
    typedef typename boost::call_traits<PARTICLE>::param_type argument   ;
    typedef std::vector<PARTICLE>                             Collection ;
    // the actual type of particle
    typedef PARTICLE                                          particle   ;
    // ======================================================================
  public:
    // ======================================================================
    /** the basic operation: comparison of the sub-tree with the
     *  actual decay
     *  @param   p the particle to be compared
     *  @return  true if the actual decay tree of the particle is
     *           described by the tree objects
     */
    virtual bool operator()  ( argument p ) const = 0 ;
    // ======================================================================
    /// clone method ("virtual constructor")
    virtual  iTree_* clone() const  = 0 ;
    // ======================================================================
    /** collect all marked elements of the decay tree
     *
     *  @code
     *
     *  typedef iTree_<XXX> iTree ;
     *
     *  Collection collection  = ... ;
     *
     *  XXX B= ... ;
     *
     *  const iTree& tree = ... ;
     *
     *  if ( tree ( B ) && tree.marked() )
     *   {
     *     tree.collect ( collection ) ;
     *   }
     *
     *  @endcode
     */
    virtual size_t collect ( Collection& ) const = 0 ;
    // ======================================================================
    // ======================================================================
  } ;
  // ========================================================================
  /** @class Tree
   *  Simple holder of the decay tree
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl.
   *  @date 2008-04-13
   */
  template <class PARTICLE>
  class Tree_ final : public Decays::iTree_<PARTICLE>
  {
  public:
    // ======================================================================
    /// constructor from the tree
    Tree_ ( const Decays::iTree_<PARTICLE>& tree ) ;
    // ======================================================================
    /// copy constructor
    Tree_ ( const Tree_&  tree ) ;
    /// move constructor
    Tree_ (       Tree_&& tree ) = default;
    /// MANDATORY: clone method ("virtual constructor")
    Tree_* clone () const override { return new Tree_(*this) ; }
    /// MANDATORY: the only one essential method
    bool operator()
    ( typename  Decays::iTree_<PARTICLE>:: argument p ) const override
    { return tree ( p ) ; }
    /// MANDATORY: the specific printout
    std::ostream& fillStream( std::ostream& s ) const override
    { return m_tree -> fillStream ( s ) ; }
    /// MANDATORY: check the validity of the node
    bool valid() const override { return m_tree->valid() ; }
    /// MANDATORY: the proper validation of the tree
    StatusCode validate
    ( const LHCb::IParticlePropertySvc* svc ) const override
    { return m_tree->validate ( svc ) ; }
    /// collect the marked elements
    size_t collect
    ( typename Decays::iTree_<PARTICLE>::Collection& output ) const override
    { return m_tree->collect ( output ) ; }
    /// reset the collection cache
    void reset() const override { m_tree->reset () ; }
    /// has marked elements in the tree?
    bool marked () const override { return m_tree->marked() ; }
    // ======================================================================
  public:
    // ======================================================================
    Tree_& operator&= ( const Decays::iTree_<PARTICLE>& tree );
    Tree_& operator|= ( const Decays::iTree_<PARTICLE>& tree );
    // ======================================================================
  public:
    // ======================================================================
    /// get the underlying tree
    inline const Decays::iTree_<PARTICLE>& tree () const { return *m_tree ; }
    // ======================================================================
  public:
    // ======================================================================
    bool tree ( typename Decays::iTree_<PARTICLE>:: argument p ) const
    { return (*m_tree) ( p ) ; }
    // ======================================================================
  public:
    // ======================================================================
    bool eval ( typename Decays::iTree_<PARTICLE>:: argument p ) const
    { return (*m_tree) ( p ) ; }
    // ======================================================================
  public:
    // ======================================================================
    /// swap two trees: it can reduce CPU for decay mathching 
    void swap ( Tree_& another ) { std::swap ( m_tree , another.m_tree ) ; }
    // ======================================================================
  public:
    // ======================================================================
    /// assignment operator
    Tree_& operator=( const  Tree_&           right ) ; // assignment
    /// move assignment oerator 
    Tree_& operator=(        Tree_&&          right ) = default; // assignment
    /// pseudo-assignment operator
    Tree_& operator=( const iTree_<PARTICLE>& right ) ; // assignment
    // ======================================================================
  private:
    // ======================================================================
    /// the default constructor is private one
    Tree_ () ; // no  default constructor
    // ======================================================================
  private:
    // ======================================================================
    /// the tree itself
    std::unique_ptr<Decays::iTree_<PARTICLE>> m_tree ; // the tree itself
    // ======================================================================
  } ;
  // ==========================================================================
  /** The standard output stream operator for the sub-tree
   *  @param s the reference to the output stream
   *  @param o the decay tree objects
   *  @return the reference to the output stream
   */
  std::ostream& operator<<( std::ostream& s , const iTree& o ) ;
  // ==========================================================================
  /** swap two decay trees 
   *  useful to reduce unnesessary cloning 
   *  @author Vanya  BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-03-25
   */
  template <class PARTICLE> 
  inline void swap ( Tree_<PARTICLE>& a , Tree_<PARTICLE>& b ) { a.swap ( b ) ; }
  // ==========================================================================
} // end of namespace Decays
// ============================================================================
// constructor from the tree
// ============================================================================
template <class PARTICLE>
Decays::Tree_<PARTICLE>::Tree_
( const Decays::iTree_<PARTICLE>& tree )
{
  typedef Decays::Tree_<PARTICLE> Self ;
  const Self* self = dynamic_cast<const Self*>( &tree ) ;
  if ( self ) { m_tree.reset( self -> m_tree-> clone () ); }
  if ( !m_tree ) { m_tree.reset( tree.clone () ) ; }
}
// ============================================================================
// copy constructor
// ============================================================================
template <class PARTICLE>
Decays::Tree_<PARTICLE>::Tree_
( const Decays::Tree_<PARTICLE>&  tree )
  : Decays::iTree_<PARTICLE> ( tree )
  , m_tree ( tree.m_tree -> clone() )
{}
// ============================================================================
// assignment operator
// ============================================================================
template <class PARTICLE>
Decays::Tree_<PARTICLE>&
Decays::Tree_<PARTICLE>::operator=
( const Decays::Tree_<PARTICLE>& right )                         // assignment
{
  m_tree.reset( right.m_tree->clone() );
  return  *this ;
}
// ============================================================================
// assignment operator
// ============================================================================
template <class PARTICLE>
Decays::Tree_<PARTICLE>&
Decays::Tree_<PARTICLE>::operator=
( const Decays::iTree_<PARTICLE>& right )                        // assignment
{
  if ( &right == this ) { return *this ; }
  typedef Decays::Tree_<PARTICLE> Self ;
  const Self* self = dynamic_cast<const Self*>( &right ) ;
  //
  Decays::iTree_<PARTICLE>* tree = nullptr ;
  if ( self )  { tree = self->m_tree-> clone () ; }
  if ( !tree ) { tree = right.         clone () ; }
  m_tree.reset( tree );
  //
  return *this ;
}
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Utils 
  {
    // ========================================================================
    template <class PARTICLE>
    inline 
    std::string toCpp ( const Decays::iTree_<PARTICLE>& p ) 
    { return "\"" + p.toString() + "\"" ; } 
    // ========================================================================
  }
  // ==========================================================================
}
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_ITREE_H
// ============================================================================
