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
#ifndef RELATIONS_RELATIONWeightedBASE_H
#define RELATIONS_RELATIONWeightedBASE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <vector>
// ============================================================================
#include "GaudiKernel/detected.h"
// ============================================================================
// Relations
// ============================================================================
#include "Relations/IRelationWeighted.h"
#include "Relations/RelationWeightedTypeTraits.h"
#include "Relations/Reserve.h"
// ============================================================================
namespace Relations {
  // ==========================================================================
  /** @class RelationWeightedBase Relations/RelationWeightedBase.h
   *
   *  The useful helper class for effective  implementation
   *  of weighted unidirectional relations
   *
   *  @warning for the current implementation the actual type of
   *  "FROM" should differ from the actual type of "TO"
   *
   *  @warning users should not deal with this class directly!
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   27/01/2002
   */
  template <class FROM, class TO, class WEIGHT>
  class RelationWeightedBase : public BaseWeightedTable {
  public:
    /// short cut for type traits
    typedef Relations::RelationWeightedTypeTraits<FROM, TO, WEIGHT> TypeTraits;
    // ========================================================================
    /// Entry type
    using Entry =
        Gaudi::cpp17::detected_or_t<Relations::WEntry_<FROM, TO, WEIGHT>, Relations::TraitsHelpers::Entry, TypeTraits>;
    /// shortcut for own type
    typedef RelationWeightedBase<FROM, TO, WEIGHT> OwnType;
    /// shortcut for inverse type
    typedef RelationWeightedBase<TO, FROM, WEIGHT> InvType;
    /// shortcut for "direct" interface
    typedef IRelationWeighted<FROM, TO, WEIGHT> IDirect;
    /// shortcut for "inverse" interface
    typedef IRelationWeighted<TO, FROM, WEIGHT> IInverse;
    /// actual "FROM" type
    using From = Gaudi::cpp17::detected_or_t<typename Entry::From, Relations::TraitsHelpers::From, TypeTraits>;
    /// actual "FROM" type
    using From_ = Gaudi::cpp17::detected_or_t<typename Entry::From_, Relations::TraitsHelpers::From_, TypeTraits>;
    /// actual "TO" type
    using To  = Gaudi::cpp17::detected_or_t<typename Entry::To, Relations::TraitsHelpers::To, TypeTraits>;
    using To_ = Gaudi::cpp17::detected_or_t<typename Entry::To_, Relations::TraitsHelpers::To_, TypeTraits>;
    /// actual "Weight" type
    using Weight  = Gaudi::cpp17::detected_or_t<typename Entry::Weight, Relations::TraitsHelpers::Weight, TypeTraits>;
    using Weight_ = Gaudi::cpp17::detected_or_t<typename Entry::Weight_, Relations::TraitsHelpers::Weight_, TypeTraits>;
    /// container type
    using Entries = Gaudi::cpp17::detected_or_t<std::vector<Entry>, Relations::TraitsHelpers::Entries, TypeTraits>;
    /// range
    using Range = Gaudi::cpp17::detected_or_t<Relations::Range_<Entries>, Relations::TraitsHelpers::Range, TypeTraits>;
    // ========================================================================
  protected:
    // ========================================================================
    /// comparison criteria for full ordering
    using Less = Gaudi::cpp17::detected_or_t<std::less<>, Relations::TraitsHelpers::Less, TypeTraits>;
    /// comparison criteria ( "less" by "From" value)
    using LessF = Gaudi::cpp17::detected_or_t<typename Entry::LessF, Relations::TraitsHelpers::LessF, TypeTraits>;
    using Less1 = Gaudi::cpp17::detected_or_t<Relations::TraitsHelpers::LessByFrom<LessF>,
                                              Relations::TraitsHelpers::Less1, TypeTraits>;
    /// comparison criteria ( "less" by "Weight" value)
    using LessW = Gaudi::cpp17::detected_or_t<typename Entry::LessW, Relations::TraitsHelpers::LessW, TypeTraits>;
    using Less2 = Gaudi::cpp17::detected_or_t<Relations::TraitsHelpers::LessByWeight<LessW>,
                                              Relations::TraitsHelpers::Less2, TypeTraits>;
    /// equality criteria   ( "equal" by "To" value)
    using EqualT = Gaudi::cpp17::detected_or_t<typename Entry::EqualT, Relations::TraitsHelpers::EqualT, TypeTraits>;
    using Equal  = Gaudi::cpp17::detected_or_t<Relations::TraitsHelpers::EqualByTo<EqualT>,
                                              Relations::TraitsHelpers::Equal, TypeTraits>;
    /** @struct  Comp1
     *  comparison/ordering criteria using "Weight" and "To" fields
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   27/01/2002
     */
    struct Comp1 {
      /** comparison/ordering criteria
       *  @param  entry1 the first entry
       *  @param  entry2 the secons entry
       *  @return true   if "Weight" field of the first entry is less
       *                 and "To" fields are equal!
       */
      bool operator()( const Entry& entry1, const Entry& entry2 ) const {
        return Equal()( entry1, entry2 ) && Less2()( entry1, entry2 );
      }
    };
    /** @struct  Comp2
     *  comparison/ordering criteria using "Weight" and "To" fields
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   27/01/2002
     */
    struct Comp2 {
      /** comparison/ordering criteria
       *  @param  entry1 the first entry
       *  @param  entry2 the secons entry
       *  @return true   if "Weight" field of the first entry is less
       *                 and "To" fields are equal!
       */
      bool operator()( const Entry& entry1, const Entry& entry2 ) const {
        return Equal()( entry1, entry2 ) && !Less2()( entry1, entry2 );
      }
    };
    // ========================================================================
  public:
    // ========================================================================
    /// retrive all relations from the given object
    auto i_relations( From_ object ) const {
      return std::equal_range( m_entries.begin(), m_entries.end(), Entry( object ), Less1() );
    };
    /// retrive ALL relations from the ALL objects
    auto i_relations() const { return std::pair{m_entries.begin(), m_entries.end()}; };
    /// retrive all relations from the object which has weigth
    auto i_relations( From_ object, Weight_ threshold, const bool flag ) const {
      // find all relations from the given object
      auto ip = i_relations( object );
      // no relations are found !!!
      if ( ip.second == ip.first ) { return ip; } // RETURN !!!
      // find the appropriate relations
      Entry entry;
      entry.m_from   = object;
      entry.m_weight = threshold;
      auto it        = std::lower_bound( ip.first, ip.second, entry, Less2() );
      return flag ? std::pair{it, ip.second} : std::pair{ip.first, it};
    }
    /// retrive all relations from the object which has weigth
    auto i_inRange( From_ object, Weight_ low, Weight_ high ) const {
      if ( low > high ) { return std::pair<typename Entries::iterator, typename Entries::iterator>{}; }
      // find all relations from the given object
      auto ip = i_relations( object );
      // no relations are found !!!
      if ( ip.second == ip.first ) return ip; // RETURN !!!
      // find the appropriate relations
      Entry entry( object );
      entry.m_weight = low;
      auto it1       = std::lower_bound( ip.first, ip.second, entry, Less2() );
      entry.m_weight = high;
      auto it2       = std::lower_bound( it1, ip.second, entry, Less2() );
      return std::pair{it1, it2};
    }
    /// make the relation between 2 objects
    StatusCode i_relate( From_ object1, To_ object2, Weight_ weight ) {
      const Entry entry( object1, object2, weight );
      return i_add( entry );
    }
    /// add the entry
    StatusCode i_add( const Entry& entry ) {
      // get all existing relations from object1
      auto ip = i_relations( entry.from() );
      // does the given relation between object1 and object2 exist ?
      auto it = std::find_if( ip.first, ip.second, [&]( const Entry& lhs ) { return Equal()( lhs, entry ); } );
      if ( ip.second != it ) { return StatusCode( StatusCode::FAILURE, true ); } // RETURN !!!
      // find the place where to insert the relation and insert it!
      it = std::lower_bound( ip.first, ip.second, entry, Less2() );
      m_entries.insert( it, entry );
      return StatusCode::SUCCESS;
    }
    /// remove the concrete relation between objects
    StatusCode i_remove( From_ object1, To_ object2 ) {
      // get all existing relations form object1
      auto ip = i_relations( object1 );
      // does the given relation between object1 and object1 exist ?
      auto it = std::find_if( ip.first, ip.second, [rhs = Entry( object1, object2 )]( const Entry& entry ) {
        return Equal()( entry, rhs );
      } );
      if ( ip.second == it ) { return StatusCode( StatusCode::FAILURE, true ); } // RETURN !!!
      // remove the relation
      m_entries.erase( it );
      return StatusCode::SUCCESS;
    }
    /// remove all relations FROM the defined object
    StatusCode i_removeFrom( From_ object ) {
      // get all existing relations form object1
      auto ip = i_relations( object );
      // no relations are found !!!
      if ( ip.second == ip.first ) { return StatusCode( StatusCode::FAILURE, true ); } // RETURN !!!
      // remove relations
      m_entries.erase( ip.first, ip.second );
      return StatusCode::SUCCESS;
    }
    /// remove all relations TO the defined object
    StatusCode i_removeTo( To_ object ) {
      // create the artificial entry
      Entry entry;
      entry.m_to = object;
      auto it    = std::remove_if( m_entries.begin(), m_entries.end(),
                                [&]( const Entry& lhs ) { return Equal()( lhs, entry ); } );
      // no relations are found!
      if ( m_entries.end() == it ) { return StatusCode( StatusCode::FAILURE, true ); } // RETURN !!
      // remove relations
      m_entries.erase( it, m_entries.end() );
      return StatusCode::SUCCESS;
    }
    /** filter out the relations FROM the defined object, which
     *  have a weight larger(smaller)than the threshold weight
     */
    StatusCode i_filterFrom( From_ object, Weight_ threshold, const bool flag ) {
      // get all relations from the object over/under the threshold
      auto ip = i_relations( object, threshold, !flag );
      // no relations are found!
      if ( ip.second == ip.first ) { return StatusCode( StatusCode::FAILURE, true ); } // RETURN !!!
      // erase relations
      m_entries.erase( ip.first, ip.second );
      return StatusCode::SUCCESS;
    }
    /** filter out the relations TO the defined object, which
     *  have a weight larger/smaller than the threshold weight
     */
    StatusCode i_filterTo( To_ object, Weight_ threshold, const bool flag ) {
      Entry entry;
      entry.m_to     = object;
      entry.m_weight = threshold;
      // remove using predicates
      auto it = flag ? std::remove_if( m_entries.begin(), m_entries.end(),
                                       [&]( const Entry& lhs ) { return Comp1()( lhs, entry ); } )
                     : std::remove_if( m_entries.begin(), m_entries.end(),
                                       [&]( const Entry& lhs ) { return Comp2()( lhs, entry ); } );
      // nothing to be removed
      if ( m_entries.end() == it ) { return StatusCode( StatusCode::FAILURE, true ); } // RETURN !!!
      // erase the relations
      m_entries.erase( it, m_entries.end() );
      return StatusCode::SUCCESS;
    }
    /** filter out all relations which
     *  have a weight larger/smaller than the threshold weight
     */
    StatusCode i_filter( Weight_ threshold, const bool flag ) {
      Entry entry;
      entry.m_weight = threshold;
      // remove using the predicates
      auto it = flag ? std::remove_if( m_entries.begin(), m_entries.end(),
                                       [&]( const Entry& lhs ) { return Less2()( lhs, entry ); } )
                     : std::remove_if( m_entries.begin(), m_entries.end(),
                                       [&]( const Entry& rhs ) { return Less2()( entry, rhs ); } );
      // nothing to be removed
      if ( m_entries.end() == it ) { return StatusCode( StatusCode::FAILURE, true ); } // RETURN
      // erase the relations
      m_entries.erase( it, m_entries.end() );
      return StatusCode::SUCCESS;
    }
    /// remove ALL relations from ALL objects to ALL objects
    StatusCode i_clear() {
      m_entries.clear();
      return StatusCode::SUCCESS;
    }
    /// reserve the space for relations
    StatusCode i_reserve( const size_t num ) {
      Relations::reserve( m_entries, num );
      return StatusCode::SUCCESS;
    }
    /** make the relation between 2 objects, but brute force.
     *  The relation entry in *APPENDED* to the end
     *  of the internal container, keeping it in INVALID state
     *
     *  This method is added to provide more
     *  efficient fill for Relation tables.
     *
     *  The subsequent call of "sort" is MANDATORY!
     *
     *  @param  object1 smart reference to the first object
     *  @param  object2 smart reference to the second object
     *  @param  weight  weigth for the relation
     */
    void i_push( From_ object1, To_ object2, Weight_ weight ) {
      m_entries.push_back( Entry( object1, object2, weight ) );
    }
    /** (re)sort the whole underlying container
     *  Call for this method is MANDATORY after usage of i_push
     */
    void i_sort() { std::stable_sort( m_entries.begin(), m_entries.end(), Less() ); }
    /// standard/default constructor
    RelationWeightedBase( const size_t reserve = 0 ) : BaseWeightedTable(), m_entries() {
      if ( 0 < reserve ) { i_reserve( reserve ).ignore(); }
    }
    /// destructor (virtual)
    virtual ~RelationWeightedBase() = default;
    /// constructor from any "direct" interface
    RelationWeightedBase( const IDirect& copy ) : BaseWeightedTable(), m_entries() {
      auto r = copy.relations();
      m_entries.insert( m_entries.end(), r.begin(), r.end() );
    }
    /** constructor from any inverse interface
     *  @param inv relations to be inverted
     *  @param int artificial agument to make the difference
     *         for stupid MicroSoft compiler
     */
    RelationWeightedBase( const IInverse& inv, const int /* flag*/ ) : BaseWeightedTable(), m_entries() {
      // get all relations from "inv"
      auto r = inv.relations();
      // reserve the space for relations
      i_reserve( r.size() ).ignore();
      // invert all relations
      for ( const auto& entry : r ) i_push( entry.to(), entry.from(), entry.weight() );
      // final sort
      i_sort();
    }
    /// copy constructor
    RelationWeightedBase( const OwnType& copy ) = default;
    // ========================================================================
  public:
    // ========================================================================
    /** add *SORTED* range into the relation table
     *  @see std::merge
     *  @param range the range to be added
     *  @return self reference
     */
    RelationWeightedBase& merge( const Range& range ) {
      if ( range.empty() ) { return *this; }
      //
      switch ( range.size() ) {
      case 1:
        //
        i_add( range[0] ).ignore();
        break; // BREAK
        //
      case 2:
        //
        i_add( range[0] ).ignore();
        i_add( range[1] ).ignore();
        break; // BREAK
        //
      case 3:
        //
        i_add( range[0] ).ignore();
        i_add( range[1] ).ignore();
        i_add( range[2] ).ignore();
        break; // BREAK
        //
      case 4:
        //
        i_add( range[0] ).ignore();
        i_add( range[1] ).ignore();
        i_add( range[2] ).ignore();
        i_add( range[3] ).ignore();
        break; // BREAK
        //
      default:
        //
        if ( range.size() > 0.1 * m_entries.size() ) {
          Entries tmp( m_entries.size() + range.size() );
          std::merge( m_entries.begin(), m_entries.end(), range.begin(), range.end(), tmp.begin(), Less() );
          // use std::swap instead of assignement
          std::swap( m_entries, tmp );
        } else {
          for ( const auto& entry : range ) this->i_add( entry ).ignore();
        }
      } // end of switch
      //
      return *this;
    }
    // ========================================================================
    /** add 'inverse' range into the relation table
     *  @param range the range to be added
     *  @return self reference
     */
    RelationWeightedBase& imerge( const typename IInverse::Range& range ) {
      if ( range.empty() ) { return *this; } // RETURN
      // invert all relations
      i_reserve( m_entries.size() + range.size() ).ignore();
      //
      switch ( range.size() ) {
      case 1:
        //
        i_relate( range[0].to(), range[0].from(), range[0].weight() ).ignore();
        break;
        //
      case 2:
        //
        i_relate( range[0].to(), range[0].from(), range[0].weight() ).ignore();
        i_relate( range[1].to(), range[1].from(), range[1].weight() ).ignore();
        break;
        //
      case 3:
        //
        i_relate( range[0].to(), range[0].from(), range[0].weight() ).ignore();
        i_relate( range[1].to(), range[1].from(), range[1].weight() ).ignore();
        i_relate( range[2].to(), range[2].from(), range[2].weight() ).ignore();
        break;
        //
      case 4:
        //
        i_relate( range[0].to(), range[0].from(), range[0].weight() ).ignore();
        i_relate( range[1].to(), range[1].from(), range[1].weight() ).ignore();
        i_relate( range[2].to(), range[2].from(), range[2].weight() ).ignore();
        i_relate( range[3].to(), range[3].from(), range[3].weight() ).ignore();
        break;
        //
      default:
        //
        if ( range.size() > 0.1 * m_entries.size() ) {
          for ( const auto& entry : range ) i_push( entry.to(), entry.from(), entry.weight() );
          i_sort();
        } else {
          for ( const auto& entry : range ) i_relate( entry.to(), entry.from(), entry.weight() ).ignore();
        }
        //
      }
      //
      return *this; // RETURN
    }
    // ========================================================================
    /** add *SORTED* range into the relation table
     *  @see std::merge
     *  @param range the range to be added
     *  @return self reference
     */
    RelationWeightedBase& operator+=( const Range& range ) { return merge( range ); }
    // ========================================================================
  private:
    // ========================================================================
    /// the actual storage of relation links
    mutable Entries m_entries; // the actual storage of relation links
    // ========================================================================
  };
  // ==========================================================================
} //  end of namespace Relations
// ============================================================================
// The End
// ============================================================================
#endif // RELATIONS_RELATIONWeightedBASE_H
