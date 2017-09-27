/* @file SOADressedTuple.h
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-05-09
 */

#ifndef SOADRESSEDTUPLE_H
#define SOADRESSEDTUPLE_H

#include <tuple>

/** @brief dress std::tuple with the get interface of SOAObjectProxy
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-05-09
 *
 * @tparam TUPLE        an instantiation of std::tuple
 * @tparam CONTAINER    underlying SOAContainer
 */
template <typename TUPLE, typename CONTAINER>
struct DressedTuple : TUPLE
{
    /// convenience typedef
    using self_type = DressedTuple<TUPLE, CONTAINER>;

    /// copy-construct from base class (TUPLE)
    DressedTuple(const TUPLE& t) : TUPLE(t) {}
    /// move-construct from base class (TUPLE)
    DressedTuple(TUPLE&& t) : TUPLE(std::move(t)) {}
    /// for everything else, use TUPLE's constructor
    using TUPLE::TUPLE;
    /// for everything else, use TUPLE's assignment operators
    using TUPLE::operator=;

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

#endif // SOADRESSEDTUPLE_H

// vim: sw=4:tw=78:ft=cpp:et
