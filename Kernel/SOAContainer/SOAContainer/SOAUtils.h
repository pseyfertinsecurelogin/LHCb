/** @file SOAUtils.h
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-04-09
 */

#ifndef SOAUTILS_H
#define SOAUTILS_H

#include <tuple>
#include <utility>
#include <cstdint>
#include <type_traits>

#include "c++14_compat.h"

/// various other utilities used by SOAContainer
namespace SOAUtils {
    /// invoke fun on given arguments, return a dummy int if fun returns void
    template <typename FUN, typename... ARG>
    auto invoke_void2int(FUN fun, ARG&&... arg) noexcept(
        noexcept(fun(std::forward<ARG>(arg)...))) ->
        typename std::enable_if<
            !std::is_same<void, typename std::result_of<FUN(ARG...)>::type>::value,
            typename std::result_of<FUN(ARG...)>::type>::type
    { return fun(std::forward<ARG>(arg)...); }
    /// invoke fun on given arguments, return a dummy int if fun returns void
    template <typename FUN, typename... ARG>
    auto invoke_void2int(FUN fun, ARG&&... arg) noexcept(
        noexcept(fun(std::forward<ARG>(arg)...))) ->
        typename std::enable_if<
            std::is_same<void, typename std::result_of<FUN(ARG...)>::type>::value,
            int>::type
    { fun(std::forward<ARG>(arg)...); return 0; }

    /// apply functor fn to each element of tuple, and return tuple with results
    template <typename FUNCTOR, typename OBJ, std::size_t... IDX>
    auto map(FUNCTOR fn, OBJ&& obj, std::index_sequence<IDX...>) noexcept(
        noexcept(std::make_tuple(invoke_void2int(fn, std::get<IDX>(std::forward<OBJ>(obj)))...))) ->
        decltype(std::make_tuple(invoke_void2int(fn, std::get<IDX>(std::forward<OBJ>(obj)))...))
    { return std::make_tuple(invoke_void2int(fn, std::get<IDX>(std::forward<OBJ>(obj)))...); }

    /// apply functor fn to each element of tuple, and return tuple with results
    template <typename FUNCTOR, typename OBJ>
    auto map(FUNCTOR fn, OBJ&& obj) noexcept(
        noexcept(map(fn, std::forward<OBJ>(obj),
                std::make_index_sequence<
                std::tuple_size<typename std::decay<OBJ>::type>::value>()))) ->
        decltype(map(fn, std::forward<OBJ>(obj),
                std::make_index_sequence<
                std::tuple_size<typename std::decay<OBJ>::type>::value>()))
    {
        return map(fn, std::forward<OBJ>(obj),
            std::make_index_sequence<
            std::tuple_size<typename std::decay<OBJ>::type>::value>());
    }

    /// implementation details of foldl
    namespace foldl_impl {
        /// implementation of foldl
        template <typename FUN, typename TUP, std::size_t LEN>
        struct foldl_impl {
            FUN fun;
            const TUP& tup;
            /// fold over tuple with more than 2 elements
            template <typename INI, std::size_t IDX, std::size_t... IDXS>
            typename std::enable_if<(LEN > 1),
                typename std::result_of<
                    foldl_impl<FUN, TUP, LEN - 1>(
                        typename std::result_of<FUN(INI,
                            typename std::tuple_element<IDX, TUP>::type)
                        >::type, std::index_sequence<IDXS...>)
                    >::type
                >::type
            operator()(INI ini,
                    std::index_sequence<IDX, IDXS...>) const noexcept(
                    noexcept(foldl_impl<FUN, TUP, LEN - 1>{fun, tup}(
                            fun(ini, std::get<IDX>(tup)),
                            std::index_sequence<IDXS...>())))
            {
                return foldl_impl<FUN, TUP, LEN - 1>{fun, tup}(
                        fun(ini, std::get<IDX>(tup)),
                        std::index_sequence<IDXS...>());
            }
            /// foldl over tuple with exactly one element
            template <typename INI, std::size_t IDX>
            typename std::enable_if<1 == LEN,
                typename std::result_of<
                    FUN(INI,
                            typename std::tuple_element<IDX, TUP>::type)>::type
                >::type
            operator()(INI ini, std::index_sequence<IDX>) const noexcept(
                    noexcept(fun(ini, std::get<IDX>(tup))))
            { return fun(ini, std::get<IDX>(tup)); }
            /// foldl over tuple with no elements
            template <typename INI>
            INI operator()(INI ini, std::index_sequence<>) const noexcept(
                    noexcept(INI(ini)))
            { return ini; }
        };
    }
    
