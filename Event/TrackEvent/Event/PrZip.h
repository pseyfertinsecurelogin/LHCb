/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/detected.h"
#include "Kernel/HeaderMapping.h"
#include "LHCbMath/SIMDWrapper.h"
#include "SOAExtensions/ZipUtils.h"

#include <boost/mp11/algorithm.hpp>
#include <boost/type_index/ctti_type_index.hpp>
#include <tuple>
namespace LHCb::Pr {
  /** Helper for getting from container type -> proxy type. When proxies are
   *  defined, specialisations of this template should also be added so that
   *  they can be found.
   */
  template <typename>
  struct Proxy {
    /** This is only defined in the unspecialised Proxy struct, it is used to
     *  detect whether or not LHCb::Pr::Proxy<T> is explicitly defined or not.
     */
    static constexpr bool unspecialised = true;
  };
} // namespace LHCb::Pr

/** Helper macros for defining LHCb::Pr::Proxy specialisations
 */

/** Start the class declaration of a proxy type
 *
 *  Typically a proxy declaration would look something like:
 *
 *  namespace Some::Appropriate::Namespace {
 *    DECLARE_PROXY( Proxy ) {
 *      PROXY_METHODS( dType, unwrap, ContainerType, m_container );
 *      auto someField() const { return unwrap_helper<dType, unwrap>( m_container->field<dType>( this->offset() ) ); }
 *    };
 *  } // namespace Some::Appropriate::Namespace
 *
 *  REGISTER_PROXY( ContainerType, Some::Appropriate::Namespace::Proxy );
 */
#define DECLARE_PROXY( Proxy )                                                                                         \
  template <typename LHCb__Pr__MergedProxy, SIMDWrapper::InstructionSet LHCb__Pr__Proxy__simd,                         \
            bool LHCb__Pr__Proxy__unwrap__tparam>                                                                      \
  struct Proxy

/** Define methods that are common to all proxy types.
 */
#define PROXY_METHODS( dType, unwrap, Tracks, m_tracks )                                                               \
  using dType                  = typename SIMDWrapper::type_map<LHCb__Pr__Proxy__simd>::type;                          \
  static constexpr bool unwrap = LHCb__Pr__Proxy__unwrap__tparam;                                                      \
  Tracks const*         m_tracks{nullptr};                                                                             \
  Proxy( Tracks const* tracks ) : m_tracks{tracks} {}                                                                  \
  auto        offset() const { return static_cast<LHCb__Pr__MergedProxy const&>( *this ).offset(); }                   \
  auto        size() const { return m_tracks->size(); }                                                                \
  static auto mask_true() {                                                                                            \
    if constexpr ( unwrap ) {                                                                                          \
      return true;                                                                                                     \
    } else {                                                                                                           \
      return dType::mask_true();                                                                                       \
    }                                                                                                                  \
  }                                                                                                                    \
  static auto mask_false() {                                                                                           \
    if constexpr ( unwrap ) {                                                                                          \
      return false;                                                                                                    \
    } else {                                                                                                           \
      return dType::mask_false();                                                                                      \
    }                                                                                                                  \
  }

/** Register the given proxy type as being the one needed to iterate over the
 *  given key type. This must be called at global scope.
 */
#define REGISTER_PROXY( KeyType, ProxyType )                                                                           \
  template <>                                                                                                          \
  struct LHCb::Pr::Proxy<KeyType> {                                                                                    \
    template <typename MergedProxy, SIMDWrapper::InstructionSet simd, bool unwrap>                                     \
    using type = ProxyType<MergedProxy, simd, unwrap>;                                                                 \
  };                                                                                                                   \
  static_assert( LHCb::Pr::is_zippable_v<KeyType>, "REGISTER_PROXY used with a non-zippable key type." )

namespace LHCb::Pr::detail {
  /** Helper to determine if the given type has a reserve( std::size_t ) method
   */
  template <typename T>
  using has_reserve_ = decltype( std::declval<T>().reserve( std::size_t{} ) );

  template <typename T>
  inline constexpr bool has_reserve_v = Gaudi::cpp17::is_detected_v<has_reserve_, T>;

