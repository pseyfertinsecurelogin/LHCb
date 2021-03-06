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
#ifndef RELATIONS_Relation2_H
#define RELATIONS_Relation2_H 1
// =============================================================================
// Include files
// =============================================================================
// STD & STL
// =============================================================================
#include <algorithm>
// =============================================================================
// from Gaudi
// =============================================================================
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/SmartRef.h"
// =============================================================================
// From Relations
// =============================================================================
#include "Relations/IRelation2D.h"
#include "Relations/Relation.h"
#include "Relations/RelationUtils.h"
// =============================================================================
namespace Relations {
  // ===========================================================================
  /** @class Relation2 Relation2.h Relations/Relation2.h
   *
   *  @brief Implementation of ordinary bidirectional relations
   *
   *  Implementation of ordinary bidirectional relations
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
   *  @see IRelation
   *  @warning for the current implementation the actual type of
   *           FROM should differ from the actual type of TO
   *
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   23/03/2002
   */
  template <class FROM, class TO>
  class Relation2 : public BaseTable, public IRelation2D<FROM, TO> {
  public:
    // ========================================================================
    /// short cut for own     type
    typedef Relation2<FROM, TO> OwnType;
    /// short cut for inverse type
    typedef Relation2<TO, FROM> InvType;
    /// short cut for interface
    typedef IRelation2D<FROM, TO> IBase;
    /// actual implementations of direct  type
    typedef Relation<FROM, TO> Base;
    /// actual implementations of inverse type
    typedef Relation<TO, FROM> InvBase;
    /// shortcut for direct base type
    using Direct = Base;
    /// shortcut for inverse base type
    using Inverse = InvBase;
    /// shortcut for direct subinterface
    using DirectType = typename IBase::DirectType;
    /// shortcut for inverse subinterface
    using InverseType = typename IBase::InverseType;
    /// import "Range" type from interface
    using Range = typename IBase::Range;
    /// import "From"  type from interface
    using From = typename IBase::From;
    /// import "From"  type from interface
    using From_ = typename IBase::From_;
    /// import "To"    type from interface
    using To = typename IBase::To;
    /// import "To"    type from interface
    using To_ = typename IBase::To_;
    /// the actual type of the entry
    using Entry = typename IBase::Entry;
    // ========================================================================
  public:
    // ========================================================================
    /// the default constructor
    Relation2( const size_t reserve = 0 ) : BaseTable(), IBase(), m_direct( reserve ), m_inverse( reserve ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /// constructor from any "direct" interface
    Relation2( const DirectType& copy ) : BaseTable(), IBase(), m_direct( copy ), m_inverse( copy, 1 ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /** constructor from any "inverse" interface
     *  @param copy object to be copied
     *  @param flag artificial argument, to distinguish from copy constructor
     */
    Relation2( const InverseType& copy, const int flag )
        : BaseTable(), IBase(), m_direct( copy, flag ), m_inverse( copy ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /** copy constructor is publc,
     *  @attention it is not recommended for normal usage
     *  @param copy object to be copied
     */
    Relation2( const OwnType& copy )
        : BaseTable( copy ), IBase( copy ), m_direct( copy.m_direct ), m_inverse( copy.m_inverse ) {
      /// set cross-links
      m_direct.setInverseBase( m_inverse.directBase() );
      m_inverse.setInverseBase( m_direct.directBase() );
    }
    /// destructor (virtual)
    virtual ~Relation2() = default;
    // ========================================================================
  public: // major functional methods (fast, 100% inline)
    // ========================================================================
    /// retrive all relations from the object (fast,100% inline)
    inline Range i_relations( From_ object ) const { return m_direct.i_relations( object ); }
    /// retrive all relations from ALL objects (fast,100% inline)
    inline Range i_relations() const { return m_direct.i_relations(); }
    /// make the relation between 2 objects (fast,100% inline method)
    inline StatusCode i_relate( From_ object1, To_ object2 ) { return i_add( Entry( object1, object2 ) ); }
    /// add the entry
    inline StatusCode i_add( const Entry& entry ) { return m_direct.i_add( entry ); }
    /// remove the concrete relation between objects (fast,100% inline method)
    inline StatusCode i_remove( From_ object1, To_ object2 ) { return m_direct.i_remove( object1, object2 ); }
    /// remove all relations FROM the defined object (fast,100% inline method)
    inline StatusCode i_removeFrom( From_ object ) { return m_direct.i_removeFrom( object ); }
    /// remove all relations TO the defined object (fast,100% inline method)
    inline StatusCode i_removeTo( To_ object ) { return m_direct.i_removeTo( object ); }
    /// remove ALL relations form ALL  object to ALL objects  (fast,100% inline)
    inline StatusCode i_clear() { return m_direct.i_clear(); }
    /// rebuild ALL relations form ALL  object to ALL objects(fast,100% inline)
    inline StatusCode i_rebuild() { return m_direct.i_rebuild(); }
    /** make the relation between 2 objects (fast,100% inline method)
     *  - Call for i_sort() is mandatory!
     */
    inline void i_push( From_ object1, To_ object2 ) { m_direct.i_push( object1, object2 ); }
    /** (re)sort the table
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
    Relation2& merge( const Range& range ) {
      m_direct.merge( range );
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation2& imerge( const typename InvType::Range& range ) {
      m_direct.imerge( range );
      return *this;
    }
    /** merge with the sorted range of relations
     *  @attention the range is assumed to be sorted!
     *  @param range the range to be added
     *  @return self-reference
     */
    Relation2& operator+=( const Range& range ) { return merge( range ); }
    // ========================================================================
  public: // abstract methods from interface
    // ========================================================================
    /// retrive all relations from the object
    Range relations( From_ object ) const override { return i_relations( object ); }
    /// retrive all relations from ALL objects
    Range relations() const override { return i_relations(); }
    /// make the relation between 2 objects
    StatusCode relate( From_ object1, To_ object2 ) override { return i_relate( object1, object2 ); }
    /// add the entry
    StatusCode add( const Entry& entry ) override { return i_add( entry ); }
    /// remove the concrete relation between objects
    StatusCode remove( From_ object1, To_ object2 ) override { return i_remove( object1, object2 ); }
    /// remove all relations FROM the defined object
    StatusCode removeFrom( From_ object ) override { return i_removeFrom( object ); }
    /// remove all relations TO the defined object
    StatusCode removeTo( To_ object ) override { return i_removeTo( object ); }
    /// remove ALL relations from ALL  object to ALL objects
    StatusCode clear() override { return i_clear(); }
    /// rebuild ALL relations from ALL  object to ALL objects
    StatusCode rebuild() override { return i_rebuild(); }
    /// update the object after POOL/ROOT reading
    virtual StatusCode update() {
      /// update the cross-links
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
  public: // abstract methods from interface
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
    /// increase the reference counter (artificial)
    unsigned long addRef() override { return 1; }
    /// release the reference counter (artificial)
    unsigned long release() override { return 1; }
    // ========================================================================
  public: // other methods
    // ========================================================================
    /// reserve the relations (for efficiency reasons)
    inline StatusCode reserve( const size_t num ) { return m_direct.reserve( num ); }
    // ========================================================================
  private:
    // ========================================================================
    /// assignement operator is private!
    Relation2& operator=( const Relation2& copy ) = delete;
    // ========================================================================
  public:
    // ========================================================================
    /// Access the size of the relations
    [[nodiscard]] inline std::size_t size() const { return m_direct.size(); }
    // ========================================================================
  private:
    // ========================================================================
    /// the tabele for direct relations
    Direct m_direct; // the tabele for direct relations
    /// the table for inverse relations
    Inverse m_inverse; // the table for inverse relations
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Relations
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_Relation2_H
