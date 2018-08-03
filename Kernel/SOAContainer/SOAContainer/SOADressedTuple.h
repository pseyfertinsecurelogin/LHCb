/* @file SOADressedTuple.h
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-05-09
 */

#ifndef SOADRESSEDTUPLE_H
#define SOADRESSEDTUPLE_H

#include <tuple>
#include <type_traits>
#include "c++14_compat.h"

/// namespace to encapsulate SOA stuff
namespace SOA {
    /// implementation details
    namespace impl {
        /** @brief base for SOA::DressedTuple to hold common functionality
         *
         * SOA::View's and SOA::Container's requirements on Dressed tuples
         * are somewhat more stringent than those of the various STL
         * implementations out there in that we want to automatically
         * promote "naked" tuples to DressedTuples if they're compatible,
         * and allow reasonable assignments as well. This will need a
         * bunch of constructors and assignment operators that do the right
         * thing, but will only be enabled (via enable_if) if the tuple
         * elements have the right type. Unfortunately, the STL's version of
         * std::is_assignable and std::is_constructible don't see through the
         * fact that every DressedTuple is also a naked tuple, so we have our
         * own versions here that use the STL's implementation, but see
         * through the various levels of inheritance at the naked tuple
         * underneath. The aim is to only enable our versions of constructors
         * and assignment operators if the underlying tuple's version won't
         * do.
         *
         * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
         * @date 2017-11-15
         */
        class DressedTupleBase {
            protected:
            /// helper to check constructibility of tuples - depends on tuples
            template <typename T1, typename T2>
            static constexpr inline std::false_type
            _is_constructible(const T1&, const T2&) noexcept { return {}; }
            template <typename... Ts1, typename... Ts2>
            static constexpr inline std::is_constructible<
                std::tuple<Ts1...>, std::tuple<Ts2...> >
            _is_constructible(const std::tuple<Ts1&&...>&,
                    const std::tuple<Ts2&&...>&) noexcept { return {}; }
            template <typename T1, typename T2>
            using is_constructible = decltype(_is_constructible(
                        std::declval<const T1&>(), std::declval<const T2&>()));

            /// helper to check assignability of tuples - depends on tuples
            template <typename T1, typename T2>
            static constexpr inline std::false_type
            _is_assignable(const T1&, const T2&) noexcept { return {}; }
            template <typename... Ts1, typename... Ts2>
            static constexpr inline std::is_assignable<
                std::tuple<Ts1...>, std::tuple<Ts2...> >
            _is_assignable(const std::tuple<Ts1&&...>&,
                    const std::tuple<Ts2&&...>&) noexcept { return {}; }
            template <typename T1, typename T2>
            using is_assignable = decltype(_is_assignable(
                        std::declval<const T1&>(), std::declval<const T2&>()));

            template <typename... Ts>
            constexpr static inline std::tuple_size<std::tuple<Ts...> >
            _tuple_size(const std::tuple<Ts...>&) noexcept { return {}; }
            /// figure out the underlying tuple's size
            template <typename T>
            using tuple_size = decltype(
                    _tuple_size(std::declval<const T&>()));
        };
    }
    /** @brief dress std::tuple with the get interface of SOAObjectProxy
     *
     * @author Manuel Schiller <Manuel.Schiller@cern.ch>
     * @date 2015-05-09
     *
     * @tparam TUPLE        an instantiation of std::tuple
     * @tparam CONTAINER    underlying SOAContainer
     */
    template <typename TUPLE, typename CONTAINER>
    class DressedTuple : public impl::DressedTupleBase, public TUPLE
    {
        public:
            /// convenience typedef
            using self_type = DressedTuple<TUPLE, CONTAINER>;
            using TUPLE::TUPLE;
            using TUPLE::operator=;

            /// (copy) assignment from a naked proxy
            /*DressedTuple& operator=(
                    const typename CONTAINER::naked_proxy& other) noexcept(
                        noexcept(std::declval<TUPLE>().operator=(typename
                                SOA::Typelist::to_tuple<typename
                                CONTAINER::fields_typelist>::value_tuple(other))))
            {
                TUPLE::operator=(typename SOA::Typelist::to_tuple<typename
                        CONTAINER::fields_typelist>::value_tuple(other));
                return *this;
            }*/