  /** Helper to deterimine if the given type has a .size() method
   */
  template <typename T>
  using has_size_ = decltype( std::declval<T>().size() );

  template <typename T>
  inline constexpr bool has_size_v = Gaudi::cpp17::is_detected_v<has_size_, T>;

  /** Helper to deterimine if the given type has a .zipIdentifier method that
   *  returns a Zipping::ZipFamilyNumber
   */
  template <typename T>
  using has_zipIdentifier_ = decltype( std::declval<T>().zipIdentifier() );

  template <typename T>
  inline constexpr bool has_zipIdentifier_v =
      std::is_same_v<Gaudi::cpp17::detected_or_t<void, has_zipIdentifier_, T>, Zipping::ZipFamilyNumber>;

  /** Helper to determine if the given type has an LHCb::Pr::Proxy
   *  specialisation.
   */
  template <typename T>
  using proxy_is_unspecialised_ = decltype( LHCb::Pr::Proxy<T>::unspecialised );

  template <typename T>
  inline constexpr bool has_proxy_specialisation_v = !Gaudi::cpp17::is_detected_v<proxy_is_unspecialised_, T>;
} // namespace LHCb::Pr::detail

namespace LHCb::Pr {
  /** Helper that checks that the given type has .size() and .zipIdentifier()
   *  methods, and a specialisation of LHCb:Pr::Proxy<>. These are the basic
   *  criteria for make_zip() and the LHCb::Pr::Zip constructor to be
   *  considered.
   */
  template <typename T>
  inline constexpr bool       is_zippable_v =
      detail::has_size_v<T>&& detail::has_zipIdentifier_v<T>&& detail::has_proxy_specialisation_v<T>;
} // namespace LHCb::Pr

namespace LHCb::Pr::detail {
  // This is a type that we can return if we conditionally copy from all
  // elements of the zip into a single new container representing the union
  // of all of these elements
  template <typename... ContainerTypes>
  class merged_t : public ContainerTypes... {
    using index_seq           = typename std::index_sequence_for<ContainerTypes...>;
    using container_ptr_tuple = std::tuple<ContainerTypes const*...>;

    // Get the type of our I-th base class
    template <std::size_t I>
    using base_t = std::tuple_element_t<I, std::tuple<ContainerTypes...>>;

  public:
    // Adopt the convention that all containers take a ZipNumber as their
    // first constructor argument, which is the new identifier for the new
    // container, and the old instance of themself as the second argument,
    // which they should copy other data (but not container contents) from.
    // The result should be that each part of the merged_t object is copied,
    // but the size is zero
    merged_t( container_ptr_tuple const& container_ptrs )
        : merged_t( Zipping::generateZipIdentifier(), container_ptrs, index_seq{} ) {}

    template <typename dType, typename Mask>
    void copy_back( container_ptr_tuple const& container_ptrs, int offset, Mask mask ) {
      // Delegate to each part of ourself
      copy_back<dType>( container_ptrs, offset, mask, index_seq{} );
    }

    std::size_t size() const { return base_t<0>::size(); }

    void reserve( std::size_t capacity ) { reserve( capacity, index_seq{} ); }

  private:
    // Call each base class constructor:
    //   A( new_family, *a ),
    //   B( new_family, *b ),
    //   ...
    // The identifier is passed first explicitly because it changes,
    // everything else that needs to be copied can be taken from the second
    // argument
    template <std::size_t... Is>
    merged_t( Zipping::ZipFamilyNumber new_family, container_ptr_tuple const& container_ptrs,
              std::index_sequence<Is...> )
        : base_t<Is>( new_family, *std::get<Is>( container_ptrs ) )... {}

    template <typename dType, typename Mask, std::size_t... Is>
    void copy_back( container_ptr_tuple const& container_ptrs, int offset, Mask mask, std::index_sequence<Is...> ) {
      // Delegate to each part of ourself...
      ( base_t<Is>::template copy_back<dType>( *std::get<Is>( container_ptrs ), offset, mask ), ... );
    }

