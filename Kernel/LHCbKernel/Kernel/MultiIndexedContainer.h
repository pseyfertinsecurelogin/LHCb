#ifndef  MULTIINDEXEDCONTAINER_H
#define  MULTIINDEXEDCONTAINER_H 1

#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>

// Range V3
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

#include "GaudiKernel/Range.h"

namespace LHCb
{
namespace Container
{

/**
 * Desirably efficient container of hits, in effect a
 * smarter wrapper around a 1D vector and an array of offsets.
 *
 * In effect, the collection of Hits* for the
 * n-dimensional detector geometry* elements is mapped to a
 * 1-dimensional vector of Hits*, together with an array
 * of offsets. Sorting by a general comparison function is
 * supported.
 *
 * Everything highlighted with the asterisk is a template argument
 * for the MultiIndexedContainer.
 *
 * Example use-cases are the PrFTHitHandler and the PrUTHitHandler.
 * In general, you want Hit Handlers to be the front-end to this
 * container.
 *
 * Numbering always starts at 0.
 *
 * TODO:
 *  - Write a story about why there is some special behaviour for the
 * single-hit insertion, such that an explicit sort *must* be called
 * in that situation.
 *
 *  - Main concerns: Laurent thinks it's probably beneficial to add a trivial
 * implementation of the getUniqueDetectorId for the 1D-case, which just
 * is the identity function. But be wary, as a 1D-call to the
 * getHits for a nested subdetector gives you all hits for the children
 * subdetectors, while the speed gain is really only in the 1D-Call/1D detector
 * case.
 *
 *  - Right now there is an assert for the number of arguments and the nesting of
 * subdetectors. However, there is no assert whether the sub-detector ID might
 * exceed the
 *
 *  - Support addHit([subdetectorId], hit) instead of just addHit(hit, [subdetectorId])
 *
 * @author  Roel Aaij <roel.aaij@cern.ch>,
 * 			Laurent Dufour <laurent.dufour@cern.ch>,
 * 			Renato Quagliani <Renato.Quagliani@cern.ch>,
 * 			Gerhard Raven <gerhard.raven@nikhef.nl>
 */


namespace detail {
    template <size_t ... J, size_t ... I>
    constexpr auto prod(std::index_sequence<I...>)
    {
        return ( ... * std::get<I>(std::array{J...}) );
    }

    template <size_t N, size_t ... I>
    constexpr auto pivot(std::index_sequence<I...>)
    {
        return std::index_sequence< N-I ... >{};
    }

    template <size_t ... Extent >
    struct Extents
    {
        template <size_t Dim>
        static constexpr int size() { return std::get<Dim>( std::array{ Extent... } ); }

        template <int M>
        static constexpr int stride() {
            constexpr auto N = sizeof...(Extent);
            static_assert(M<N);
            if constexpr (M==N-1) {
               return 1;
            } else {
               return prod<Extent...>(pivot<N-1>( std::make_index_sequence<(N-1)-M>()));
            }
        }

        template <typename T, size_t N, size_t ... Is >
        static constexpr int index_offset( std::array<T,N> i, std::index_sequence<Is...>)
        {
            return ( ... + (std::get<Is>(i)*stride<Is>()) );
        }

        template <typename ... Is>
        static constexpr int offset( Is ... is )
        {
            return index_offset( std::array{ is... }, std::index_sequence_for<Is...>{} );
        }
    };
}

template<class Hit, size_t... sizes>
class MultiIndexedContainer
{
public:
  using offset_t = unsigned short;
  using const_reference = const Hit&;
  using value_type = Hit;

  using Hits = typename std::vector<value_type>;
  using iterator = typename Hits::iterator;
  using const_iterator = typename Hits::const_iterator;
  using const_reverse_iterator = typename Hits::const_reverse_iterator;

