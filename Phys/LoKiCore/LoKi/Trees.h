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
#ifndef DECAYS_TREES_H
#define DECAYS_TREES_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <iterator>
#include <type_traits>
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/Nodes.h"
#include "Kernel/ParticleID.h"
#include "Kernel/iNode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/DecayDescriptor.h"
#include "LoKi/iTree.h"
// ============================================================================
namespace Decays {
  // ==========================================================================
  template <class PARTICLE>
  class TreeList_;
  // ==========================================================================
  namespace Trees {
    // ========================================================================
    /** @class Marked_
     *  Simple "marked" sub-tree
     *
     *  @code
     *
     *   typedef ...   PARTICKLE ;
     *
     *   const iTree_<PARTICLE>& decay = ... ;
     *
     *   // create marked decay:
     *   const Marked_<PARTICLE> marked ( decay );
     *
     *  @endcode
     *
     *  For the decay descrptors, the marked component is
     *  denoted with "^"-sumbol:
     *
     *  @attention the class caches the event-data!!
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-30
     */
    template <class PARTICLE>
    class Marked_ : public Decays::iTree_<PARTICLE> {
    public:
      // ======================================================================
      /// constructor from the tree
      Marked_( const Decays::iTree_<PARTICLE>& tree );
      /// copy constructor (ignore the marked particle)
      Marked_( const Marked_& right );
      /// move  constructor (ignore the marked particle)
      Marked_( Marked_&& right );
      /// MANDATORY: clone method ("virtual constructor")
      Marked_* clone() const override { return new Marked_( *this ); }
      /// MANDATORY: the only one essential method:
      bool operator()( typename Decays::iTree_<PARTICLE>::argument p ) const override;
      /// MANDATORY: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      /// Check the validity
      bool valid() const override;
      /// validate it
      StatusCode validate( const LHCb::IParticlePropertySvc* svc ) const override;
      /// reset the collection cache
      void reset() const override;
      /// collect the marked elements
      size_t collect( typename Decays::iTree_<PARTICLE>::Collection& output ) const override;
      /// has marked elements in the tree ?
      bool marked() const override { return true; }
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      Marked_(); // no default constructor
      // ======================================================================
    private:
      // ======================================================================
      /// the actual tree
      Decays::Tree_<PARTICLE> m_tree; // the actual tree
      // ======================================================================
      /// the particle itself
      mutable PARTICLE m_particle; // the particle itself
      // ======================================================================
    };
    // ========================================================================
    /** @class Any_
     *  the most trivial pseudo-tree, which matches to *any* tree
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2008-04-30
     */
    template <class PARTICLE>
    class Any_ : public Decays::iTree_<PARTICLE> {
    private:
      // ======================================================================
      /// get the actual argument type form the base
      typedef typename Decays::iTree_<PARTICLE>::argument argument;
      /// get the actual collection type form the base
      typedef typename Decays::iTree_<PARTICLE>::Collection Collection;
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Any_* clone() const override { return new Any_( *this ); }
      /// MANDATORY: the only one essential method:
      bool operator()( argument /* p */ ) const override { return true; }
      /// MANDATORY: the printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << " X "; }
      /// Check the validity
      bool valid() const override { return true; }
      /// validate it
      StatusCode validate( const LHCb::IParticlePropertySvc* /* svc */ ) const override {
        return StatusCode( StatusCode::SUCCESS, true );
      }
      /// reset the collection cache
      void reset() const override {}
      /// collect the marked elements
      size_t collect( Collection& /* output */ ) const override { return 0; }
      /// has marked elements in the tree ?
      bool marked() const override { return false; }
      // ======================================================================
    };
    // ========================================================================
    /** @class None_
     *  the most trivial pseudo-tree, which does not matches to *any* tree
     *  @author Vanya BELYAEV Ivan.B3lyaev@nikhef.nl
     *  @date 2010-10-24
     */
    template <class PARTICLE>
    class None_ : public Decays::Trees::Any_<PARTICLE> {
    private:
      // ======================================================================
      /// get the actual argument type form the base
      typedef typename Decays::iTree_<PARTICLE>::argument argument;
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      None_* clone() const override { return new None_( *this ); }
      /// MANDATORY: the only one essential method:
      bool operator()( argument /* p */ ) const override { return false; }
      /// MANDATORY: the printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << " None "; }
      // ======================================================================
    };
    // ========================================================================
    /** @class Invalid_
     *  helper class to represent the invalid tree
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-05-08
     */
    template <class PARTICLE>
    class Invalid_ : public Decays::iTree_<PARTICLE> {
    private:
      // ======================================================================
      /// get the actual argument type form the base
      typedef typename Decays::iTree_<PARTICLE>::argument argument;
      /// get the actual collection type form the base
      typedef typename Decays::iTree_<PARTICLE>::Collection Collection;
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: default constructor
      Invalid_() = default;
      /// MANDATORY: clone method ("virtual constructor")
      Invalid_* clone() const override { return new Invalid_( *this ); }
      /// MANDATORY: the only one essential method:
      bool operator()( argument /* p */ ) const override { return false; }
      /// MANDATORY: the printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << " <INVALID> "; }
      /// Check the validity
      bool valid() const override { return false; }
      /// validate it
      StatusCode validate( const LHCb::IParticlePropertySvc* /* svc */ ) const override {
        return StatusCode( StatusCode::FAILURE, true );
      }
      /// reset the collection cache
      void reset() const override {}
      /// collect the marked elements
      size_t collect( Collection& /* output */ ) const override { return 0; }
      /// has marked elements in the tree ?
      bool marked() const override { return false; }
      // ======================================================================
    };
    // ========================================================================
    /** @class Stable_
     *  Simple class to represent the stable tree
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-05-08
     */
    template <class PARTICLE>
    class Stable_ : public Decays::iTree_<PARTICLE> {
    private:
      // ======================================================================
      /// get the actual argument type form the base
      typedef typename Decays::iTree_<PARTICLE>::argument argument;
      /// get the actual collection type form the base
      typedef typename Decays::iTree_<PARTICLE>::Collection Collection;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the decay head
      Stable_( const Decays::iNode& head );
      /// constructor from the decay head
      Stable_( const std::string& head );
      /// constructor from the decay head
      Stable_( const LHCb::ParticleID& head );
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Stable_* clone() const override { return new Stable_( *this ); }
      /// MANDATORY: the only one essential method:
      bool operator()( typename Decays::iTree_<PARTICLE>::argument p ) const override;
      /// MANDATORY: the printout
      std::ostream& fillStream( std::ostream& s ) const override;
      /// Check the validity
      bool valid() const override;
      /// validate it
      StatusCode validate( const LHCb::IParticlePropertySvc* svc ) const override;
      /// reset the collection cache
      void reset() const override {}
      /// collect the marked elements
      size_t collect( typename Decays::iTree_<PARTICLE>::Collection& /* output */ ) const override { return 0; }
      /// has marked elements in the tree ?
      bool marked() const override { return false; }
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      Stable_(); // the default constructor is disabled
      // ======================================================================
    private:
      // ======================================================================
      /// the actual decay head
      Decays::Node m_head; // the actual decay head
      // ======================================================================
    };
    // ========================================================================
    /** @class _Tree_
     *  Helper class needed to hold temporary Tree-objects
     *  and for the proper permutations
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-05-08
     */
    template <class PARTICLE>
    struct _Tree_ {
      // ======================================================================
    public:
      // ======================================================================
      /// invalid class
      typedef Decays::Trees::Invalid_<PARTICLE> Invalid;
      // ======================================================================
    public:
      // ======================================================================
      /// the default constructor   (Assign invalid ID)
      _Tree_() : m_tree( Invalid() ), m_id( 0 ) {}
      /// the constructor from the tree (Assign unique ID)
      _Tree_( const Decays::iTree_<PARTICLE>& tree ) : m_tree( tree ), m_id( 0 ) { m_id = getID(); }
      /// the constructor from the tree (Assign unique ID)
      _Tree_( const Decays::Tree_<PARTICLE>& tree ) : m_tree( tree ), m_id( 0 ) { m_id = getID(); }
      /// the constructor from the tree (Assign unique ID)
      _Tree_( Decays::Tree_<PARTICLE>&& tree ) : m_tree( std::move( tree ) ), m_id( 0 ) { m_id = getID(); }
      /// copy constructor  (Copy the unique ID)
      _Tree_( const _Tree_& tree ) : m_tree( tree.m_tree ), m_id( tree.m_id ) {}
      /// move constructor  (Copy the unique ID)
      _Tree_( _Tree_&& tree ) : m_tree( std::move( tree.m_tree ) ), m_id( tree.m_id ) {}
      // ======================================================================
    public:
      // ======================================================================
      /// the main method
      inline bool operator()( typename Decays::iTree_<PARTICLE>::argument p ) const { return m_tree.tree( p ); }
      // valid ?
      inline bool valid() const { return m_tree.tree().valid(); }
      // validate
      inline StatusCode validate( const LHCb::IParticlePropertySvc* svc ) const {
        return m_tree.tree().validate( svc );
      }
      /// collect the marked elements
      inline size_t collect( typename Decays::iTree_<PARTICLE>::Collection& o ) const {
        return m_tree.tree().collect( o );
      }
      /// reset the cache
      inline void reset() const { m_tree.reset(); }
      /// the specific printout
      inline std::ostream& fillStream( std::ostream& s ) const { return m_tree.fillStream( s ); }
      /// has marked elements in the tree ?
      inline bool marked() const { return m_tree.marked(); }
      // ======================================================================
    public:
      // ======================================================================
      /// needed for std::find
      inline bool operator==( typename Decays::iTree_<PARTICLE>::argument p ) const { return m_tree.tree( p ); }
      // ======================================================================
      /// needed for std::find
      inline bool operator!=( typename Decays::iTree_<PARTICLE>::argument p ) const { return !m_tree.tree( p ); }
      // ======================================================================
    public:
      // ======================================================================
      /// assignement operator, copy the unique ID
      _Tree_& operator=( const _Tree_& right ) {
        if ( &right == this ) { return *this; }
        m_tree = right.m_tree;
        /// copy the unique ID
        m_id = right.m_id; // NB! Copy the unique ID
        return *this;
      }
      /// move assignement operator, copy the unique ID
      _Tree_& operator=( _Tree_&& right ) {
        if ( &right == this ) { return *this; }
        m_tree = std::move( right.m_tree );
        /// copy the unique ID
        m_id = right.m_id; // NB! Copy the unique ID
        return *this;
      }
      // ======================================================================
    public:
      // ====================================================================
      _Tree_& operator&=( const Decays::iTree_<PARTICLE>& right );
      _Tree_& operator|=( const Decays::iTree_<PARTICLE>& right );
      _Tree_& operator&=( const _Tree_& right ) { return ( *this ) &= right.tree(); }
      _Tree_& operator|=( const _Tree_& right ) { return ( *this ) |= right.tree(); }
      // ====================================================================
    public:
      // ======================================================================
      /// get the actual tree
      inline const Decays::iTree_<PARTICLE>& tree() const { return m_tree.tree(); }
      // ======================================================================
      /// cast to the actual underlying tree
      operator const Decays::iTree_<PARTICLE>&() const { return tree(); }
      /// non-const cast to the tree-holder
      operator Decays::Tree_<PARTICLE>&() { return m_tree; }
      // ======================================================================
    public:
      // ======================================================================
      /// swapping: makes container operations (e.g. sorting) much faster
      void swap( _Tree_& another ) {
        Decays::swap( m_tree, another.m_tree );
        std::swap( m_id, another.m_id );
      }
      // ======================================================================
    public:
      // ======================================================================
      /// ordering (nesessary for the proper permutations)
      friend bool operator<( const _Tree_& lhs, const _Tree_& rhs ) { return lhs.m_id < rhs.m_id; }
      // ======================================================================
    private:
      // ======================================================================
      /// the actual tree
      Decays::Tree_<PARTICLE> m_tree; // the actual tree for matching
      // ======================================================================
      /// own unique ID
      size_t m_id; // own unique ID
      // ======================================================================
    private:
      // ======================================================================
      /// the source of IDs
      static size_t getID(); // the source of IDs
      // ======================================================================
    };
    // ========================================================================
    /** swap two elements to speed-up sorting and other manipulations
     *  with arrays of trees
     *  @author Vanya BELYAEV Ivan.Belyave@nikhef.nl
     *  @date 2015-03-25
     */
    template <class PARTICLE>
    inline void swap( _Tree_<PARTICLE>& a, _Tree_<PARTICLE>& b ) {
      a.swap( b );
    }
    // ========================================================================
    /** @struct CheckTree
     *  Helper struture to use the whole power of STL algorithms
     *  for decay finder tools
     *  @author Vanya BELYAEV Ivan.Belyave@nikhef.nl
     *  @date 2008-07-07
     */
    template <class PARTICLE>
    struct CheckTree {
      // ======================================================================
      /// compare tree and the argument
      inline bool operator()( typename Decays::iTree_<PARTICLE>::argument p,
                              const Decays::Trees::_Tree_<PARTICLE>&      t ) const {
        return t( p );
      }
      // ======================================================================
      /// compare tree and the argument
      inline bool operator()( const Decays::Trees::_Tree_<PARTICLE>&      t,
                              typename Decays::iTree_<PARTICLE>::argument p ) const {
        return t( p );
      }
      // ======================================================================
    };
    // ========================================================================
  } // namespace Trees
  // ==========================================================================
  /** @class TreeList_
   *  helper class to represent the list of trees
   *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
   *  @date 2009-05-23
   */
  template <class PARTICLE>
  class TreeList_ final {
  public:
    // =======================================================================
    /// the element type
    typedef Decays::Trees::_Tree_<PARTICLE> _Tree_;
    typedef Decays::Tree_<PARTICLE>         Tree;
    typedef Decays::iTree_<PARTICLE>        iTree;
    /// the actual container
    typedef std::vector<_Tree_>              Trees_;
    typedef typename Trees_::iterator        iterator;
    typedef typename Trees_::const_iterator  const_iterator;
    typedef typename Trees_::const_reference const_reference;
    // =======================================================================
  public:
    // =======================================================================
    // constructor from the trees
    TreeList_( const Trees_& trees ) : m_trees( trees ) {}
    /// default constructor
    TreeList_() = default;
    /// copy constructor
    TreeList_( const TreeList_& right ) = default;
    /// move constructor
    TreeList_( TreeList_&& right ) = default;
    // ========================================================================
  public:
    // ========================================================================
    // constructor from iterators
    template <typename ITERATOR,
              typename = std::enable_if_t<std::is_base_of<
                  std::input_iterator_tag, typename std::iterator_traits<ITERATOR>::iterator_category>::value>>
    TreeList_( ITERATOR first, ITERATOR last ) : m_trees( first, last ) {}
    // ========================================================================
  public:
    // ========================================================================
    /// assignement operator
    TreeList_& operator=( const TreeList_& right ) = default;
    /// move assignement operator
    TreeList_& operator=( TreeList_&& right ) = default;
    // ========================================================================
  public:
    // ========================================================================
    /// swapping
    void swap( TreeList_& right ) { std::swap( m_trees, right.m_trees ); }
    // ========================================================================
  public:
    // ========================================================================
    TreeList_& operator+=( const _Tree_& tree ) {
      m_trees.push_back( tree );
      return *this;
    }
    TreeList_& operator+=( _Tree_&& tree ) // movable
    {
      m_trees.push_back( tree );
      return *this;
    }
    TreeList_& operator+=( const Tree& tree ) {
      m_trees.push_back( tree );
      return *this;
    }
    TreeList_& operator+=( Tree&& tree ) // movable ?
    {
      m_trees.push_back( tree );
      return *this;
    }
    TreeList_& operator+=( const iTree& tree ) {
      m_trees.push_back( tree );
      return *this;
    }
    TreeList_& operator+=( const Trees_& trees ) {
      m_trees.insert( m_trees.end(), trees.begin(), trees.end() );
      return *this;
    }
    TreeList_& operator+=( const TreeList_& trees ) {
      m_trees.insert( m_trees.end(), trees.begin(), trees.end() );
      return *this;
    }
    // ========================================================================
  public:
    // ========================================================================
    const_iterator begin() const { return m_trees.begin(); }
    const_iterator end() const { return m_trees.end(); }
    iterator       begin() { return m_trees.begin(); }
    iterator       end() { return m_trees.end(); }
    bool           empty() const { return m_trees.empty(); }
    size_t         size() const { return m_trees.size(); }
    void           clear() { m_trees.clear(); }
    // ========================================================================
  public:
    // ========================================================================
    const_reference front() const { return m_trees.front(); }
    const_reference back() const { return m_trees.back(); }
    // ========================================================================
  public:
    // ========================================================================
    const Trees_& trees() const { return m_trees; }
                  operator const Trees_&() const { return trees(); }
    // ========================================================================
  private:
    // ========================================================================
    ///  the actual container of trees
    Trees_ m_trees; //  the actual container of trees
    // ========================================================================
  };
  // ==========================================================================
  /// swap two lists
  template <class PARTICLE>
  inline void swap( TreeList_<PARTICLE>& a, TreeList_<PARTICLE>& b ) {
    a.swap( b );
  }
  // ==========================================================================
  namespace Trees {
    // ========================================================================
    /** @class  Op_
     *  Simple sub-tree which matches "operation"
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-13
     */
    template <class PARTICLE>
    class Op_ : public Decays::iTree_<PARTICLE> {
    public:
      // ======================================================================
      /// the actual type for subtrees
      typedef Decays::TreeList_<PARTICLE> TreeList;
      // ======================================================================
    protected:
      // ======================================================================
      typedef typename TreeList::iterator       iterator;
      typedef typename TreeList::const_iterator const_iterator;
      // ======================================================================
    protected:
      // ======================================================================
      /// constructor from two sub-trees
      Op_();
      /// copy constructor
      Op_( const Op_& right );
      /// move constructor
      Op_( Op_&& right );
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: check the validness
      bool valid() const override;
      /// MANDATORY: the proper validation of the tree
      StatusCode validate( const LHCb::IParticlePropertySvc* svc ) const override;
      /// MANDATORY: reset method
      void reset() const override { return i_reset(); }
      /// MANDATORY: collect all marked elements
      size_t collect( typename Decays::iTree_<PARTICLE>::Collection& ) const override;
      /// has marked elements in the tree ?
      bool marked() const override;
      // ======================================================================
    protected:
      // ======================================================================
      inline const TreeList& trees() const { return m_trees; }
      inline TreeList&       trees() { return m_trees; }
      // ======================================================================
      inline iterator       begin() { return m_trees.begin(); }
      inline const_iterator begin() const { return m_trees.begin(); }
      inline iterator       end() { return m_trees.end(); }
      inline const_iterator end() const { return m_trees.end(); }
      // ======================================================================
    protected:
      // ======================================================================
      size_t push_back( const Decays::iTree_<PARTICLE>& tree ) {
        m_trees += tree;
        return m_trees.size();
      }
      // ======================================================================
    protected:
      // ======================================================================
      /// inline form of reset
      inline void i_reset() const;
      // ======================================================================
    protected:
      // ======================================================================
      /// the actual list of trees
      mutable TreeList m_trees; // the actual list of trees
      // ======================================================================
    };
    // ========================================================================
    /** @class  And_
     *  Simple sub-tree which matches .AND. for other subtrees
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-13
     */
    template <class PARTICLE>
    class And_ : public Decays::Trees::Op_<PARTICLE> {
    protected:
      // =====================================================================
      /// base class
      typedef typename Decays::Trees::Op_<PARTICLE> _Base;
      // ======================================================================
    public:
      // ======================================================================
      /// the actual type for subtrees
      typedef typename _Base::TreeList TreeList;
      // ======================================================================
    protected:
      // ======================================================================
      typedef typename _Base::iterator       iterator;
      typedef typename _Base::const_iterator const_iterator;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from two sub-trees
      And_( const Decays::iTree_<PARTICLE>& n1, const Decays::iTree_<PARTICLE>& n2 );
      /// constructor from list of sub-trees
      And_( const TreeList& trees );
      /// copy constructor
      And_( const And_& right );
      /// move constructor
      And_( And_&& right );
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      And_* clone() const override { return new And_( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( typename Decays::iTree_<PARTICLE>::argument p ) const override;
      /// MANDATORY: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      And_& operator&=( const Decays::iTree_<PARTICLE>& tree ) {
        this->addTree( tree );
        return *this;
      }
      And_& operator&=( const Decays::Trees::_Tree_<PARTICLE>& tree ) {
        this->addTree( tree.tree() );
        return *this;
      }
      And_& operator&=( const TreeList& tree ) {
        this->addTree( tree );
        return *this;
      }
      // ======================================================================
      And_& operator+=( const Decays::iTree_<PARTICLE>& tree ) {
        this->addTree( tree );
        return *this;
      }
      And_& operator+=( const Decays::Trees::_Tree_<PARTICLE>& tree ) {
        this->addTree( tree.tree() );
        return *this;
      }
      And_& operator+=( const TreeList& tree ) {
        this->addTree( tree );
        return *this;
      }
      // ======================================================================
    protected:
      // ======================================================================
      size_t addTree( const Decays::iTree_<PARTICLE>& tree );
      size_t addTree( const TreeList& trees );
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      And_(); // the default constructor is disabled
      // ======================================================================
    };
    // ========================================================================
    /** @class  Or_
     *  Simple sub-tree which matches .OR. for other subtrees
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-13
     */
    template <class PARTICLE>
    class Or_ : public Decays::Trees::Op_<PARTICLE> {
    protected:
      // ======================================================================
      /// base class
      typedef typename Decays::Trees::Op_<PARTICLE> _Base;
      // ======================================================================
    public:
      // ======================================================================
      /// the actual type for subtrees
      typedef typename _Base::TreeList TreeList;
      // ======================================================================
    protected:
      // ======================================================================
      typedef typename _Base::iterator       iterator;
      typedef typename _Base::const_iterator const_iterator;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from two sub-trees
      Or_( const Decays::iTree_<PARTICLE>& n1, const Decays::iTree_<PARTICLE>& n2 );
      /// constructor from list of sub-trees
      Or_( const TreeList& trees );
      /// copy constructor
      Or_( const Or_& right );
      /// move constructor
      Or_( Or_&& right );
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Or_* clone() const override { return new Or_( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( typename Decays::iTree_<PARTICLE>::argument p ) const override;
      /// MANDATORY: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      Or_& operator|=( const Decays::iTree_<PARTICLE>& tree ) {
        this->addTree( tree );
        return *this;
      }
      Or_& operator|=( const Decays::Trees::_Tree_<PARTICLE>& tree ) {
        this->addTree( tree.tree() );
        return *this;
      }
      Or_& operator|=( const TreeList& tree ) {
        this->addTree( tree );
        return *this;
      }
      // ======================================================================
      Or_& operator+=( const Decays::iTree_<PARTICLE>& tree ) {
        this->addTree( tree );
        return *this;
      }
      Or_& operator+=( const Decays::Trees::_Tree_<PARTICLE>& tree ) {
        this->addTree( tree.tree() );
        return *this;
      }
      Or_& operator+=( const TreeList& tree ) {
        this->addTree( tree );
        return *this;
      }
      // ======================================================================
    protected:
      // ======================================================================
      size_t addTree( const Decays::iTree_<PARTICLE>& tree );
      size_t addTree( const TreeList& trees );
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      Or_(); // the default constructor is disabled
      // ======================================================================
    };
    // ========================================================================
    /** @class  List_
     *  Simple sub-tree which matches .OR. for other subtrees
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-13
     */
    template <class PARTICLE>
    class List_ : public Decays::Trees::Or_<PARTICLE> {
    protected:
      // =====================================================================
      /// base class
      typedef typename Decays::Trees::Or_<PARTICLE> _Base;
      // ======================================================================
    public:
      // ======================================================================
      /// the actual type for subtrees
      typedef typename _Base::TreeList TreeList;
      // ======================================================================
    protected:
      // ======================================================================
      typedef typename _Base::iterator       iterator;
      typedef typename _Base::const_iterator const_iterator;
      // ======================================================================
    public:
      // ======================================================================
      /// forward to base class constructor
      using Decays::Trees::Or_<PARTICLE>::Or_;
      /// copy constructor
      List_( const List_& right ) = default;
      /// move constructor
      List_( List_&& right ) = default;
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      List_* clone() const override { return new List_( *this ); }
      /// MANDATORY: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// swap two lists
      void swap( List_& right ) { swap( this->m_trees, right->m_trees ); }
      // ======================================================================
    public:
      // ======================================================================
      List_& operator|=( const Decays::iTree_<PARTICLE>& tree ) {
        this->addTree( tree );
        return *this;
      }
      List_& operator|=( const Decays::Trees::_Tree_<PARTICLE>& tree ) {
        this->addTree( tree.tree() );
        return *this;
      }
      List_& operator|=( const TreeList& tree ) {
        this->addTree( tree );
        return *this;
      }
      // ======================================================================
      List_& operator+=( const Decays::iTree_<PARTICLE>& tree ) {
        this->addTree( tree );
        return *this;
      }
      List_& operator+=( const Decays::Trees::_Tree_<PARTICLE>& tree ) {
        this->addTree( tree.tree() );
        return *this;
      }
      List_& operator+=( const TreeList& tree ) {
        this->addTree( tree );
        return *this;
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      List_(); // the default constructor is disabled
      // ======================================================================
    };
    // ========================================================================
    /// swap two lists
    template <class PARTICLE>
    inline void swap( List_<PARTICLE>& a, List_<PARTICLE>& b ) {
      a.swap( b );
    }
    // ========================================================================
    /** @class  Not_
     *
     *  @attention Not_ blocks the marked elements!
     *
     *  Simple sub-tree which matches .NOT. for subtree
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-13
     */
    template <class PARTICLE>
    class Not_ : public Decays::iTree_<PARTICLE> {
    public:
      // ======================================================================
      /// constructor from the tree
      Not_( const Decays::iTree_<PARTICLE>& tree ) : m_tree( tree ) {}
      /// constructor from the tree
      Not_( const Decays::Tree_<PARTICLE>& tree ) : m_tree( tree ) {}
      /// constructor from the tree
      Not_( Decays::Tree_<PARTICLE>&& tree ) : m_tree( std::move( tree ) ) {}
      // ======================================================================
    public:
      // ======================================================================
      // MANDATORY: clone method ("virtual constructor")
      Not_* clone() const override { return new Not_( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( typename Decays::iTree_<PARTICLE>::argument p ) const override {
        const bool result = m_tree.tree( p );
        m_tree.tree().reset();
        return !result;
      }
      /// MANDATORY: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << " ~(" << m_tree << ") "; }
      /// MANDATORY: check the validness
      bool valid() const override { return m_tree.valid(); }
      /// MANDATORY: the proper validation of the tree
      StatusCode validate( const LHCb::IParticlePropertySvc* svc ) const override { return m_tree.validate( svc ); }
      /// MANDATORY: collect all marked elements
      size_t collect( typename Decays::iTree_<PARTICLE>::Collection& ) const override { return 0; }
      /// reset the collection cache
      void reset() const override { m_tree.reset(); }
      /// has marked elements in the tree ?
      bool marked() const override { return false; }
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      Not_(); // the default constructor is disabled
      // ======================================================================
    private:
      // ======================================================================
      /// the tree itself
      Decays::Tree_<PARTICLE> m_tree; // the tree itself
      // ======================================================================
    };
    // ========================================================================
    /** The standard output stream operator for the sub-tree
     *  @param s the reference to the outptu stream
     *  @param o the decay tree objects
     *  @return the reference to the output stream
     */
    template <class PARTICLE>
    inline std::ostream& operator<<( std::ostream& s, const _Tree_<PARTICLE>& o ) {
      return o.fillStream( s );
    }
    // ==========================================================================
  } // namespace Trees
  // ==========================================================================
  /** The helper function to "mark" the decay tree:
   *
   *  @code
   *
   *   typedef ... PARTICLE ;
   *
   *   const Decays::iTree_<PARTICLE>& tree = ....;
   *
   *   const Decays::Tree_<PARTICLE> marked = mark ( tree ) ;
   *
   *  @endcode
   *
   *  @param tree the deay tree to be marked
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-08-04
   */
  template <class PARTICLE>
  inline Decays::Trees::Marked_<PARTICLE> mark( const Decays::iTree_<PARTICLE>& tree ) {
    return Decays::Trees::Marked_<PARTICLE>( tree );
  }
  // ==========================================================================
  /** Create the "AND" of two trees
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-04-12
   */
  template <class PARTICLE>
  inline Decays::Trees::And_<PARTICLE> operator&&( const Decays::iTree_<PARTICLE>& o1,
                                                   const Decays::iTree_<PARTICLE>& o2 ) {
    return Decays::Trees::And_<PARTICLE>( o1, o2 );
  }
  // ==========================================================================
  /** Create the "Or" of two trees
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-04-12
   */
  template <class PARTICLE>
  inline Decays::Trees::Or_<PARTICLE> operator||( const Decays::iTree_<PARTICLE>& o1,
                                                  const Decays::iTree_<PARTICLE>& o2 ) {
    return Decays::Trees::Or_<PARTICLE>( o1, o2 );
  }
  // ==========================================================================
  /** Create the "Not" for the node
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-04-12
   */
  template <class PARTICLE>
  inline Decays::Trees::Not_<PARTICLE> operator~( const Decays::iTree_<PARTICLE>& o ) {
    return Decays::Trees::Not_<PARTICLE>( o );
  }
  // ==========================================================================
  /** "right" for of equality operator
   *  (needed for std::find)
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-07-07
   */
  template <class PARTICLE>
  inline bool operator==( typename Decays::iTree_<PARTICLE>::argument p, const Decays::iTree_<PARTICLE>& t ) {
    return t == p;
  }
  // ==========================================================================
  /** sequncer operator  (the same as "OR" here)
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-04-12
   */
  template <class PARTICLE>
  inline Decays::Trees::List_<PARTICLE> operator,( const Decays::iTree_<PARTICLE>& o1,
                                                   const Decays::iTree_<PARTICLE>& o2 ) {
    return Decays::Trees::List_<PARTICLE>( o1, o2 );
  }
  // ==========================================================================
  namespace Trees {
    // ========================================================================
    /** @struct Types
     *  Helper structure to define the proper types
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2000-05-06
     */
    template <class PARTICLE>
    struct Types_ {
      // ======================================================================
      /// the actual particle type
      typedef PARTICLE Type;
      /// the actual particle type
      typedef PARTICLE Particle;
      /// the actual type of interface
      typedef Decays::iTree_<PARTICLE> iTree;
      /// the actual type of assignable
      typedef Decays::Tree_<PARTICLE> Tree;
      /// the actual type of vector of assignables
      typedef Decays::TreeList_<PARTICLE> TreeList;
      // the actual type of "Any"
      typedef Decays::Trees::Any_<PARTICLE> Any;
      /// the actual type of vector of assignables
      typedef Decays::Trees::Invalid_<PARTICLE> Invalid;
      // OR
      typedef Decays::Trees::Or_<PARTICLE> Or;
      // AND
      typedef Decays::Trees::And_<PARTICLE> And;
      // LIST
      typedef Decays::Trees::List_<PARTICLE> List;
      // ======================================================================
    };
    // ========================================================================
    /** @enum Errors
     *  enum with various error codes
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-04-30
     */
    enum Errors {
      InvalidArrow = 200, // Invalid arrow type
      InclusiveOptional,  // Inclusive & Optional in conflict
      InclusivePhotos,    // Inclusive & Photos   in conflict
      InvalidDecayOnly,   // Invalid 'Decay-Only' flag
      InvalidOscillated,  // Invalid 'Oscillated' flag
      InvalidPhotos,      // Invalid 'Photos'     flag
      ChildrenForStable,  // Children   for Stable
      OptionalForStable,  // Optional   for Stable
      InclusiveForStable, // Inclusive  for Stable
      InclusiveMarked,    // Inclusive & Marked in conflict
      InvalidBranch       // Invalid combination of flags
    };
    // ========================================================================
  } // namespace Trees
  // ==========================================================================
} //                                                    end of namespace Decays
// ============================================================================
#include "LoKi/Trees.icpp"
// ============================================================================
// The END
// ============================================================================
#endif // DECAYS_TREES_H