    template <std::size_t... Is>
    void reserve( std::size_t capacity, std::index_sequence<Is...> ) {
      ( call_reserve<Is>( capacity ), ... );
    }

    // Call reserve() on the I-th base class if it exists
    template <std::size_t I>
    void call_reserve( [[maybe_unused]] std::size_t capacity ) {
      if constexpr ( has_reserve_v<base_t<I>> ) { base_t<I>::reserve( capacity ); }
    }
  };

  // Make a new proxy type that inherits from all the component ones
  // The data structure looks something like:
  //
  //    ProxyA           ProxyB
  //  ----------       ----------
  // |    A*    |     |    B*    |
  // |    a()   |     |    b()   |
  //  ----------       ----------
  //       |               |
  //  ---------------------------
  // |           offset          |
  //  ---------------------------
  //       proxy_type<A, B>
  //
  // So the "zipped" proxy inherits from the two component proxy types, and
  // the offset (which is by construction valid into containers A and B) is
  // stored in the derived class. ProxyA and ProxyB access the offset using
  // CRTP.
  template <SIMDWrapper::InstructionSet simd, bool unwrap, typename... ContainerTypes>
  struct proxy_type
      : public Proxy<ContainerTypes>::template type<proxy_type<simd, unwrap, ContainerTypes...>, simd, unwrap>... {
    // Get the type of the first proxy type we inherit from, this lets us
    // query the size of the underlying containers via that proxy's pointer
    // to the underlying container. By construction all of the underlying
    // containers are the same length, this should be checked in the
    // constructor.
    using first_container_t = typename std::tuple_element_t<0, std::tuple<ContainerTypes...>>;
    using first_proxy_t =
        typename Proxy<first_container_t>::template type<proxy_type<simd, unwrap, ContainerTypes...>, simd, unwrap>;
    using simd_t = typename SIMDWrapper::type_map<simd>::type;
    static constexpr bool is_proxy{true};
    int                   m_offset{0};
    auto                  width() const { return simd_t::size; }
    auto                  offset() const { return m_offset; }
    auto                  loop_mask() const {
      if constexpr ( unwrap ) {
        return true;
      } else {
        return simd_t::loop_mask( m_offset, first_proxy_t::size() );
      }
    }
    // Make the static methods added by the PROXY_METHODS macro visible in
    // the zipped proxy types -- it doesn't matter which parent type we take
    // them from, the first one is simplest...
    using first_proxy_t::mask_false;
    using first_proxy_t::mask_true;

    proxy_type( ContainerTypes const*... containers, int offset )
        : Proxy<ContainerTypes>::template type<proxy_type<simd, unwrap, ContainerTypes...>, simd, unwrap>(
              containers )...
        , m_offset{offset} {}
  };

  template <typename>
  struct merged_object_helper;
} // namespace LHCb::Pr::detail

namespace LHCb::Pr {
  /** This is the type returned by LHCb::Pr::make_zip */
  template <SIMDWrapper::InstructionSet def_simd, bool def_unwrap, typename... ContainerTypes>
  class Zip {
    // Shorthand for the tuple of pointers to the underlying containers, these
    // are things like LHCb::Pr::Velo::Tracks const* and so on, which are
    // assumed to be stored on the TES and have a sufficiently long lifetime
    using container_ptr_tuple = std::tuple<ContainerTypes const*...>;

    // Storage of underlying container pointers for the iterable tracks wrapper
    container_ptr_tuple m_containers;

    // This is needed so that we can have copy constructors that re-bind the
    // default SIMD and unwrapping settings to different values.
    template <SIMDWrapper::InstructionSet, bool, typename...>
    friend class Zip;

    template <typename>
    friend struct detail::merged_object_helper;

  public:
    static constexpr auto default_simd   = def_simd;
    static constexpr bool default_unwrap = def_unwrap;
    using default_simd_t                 = typename SIMDWrapper::type_map<def_simd>::type;