  /**
   * For the offsets we reserve one more, to make the calculation of the
   * number of hits quicker (can always do n(uniqueSubDetId).second - n(uniqueSubDetId).first )
   */
  using Offsets = std::array<std::pair<offset_t, offset_t>, ( ...*sizes )  >;
  using Ids = std::array<size_t, ( ... * sizes ) >;

  using HitRange=Gaudi::Range_<MultiIndexedContainer<Hit, sizes...> >;

  /**
   * Constructor for a relatively empty hit container.
   */
  MultiIndexedContainer(size_t reserve = 0)
  {
    if (reserve != 0) m_hits.reserve ( reserve );
    clearOffsets();
    std::fill ( begin(m_nIds), end(m_nIds), 0u);
  }

  MultiIndexedContainer(std::vector<Hit>&& hits, Offsets&& offsets)
  : m_hits(std::move(hits)), m_offsets(std::move(offsets))
  {
  }

  void reserve( size_t reserve )
  {
    m_hits.reserve ( reserve );
  }

  template<typename ... Args>
  std::pair<iterator, iterator> range_( Args&&...args )
  {
    auto rng = getOffset(std::forward<Args>(args)...);
    return { std::next( std::begin(m_hits), rng.first ),
             std::next( std::begin(m_hits), rng.second )};
  }

  template<typename ... Args>
  HitRange range ( Args&&...args ) const
  {
    auto rng = getOffset(std::forward<Args>(args)...);
    return { std::next( begin(m_hits), rng.first ),
             std::next( begin(m_hits), rng.second )};
  }

  /**
   * Returns a range of all the hits in this container.
   *
   * The sorting of this range depends on the possibly
   * used sort() method, but is always segmented
   * in unique detector ids.
   */
  HitRange range() const
  {
    return {std::begin(m_hits), std::end(m_hits)};
  }

  Hit& hit(size_t index)
  {
    return m_hits[index];
  }

  const Hit& hit(size_t index) const
  {
    return m_hits[index];
  }

  /**
   * Returns the total number of hits in this container.
   */
  size_t size() const
  {
    return m_hits.size ();
  }

  /**
   * Returns the number of hits in this container for a given
   * sub-detector.
   */
  template<typename ... Args>
  constexpr size_t size(Args&&...args) const
  {
    auto [ obegin, oend] = getOffset(std::forward(args)...);
    assert(oend >= obegin && "ill-formed offsets");
    return oend - obegin;
  }

  template<typename ... Args>
  constexpr bool empty(Args&&...args) const
  {
    auto [ obegin, oend ] = getOffset(std::forward(args)...);
    assert(oend >= obegin && "ill-formed offsets");
    return oend == obegin;
  }

  static constexpr size_t nSubDetectors() { return ( ... * sizes ); }

  template <size_t N>
  static constexpr size_t subSize() { return std::get<N>( std::array{ sizes... } ); }

  /**
   * Function to insert a range of hits in (detectorElementId).
   *
   * The argument are two r-valued references, but in practice this will
   * take its form as two iterators over the hits to be inserted.
   *
   * The responsibility of passing these iterators and generating the
   * hits to be inserted is found in the HitManagers
   * (e.g. {@see TStationHitManager::i_prepareHits}).
   *
   * Current practice: give a range instead of the raw begin() and end().
   *
   */
  template<typename I, typename ... Args>
  void insert(I&& b, I&& e, Args&& ... args)
  {
    auto n = std::distance( b, e );
    m_hits.reserve( m_hits.size () + n );

    const auto subDetectorId = getUniqueDetectorElementId( std::forward<Args>(args)... );

    assert( std::none_of( std::next ( begin(m_offsets), subDetectorId + 1 ),
                          std::end ( m_offsets ),
                         [](typename Offsets::value_type& offset){
                           return offset.first != offset.second; })
            && "Values can only be inserted at the back; not in the middle.");

    // Only initialize offsets when they are not yet filled
    if( m_offsets[subDetectorId].first == m_offsets[subDetectorId].second ) {
      m_offsets[subDetectorId].first = m_offsets[subDetectorId].second = m_hits.size();
    }

    m_hits.insert( std::next( begin( m_hits ), m_offsets[subDetectorId].second ),
                   std::forward<I>( b ), std::forward<I>( e ) );

    // Add number of entries to the end offset
    m_offsets[subDetectorId].second += n;
  }