        private:
            /// helper for fallback constructors
            template <std::size_t... IDXS, typename T>
            explicit constexpr DressedTuple(std::index_sequence<IDXS...>,
                    T&& t) noexcept(noexcept(
                            TUPLE(std::get<IDXS>(std::forward<T>(t))...))) :
                TUPLE(std::get<IDXS>(std::forward<T>(t))...)
            {}
            /// helper for fallback assignment operators
            template <std::size_t... IDXS, typename T>
            void assign(std::index_sequence<IDXS...>, T&& t) noexcept(noexcept(
                        std::forward_as_tuple(((std::get<IDXS>(
                                        std::declval<self_type&>()) =
                                    std::get<IDXS>(
                                        std::forward<T>(t))), 0)...)))
            {
                std::forward_as_tuple(((std::get<IDXS>(*this) =
                                std::get<IDXS>(std::forward<T>(t))), 0)...);
            }

        public:
            /// fallback constructor to see past dressed tuples
            template <typename T, typename TDUMMY = T,
                     typename = typename std::enable_if<
                         is_constructible<TUPLE, TDUMMY>::value &&
                         !std::is_base_of<TUPLE, TDUMMY>::value>,
                     std::size_t N = tuple_size<TDUMMY>::value >
            constexpr DressedTuple(T&& t) noexcept(noexcept(
                DressedTuple(std::make_index_sequence<N>(),
                    std::forward<T>(t)))) :
                DressedTuple(std::make_index_sequence<N>(),
                        std::forward<T>(t))
            {}
            /// fallback assignment operator to see past dressed tuples
            template <typename T>
            typename std::enable_if<is_assignable<TUPLE, T>::value && !std::is_base_of<TUPLE, T>::value,
                     self_type>::type&
            operator=(T&& t) noexcept(noexcept(std::declval<
                        DressedTuple<TUPLE, CONTAINER> >().assign(
                            std::make_index_sequence<tuple_size<T>::value>(),
                            std::forward<T>(t))))
            {
                if (static_cast<void*>(this) != &t)
                    assign(std::make_index_sequence<tuple_size<T>::value>(),
                            std::forward<T>(t));
                return *this;
            }

            /// provide the member function template get interface of proxies
            template<typename CONTAINER::size_type MEMBERNO>
            auto get() noexcept -> decltype(std::get<MEMBERNO>(std::declval<self_type&>()))
            { return std::get<MEMBERNO>(*this); }

            /// provide the member function template get interface of proxies
            template<typename CONTAINER::size_type MEMBERNO>
            auto get() const noexcept -> decltype(std::get<MEMBERNO>(
                        std::declval<const self_type&>()))
            { return std::get<MEMBERNO>(*this); }

            /// provide the member function template get interface of proxies
            template<typename MEMBER, typename CONTAINER::size_type MEMBERNO =
                CONTAINER::template memberno<MEMBER>()>
            auto get() noexcept -> decltype(std::get<MEMBERNO>(std::declval<self_type&>()))
            {
                static_assert(CONTAINER::template memberno<MEMBER>() ==
                        MEMBERNO, "Called with wrong template argument(s).");
                return std::get<MEMBERNO>(*this);
            }

            /// provide the member function template get interface of proxies
            template<typename MEMBER, typename CONTAINER::size_type MEMBERNO =
                CONTAINER::template memberno<MEMBER>()>
            auto get() const noexcept -> decltype(std::get<MEMBERNO>(
                        std::declval<const self_type&>()))
            {
                static_assert(CONTAINER::template memberno<MEMBER>() ==
                        MEMBERNO, "Called with wrong template argument(s).");
                return std::get<MEMBERNO>(*this);
            }
    };
} // namespace SOA

#endif // SOADRESSEDTUPLE_H

// vim: sw=4:tw=78:ft=cpp:et
