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
#ifndef RELATIONS_RelationWeighted1D_H
#define RELATIONS_RelationWeighted1D_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
// ============================================================================
// from Gaudi
// ============================================================================
#include "GaudiKernel/DataObject.h"
// ============================================================================
// From Relation
// ============================================================================
#include "Relations/IRelationWeighted.h"
#include "Relations/RelationUtils.h"
#include "Relations/RelationWeighted.h"
// ============================================================================
namespace LHCb {
  /** @class RelationWeighted1D Relations/RelationWeighted1D.h
   *
   *  The simplest implementation of unidirectional weighted relations.
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   28/01/2002
   */
  template <class FROM, class TO, class WEIGHT>
  class RelationWeighted1D : public DataObject,
                             public Relations::BaseWeightedTable,
                             public IRelationWeighted<FROM, TO, WEIGHT> {
  public:
    // ========================================================================
    /// shortcut for own type
    typedef RelationWeighted1D<FROM, TO, WEIGHT> OwnType;
    /// shortcut for inverse type
    typedef RelationWeighted1D<TO, FROM, WEIGHT> InvType;
    /// short cut for interface
    typedef IRelationWeighted<FROM, TO, WEIGHT> IBase;
    /// import "Range" type from the base
    using Range = typename IBase::Range;
    /// import "From"  type from the base
    using From  = typename IBase::From;
    using From_ = typename IBase::From_;
    /// import "To"    type from the base
    using To  = typename IBase::To;
    using To_ = typename IBase::To_;
    /// import "Weight" type from the base
    using Weight  = typename IBase::Weight;
    using Weight_ = typename IBase::Weight_;
    /// short cut for the actual implementation type
    typedef typename Relations::RelationWeighted<FROM, TO, WEIGHT> Base;
    // shortcut for "direct" interface
    using IDirect = typename IBase::DirectType;
    // shortcut for "inverse" interface
    using IInverse = typename IBase::InverseType;
    /// the actual type of the entry
    using Entry = typename IBase::Entry;
    // ========================================================================
  public:
    /// the standard/default constructor
    RelationWeighted1D( const size_t reserve = 0 )
        : DataObject(), Relations::BaseWeightedTable(), IBase(), m_base( reserve ) {
#ifdef COUNT_INSTANCES
      Relations::InstanceCounter::instance().increment( type() );
#endif // COUNT_INSTANCES
    }
    /// constructor from any direct interface
    RelationWeighted1D( const IDirect& copy ) : DataObject(), Relations::BaseWeightedTable(), IBase(), m_base( copy ) {
#ifdef COUNT_INSTANCES
      Relations::InstanceCounter::instance().increment( type() );
#endif // COUNT_INSTANCES
    }
    /** constructor from "inverted interface"
     *  @param inv object to be inverted
     *  @param flag artificial argument to distinguisch from
     *  copy constructor
     */
    RelationWeighted1D( const IInverse& inv, const int flag )
        : DataObject(), Relations::BaseWeightedTable(), IBase(), m_base( inv, flag ) {
#ifdef COUNT_INSTANCES
      Relations::InstanceCounter::instance().increment( type() );
#endif // COUNT_INSTANCES
    }
    /// copy constructor
    RelationWeighted1D( const OwnType& copy )
        : DataObject( copy ), Relations::BaseWeightedTable( copy ), IBase( copy ), m_base( copy.m_base ) {
#ifdef COUNT_INSTANCES
      Relations::InstanceCounter::instance().increment( type() );
#endif // COUNT_INSTANCES
    }
    /// destructor (virtual)
    ~RelationWeighted1D() override {
#ifdef COUNT_INSTANCES
      Relations::InstanceCounter::instance().decrement( type() );
#endif // COUNT_INSTANCES
    }
    /// the type name
    [[nodiscard]] const std::string& type() const {
      static const std::string s_type( System::typeinfoName( typeid( OwnType ) ) );
      return s_type;
    }
    /// object identifier (static method)
    static const CLID& classID() {
      static const CLID s_clid = Relations::clid( System::typeinfoName( typeid( OwnType ) ) );
      return s_clid;
    }
    /// object identification (virtual method)
    [[nodiscard]] const CLID& clID() const override { return classID(); }
    // ========================================================================
  public: // major functional methods (fast, 100% inline)
    // ========================================================================
    /// retrive all relations from the object (fast,100% inline)
    inline Range i_relations( From_ object ) const { return m_base.i_relations( object ); }
    /// retrive ALL relations from ALL objects (fast,100% inline)
    inline Range i_relations() const { return m_base.i_relations(); }
    /// retrive all relations from the object (fast,100% inline)
    inline Range i_relations( From_ object, Weight_ threshold, const bool flag ) const {
      return m_base.i_relations( object, threshold, flag );
    }
    /// retrive all relations from the object (fast,100% inline)
    inline Range i_inRange( From_ object, Weight_ low, Weight_ high ) const {
      return m_base.i_inRange( object, low, high );
    }
    /// make the relation between 2 objects (fast,100% inline)
    inline StatusCode i_relate( From_ object1, To_ object2, Weight_ weight ) {
      const Entry entry( object1, object2, weight );
      return i_add( entry );
    }
    /// add the entry
    inline StatusCode i_add( const Entry& entry ) { return m_base.i_add( entry ); }
    /// remove the concrete relation between objects (fast,100% inline)
    inline StatusCode i_remove( From_ object1, To_ object2 ) { return m_base.i_remove( object1, object2 ); }
    /// remove all relations FROM the defined object (fast,100% inline)
    inline StatusCode i_removeFrom( From_ object ) { return m_base.i_removeFrom( object ); }
    /// remove all relations TO the defined object (fast,100% inline)
    inline StatusCode i_removeTo( To_ object ) { return m_base.i_removeTo( object ); }
    /// filter out the relations FROM the defined object (fast,100% inline)
    inline StatusCode i_filterFrom( From_ object, Weight_ threshold, const bool flag ) {
      return m_base.i_filterFrom( object, threshold, flag );
    }
    /// filter out the relations TO the defined object (fast,100% inline)
    inline StatusCode i_filterTo( To_ object, Weight_ threshold, const bool flag ) {
      return m_base.i_filterTo( object, threshold, flag );
    }
    /// filter out all relations (fast,100% inline)
    inline StatusCode i_filter( Weight_ threshold, const bool flag ) { return m_base.i_filter( threshold, flag ); }
    /// remove ALL relations from ALL objects to ALL objects (fast,100% inline)
    inline StatusCode i_clear() { return m_base.i_clear(); }
    /// rebuild ALL relations form ALL  object to ALL objects(fast,100% inline)
    inline StatusCode i_rebuild() { return m_base.i_rebuild(); }
    /** make the relation between 2 objects (fast,100% inline)
     *  the subsequent call for i_sort is mandatory
     */
    inline void i_push( From_ object1, To_ object2, Weight_ weight ) { m_base.i_push( object1, object2, weight ); }
    /** (re)sort of the table
     *   mandatory to use after i_push
     */
    inline void i_sort() { m_base.i_sort(); }
    // ========================================================================
  public: // merge
    // ========================================================================
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    RelationWeighted1D& merge( const Range& range ) {
      m_base.merge( range );
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    RelationWeighted1D& imerge( const typename IInverse::Range& range ) {
      m_base.imerge( range );
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    RelationWeighted1D& operator+=( const Range& range ) { return merge( range ); }
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
    /** update the object after POOL/ROOT reading
     *  @see DataObject
     *  @return status code
     */
    StatusCode update() override { return i_rebuild(); }
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
      } else {
        return StatusCode::FAILURE;
      } //  RETURN !!!
      ///
      addRef();
      return StatusCode::SUCCESS;
    }
    /// increase the reference counter
    unsigned long addRef() override { return DataObject::addRef(); }
    /// release the reference counter
    unsigned long release() override { return DataObject::release(); }
    // ========================================================================
  public:
    // ========================================================================
    /// POOL identifier
    static std::string GUID() { return Relations::guid( classID() ); }
    // ========================================================================
  private:
    // ========================================================================
    /// the assignment operator is disabled
    RelationWeighted1D& operator=( const RelationWeighted1D& ) = delete;
    // ========================================================================
  private:
    // ========================================================================
    /// the actual storage of relations
    Base m_base; // the actual storage of relations
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LHCb
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_RelationWeighted1D_H