  /**
   * Function to create a single hit in (detectorElementId).
   * A reference to the hit created is returned.
   *
   * The user of this function is responsible for giving the right
   * constructor arguments. Unexpected errors can occur when this does not
   * match the constructor of Hit.
   *
   * Note that due to the implementation of the addHit algorithm,
   * one should explicitly call setOffsets before further using the
   * hit manager.
   */
  template<typename Tuple, typename ... LocArgs>
  Hit& addHit(Tuple&& cargs, LocArgs&&... subDetectorElement)
  {
		static_assert((sizeof...(subDetectorElement) <= sizeof...(sizes)),
				"The number of indices provided is strictly higher than the nesting for this subdetector.");

		auto id = getUniqueDetectorElementId ( std::forward<LocArgs>(subDetectorElement)... );
#ifndef NDEBUG
		m_ids.emplace_back ( id );
#endif
		++m_nIds[id];

		return std::apply( [this]( auto&&... args ) -> decltype(auto)
		                    { return m_hits.emplace_back( std::forward<decltype(args)>(args)... ); },
			                std::forward<Tuple>( cargs ) );
	}

  void setOffsets(){
    // Set the offsets
    constexpr auto nSub = nSubDetectors();
    m_offsets[0] = { 0, m_nIds[0] };
    for (size_t i = 1; i < nSub; ++i) {
      m_offsets[i] = { m_offsets[i-1].second, m_offsets[i-1].second + m_nIds[i] };
    }
  }

  template <typename...>
  struct always_false : std::bool_constant<false> {};

  /**
   * Function used only for debug ordering check
   */
  template<typename COMPARE>
  bool is_sorted(const COMPARE& compare) const {
#ifdef NDEBUG
    static_assert(always_false<COMPARE>::value,
                  "this function can be called only on debug mode (NDEBUG should not be defined)");
    return true;
#else

    // Zip the ids and hits together
    auto zipped = ranges::view::zip(m_ids, m_hits);

    // Sorting predicate that defers hit base sorting to the compare
    // function passed in
    auto pred = [&compare](const std::tuple<unsigned, Hit>& t1,
        const std::tuple<unsigned, Hit>& t2) {
      if (std::get<0>(t1) < std::get<0>(t2)) {
        return true;
      } else if (std::get<0>(t2) < std::get<0>(t1)) {
        return false;
      } else {
        return compare(std::get<1>(t1), std::get<1>(t2));
      }
    };

    return std::is_sorted(zipped.begin(), zipped.end(), pred );
#endif
  }

  auto offsets() const
  {
    return m_offsets;
  }
private:
  std::vector<Hit> m_hits;

  Offsets m_offsets;

  /**
   * These two arrays are here to enable faster functionality
   * for adding hits one-by-one.
   */
#ifndef NDEBUG
  std::vector<unsigned> m_ids; // used only for debug
#endif
  Ids m_nIds;

  template <typename ... I>
  static constexpr int getUniqueDetectorElementId( I... i )
  {
      return detail::Extents<sizes...>::offset( i... );
  }

  template<typename ... Args>
  constexpr typename Offsets::value_type getOffset(Args&& ... args) const
  {
    auto uniqueDetectorElementId = getUniqueDetectorElementId( std::forward<Args>(args)... );
    return m_offsets[ uniqueDetectorElementId ];
  }

  void clearOffsets()
  {
    constexpr auto zero = typename Offsets::value_type{ 0, 0 };
    std::fill ( std::begin ( m_offsets ), std::end ( m_offsets ), zero );
  }
};

} // namespace Containers
} // namespace LHCb

#endif
