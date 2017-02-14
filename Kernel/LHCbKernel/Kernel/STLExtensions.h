
#ifndef LHCBKERNEL_STLExtensions
#define LHCBKERNEL_STLExtensions 1

// This header contains methods that are likely to become parts of the STL
// at some point in the future, but currently are not available.

#include <array>
#include <type_traits>

namespace LHCb
{
  /// An implementation  of 'std::make_array' based on
  /// http://en.cppreference.com/w/cpp/experimental/make_array
  /// Once we target gcc 6.1 or newer only, this can be removed
  /// and <experimental/array> directly used instead.

  namespace details {

    template<class...> struct conjunction : std::true_type { };
    template<class B1> struct conjunction<B1> : B1 { };
    template<class B1, class... Bn>
    struct conjunction<B1, Bn...>
        : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {};

    template<class B>
    struct negation : std::integral_constant<bool, !B::value> { };

    template<class> struct is_ref_wrapper : std::false_type {};
    template<class T> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

    template<class T>
    using not_ref_wrapper = negation<is_ref_wrapper<typename std::decay<T>::type>>;

    template <class D, class...> struct return_type_helper { using type = D; };
    template <class... Types>
    struct return_type_helper<void, Types...> : std::common_type<Types...> {
        static_assert(conjunction<not_ref_wrapper<Types>...>::value,
                      "Types cannot contain reference_wrappers when D is void");
    };

    template <class D, class... Types>
    using return_type = std::array<typename return_type_helper<D, Types...>::type,
                                   sizeof...(Types)>;
  }

  template < class D = void, class... Types>
  constexpr details::return_type<D, Types...> make_array(Types&&... t) {
    return {std::forward<Types>(t)... };
  }


}

#endif // LHCBKERNEL_STLExtensions