    /** @brief foldl (left fold) on tuples
     *
     * Given an index sequence i1, ..., iN, a tuple with elements (e_1, ...,
     * e_N-1, e_N), a fold function fun, and an initial value ini, "fold" the
     * tuple elements given in the index sequence using the functor fun
     * starting from the left, i.e. return fun(fun(fun(...fun(ini, e_i1),
     * ...), e_iN-1), e_iN), or return ini for the empty tuple.
     *
     * @tparam INI  type of initial value
     * @tparam FUN  type of functor used in the fold
     * @tparam TUP  type of the tuple to be folded
     *
     * @param fun   functor used to "fold" tuple
     * @param tup   tuple to fold
     * @param ini   initial value
     *
     * @returns     value of the left fold
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2016-12-12
     */
    template <typename INI, typename FUN, typename TUP, std::size_t... IDXS>
    typename std::result_of<foldl_impl::foldl_impl<FUN, TUP, sizeof...(IDXS)>(INI, std::index_sequence<IDXS...>)>::type
    foldl(FUN fun, const TUP& tup, INI ini, std::index_sequence<IDXS...>) noexcept(
            noexcept(foldl_impl::foldl_impl<FUN, TUP, sizeof...(IDXS)>{fun, tup}(
                    ini, std::index_sequence<IDXS...>())))
    {
        return foldl_impl::foldl_impl<FUN, TUP, sizeof...(IDXS)>{fun, tup}(
                ini, std::index_sequence<IDXS...>());
    }
    
    /** @brief foldl (left fold) on tuples
     *
     * Given a tuple with elements (e_1, ..., e_N-1, e_N), a fold function
     * fun, and an initial value ini, "fold" the tuple using the functor fun
     * starting from the left, i.e. return fun(fun(fun(...fun(ini, e_1), ...),
     * e_N-1), e_N), or return ini for the empty tuple.
     *
     * @tparam INI  type of initial value
     * @tparam FUN  type of functor used in the fold
     * @tparam TUP  type of the tuple to be folded
     *
     * @param fun   functor used to "fold" tuple
     * @param tup   tuple to fold
     * @param ini   initial value
     *
     * @returns     value of the left fold
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2016-12-12
     */
    template <typename INI, typename FUN, typename TUP>
    auto foldl(FUN fun, const TUP& tup, INI ini = INI()) noexcept(
            noexcept(foldl(fun, tup, ini,
                    std::make_index_sequence<std::tuple_size<TUP>::value>()))) ->
        decltype(foldl(fun, tup, ini,
                    std::make_index_sequence<std::tuple_size<TUP>::value>()))
    {
        return foldl(fun, tup, ini,
                std::make_index_sequence<std::tuple_size<TUP>::value>());
    }
    
    /// implementation details for zip
    namespace zip_impl {
        /// little helper for zip
        template <std::size_t IDXINNER, std::size_t... IDXOUTER, typename... OBJS>
        auto zip(std::index_sequence<IDXOUTER...>, OBJS&&... objs) noexcept(noexcept(
                    std::forward_as_tuple(std::get<IDXINNER>(std::get<IDXOUTER>(
                                std::forward_as_tuple(std::forward<OBJS>(objs)...)))...))) -> decltype(
                std::forward_as_tuple(std::get<IDXINNER>(std::get<IDXOUTER>(
                            std::forward_as_tuple(std::forward<OBJS>(objs)...)))...))
        {
            return std::forward_as_tuple(std::get<IDXINNER>(std::get<IDXOUTER>(
                            std::forward_as_tuple(std::forward<OBJS>(objs)...)))...);
        }

        /// little helper for zip
        template <typename... OBJS, std::size_t... IDXOUTER, std::size_t... IDXINNER>
        auto zip(std::index_sequence<IDXOUTER...> outer,
                std::index_sequence<IDXINNER...>, OBJS&&... objs) noexcept(noexcept(
                        std::make_tuple(zip<IDXINNER>(
                                outer, std::forward<OBJS>(objs)...)...))) -> decltype(
                    std::make_tuple(zip<IDXINNER>(
                            outer, std::forward<OBJS>(objs)...)...))
        {
            return std::make_tuple(zip<IDXINNER>(
                        outer, std::forward<OBJS>(objs)...)...);
        }
    }

    /** @brief zip together tuples
     *
     * @tparam OBJS... parameter pack of types of tuples to zip
     *
     * @param objs       tuples to zip
     * @returns          a tuple of zipped tuples
     *
     * Given N tuples with elements (a0, a1, ..., a_M), (b0, b1, ... b_M), ...,
     * returns a tuple ((a0, b0, ...), (a1, b1, ...), ...), i.e. zip "zips" up
     * the contents of the tuples passed as arguments.
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2017-01-11
     */
    template <typename... OBJS>
    auto zip(OBJS&&... objs) noexcept(noexcept(
                zip_impl::zip(std::make_index_sequence<sizeof...(OBJS)>(),
                    std::make_index_sequence<std::tuple_size<
                    typename std::tuple_element<0, std::tuple<
                    typename std::decay<OBJS>::type...> >::type>::value>(),
                    std::forward<OBJS>(objs)...))) -> decltype(
            zip_impl::zip(std::make_index_sequence<sizeof...(OBJS)>(),
                std::make_index_sequence<std::tuple_size<
                typename std::tuple_element<0, std::tuple<
                typename std::decay<OBJS>::type...> >::type>::value>(),
                std::forward<OBJS>(objs)...))
    {
        return zip_impl::zip(std::make_index_sequence<sizeof...(OBJS)>(),
                std::make_index_sequence<std::tuple_size<
                typename std::tuple_element<0, std::tuple<
                typename std::decay<OBJS>::type...> >::type>::value>(),
                std::forward<OBJS>(objs)...);
    }