    template <SIMDWrapper::InstructionSet simd, bool unwrap>
    struct Iterator {
      using value_type        = detail::proxy_type<simd, unwrap, ContainerTypes...>;
      using pointer           = value_type const*;
      using reference         = value_type const&;
      using difference_type   = int;
      using iterator_category = std::random_access_iterator_tag;
      using simd_t            = typename SIMDWrapper::type_map<simd>::type;
      container_ptr_tuple m_containers;
      int                 m_offset{0};
      Iterator( container_ptr_tuple containers, int offset ) : m_containers{containers}, m_offset{offset} {}
      auto operator*() const {
        return std::make_from_tuple<value_type>( std::tuple_cat( m_containers, std::tuple{m_offset} ) );
      }
      Iterator operator++() {
        m_offset += simd_t::size;
        return *this;
      }
      Iterator operator--() {
        m_offset -= simd_t::size;
        return *this;
      }
      Iterator& operator+=( difference_type n ) {
        m_offset += n * simd_t::size;
        return *this;
      }
      friend bool operator!=( Iterator const& lhs, Iterator const& rhs ) {
        return lhs.m_containers != rhs.m_containers || lhs.m_offset != rhs.m_offset;
      }
      friend difference_type operator-( Iterator const& lhs, Iterator const& rhs ) {
        return ( lhs.m_offset - rhs.m_offset );
      }
    };

    using value_type = detail::proxy_type<default_simd, default_unwrap, ContainerTypes...>;

    /** Construct an iterable zip of the given containers. */
    template <typename std::enable_if_t<( is_zippable_v<ContainerTypes> && ... ), int> = 0>
    Zip( ContainerTypes const&... containers ) : m_containers{&containers...} {
      // We assume that size() and zipIdentifier() can just be taken from the
      // 0th container, now's the time to check that assumption...!
      if ( !Zipping::areSemanticallyCompatible( containers... ) ) {
        throw GaudiException{"Asked to zip containers that are not semantically compatible", "LHCb::Pr::Zip",
                             StatusCode::FAILURE};
      }
      if ( !Zipping::areSameSize( containers... ) ) {
        throw GaudiException{"Asked to zip containers that are not the same size", "LHCb::Pr::Zip",
                             StatusCode::FAILURE};
      }
    }

    /** Move constructor that allows the vector and unwrapping behaviour to be
     *  changed.
     */
    template <SIMDWrapper::InstructionSet other_default_simd, bool other_default_unwrap>
    Zip( Zip<other_default_simd, other_default_unwrap, ContainerTypes...>&& other )
        : m_containers{std::move( other.m_containers )} {}

    /** Copy constructor that allows the vector and unwrapping behaviour to be
     *  changed.
     */
    template <SIMDWrapper::InstructionSet other_default_simd, bool other_default_unwrap>
    Zip( Zip<other_default_simd, other_default_unwrap, ContainerTypes...> const& other )
        : m_containers{other.m_containers} {}

    /** Get an iterator to the first element in the containers.
     *  The iterator only refers to the underlying containers, and may persist
     *  after the lifetime of the iterable zip itself (Zip) has
     *  ended.
     */
    template <SIMDWrapper::InstructionSet simd = default_simd, bool unwrap = default_unwrap>
    Iterator<simd, unwrap> begin() const {
      return {m_containers, 0};
    }

    /** Get an iterator to one past the final element in the containers.
     *  As with begin(), the iterator only refers to the underlying containers
     *  and may be used after the lifetime of the iterable zip object has ended
     */
    template <SIMDWrapper::InstructionSet simd = default_simd, bool unwrap = default_unwrap>
    Iterator<simd, unwrap> end() const {
      using simd_t = typename SIMDWrapper::type_map<simd>::type;
      // m_offset is incremented by dType::size each time, so repeatedly
      // incrementing begin() generally misses {m_tracks, m_tracks->size()}
      int num_chunks = ( size() + simd_t::size - 1 ) / simd_t::size;
      int max_offset = num_chunks * simd_t::size;
      return {m_containers, max_offset};
    }

