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
#ifndef RELATIONS_Relation2Weighted_H
#define RELATIONS_Relation2Weighted_H 1
// =============================================================================
// Include files
// =============================================================================
// STD & STL
// =============================================================================
#include <algorithm>
// =============================================================================
// From Relations
// =============================================================================
#include "Relations/IRelationWeighted2D.h"
#include "Relations/RelationUtils.h"
#include "Relations/RelationWeighted.h"
// =============================================================================

namespace Relations {
  /** @class Relation2Weighted Relations/Relation2Weighted.h
   *
   *  @brief Implementation of weighted bidirectional relations
   *
   *  Implementation of weighed bidirectional relations
   *  from objects of type "FROM" to objects of type "TO" with weight "WEIGHT"
   *
   *  Data types for "FROM", "TO" and "WEIGHT"
   *
   *   - either inherits from ContainedObject/DataObject
   *   - or have the templated specializations through
   *     ObjectTypeTraits structure with defined streamer operator
   *     to/from StreamBuffer class
   *
   *  @see ObjectTypeTraits
   *  @see IRelationWeihted
   *  @see IRelationWeihted2D
   *  @warning for the current implementation the actual type of
   *           FROM should differ from the actual type of TO
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   23/03/2002
   */
  template <class FROM, class TO, class WEIGHT>
  class Relation2Weighted : public BaseWeightedTable, public IRelationWeighted2D<FROM, TO, WEIGHT> {
  public:
    // ========================================================================
    /// short cut for own     type
    typedef Relation2Weighted<FROM, TO, WEIGHT> OwnType;
    /// short cut for inverse type
    typedef Relation2Weighted<TO, FROM, WEIGHT> InvType;
    /// short cut for interface
    typedef IRelationWeighted2D<FROM, TO, WEIGHT> IBase;
    /// actual implementations of direct  type
    typedef RelationWeighted<FROM, TO, WEIGHT> Base;
    /// actual implementations of inverse type
    typedef RelationWeighted<TO, FROM, WEIGHT> InvBase;
    /// shortcut for direct base type
    using Direct = Base;
    /// shortcut for inverse base type
    using Inverse = InvBase;
    /// shortcut for direct subinterface
    using DirectType = typename IBase::DirectType;
    /// shortcut for inverse subinterface
    using InverseType = typename IBase::InverseType;
    /// import basic types from Interface
    using Range = typename IBase::Range;
    /// import basic types from Interface
    using From  = typename IBase::From;
    using From_ = typename IBase::From_;
    /// import basic types from Interface
    using To  = typename IBase::To;
    using To_ = typename IBase::To_;
    /// import basic types from Interface
    using Weight  = typename IBase::Weight;
    using Weight_ = typename IBase::Weight_;
    /// the actual type of the entry
    using Entry = typename IBase::Entry;
    // ========================================================================
  public:
    // ========================================================================
    /// the default constructor
    Relation2Weighted( const size_t reserve = 0 )
        : BaseWeightedTable(), IBase(), m_direct( reserve ), m_inverse( reserve ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /// constructor from any "direct" interface
    Relation2Weighted( const DirectType& copy ) : BaseWeightedTable(), IBase(), m_direct( copy ), m_inverse( copy, 1 ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /** constructor from any "inverse" interface
     *  @param copy object to be copied
     *  @param flag artificial argument, to distinguish from copy constructor
     */
    Relation2Weighted( const InverseType& copy, const int flag )
        : BaseWeightedTable(), IBase(), m_direct( copy, flag ), m_inverse( copy ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /// copy constructor is publc @attention it is not recommended for normal usage
    Relation2Weighted( const OwnType& copy )
        : BaseWeightedTable( copy ), IBase( copy ), m_direct( copy.m_direct ), m_inverse( copy.m_inverse ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /// destructor (virtual)
    virtual ~Relation2Weighted() = default;

    // ========================================================================
  public: // major functional methods (fast, 100% inline)
    // ========================================================================
    /// retrive all relations from the object (fast,100% inline)
    inline Range i_relations( From_ object ) const { return m_direct.i_relations( object ); }
    /// retrive ALL relations from ALL objects (fast,100% inline)
    inline Range i_relations() const { return m_direct.i_relations(); }
    /// retrive all relations from the object (fast,100% inline)
    inline Range i_relations( From_ object, Weight_ threshold, const bool flag ) const {
      return m_direct.i_relations( object, threshold, flag );
    }
    /// retrive all relations from the object (fast,100% inline)
    inline Range i_inRange( From_ object, Weight_ low, Weight_ high ) const {
      return m_direct.i_inRange( object, low, high );
    }
    /// make the relation between 2 objects (fast,100% inline)
    inline StatusCode i_relate( From_ object1, To_ object2, Weight_ weight ) {
      const Entry entry( object1, object2, weight );
      return i_add( entry );
    }
    /// add the entry
    inline StatusCode i_add( const Entry& entry ) { return m_direct.i_add( entry ); }
    /// remove the concrete relation between objects (fast,100% inline)
    inline StatusCode i_remove( From_ object1, To_ object2 ) { return m_direct.i_remove( object1, object2 ); }
    /// remove all relations FROM the defined object (fast,100% inline)
    inline StatusCode i_removeFrom( From_ object ) { return m_direct.i_removeFrom( object ); }
    /// remove all relations TO the defined object (fast,100% inline)
    inline StatusCode i_removeTo( To_ object ) { return m_direct.i_removeTo( object ); }
    /// filter out the relations FROM the defined object (fast,100% inline)
    inline StatusCode i_filterFrom( From_ object, Weight_ threshold, const bool flag ) {
      return m_direct.i_filterFrom( object, threshold, flag );
    }
    /// filter out the relations TO the defined object (fast,100% inline)
    inline StatusCode i_filterTo( const To_ object, Weight_ threshold, const bool flag ) {
      return m_direct.i_filterTo( object, threshold, flag );
    }
    /// filter out all relations (fast,100% inline)
    inline StatusCode i_filter( Weight_ threshold, const bool flag ) { return m_direct.i_filter( threshold, flag ); }
    /// remove ALL relations from ALL objects to ALL objects (fast,100% inline)
    inline StatusCode i_clear() { return m_direct.i_clear(); }
    /// rebuild ALL relations form ALL  object to ALL objects(fast,100% inline)
    inline StatusCode i_rebuild() { return m_direct.i_rebuild(); };
    /** make the relation between 2 objects (fast,100% inline)
     *  call for i_sort() is mandatory!
     */
    inline void i_push( From_ object1, To_ object2, Weight_ weight ) { m_direct.i_push( object1, object2, weight ); }
    /** (re)sort of the table
     *   mandatory to use after i_push
     */
    inline void i_sort() { m_direct.i_sort(); }
    // ========================================================================
  public: // merge
    // ========================================================================
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation2Weighted& merge( const Range& range ) {
      m_direct.merge( range );
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation2Weighted& imerge( const typename InvType::Range& range ) {
      m_direct.imerge( range );
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation2Weighted& operator+=( const Range& range ) { return merge( range ); }
    // ========================================================================
  public: // abstract methods from interface
    // ========================================================================
    /// retrive all relations from the object
    Range relations( From_ object ) const override { return i_relations( object ); }
    /// retrive ALL relations from ALL objects
    Range relations() const override { return i_relations(); }
    /** retrive all relations from the object which has weigth
     *  larger/smaller than the threshold value
     */
    Range relations( From_ object, Weight_ threshold, const bool flag ) const override {
      return i_relations( object, threshold, flag );
    }
    /** retrive all relations from the object which has weigth
     *  withing the specified range
     */
    Range inRange( From_ object, Weight_ low, Weight_ high ) const override { return i_inRange( object, low, high ); }
    /// make the relation between 2 objects
    StatusCode relate( From_ object1, To_ object2, Weight_ weight ) override {
      return i_relate( object1, object2, weight );
    }
    /// add the entry
    StatusCode add( const Entry& entry ) override { return i_add( entry ); }
    /// remove the concrete relation between objects
    StatusCode remove( From_ object1, To_ object2 ) override { return i_remove( object1, object2 ); }
    /// remove all relations FROM the defined object
    StatusCode removeFrom( From_ object ) override { return i_removeFrom( object ); }
    /// remove all relations TO the defined object
    StatusCode removeTo( To_ object ) override { return i_removeTo( object ); }
    /** filter out the relations FROM the defined object, which
     *  have a weight larger(smaller)than the threshold weight
     */
    StatusCode filterFrom( From_ object, Weight_ threshold, const bool flag ) override {
      return i_filterFrom( object, threshold, flag );
    }
    /** filter out the relations TO the defined object, which
     *  have a weight larger/smaller than the threshold weight
     */
    StatusCode filterTo( To_ object, Weight_ threshold, const bool flag ) override {
      return i_filterTo( object, threshold, flag );
    }
    /** filter out all relations which
     *  have a weight larger/smaller than the threshold weight
     */
    StatusCode filter( Weight_ threshold, const bool flag ) override { return i_filter( threshold, flag ); }
    /// remove ALL relations from ALL objects to ALL objects
    StatusCode clear() override { return i_clear(); }
    /// rebuild ALL relations from ALL  object to ALL objects
    StatusCode rebuild() override { return i_rebuild(); }
    /// update the object after POOL/ROOT reading
    virtual StatusCode update() {
      /// update/reset cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
      return i_rebuild();
    }
    // ========================================================================
  public:
    // ========================================================================
    /// get the "direct" interface
    inline Direct* i_direct() { return &m_direct; }
    /// get the "direct" interface  (const-version)
    inline const Direct* i_direct() const { return &m_direct; }
    /// get the "inverse" interface
    inline Inverse* i_inverse() { return &m_inverse; }
    /// get the "inverse" interface  (const version)
    inline const Inverse* i_inverse() const { return &m_inverse; }
    // ========================================================================
  public:
    // ========================================================================
    /// get the "direct" interface
    DirectType* direct() override { return i_direct(); }
    /// get the "direct" interface  (const-version)
    const DirectType* direct() const override { return i_direct(); }
    /// get the "inverse" interface
    InverseType* inverse() override { return i_inverse(); }
    /// get the "inverse" interface  (const version)
    const InverseType* inverse() const override { return i_inverse(); }
    // ========================================================================
  public:
    // ========================================================================
    /// query the interface
    StatusCode queryInterface( const InterfaceID& id, void** ret ) override {
      if ( nullptr == ret ) { return StatusCode::FAILURE; } // RETURN !!!
      if ( IInterface::interfaceID() == id ) {
        *ret = static_cast<IInterface*>( this );
      } else if ( IBase::interfaceID() == id ) {
        *ret = static_cast<IBase*>( this );
      } else if ( IBase::DirectType::interfaceID() == id ) {
        *ret = static_cast<typename IBase::DirectType*>( this );
      } else {
        return StatusCode::FAILURE;
      } //  RETURN !!!
      ///
      addRef();
      return StatusCode::SUCCESS;
    }
    // ========================================================================
  public: // other methods
    // ========================================================================
    /// increase the reference counter (artificial)
    unsigned long addRef() override { return 1; }
    /// release the reference counter (artificial)
    unsigned long release() override { return 1; }
    /// reserve the relations (for efficiency reasons)
    inline StatusCode reserve( const size_t num ) { return m_direct.reserve( num ); }
    // ========================================================================
  private:
    // ========================================================================
    /// assignement operator is private!
    Relation2Weighted& operator=( const Relation2Weighted& copy ) = delete;
    // ========================================================================
  private:
    // ========================================================================
    /// the direct table
    Direct m_direct;
    /// the inverse table
    Inverse m_inverse;
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Relations
// =============================================================================
// The End
// =============================================================================
#endif // RELATIONS_RELation2Weighted_H