    /// little tool to call a callable using the arguments given in a tuple
    template <typename F, typename T, size_t sz = std::tuple_size<T>::value>
    struct caller {
        const F& m_fn; ///< callable
        /// constructor
        caller(const F& fn) : m_fn(fn) { }

        /// call operator (unpacks tuple one by one, perfect forwarding)
        template <typename... ARGS>
        auto operator()(T&& tuple, ARGS&&... args) -> decltype(
            caller<F, T, sz - 1>(m_fn)(std::forward<T>(tuple),
                std::move(std::get<sz - 1>(tuple)), std::forward<ARGS>(args)...))
        {
            return caller<F, T, sz - 1>(m_fn)(std::forward<T>(tuple),
                std::move(std::get<sz - 1>(tuple)), std::forward<ARGS>(args)...);
        }

        /// call operator (unpacks tuple one by one)
        template <typename... ARGS>
        auto operator()(const T& tuple, const ARGS&... args) -> decltype(
            caller<F, T, sz - 1>(m_fn)(tuple,
                std::get<sz - 1>(tuple), args...))
        {
            return caller<F, T, sz - 1>(m_fn)(tuple,
                std::get<sz - 1>(tuple), args...);
        }
    };

    /// little tool to call using unpacked tuple arguments (specialisation)
    template <typename F, typename T>
    struct caller<F, T, size_t(0)> {
        const F& m_fn; ///< callable
        /// constructor
        caller(const F& fn) : m_fn(fn) { }

        /// call operator (unpacks tuple one by one, perfect forwarding)
        template <typename... ARGS>
        auto operator()(T&& /* tuple */, ARGS&&... args) -> decltype(
            m_fn(std::forward<ARGS>(args)...))
        { return m_fn(std::forward<ARGS>(args)...); }

        /// call operator (unpacks tuple one by one)
        template <typename... ARGS>
        auto operator()(const T& /* tuple */,
            const ARGS&... args) -> decltype(m_fn(args...))
        { return m_fn(args...); }
    };

    /// little helper to call callable f with contents of t as arguments (perfect forwarding)
    template<typename F, typename T>
    auto call(const F& f, T&& t) -> decltype(
        caller<F, typename std::decay<T>::type>(f)(std::forward<T>(t)))
    { return caller<F, typename std::decay<T>::type>(f)(std::forward<T>(t)); }

    /// little helper to call callable f with contents of t as arguments
    template<typename F, typename T>
    auto call(const F& f, const T& t) -> decltype(
        caller<F, typename std::decay<T>::type>(f)(t))
    { return caller<F, typename std::decay<T>::type>(f)(t); }
    /// little helper to implment ANY
    template <template <typename> class PRED, typename HEAD, typename... TAIL>
    struct _ANY : public std::conditional<bool(PRED<HEAD>::value),
            std::true_type, _ANY<PRED, TAIL...> >::type { };
    /// little helper to implement ANY: recursion anchor
    template <template <typename> class PRED, typename ARG>
    struct _ANY<PRED, ARG> : public PRED<ARG> { };
    /// is any of a type-trait predicate applied to a list of ARGS true?
    template <template <typename> class PRED, typename... ARGS>
    struct ANY : public _ANY<PRED, ARGS...> { };
    /// specialisation of ANY: no arguments
    template <template <typename> class PRED>
    struct ANY<PRED> : public std::false_type { };
    /// little helper to implement ALL
    template <template <typename> class PRED, typename HEAD, typename... TAIL>
    struct  _ALL : public std::conditional<bool(!PRED<HEAD>::value),
        std::false_type, _ALL<PRED, TAIL...> >::type { };
    /// little helper to implement ALL: recursion anchor
    template <template <typename> class PRED, typename ARG>
    struct _ALL<PRED, ARG> : public PRED<ARG> { };
    /// are ALL of a type-trait predicate applied to a list of ARGS true?
    template <template <typename> class PRED, typename... ARGS>
    struct ALL : public _ALL<PRED, ARGS...> { };
    /// specialisation of ALL: no arguments
    template <template <typename> class PRED>
    struct ALL<PRED> : public std::true_type { };
}

#endif // SOAUTILS_H

// vim: sw=4:tw=78:ft=cpp:et