    /** Return a proxy object referring to the given element of the underlying
     *  containers. If the iterable zip object was constructed with a vector
     *  'dType' then the proxy returned here will also yield vector outputs
     *  starting from the given offset.
     */
    template <SIMDWrapper::InstructionSet simd = default_simd, bool unwrap = default_unwrap>
    auto operator[]( int offset ) const {
      return std::make_from_tuple<detail::proxy_type<simd, unwrap, ContainerTypes...>>(
          std::tuple_cat( m_containers, std::tuple{offset} ) );
    }

    /** Get the size of the underlying container
     */
    std::size_t size() const { return std::get<0>( m_containers )->size(); }

    /** Check if the underlying container is empty
     */
    bool empty() const { return !size(); }

    /** Retrieve the zip family of the underlying container.
     */
    Zipping::ZipFamilyNumber zipIdentifier() const { return std::get<0>( m_containers )->zipIdentifier(); }

    /** Get a component of the zip.
     */
    template <typename T>
    T const& get() const {
      return *std::get<T const*>( m_containers );
    }

    /** Make a new structure by conditionally copying the underlying structures
     */
    template <SIMDWrapper::InstructionSet simd = default_simd, bool unwrap = default_unwrap, typename F>
    auto filter( F filt ) const {
      using simd_t = typename SIMDWrapper::type_map<simd>::type;
      // If the current object is a zip of multiple containers, we need to
      // return a detail::merged_t object. If there's only one container, we
      // can just return a new one of those.
      if constexpr ( sizeof...( ContainerTypes ) > 1 ) {
        // Zip contains multiple inputs, need a custom output type merging them
        // together.
        detail::merged_t out{m_containers};
        out.reserve( this->size() );
        for ( auto iter = begin<simd, unwrap>(); iter != end<simd, unwrap>(); ++iter ) {
          auto const& chunk     = *iter;
          auto        filt_mask = std::invoke( filt, chunk );
          out.template copy_back<simd_t>( m_containers, chunk.offset(), chunk.loop_mask() && filt_mask );
        }
        return out;
      } else {
        // If we're a "zip" of just one container, we can directly output a new
        // instance of that container instead of a merged_t wrapper around it
        using container_t = typename std::tuple_element_t<0, std::tuple<ContainerTypes...>>;
        auto const& old_container{*std::get<0>( m_containers )};
        container_t out{Zipping::generateZipIdentifier(), old_container};
        if constexpr ( detail::has_reserve_v<container_t> ) out.reserve( this->size() );
        for ( auto iter = begin<simd, unwrap>(); iter != end<simd, unwrap>(); ++iter ) {
          auto const& chunk     = *iter;
          auto        filt_mask = std::invoke( filt, chunk );
          out.template copy_back<simd_t>( old_container, chunk.offset(), chunk.loop_mask() && filt_mask );
        }
        return out;
      }
    }

    /** Return a copy of ourself that has a different default vector backend.
     */
    template <SIMDWrapper::InstructionSet simd>
    auto with() const {
      static_assert( !default_unwrap,
                     "with() asked to change vector backend of an unwrapping zip. This doesn't make sense." );
      return Zip<simd, default_unwrap, ContainerTypes...>{*this};
    }

    /** Return a version of ourself that behaves 'traditionally', i.e. provides
     *  scalar iteration and returns plain C++ data types from accessors.
     */
    auto unwrap() const { return Zip<SIMDWrapper::InstructionSet::Scalar, true, ContainerTypes...>{*this}; }

    /** Two zips are the same if they have the same type and they have the same
     *  set of pointers to the actual owning containers.
     */
    friend bool operator==( Zip const& lhs, Zip const& rhs ) { return lhs.m_containers == rhs.m_containers; }
  };

  namespace detail {
    template <typename T>
    struct merged_object_helper {
      using tuple_t = std::tuple<T>;
      static constexpr auto decompose( T const& x ) { return std::tie( x ); }
      static constexpr bool is_zippable_v = ::LHCb::Pr::is_zippable_v<T>;
    };

    template <typename... T>
    struct merged_object_helper<merged_t<T...>> {
      using tuple_t = std::tuple<T...>;
      static constexpr auto decompose( merged_t<T...> const& x ) { return std::tie( static_cast<T const&>( x )... ); }
      static constexpr bool is_zippable_v = ( ::LHCb::Pr::is_zippable_v<T> && ... );
    };

