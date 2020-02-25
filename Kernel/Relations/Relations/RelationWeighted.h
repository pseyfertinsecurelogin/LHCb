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
#ifndef RELATIONS_RelationWeighted_H
#define RELATIONS_RelationWeighted_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
// ============================================================================
// From Relation
// ============================================================================
#include "Relations/IRelationWeighted.h"
#include "Relations/RelationUtils.h"
#include "Relations/RelationWeightedBase.h"
// ============================================================================

namespace Relations {
  /** @class RelationWeighted Relations/RelationWeighted.h
   *
   *  The simplest implementation of unidirectional weighted relations.
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   28/01/2002
   */
  template <class FROM, class TO, class WEIGHT>
  class RelationWeighted : public BaseWeightedTable, public IRelationWeighted<FROM, TO, WEIGHT> {
  public:
    // ========================================================================
    /// shortcut for own type
    typedef RelationWeighted<FROM, TO, WEIGHT> OwnType;
    /// shortcut for inverse type
    typedef RelationWeighted<TO, FROM, WEIGHT> InvType;
    /// short cut for interface
    typedef IRelationWeighted<FROM, TO, WEIGHT> IBase;
    /// short cut for the actual implementation type
    typedef Relations::RelationWeightedBase<FROM, TO, WEIGHT> Base;
    /// shortcut for inverse base
    typedef Relations::RelationWeightedBase<TO, FROM, WEIGHT> InvBase;
    /// shortcut for direct base type
    using Direct = Base;
    /// shortcut for inverse base type
    using Inverse = InvBase;
    /// shortcut for direct  interface
    using IDirect = IBase;
    /// shortcut for inverse base type
    using IInverse = typename IDirect::InverseType;
    /// import basic types from Interface
    using Range = typename IBase::Range;
    /// import basic types from Interface
    using From = typename IBase::From;
    /// import basic types from Interface
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
    /// the standard/default constructor
    RelationWeighted( const size_t reserve = 0 )
        : BaseWeightedTable(), IBase(), m_direct( reserve ), m_inverse_aux( 0 ) {}
    /// constructor from arbitrary "direct" interface
    RelationWeighted( const IDirect& copy ) : BaseWeightedTable(), IBase(), m_direct( copy ), m_inverse_aux( 0 ) {}
    /** constructor from "inverse interface"
     *  It is an efficient way to "invert" relation
     *  @param inv interface to be inverted
     *  @param flag artificial argument, to distinguish from copy constructor
     */
    RelationWeighted( const IInverse& inv, const int flag )
        : BaseWeightedTable(), IBase(), m_direct( inv, flag ), m_inverse_aux( 0 ) {}
    /** copy constructor is publc,
     *  but it is not recommended for direct usage
     *  @param copy object to be copied
     */
    RelationWeighted( const OwnType& copy )
        : BaseWeightedTable( copy ), IBase( copy ), m_direct( copy.m_direct ), m_inverse_aux( 0 ) {}
    /// destructor (virtual)
    virtual ~RelationWeighted() = default;
    // ========================================================================
  public: // major functional methods (fast, 100% inline)
    // ========================================================================
    /// retrive all relations from the object (fast,100% inline)
    Range i_relations( From_ object ) const {
      auto ip = m_direct.i_relations( object );
      return {ip.first, ip.second};
    }
    /// retrive ALL relations from ALL objects (fast,100% inline)
    Range i_relations() const {
      auto ip = m_direct.i_relations();
      return {ip.first, ip.second};
    }
    /// retrive all relations from the object (fast,100% inline)
    Range i_relations( From_ object, Weight_ threshold, bool flag ) const {
      auto ip = m_direct.i_relations( object, threshold, flag );
      return {ip.first, ip.second};
    }
    /// retrive all relations in the range from the object (fast,100% inline)
    Range i_inRange( From_ object, Weight_ low, Weight_ high ) const {
      auto ip = m_direct.i_inRange( object, low, high );
      return {ip.first, ip.second};
    }
    /// make the relation between 2 objects (fast,100% inline)
    StatusCode i_relate( From_ object1, To_ object2, Weight_ weight ) {
      const Entry entry( object1, object2, weight );
      return i_add( entry );
    }
    /// add the entry
    StatusCode i_add( const Entry& entry ) {
      StatusCode sc = m_direct.i_add( entry );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      const typename Inverse::Entry ientry( entry.to(), entry.from(), entry.weight() );
      return m_inverse_aux->i_add( ientry );
    }
    /// remove the concrete relation between objects (fast,100% inline)
    StatusCode i_remove( From_ object1, To_ object2 ) {
      StatusCode sc = m_direct.i_remove( object1, object2 );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_remove( object2, object1 );
    }
    /// remove all relations FROM the defined object (fast,100% inline)
    StatusCode i_removeFrom( From_ object ) {
      StatusCode sc = m_direct.i_removeFrom( object );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_removeTo( object );
    }
    /// remove all relations TO the defined object (fast,100% inline)
    StatusCode i_removeTo( To_ object ) {
      StatusCode sc = m_direct.i_removeTo( object );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_removeFrom( object );
    }
    /// filter out the relations FROM the defined object (fast,100% inline)
    StatusCode i_filterFrom( From_ object, Weight_ threshold, const bool flag ) {
      StatusCode sc = m_direct.i_filterFrom( object, threshold, flag );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_filterTo( object, threshold, flag );
    }
    /// filter out the relations TO the defined object (fast,100% inline)
    StatusCode i_filterTo( To_ object, Weight_ threshold, const bool flag ) {
      StatusCode sc = m_direct.i_filterTo( object, threshold, flag );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_filterFrom( object, threshold, flag );
    }
    /// filter out all relations (fast,100% inline)
    StatusCode i_filter( Weight_ threshold, const bool flag ) {
      StatusCode sc = m_direct.i_filter( threshold, flag );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_filter( threshold, flag );
    }
    /// remove ALL relations from ALL objects to ALL objects (fast,100% inline)
    StatusCode i_clear() {
      StatusCode sc = m_direct.i_clear();
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_clear();
    }
    /// rebuild ALL relations from ALL object to ALL objects (fast,100% inline)
    StatusCode i_rebuild() {
      using _Entry = typename IBase::Entry;
      // 1) get all relations
      Range r = i_relations();
      // 2) copy them into temporary storage
      auto _e = std::vector<_Entry>{r.begin(), r.end()};
      // 3) clear all existing rleations
      StatusCode sc = i_clear();
      if ( sc.isFailure() ) { return sc; }
      // 4) reserve space for new relations
      sc = reserve( _e.size() );
      if ( sc.isFailure() ) { return sc; }
      // 5) build new relations
      for ( const auto& entry : _e ) i_push( entry.from(), entry.to(), entry.weight() );
      // (re)sort
      i_sort();
      //
      return StatusCode::SUCCESS;
    }
    /** make the relation between 2 objects (fast,100% inline)
     *  subsequent call for i_sort is mandatory!
     */
    void i_push( From_ object1, To_ object2, Weight_ weight ) {
      m_direct.i_push( object1, object2, weight );
      if ( 0 != m_inverse_aux ) { m_inverse_aux->i_push( object2, object1, weight ); }
    }
    /** (re)sort of the table
     *   mandatory to use after i_push
     */
    void i_sort() {
      m_direct.i_sort();
      if ( 0 != m_inverse_aux ) { m_inverse_aux->i_sort(); }
    }
    // ========================================================================
  public: // "merge"
    // ========================================================================
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    RelationWeighted& merge( const Range& range ) {
      if ( range.empty() ) { return *this; }
      m_direct.merge( range );
      if ( 0 != m_inverse_aux ) { m_inverse_aux->imerge( range ); }
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    RelationWeighted& imerge( const typename IInverse::Range& range ) {
      if ( range.empty() ) { return *this; }
      m_direct.imerge( range );
      if ( 0 != m_inverse_aux ) { m_inverse_aux->merge( range ); }
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    RelationWeighted& operator+=( const Range& range ) { return merge( range ); }
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
    virtual StatusCode update() { return i_rebuild(); }

    // ========================================================================
  public:
    // ========================================================================
    /// query the interface
    StatusCode queryInterface( const InterfaceID& iID, void** ppI ) override {
      if ( nullptr == ppI ) { return StatusCode::FAILURE; } // RETURN !!!
      if ( IInterface::interfaceID() == iID ) {
        *ppI = static_cast<IInterface*>( this );
      } else if ( IBase::interfaceID() == iID ) {
        *ppI = static_cast<IBase*>( this );
      } else {
        return StatusCode::FAILURE;
      } //  RETURN !!!
      ///
      addRef();
      return StatusCode::SUCCESS;
    }
    /// increase the reference counter
    unsigned long addRef() override { return 1; }
    /// release the reference counter
    unsigned long release() override { return 1; }
    /// get the pointer to direct table
    Direct* directBase() { return &m_direct; }
    /// get the reference to direct table
    const Direct& _direct() const { return m_direct; }
    /** set new inverse table
     *  @attention the method is not for public usage !!!
     */
    void setInverseBase( Inverse* inverse ) { m_inverse_aux = inverse; }
    /// reserve the relations (for efficiency reasons)
    StatusCode reserve( const size_t num ) {
      if ( 0 != m_inverse_aux ) {
        m_inverse_aux->i_reserve( num ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      }
      return m_direct.i_reserve( num );
    }
    // ========================================================================
  private:
    // ========================================================================
    /// assignement operator is private!
    RelationWeighted& operator=( const RelationWeighted& copy ) = delete;
    // ========================================================================
  private:
    // ========================================================================
    /// the actual store of relations
    Direct m_direct; // the actual store of relations
    /// the pointer to inverse table
    Inverse* m_inverse_aux; // the pointer to inverse table
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_RelationWeighted_H
