/*****************************************************************************\
* (c) Copyright 2020 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include <algorithm>
#include <iterator>

namespace LHCb::cxx {

  namespace details {
    // this version works for forward iterators, loops simpler, but swaps up to N-1 times
    template <typename Iterator, typename Predicate>
    Iterator partitionPosition( Iterator first, Iterator last, Predicate pred, std::forward_iterator_tag ) {
      for ( ; first != last && pred( first ); ++first )
        ;
      if ( first == last ) return first;
      for ( auto i = std::next( first ); i != last; ++i ) {
        if ( pred( i ) ) {
          std::iter_swap( i, first ); // the 'bulldozer` approach...
          ++first;
        }
      }
      return first;
    }
    // this version requires bidirectional iterators, and does the minimal number of swaps
    // (namely N/2) at the cost of more complex looping / memory access patterns...
    template <typename Iterator, typename Predicate>
    Iterator partitionPosition( Iterator first, Iterator last, Predicate pred, std::bidirectional_iterator_tag ) {
      for ( ; first != last && pred( first ); ++first )
        ; /* empty on purpose */
      for ( ; std::distance( first, last ) > 1 && !pred( std::prev( last ) ); --last )
        ; /* empty on purpose */
      if ( std::distance( first, last ) < 2 ) return first;
      std::iter_swap( first++, --last );
      return partitionPosition( first, last, pred, std::bidirectional_iterator_tag{} );
    }
  } // namespace details

  // Reorders the elements in the range [first, last) in such a way that
  // all elements for which the predicate p returns true precede the
  // elements for which predicate p returns false.
  // Relative order of the elements is preserved
  //
  // The difference with `std::stable_partition` is that here, the
  // predicate is given the _position_ of the item as argument, and
  // not the item itself. This allows to partition based on eg. an
  // external bitset
  //
  // See the unit test for an example...
  //
  // adapted from https://sean-parent.stlab.cc/presentations/2015-09-23-data-structures/data-structures.pdf,
  // page 31
  //
  // TODO??: add constraint that pred(first) must return a bool?
  //
  template <typename Iterator, typename Predicate>
  Iterator stablePartitionPosition( Iterator first, Iterator last, Predicate pred ) {
    auto n = std::distance( first, last );
    if ( n == 0 ) return first;
    if ( n == 1 ) return first + pred( first );
    auto middle = std::next( first, n / 2 );
    return std::rotate( stablePartitionPosition( first, middle, pred ), middle,
                        stablePartitionPosition( middle, last, pred ) );
  }

  // Reorders the elements in the range [first, last) in such a way that
  // all elements for which the predicate p returns true precede the
  // elements for which predicate p returns false.
  // Relative order of the elements is _not_ preserved
  //
  // The difference with `std::partition` is that here, the
  // predicate is given the _position_ of the item as argument, and
  // not the item itself. This allows to partition based on eg. an
  // external bitset
  template <typename Iterator, typename Predicate>
  Iterator partitionPosition( Iterator first, Iterator last, Predicate pred ) {
    return details::partitionPosition( first, last, pred,
                                       typename std::iterator_traits<Iterator>::iterator_category{} );
  }

  // Reorders the elements in `range` in such a way that
  // all elements selected by `mask` precede the
  // elements not selected by `mask`.
  // Relative order of the elements is preserved
  //
  template <typename Range, typename Mask>
  auto stablePartitionByMask( Range range, Mask mask ) {
    using std::begin;
    using std::end;
    auto first = begin( range );
    auto pivot =
        stablePartitionPosition( first, end( range ), [&]( auto i ) { return mask[std::distance( first, i )]; } );
    return std::distance( first, pivot );
  }

  // Reorders the elements in `range` in such a way that
  // all elements selected by `mask` precede the
  // elements not selected by `mask`.
  // Relative order of the elements is _not_ preserved
  //
  template <typename Range, typename Mask>
  auto partitionByMask( Range range, Mask mask ) {
    using std::begin;
    using std::end;
    auto first = begin( range );
    auto pivot = partitionPosition( first, end( range ), [&]( auto i ) { return mask[std::distance( first, i )]; } );
    return std::distance( first, pivot );
  }

} // namespace LHCb::cxx