    template <SIMDWrapper::InstructionSet def_simd, bool def_unwrap, typename... T>
    struct merged_object_helper<::LHCb::Pr::Zip<def_simd, def_unwrap, T...>> {
      using tuple_t = std::tuple<T...>;
      // Convert std::tuple<A const*, ...> to std::tuple<A const&, ...>
      static constexpr auto decompose( ::LHCb::Pr::Zip<def_simd, def_unwrap, T...> const& x ) {
        return std::tie( *std::get<T const*>( x.m_containers )... );
      }
      // This was already a zip, so we know that the contents were zippable
      static constexpr bool is_zippable_v = true;
    };

    /** Helper to sorting a std::tuple of const references according to a
     *  different tuple type's ordering. e.g. given an instance of
     *    std::tuple<A const&, B const&>
     *  and the template parameter
     *    std::tuple<B, A>
     *  return an instance of std::tuple<B const&, A const&> populated from the
     *  given instance. This only works if the types A, B, ... are unique in
     *  each tuple.
     */
    template <typename>
    struct tuple_sort {};

    template <typename... SArgs>
    struct tuple_sort<std::tuple<SArgs...>> {
      template <typename... Args>
      constexpr static auto apply( std::tuple<Args...>&& in ) {
        return std::tuple<SArgs const&...>{std::get<SArgs const&>( std::move( in ) )...};
      }
    };

    /** Comparison of two types using Boost CTTI
     */
    template <typename T1, typename T2>
    struct ctti_sort {
      using tidx                  = boost::typeindex::ctti_type_index;
      constexpr static bool value = tidx::type_id<T1>() < tidx::type_id<T2>();
    };

    /** Do some template magic to figure get from some parameter pack Args...
     *  that could be passed to make_zip() to the sorted list of unpacked types
     *  that will be passed as a template parameter to Zip. This involves
     *  applying unpacking rules for merged_t and Zip arguments and then
     *  sorting the result using Boost CTTI.
     *
     *  e.g.
     *    sorted_t<merged_t<B, C>, A>
     *  might yield
     *    std::tuple<A, B, C>
     *  (although the actual ordering is unspecified, we rely on Boost to
     *  ensure that it is stable)
     */
    template <typename... Args>
    using sorted_t =
        boost::mp11::mp_sort<boost::mp11::mp_append<typename detail::merged_object_helper<Args>::tuple_t...>,
                             ctti_sort>;

    /** Helper for full_zip_t
     */
    template <typename>
    struct full_zip {};

    template <typename... Ts>
    struct full_zip<std::tuple<Ts...>> {
      template <SIMDWrapper::InstructionSet def_simd, bool def_unwrap>
      using type = Zip<def_simd, def_unwrap, Ts...>;
    };

    /** Deduce the return type of make_zip from a parameter pack representing
     *  its arguments. This is a relatively trivial wrapper around sorted_t.
     *  The "full" in the name is because the user is required to explicitly
     *  specify the extra template parameters def_simd and def_unwrap.
     *
     *  e.g.
     *    full_zip_t<def_simd, def_unwrap, merged_t<B, C>, A>
     *  might yield
     *    Zip<def_simd, def_unwrap, A, B, C>
     */
    template <SIMDWrapper::InstructionSet def_simd, bool def_unwrap, typename... Args>
    using full_zip_t = typename full_zip<sorted_t<Args...>>::template type<def_simd, def_unwrap>;
  } // namespace detail

