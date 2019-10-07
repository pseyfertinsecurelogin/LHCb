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
#include "LHCbMath/SIMDWrapper.h"
#include "SOAExtensions/ZipUtils.h"

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

  public:
    static constexpr auto default_simd   = def_simd;
    static constexpr bool default_unwrap = def_unwrap;
    using default_simd_t                 = typename SIMDWrapper::type_map<def_simd>::type;

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
    //     Zip<A, B>::proxy_type
    //
    // So the "zipped" proxy inherits from the two component proxy types, and
    // the offset (which is by construction valid into containers A and B) is
    // stored in the derived class. ProxyA and ProxyB access the offset using
    // CRTP.
    template <typename dType, bool unwrap>
    struct proxy_type : public Proxy<ContainerTypes>::template type<proxy_type<dType, unwrap>, dType, unwrap>... {
      // Get the type of the first proxy type we inherit from, this lets us
      // query the size of the underlying containers via that proxy's pointer
      // to the underlying container. By construction all of the underlying
      // containers are the same length, this should be checked in the
      // constructor.
      using first_container_t = typename std::tuple_element_t<0, std::tuple<ContainerTypes...>>;
      using first_proxy_t = typename Proxy<first_container_t>::template type<proxy_type<dType, unwrap>, dType, unwrap>;
      static constexpr bool is_proxy{true};
      int                   m_offset{0};
      auto                  width() const { return dType::size; }
      auto                  offset() const { return m_offset; }
      auto                  loop_mask() const {
        if constexpr ( unwrap ) {
          return true;
        } else {
          return dType::loop_mask( m_offset, first_proxy_t::size() );
        }
      }

      proxy_type( ContainerTypes const*... containers, int offset )
          : Proxy<ContainerTypes>::template type<proxy_type<dType, unwrap>, dType, unwrap>( containers )...
          , m_offset{offset} {}
    };

    template <typename dType, bool unwrap>
    struct Iterator {
      using value_type        = proxy_type<dType, unwrap>;
      using pointer           = value_type const*;
      using reference         = value_type const&;
      using difference_type   = int;
      using iterator_category = std::random_access_iterator_tag;
      container_ptr_tuple m_containers;
      int                 m_offset{0};
      Iterator( container_ptr_tuple containers, int offset ) : m_containers{containers}, m_offset{offset} {}
      auto operator*() const {
        return std::make_from_tuple<value_type>( std::tuple_cat( m_containers, std::tuple{m_offset} ) );
      }
      Iterator operator++() {
        m_offset += dType::size;
        return *this;
      }
      Iterator operator--() {
        m_offset -= dType::size;
        return *this;
      }
      Iterator& operator+=( difference_type n ) {
        m_offset += n * dType::size;
        return *this;
      }
      friend bool operator!=( Iterator const& lhs, Iterator const& rhs ) {
        return lhs.m_containers != rhs.m_containers || lhs.m_offset != rhs.m_offset;
      }
      friend difference_type operator-( Iterator const& lhs, Iterator const& rhs ) {
        return ( lhs.m_offset - rhs.m_offset );
      }
    };

    using value_type = proxy_type<default_simd_t, default_unwrap>;

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
    template <typename dType = default_simd_t, bool unwrap = default_unwrap>
    Iterator<dType, unwrap> begin() const {
      return {m_containers, 0};
    }

    /** Get an iterator to one past the final element in the containers.
     *  As with begin(), the iterator only refers to the underlying containers
     *  and may be used after the lifetime of the iterable zip object has ended
     */
    template <typename dType = default_simd_t, bool unwrap = default_unwrap>
    Iterator<dType, unwrap> end() const {
      // m_offset is incremented by dType::size each time, so repeatedly
      // incrementing begin() generally misses {m_tracks, m_tracks->size()}
      int num_chunks = ( size() + dType::size - 1 ) / dType::size;
      int max_offset = num_chunks * dType::size;
      return {m_containers, max_offset};
    }

    /** Return a proxy object referring to the given element of the underlying
     *  containers. If the iterable zip object was constructed with a vector
     *  'dType' then the proxy returned here will also yield vector outputs
     *  starting from the given offset.
     */
    template <typename dType = default_simd_t, bool unwrap = default_unwrap>
    auto operator[]( int offset ) const {
      return std::make_from_tuple<proxy_type<dType, unwrap>>( std::tuple_cat( m_containers, std::tuple{offset} ) );
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

    /** Make a new structure by conditionally copying the underlying structures
     */
    template <typename dType = default_simd_t, bool unwrap = default_unwrap, typename F>
    auto filter( F filt ) const {
      detail::merged_t out{m_containers};
      out.reserve( this->size() );
      for ( auto iter = begin<dType, unwrap>(); iter != end<dType, unwrap>(); ++iter ) {
        auto const& chunk     = *iter;
        auto        loop_mask = chunk.loop_mask();
        auto        filt_mask = std::invoke( filt, chunk );
        out.template copy_back<dType>( m_containers, chunk.offset(), loop_mask && filt_mask );
      }
      return out;
    }

    /** Return a copy of ourself that has a different default vector backend.
     */
    template <SIMDWrapper::InstructionSet dType>
    auto with() const {
      return Zip<dType, default_unwrap, ContainerTypes...>{*this};
    }

    /** Return a version of ourself that behaves 'traditionally', i.e. provides
     *  scalar iteration and returns plain C++ data types from accessors.
     */
    auto unwrap() const { return Zip<SIMDWrapper::InstructionSet::Scalar, true, ContainerTypes...>{*this}; }
  };

  namespace detail {
    template <typename T>
    struct merged_object_helper {
      static auto           decompose( T const& x ) { return std::tie( x ); }
      static constexpr bool is_zippable_v = ::LHCb::Pr::is_zippable_v<T>;
    };

    template <typename... T>
    struct merged_object_helper<merged_t<T...>> {
      static auto           decompose( merged_t<T...> const& x ) { return std::tie( static_cast<T const&>( x )... ); }
      static constexpr bool is_zippable_v = ( ::LHCb::Pr::is_zippable_v<T> && ... );
    };

    template <SIMDWrapper::InstructionSet def_simd, bool def_unwrap, typename... T,
              typename std::enable_if_t<( is_zippable_v<T> && ... ), int> = 0>
    auto make_zip( T const&... tracks ) {
      return Zip<def_simd, def_unwrap, T...>{tracks...};
    }
  } // namespace detail

  /** Construct an appropriate iterable wrapper from the given container(s)
   *
   *  e.g. transform LHCb::Pr::Velo::Tracks to an iterable wrapper around that,
   *  this is just a helper function that saves writing out so many explicit
   *  types...
   *
   *  There is some special handling in case some of the arguments are
   *  instances of merged_t. This is the type produced by filtering a zip. The
   *  special handling here ensures that `iterable( zip.filter(...) )` has the
   *  same type as `zip`.
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

    // Unpack the arguments, as specified above, into a tuple of const&
    auto expanded_tracks = std::tuple_cat( detail::merged_object_helper<PrTracks>::decompose( tracks )... );

    // Finally unpack this tuple into the argument list of our helper function
    return std::apply( []( auto const&... x ) { return detail::make_zip<def_simd, def_unwrap>( x... ); },
                       expanded_tracks );
  }

  // Helper to get the type of a zip of the types T...
  template <typename... T>
  using zip_t = decltype( LHCb::Pr::make_zip( std::declval<T>()... ) );

  // Helper to get the type of an unwrapped (scalar, plain C++ data) version
  // of zip_t<T...>
  template <typename... T>
  using unwrapped_zip_t = decltype( std::declval<zip_t<T...>>().unwrap() );

} // namespace LHCb::Pr
