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
#ifndef LOKI_Combiner_H
#define LOKI_Combiner_H 1

#include "GaudiKernel/Range.h"

#include <algorithm>
#include <functional>
#include <vector>

// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class Combiner_ Combiner.h LoKi/Combiner.h
   *
   *  Definition of (multy) container with (multy)iterator
   *  Helpful class for implemenattion of variouse looping
   *  techniques
   *
   *  It is in the spirit of former CLHEP Chooser/Combiner_ class,
   *  A. Alexandresku's Loki library and and T. Glebe's (Hera-B)
   *  GCombiner package
   *
   *  The class allows combine "everything" - the content of the
   *  containers is irelevant, it could be objects, pointers, etc.,
   *  even the primitive scalars are allowed.
   *  E.g the examples below shows the loop over all unique
   *  combinations of integers from 2 containers ("double loop").
   *
   *  @code
   *
   *
   *  void  example ()
   *  {
   *
   *  using namespace LoKi;
   *  typedef std::vector<int> Vect;
   *
   *  typedef Combiner_<Vect>  Comp;
   *
   *  typedef Range_<Vect>     Range;
   *
   *  int a1[] = { 1  , 2 , 3 , 4 , 5   };
   *  int a2[] = { 6  , 7 , 8 , 9 , 10  };
   *
   *  Vect v1( a1 , a1 + 5 ) ;
   *  Vect v2( a2 , a2 + 5 ) ;
   *
   *  Comp Combiner;
   *
   *  Range r1( v1.begin() , v1.end() ) ;
   *  Range r2( v2.begin() , v2.end() ) ;
   *
   *  Combiner.add( r1 ) ;
   *  Combiner.add( r2 ) ;
   *
   *  std::cout
   *    << " Combiner parameters "
   *    << " dim  = "       << Combiner.dim()
   *    << " total size = " << Combiner.size() << std::endl ;
   *
   *  int unique = 0 ;
   *  for ( ; Combiner.valid() ; ++Combiner  )
   *  {
   *    if( !Combiner.unique() ) { continue ; }
   *
   *    const Comp::Select& sel = Combiner.current() ;
   *    std::cout
   *      << "  Unique combination #"
   *      << ++unique << " \t selected : \t" ;
   *    for ( Comp::Select::const_iterator it = sel.begin() ;
   *           sel.end() != it ; ++it )
   *    { std::cout << (**it) << " \t " ; }
   *    //
   *    std::cout << std::endl;
   *  }
   *
   *}; // end of the function exmaple
   *
   *  @endcode
   *
   *  @attention The input data are not ownered by
   *  combiner, it operates only with iterators !
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-11
   */
  template <class CONTAINER>
  class Combiner_ {
  public:
    // ========================================================================
    /// type for container itself
    typedef CONTAINER Container;
    /// type for effective range
    typedef Gaudi::Range_<Container> Range;
    /// iterator type
    typedef typename Range::const_iterator iterator;
    /// definition of (multy)iterator
    typedef std::vector<iterator> Select;
    /// type for back-up
    typedef std::pair<Select, size_t> BackUp;
    /// container of back-ups
    typedef std::vector<BackUp> BackUps;
    // ========================================================================
  private:
    // ========================================================================
    /// the Combiner_ itself
    typedef std::vector<Range> Ranges;
    /// iterator over selected combination
    typedef typename Select::iterator it;
    /// iterator over selected combination (const version)
    typedef typename Select::const_iterator cit;
    /// iterator over the Combiner_
    typedef typename Ranges::iterator rit;
    /// iterator over the Combiner_ (cont version)
    typedef typename Ranges::const_iterator rcit;
    // ========================================================================
  public:
    // ========================================================================
    /** default constructor, constructs empty Combiner_
     *  the state in "invalid"!
     */
    Combiner_() : m_size( 0 ), m_index( 0 ) {
      backup(); // make the backup of initial empty state
    }
    // ========================================================================
    /// destructor
    ~Combiner_() { clear(); } // destructor
    // ========================================================================
    /// current dimentions (=number of components)  of the Combiner_
    size_t dim() const { return m_ranges.size(); }
    // ========================================================================
    /// current size of the Combiner_  (include ALL combinations)
    size_t size() const { return m_size; }
    // ========================================================================
    /** add one more "container" to the existing Combiner_.
     *  @param range  the sequence range
     *  return self reference
     */
    Combiner_& add( const Range& range ) {
      // extend the Combiner_
      m_ranges.push_back( range );
      m_current.push_back( range.begin() );
      m_size = 1;
      for ( rit r = m_ranges.begin(); m_ranges.end() != r; ++r ) { m_size *= r->size(); }
      // reset current  (multy) iterator
      reset();
      // save the state
      m_backups.clear();
      backup();
      //
      return *this;
    }
    // ========================================================================
    /** pre-increment operator for the Combiner_ (advance current iterator)
     *  @return self reference
     */
    Combiner_& operator++() {
      /// advance the 'current' iterator
      next();
      return *this;
    }
    // ========================================================================
    /** post-increment operator for the Combiner_ (advance current iterator)
     *  @attention the same as pre-increment
     *  @return self reference
     */
    Combiner_& operator++( int ) { return ++( *this ); }
    // ========================================================================
    /** clear the Combiner
     *  @return self reference
     */
    Combiner_& clear() {
      // clear the Combiner
      m_ranges.clear();
      // clear all (multy)iterators
      m_current.clear();
      // size
      m_size = 0;
      // index
      m_index = 0;
      // clear the backups
      m_backups.clear();
      backup();
      // return
      return *this;
    }
    // ========================================================================
    /** reset current (multy)iterator
     *  @return self-reference
     */
    Combiner_& reset() {
      // reset current  (multi) iterator
      std::transform( m_ranges.begin(), m_ranges.end(), m_current.begin(), []( const Range& r ) { return r.begin(); } );
      // reset the current index
      m_index = 0;
      // return
      return *this;
    }
    // ========================================================================
    /** access to the current (multy)iterator (const version)
     *  @return 'current' (multy)iterator
     */
    const Select& current() const { return m_current; }
    // ========================================================================
    /** end
     *  @return true if there is no more combinatios
     */
    bool end() const { return !( m_index < m_size ); }
    // ========================================================================
    /** check the validity of current (multi)iterator
     *  @return true if no "actual" sub-iterators are invalid
     */
    bool valid() const { return m_size != 0 && m_index < m_size; }
    // ========================================================================
    /** advance 'current' (multi)iterator.
     *  I guess it the most tricky funtion of the whole class.
     *  It is the most primitive one, but I've spent few hours
     *  trying to debug it :-)
     *  @return 'current' (multi)iterator after the advance
     */
    inline const Select& next() {
      // valid index?
      if ( m_index < m_size ) { ++m_index; }
      // the last combination
      if ( m_index == m_size ) { return invalidate(); }
      // evaluate the value of the current iterator from the current index
      size_t prev  = 1;
      auto   range = m_ranges.begin();
      auto   curr  = m_current.begin();
      for ( ; m_current.end() != curr; ++curr, ++range ) {
        size_t index = ( m_index / prev ) % range->size();
        prev *= range->size();
        *curr = std::next( range->begin(), index );
      }
      // the end
      return m_current;
    }
    // ========================================================================
  public:
    // ========================================================================
    /// copy the content of current iterator (with dereferencing!)
    template <class OUTPUT>
    OUTPUT current( OUTPUT out ) const {
      // copy only the valid combinations
      if ( !valid() ) { return out; }
      std::transform( m_current.begin(), m_current.end(), out, []( const iterator& i ) { return *i; } );
      return out; // RETURN
    }
    // ========================================================================
    /** check the validity/uniquiness/ordering of the objects using the external
     *  comparison criteria
     *  @param cmp the comparions criteria
     */
    template <class CMP>
    bool unique( const CMP& cmp ) const {
      // check only "valid" combinations
      if ( !valid() ) { return false; }
      // double loop over the combination
      for ( auto it1 = m_current.begin(); m_current.end() != it1; ++it1 ) {
        for ( auto it2 = std::next( it1 ); m_current.end() != it2; ++it2 ) {
          if ( !cmp( **it1, **it2 ) ) { return false; } // RETURN
        }
      }
      return true;
    }
    // ========================================================================
    /** unique
     *  @return true if current combination  is 'unique'
     *  @attention ut has no sence fro invalid combinations!
     */
    bool unique() const {
      // get the proper uniquiness criteria:
      return unique( std::not_equal_to<typename CONTAINER::value_type>() );
    }
    // ========================================================================
  public:
    // ========================================================================
    /** save ('back-up')the current state of the combiner
     *  (could be restored later)
     *  @return number of backups
     */
    size_t backup() {
      m_backups.emplace_back( m_current, m_index );
      return m_backups.size();
    }
    /** restore the combiner from backup
     *  The method could be used several
     *  times to go to previous-previous-...-previous state
     *  @return self-reference
     */
    Combiner_& restore() {
      // restore the current iterator
      m_current = m_backups.back().first;
      // restore the current index
      m_index = m_backups.back().second;
      // remove the last backup state
      if ( m_backups.size() > 1 ) { m_backups.pop_back(); }
      // return
      return *this;
    }
    // ========================================================================
  private:
    // ========================================================================
    /** invalidate the current iterator (and index)
     *  @return corrent iterator
     */
    const Select& invalidate() {
      // invalidate the current index
      m_index = m_size;
      // invalidate current  (multy) iterator to global 'end' position
      std::transform( m_ranges.begin(), m_ranges.end(), m_current.begin(), []( const Range& r ) { return r.end(); } );
      // return invalid iterator
      return m_current;
    }
    // ========================================================================
  private:
    // ========================================================================
    /// Combiner_ itself
    Ranges m_ranges; // Combiner_ itself
    /// "current" iterator
    Select m_current; // "current" iterator
    /// total number of combinations
    size_t m_size; // total number of combinations
    /// index of current combination
    size_t m_index; // index of current combination
    /// 'State' of the combiner
    BackUps m_backups; // 'State' of the combiner
    // ========================================================================
  }; //                                                  end of class Combiner_
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_Combiner_H