  /** Construct an appropriate iterable wrapper from the given container(s)
   *
   *  e.g. transform LHCb::Pr::Velo::Tracks to an iterable wrapper around that
   *
   *  This is a helper function that avoids writing out too many explicit types
   *  and handles various special cases to try and minimise the number of
   *  different return types that we have to deal with.
   *
   *  There is some special handling in case some of the arguments are
   *  instances of merged_t. This is the type produced by filtering a zip. The
   *  special handling here ensures that `iterable( zip.filter(...) )` has the
   *  same type as `zip`.
   *
   *  There is also special handling for the case that some of the arguments
   *  are instances of LHCb::Pr::Zip. The special handling ensures that if one
   *  does
   *    auto x = LHCb::Pr::make_zip( a, b );
   *    auto y = LHCb::Pr::make_zip( x, c, d );
   *    auto z = LHCb::Pr::make_zip( a, b, c, d );
   *  then y and z should have the same type.
   *
   *  Finally, there is a step that re-orders the template parameters
   *  representing the types of a, b, etc. above so that the argument order
   *  doesn't matter. Note that given the expansion behaviour discussed just
   *  above, one could not always easily achieve this by hand.
   */
  template <SIMDWrapper::InstructionSet def_simd = SIMDWrapper::InstructionSet::Best, bool def_unwrap = false,
            typename... PrTracks,
            typename std::enable_if_t<( detail::merged_object_helper<PrTracks>::is_zippable_v && ... ), int> = 0>
  auto make_zip( PrTracks const&... tracks ) {
    // If some of PrTracks... are merged_t<T...> then try to unpack, e.g.
    // PrTracks = {A, merged_t<B, C>} (types)
    //   tracks = {a, bc}             (const&)
    // Should unpack to
    // {A, B, C}                                                     (types)
    // {a, static_cast<B const&>( bc ), static_cast<C const&>( bc )} (const&)
    //
    // (Note that merged_t<B, C> inherits from B and C)

    // Get something like std::tuple<B, A, C> that has been unpacked following
    // the rules above and sorted using CTTI. Reordering the arguments passed
    // to this function (i.e. "tracks") would not affect sorted_t.
    using sorted_t = detail::sorted_t<PrTracks...>;

    // Get the full Zip<def_simd, def_unwrap, B, A, C> type that we'll return
    using ret_t = typename detail::full_zip<sorted_t>::template type<def_simd, def_unwrap>;

    // Unpack the arguments, as specified above, into a tuple of const&. This
    // will generally not be ordered correctly (i.e. it will be something like
    // std::tuple<A const&, B const&, C const&> that doesn't match sorted_t).
    auto expanded_tracks = std::tuple_cat( detail::merged_object_helper<PrTracks>::decompose( tracks )... );

    // Reorder to make a tuple of references in the order demanded by sorted_t
    auto sorted_tracks = detail::tuple_sort<sorted_t>::apply( std::move( expanded_tracks ) );

    // Finally, construct the zip object using this sorted list of references
    return std::make_from_tuple<ret_t>( std::move( sorted_tracks ) );
  }

  // Helper to get the type of a zip of the types T...
  template <typename... T>
  using zip_t = detail::full_zip_t<SIMDWrapper::InstructionSet::Best, false, T...>;

  // Helper to get a scalar zip of the types T...
  template <typename... T>
  using scalar_zip_t = detail::full_zip_t<SIMDWrapper::InstructionSet::Scalar, false, T...>;

  // Helper to get the type of an unwrapped (scalar, plain C++ data) version
  // of zip_t<T...>
  template <typename... T>
  using unwrapped_zip_t = detail::full_zip_t<SIMDWrapper::InstructionSet::Scalar, true, T...>;
} // namespace LHCb::Pr

// Enable header lookup for non-owning zips
template <SIMDWrapper::InstructionSet def_simd, bool def_unwrap, typename... ContainerTypes>
struct LHCb::header_map<LHCb::Pr::Zip<def_simd, def_unwrap, ContainerTypes...>> {
  static constexpr auto value = ( LHCb::header_map_v<ContainerTypes> + ... ) + "Event/PrZip.h";
};

// Enable header lookup of non-owning zip proxies
template <SIMDWrapper::InstructionSet def_simd, bool def_unwrap, typename... ContainerTypes>
struct LHCb::header_map<typename LHCb::Pr::detail::proxy_type<def_simd, def_unwrap, ContainerTypes...>> {
  static constexpr auto value = ( LHCb::header_map_v<ContainerTypes> + ... ) + "Event/PrZip.h";
};