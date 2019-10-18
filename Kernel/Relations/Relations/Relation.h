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
#ifndef RELATIONS_Relation_H
#define RELATIONS_Relation_H 1
// =============================================================================
// Include files
// =============================================================================
// STD & STL
// =============================================================================
#include <algorithm>
// =============================================================================
// from Gaudi
// =============================================================================
#include "GaudiKernel/IInterface.h"
// =============================================================================
// From Relations
// =============================================================================
#include "Relations/IRelation.h"
#include "Relations/RelationBase.h"
#include "Relations/RelationUtils.h"
// =============================================================================
namespace Relations {
  // ==========================================================================
  /** @class Relation Relation.h Relations/Relation.h
   *
   *  @brief Implementation of ordinary unidirectional relations
   *
   *  Implementation of ordinary unidirectional relations
   *  from objects of type "FROM" to objects of type "TO".
   *
   *  Data types for "FROM" and "TO":
   *
   *   - either inherits from ContainedObject/DataObject
   *   - or have the templated specializations through
   *     ObjectTypeTraits structure with defined streamer operator
   *     to/from StreamBuffer class
   *
   *  @see ObjectTypeTraits
   *  @see StreamBuffer
   *  @see IRelations
   *  @warning for the current implementation the actual type of
   *           FROM should differ from the actual type of TO
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   25/01/2002
   */
  template <class FROM, class TO>
  class Relation : public BaseTable, public IRelation<FROM, TO> {
  public:
    // ========================================================================
    /// short cut for own     type
    typedef Relation<FROM, TO> OwnType;
    /// short cut for inverse type
    typedef Relation<TO, FROM> InvType;
    /// short cut for interface
    typedef IRelation<FROM, TO> IBase;
    /// short cut for the actual implementation type
    typedef typename Relations::RelationBase<FROM, TO> Base;
    /// shortcut for inverse base
    typedef typename Relations::RelationBase<TO, FROM> InvBase;
    /// shortcut for direct base type
    using Direct = Base;
    /// shortcut for inverse base type
    using Inverse = InvBase;
    /// shortcut for direct  interface
    using IDirect = IBase;
    /// shortcut for inverse base type
    using IInverse = typename IDirect::InverseType;
    /// basic types from Interface
    using Range = typename IBase::Range;
    /// basic types from Interface
    using From_ = typename IBase::From_;
    using From  = typename IBase::From;
    /// basic types from Interface
    using To_ = typename IBase::To_;
    using To  = typename IBase::To;
    /// the actual type of the entry
    using Entry = typename IBase::Entry;
    // ========================================================================
  public:
    // ========================================================================
    /// the default constructor
    Relation( const size_t reserve = 0 ) : BaseTable(), IBase(), m_direct( reserve ), m_inverse_aux( 0 ) {}
    /** constructor from arbitrary "direct" interface
     *  @param copy object to be copied
     */
    Relation( const IDirect& copy ) : BaseTable(), IBase(), m_direct( copy ), m_inverse_aux( 0 ) {}
    /** constructor from "inverse interface"
     *  It is an efficient way to "invert" relation
     *  @param inv interface to be inverted
     *  @param flag artificial argument, to distinguish from copy constructor
     */
    Relation( const IInverse& inv, const int flag ) : BaseTable(), IBase(), m_direct( inv, flag ), m_inverse_aux( 0 ) {}
    /** copy constructor is publc,
     *  but it is not recommended for direct usage
     *  @param copy object to be copied
     */
    Relation( const OwnType& copy ) : BaseTable( copy ), IBase( copy ), m_direct( copy.m_direct ), m_inverse_aux( 0 ) {}
    /// destructor (virtual)
    virtual ~Relation() = default;
    // ========================================================================
  public: // major functional methods (fast, 100% inline)
    // ========================================================================
    /// retrive all relations from the object (fast,100% inline)
    Range i_relations( From_ object ) const {
      auto ip = m_direct.i_relations( object );
      return Range( ip.first, ip.second );
    }
    /// retrive all relations from ALL objects (fast,100% inline)
    Range i_relations() const {
      auto ip = m_direct.i_relations();
      return Range( ip.first, ip.second );
    }
    /// make the relation between 2 objects (fast,100% inline method)
    StatusCode i_relate( From_ object1, To_ object2 ) {
      const Entry entry( object1, object2 );
      return i_add( entry );
    }
    /// add the entry
    StatusCode i_add( const Entry& entry ) {
      StatusCode sc = m_direct.i_add( entry );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      const typename Inverse::Entry ientry( entry.to(), entry.from() );
      return m_inverse_aux->i_add( ientry );
    }
    /// remove the concrete relation between objects (fast,100% inline method)
    StatusCode i_remove( From_ object1, To_ object2 ) {
      StatusCode sc = m_direct.i_remove( object1, object2 );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_remove( object2, object1 );
    }
    /// remove all relations FROM the defined object (fast,100% inline method)
    StatusCode i_removeFrom( From_ object ) {
      StatusCode sc = m_direct.i_removeFrom( object );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_removeTo( object );
    }
    /// remove all relations TO the defined object (fast,100% inline method)
    StatusCode i_removeTo( To_ object ) {
      StatusCode sc = m_direct.i_removeTo( object );
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_removeFrom( object );
    }
    /// remove ALL relations form ALL  object to ALL objects (fast,100% inline)
    StatusCode i_clear() {
      StatusCode sc = m_direct.i_clear();
      if ( sc.isFailure() || 0 == m_inverse_aux ) { return sc; }
      return m_inverse_aux->i_clear();
    }
    /// rebuild ALL relations form ALL  object to ALL objects (fast,100% inline)
    StatusCode i_rebuild() {
      using _Entry   = typename IBase::Entry;
      using _Entries = typename std::vector<_Entry>;
      // 1) get all relations
      Range r = i_relations();
      // 2) copy them into temporary storage
      _Entries _e( r.begin(), r.end() );
      // 3) clear all existing relations
      StatusCode sc = i_clear();
      if ( sc.isFailure() ) { return sc; }
      // 4) reserve space for new relations
      sc = reserve( _e.size() );
      if ( sc.isFailure() ) { return sc; }
      // 5) build new relations
      for ( const auto& entry : _e ) i_push( entry.from(), entry.to() );
      // (re)sort
      i_sort();
      //
      return StatusCode::SUCCESS;
    }
    /** make the relation between 2 objects (fast,100% inline method)
     *  - Call for i_sort() is mandatory!
     */
    void i_push( From_ object1, To_ object2 ) {
      m_direct.i_push( object1, object2 );
      if ( 0 != m_inverse_aux ) { m_inverse_aux->i_push( object2, object1 ); }
    }
    /** (re)sort of the table
     *   mandatory to use after i_push
     */
    void i_sort() {
      m_direct.i_sort();
      if ( 0 != m_inverse_aux ) { m_inverse_aux->i_sort(); }
    }
    // ========================================================================
  public: // merge
    // ========================================================================
    /** merge with the sorted range of relations
     *  @attention the raneg is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation& merge( const Range& range ) {
      if ( range.empty() ) { return *this; }
      m_direct.merge( range );
      if ( 0 != m_inverse_aux ) { m_inverse_aux->imerge( range ); }
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the raneg is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation& imerge( const typename IInverse::Range& range ) {
      if ( range.empty() ) { return *this; }
      m_direct.imerge( range );
      if ( 0 != m_inverse_aux ) { m_inverse_aux->merge( range ); }
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the raneg is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation& operator+=( const Range& range ) { return merge( range ); }
    // ========================================================================
  public: // abstract methods from interface
    // ========================================================================
    /// retrive all relations from the object
    Range relations( From_ object ) const override { return i_relations( object ); }
    /// retrive all relations from ALL objects
    Range relations() const override { return i_relations(); }
    /// make the relation between 2 objects
    StatusCode relate( From_ object1, To_ object2 ) override { return i_relate( object1, object2 ); }
    /// add teh relation
    StatusCode add( const Entry& entry ) override { return i_add( entry ); }
    /// remove the concrete relation between objects
    StatusCode remove( From_ object1, To_ object2 ) override { return i_remove( object1, object2 ); }
    /// remove all relations FROM the defined object
    StatusCode removeFrom( From_ object ) override { return i_removeFrom( object ); }
    /// remove all relations TO the defined object
    StatusCode removeTo( To_ object ) override { return i_removeTo( object ); }
    /// remove ALL relations form ALL  object to ALL objects
    StatusCode clear() override { return i_clear(); }
    /// rebuild ALL relations form ALL  object to ALL objects
    StatusCode rebuild() override { return i_rebuild(); }
    /// update the object after POOL/ROOT reading
    virtual StatusCode update() { return i_rebuild(); }
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
    /// increase the reference counter (artificial)
    unsigned long addRef() override { return 1; }
    /// release the reference counter (artificial)
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
      if ( 0 != m_inverse_aux ) { m_inverse_aux->i_reserve( num ); }
      return m_direct.i_reserve( num );
    }
    // ========================================================================
  private:
    // ========================================================================
    /// assignement operator is private!
    Relation& operator=( const OwnType& copy ) = delete;
    // ========================================================================
  public:
    // ========================================================================
    /// Access the size of the relations
    [[nodiscard]] std::size_t size() const { return m_direct.size(); }
    // ========================================================================
  private:
    // ========================================================================
    /// the holder of all direct relations
    Direct m_direct; // the holder of all direct relations
    ///  the pointer to inverse table
    Inverse* m_inverse_aux; // the pointer to inverse table
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_Relation_H
