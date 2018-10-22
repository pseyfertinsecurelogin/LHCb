#ifndef LHCB_CALLABLE_TRAITS
#define LHCB_CALLABLE_TRAITS
#include <type_traits>
#include <tuple>
#include <functional>
#include <utility>

namespace LHCb {
namespace cxx {

namespace details {
    template <typename Signature> struct callable_helper;

    template <typename R, typename... Args>
    struct callable_helper<R(Args...)>  {
        using return_type = R;
        template <size_t N>
        using argument_type = std::tuple_element_t<N,std::tuple<Args...>>;
    };
}

template <typename Signature> struct CallableTraits;

template<typename R, typename C, bool Nx, typename... Args>
struct CallableTraits< R(C::*)(Args...) noexcept(Nx)>
: details::callable_helper<R(C&, Args...)> {};

template<typename R, typename C, bool Nx, typename... Args>
struct CallableTraits<R(C::*)(Args...)const  noexcept(Nx)>
: details::callable_helper<R(const C&, Args...)>{};

template<typename R, typename C, bool Nx, typename... Args>
struct CallableTraits<R(C::*)(Args...) &&  noexcept(Nx)>
: details::callable_helper<R(C&&, Args...)>{};

template<typename R, typename C, bool Nx, typename... Args>
struct CallableTraits<R(C::*)(Args...) const &&  noexcept(Nx)>
: details::callable_helper<R(const C&&, Args...)>{};

template<typename R, typename... Args>
struct CallableTraits<R(Args...)>
: details::callable_helper<R(Args...)> {};

template<typename R, typename... Args>
struct CallableTraits<std::function<R(Args...)>>
: details::callable_helper<R(Args...)> {};

template <typename T>
struct CallableTraits
: CallableTraits<decltype(std::function{std::declval<T&&>()})> {};

}
}
#endif
