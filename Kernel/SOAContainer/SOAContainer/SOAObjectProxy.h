/*****************************************************************************\
* (c) Copyright 2015-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
/** @file SOAObjectProxy.h
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-04-10
 */

#ifndef SOAOBJECTPROXY_H
#define SOAOBJECTPROXY_H

#include <tuple>
#include <memory>

#include "SOATypelist.h"
#include "SOATypelistUtils.h"
#include "SOAUtils.h"
#include "c++14_compat.h"

/// namespace to encapsulate SOA stuff
namespace SOA {
    // forward declarations
    template <class STORAGE,
             template <typename> class SKIN, typename... FIELDS>
                 class _View;
    template <template <typename...> class CONTAINER,
             template <typename> class SKIN, typename... FIELDS>
                 class _Container;

    /** @brief proxy object for the elements stored in the container.
     *
     * @author Manuel Schiller <Manuel.Schiller@cern.ch>
     * @date 2015-04-10
     *
     * Conceptually, the _Container contains a collection of objects
     * which have some data members. To optimise the data access patterns
     * in memory, the _Container doesn't store the objects themselves,
     * but containers which each store a different member. That means the
     * conceptual objects mentioned above do not exist as such. The
     * ObjectProxy class stands in for these objects, and provides
     * support for accessing members, assigment of the whole conceptual
     * object from a tuple of its members, and similar functionality.
     */
    template <typename POSITION>
    class ObjectProxy : protected POSITION {
    public:
        /// type of parent container
        using parent_type = typename POSITION::parent_type;
        /// type to refer to this type
        using self_type = ObjectProxy<POSITION>;
        /// type to hold the distance between two iterators
        using difference_type = typename parent_type::difference_type;
        /// type to hold the size of a container
        using size_type = typename parent_type::size_type;
        /// type to which ObjectProxy converts and can be assigned from
        using value_type = typename parent_type::value_type;
        /// type for tuple of references to members
        using reference = typename parent_type::value_reference;
        /// type for tuple of const references to members
        using const_reference = typename parent_type::value_const_reference;
        /// type of a pointer
        using pointer = typename parent_type::pointer;
        /// type of a const pointer
        using const_pointer = typename parent_type::const_pointer;

    protected:
        /// type used by the parent container to hold the SOA data
        using SOAStorage = typename parent_type::SOAStorage;
        /// typelist of fields
        using fields_typelist = typename parent_type::fields_typelist;

        using POSITION::stor;
        using POSITION::idx;

        /// corresponding _Containers are friends
        template <template <typename...> class CONTAINER,
                  template <typename> class SKIN, typename... FIELDS>
        friend class _Container;

    public:
        // magic constructor
        template <typename POS,
                  typename = typename std::enable_if<
                          std::is_base_of<POSITION, POS>::value>::type>
        constexpr explicit ObjectProxy(POS&& pos) noexcept
                : POSITION(std::forward<POS>(pos))
        {
        }

    private:
        /// little helper to implement conversion to tuple
        template <std::size_t... IDXS>
        static auto
        _to_tuple(const self_type& t, std::index_sequence<IDXS...>) noexcept(
                noexcept(std::forward_as_tuple(std::get<IDXS>(
                        std::declval<const typename
                        parent_type::SOAStorage&>())[0]...)))
                -> decltype(std::forward_as_tuple(std::get<IDXS>(
                                std::declval<const typename
                                parent_type::SOAStorage&>())[0]...))
        {
            return std::forward_as_tuple(std::get<IDXS>(
                    const_cast<const typename parent_type::SOAStorage&>(
                            *t.stor()))[t.idx()]...);
        }
        template <std::size_t... IDXS>
        static auto
        _to_tuple(self_type& t, std::index_sequence<IDXS...>) noexcept(
                noexcept(std::forward_as_tuple(std::get<IDXS>(
                        std::declval<typename
                        parent_type::SOAStorage&>())[0]...)))
                -> decltype(std::forward_as_tuple(std::get<IDXS>(
                                std::declval<typename
                                parent_type::SOAStorage&>())[0]...))
        {
            return std::forward_as_tuple(
                    std::get<IDXS>(*t.stor())[t.idx()]...);
        }
        template <std::size_t... IDXS>
        static auto
        _to_tuple(self_type&& t, std::index_sequence<IDXS...>) noexcept(
                noexcept(std::forward_as_tuple(std::get<IDXS>(
                        std::declval<typename
                        parent_type::SOAStorage&&>())[0]...)))
                -> decltype(std::forward_as_tuple(std::get<IDXS>(
                                std::declval<typename
                                parent_type::SOAStorage&&>())[0]...))
        {
            return std::forward_as_tuple(
                    std::get<IDXS>(*t.stor())[t.idx()]...);
        }

        static auto to_tuple(self_type& tup) noexcept(noexcept(_to_tuple(
                tup, std::make_index_sequence<fields_typelist::size()>())))
                -> decltype(_to_tuple(
                        tup,
                        std::make_index_sequence<fields_typelist::size()>()))
        {
            return _to_tuple(
                    tup, std::make_index_sequence<fields_typelist::size()>());
        }
        static auto
        to_tuple(const self_type& tup) noexcept(noexcept(_to_tuple(
                tup, std::make_index_sequence<fields_typelist::size()>())))
                -> decltype(_to_tuple(
                        tup,
                        std::make_index_sequence<fields_typelist::size()>()))
        {
            return _to_tuple(
                    tup, std::make_index_sequence<fields_typelist::size()>());
        }
        static auto to_tuple(self_type&& tup) noexcept(noexcept(_to_tuple(
                std::move(tup),
                std::make_index_sequence<fields_typelist::size()>())))
                -> decltype(_to_tuple(
                        std::move(tup),
                        std::make_index_sequence<fields_typelist::size()>()))
        {
            return _to_tuple(
                    std::move(tup),
                    std::make_index_sequence<fields_typelist::size()>());
        }

        template <std::size_t... IDXS>
        static auto
        _to_tuple_val(const self_type& t, std::index_sequence<IDXS...>) noexcept(
                noexcept(std::make_tuple(
                        std::get<IDXS>(std::declval<const typename
                            parent_type::SOAStorage&>())[0]...)))
                -> decltype(std::make_tuple(
                        std::get<IDXS>(std::declval<const typename
                            parent_type::SOAStorage&>())[0]...))
        {
            return std::make_tuple(std::get<IDXS>(*t.stor())[t.idx()]...);
        }

        static auto
        to_tuple_val(const self_type& tup) noexcept(noexcept(_to_tuple_val(
                tup, std::make_index_sequence<fields_typelist::size()>())))
                -> decltype(_to_tuple_val(
                        tup,
                        std::make_index_sequence<fields_typelist::size()>()))
        {
            return _to_tuple_val(
                    tup, std::make_index_sequence<fields_typelist::size()>());
        }

        template <typename T, typename U, std::size_t... IDXS>
        static void
        _assign(T&& t, U&& u, std::index_sequence<IDXS...>) noexcept(
                noexcept(std::forward_as_tuple(
                        (std::get<IDXS>(std::forward<T>(t)) =
                                 std::get<IDXS>(std::forward<U>(u)),
                         0)...)))
        {
            std::forward_as_tuple((std::get<IDXS>(std::forward<T>(t)) =
                                           std::get<IDXS>(std::forward<U>(u)),
                                   0)...);
        }
        template <typename T, typename U>
        static void assign(T&& t, U&& u) noexcept(noexcept(
                _assign(std::forward<T>(t), std::forward<U>(u),
                        std::make_index_sequence<fields_typelist::size()>())))
        {
            _assign(std::forward<T>(t), std::forward<U>(u),
                    std::make_index_sequence<fields_typelist::size()>());
        }

        struct swapHelper {
            size_type m_idx1;
            size_type m_idx2;
            template <typename T>
            void operator()(std::tuple<T&, T&> obj) const
                    noexcept(noexcept(std::swap(std::get<0>(obj)[m_idx1],
                                                std::get<1>(obj)[m_idx2])))
            {
                std::swap(std::get<0>(obj)[m_idx1], std::get<1>(obj)[m_idx2]);
            }
        };

    public:
        /// default constructor
        ObjectProxy() = default;
        /// copy constructor
        ObjectProxy(const self_type& other) = default;
        /// move constructor
        ObjectProxy(self_type&& other) = default;

        /// convert to tuple of member contents
        constexpr operator value_type() const
                noexcept(noexcept(to_tuple_val(std::declval<self_type>())))
        { return to_tuple_val(*this); }
        /// convert to tuple of references to members
        operator reference() noexcept(
                noexcept(to_tuple(std::declval<self_type>())))
        { return to_tuple(*this); }
        /// convert to tuple of const references to members
        constexpr operator const_reference() const
                noexcept(noexcept(to_tuple(std::declval<self_type>())))
        { return to_tuple(*this); }

        /// assign from other ObjectProxy - copy fields
        self_type&
        operator=(const self_type& other) noexcept(noexcept(self_type::assign(
                self_type::to_tuple(std::declval<self_type&>()),
                self_type::to_tuple(other))))
        {
            self_type::assign(self_type::to_tuple(*this),
                              self_type::to_tuple(other));
            return *this;
        }
        /// assign from other ObjectProxy - move fields
        self_type&
        operator=(self_type&& other) noexcept(noexcept(self_type::assign(
                self_type::to_tuple(std::declval<self_type&>()),
                self_type::to_tuple(std::move(other)))))
        {
            self_type::assign(self_type::to_tuple(*this),
                              self_type::to_tuple(std::move(other)));
            return *this;
        }
        // assign from other tuple-like object
        template <typename TUP>
        typename std::enable_if<
                !std::is_same<self_type,
                              typename std::decay<TUP>::type>::value,
                self_type&>::type
        operator=(TUP&& tup) noexcept(noexcept(self_type::assign(
                self_type::to_tuple(std::declval<self_type&>()),
                std::forward<TUP>(tup))))
        {
            self_type::assign(self_type::to_tuple(*this),
                              std::forward<TUP>(tup));
            return *this;
        }

        /// access to member by number
        template <size_type MEMBERNO>
        auto get() noexcept -> decltype(std::get<MEMBERNO>(
                *std::declval<POSITION&>()
                         .stor())[std::declval<POSITION&>().idx()])
        {
            static_assert(std::is_base_of<POSITION, self_type>::value,
                          "SOAObjectProxy must be derived from POSITION");
            return std::get<MEMBERNO>(*stor())[idx()];
        }
        /// access to member by "member tag"
        template <
                typename MEMBER,
                size_type MEMBERNO = parent_type::template memberno<MEMBER>()>
        auto get() noexcept -> decltype(std::get<MEMBERNO>(
                *std::declval<POSITION&>()
                         .stor())[std::declval<POSITION&>().idx()])
        {
            static_assert(std::is_base_of<POSITION, self_type>::value,
                          "SOAObjectProxy must be derived from POSITION");
            static_assert(parent_type::template memberno<MEMBER>() ==
                                  MEMBERNO,
                          "Called with wrong template argument(s).");
            return std::get<MEMBERNO>(*stor())[idx()];
        }
        /// access to member by number (read-only)
        template <size_type MEMBERNO>
        auto get() const noexcept -> decltype(std::get<MEMBERNO>(
                *std::declval<const POSITION&>()
                         .stor())[std::declval<const POSITION&>().idx()])
        {
            static_assert(std::is_base_of<POSITION, self_type>::value,
                          "SOAObjectProxy must be derived from POSITION");
            return std::get<MEMBERNO>(*stor())[idx()];
        }
        /// access to member by "member tag" (read-only)
        template <
                typename MEMBER,
                size_type MEMBERNO = parent_type::template memberno<MEMBER>()>
        auto get() const noexcept -> decltype(std::get<MEMBERNO>(
                *std::declval<const POSITION&>()
                         .stor())[std::declval<const POSITION&>().idx()])
        {
            static_assert(std::is_base_of<POSITION, self_type>::value,
                          "SOAObjectProxy must be derived from POSITION");
            static_assert(parent_type::template memberno<MEMBER>() ==
                                  MEMBERNO,
                          "Called with wrong template argument(s).");
            return std::get<MEMBERNO>(*stor())[idx()];
        }

        /// swap the contents of two ObjectProxy instances
        void swap(self_type& other) noexcept(noexcept(SOA::Utils::map(
                swapHelper{other.idx(), other.idx()},
                SOA::Utils::zip(*other.stor(), *other.stor()))))
        {
            SOA::Utils::map(swapHelper{idx(), other.idx()},
                            SOA::Utils::zip(*stor(), *other.stor()));
        }

        /// comparison (equality)
        bool operator==(const value_type& other) const noexcept
        {
            return const_reference(*this) == other;
        }
        /// comparison (inequality)
        bool operator!=(const value_type& other) const noexcept
        {
            return const_reference(*this) != other;
        }
        /// comparison (less than)
        bool operator<(const value_type& other) const noexcept
        {
            return const_reference(*this) < other;
        }
        /// comparison (greater than)
        bool operator>(const value_type& other) const noexcept
        {
            return const_reference(*this) > other;
        }
        /// comparison (less than or equal to)
        bool operator<=(const value_type& other) const noexcept
        {
            return const_reference(*this) <= other;
        }
        /// comparison (greater than or equal to)
        bool operator>=(const value_type& other) const noexcept
        {
            return const_reference(*this) >= other;
        }

        /// comparison (equality)
        bool operator==(const self_type& other) const noexcept
        {
            return const_reference(*this) == const_reference(other);
        }
        /// comparison (inequality)
        bool operator!=(const self_type& other) const noexcept
        {
            return const_reference(*this) != const_reference(other);
        }
        /// comparison (less than)
        bool operator<(const self_type& other) const noexcept
        {
            return const_reference(*this) < const_reference(other);
        }
        /// comparison (greater than)
        bool operator>(const self_type& other) const noexcept
        {
            return const_reference(*this) > const_reference(other);
        }
        /// comparison (less than or equal to)
        bool operator<=(const self_type& other) const noexcept
        {
            return const_reference(*this) <= const_reference(other);
        }
        /// comparison (greater than or equal to)
        bool operator>=(const self_type& other) const noexcept
        {
            return const_reference(*this) >= const_reference(other);
        }

        /// return pointer to element pointed to be this proxy
        pointer operator&() noexcept { return pointer{POSITION(*this)}; }
        /// return const pointer to element pointed to be this proxy
        constexpr const_pointer operator&() const noexcept
        {
            return const_pointer{POSITION(*this)};
        }
    };

    /// comparison (equality)
    template <typename T>
    bool operator==(const typename ObjectProxy<T>::value_type& a,
            const ObjectProxy<T>& b) noexcept { return b == a; }
    /// comparison (inequality)
    template <typename T>
    bool operator!=(const typename ObjectProxy<T>::value_type& a,
            const ObjectProxy<T>& b) noexcept { return b != a; }
    /// comparison (less than)
    template <typename T>
    bool operator<(const typename ObjectProxy<T>::value_type& a,
            const ObjectProxy<T>& b) noexcept { return b > a; }
    /// comparison (greater than)
    template <typename T>
    bool operator>(const typename ObjectProxy<T>::value_type& a,
            const ObjectProxy<T>& b) noexcept { return b < a; }
    /// comparison (less than or equal to)
    template <typename T>
    bool operator<=(const typename ObjectProxy<T>::value_type& a,
            const ObjectProxy<T>& b) noexcept { return b >= a; }
    /// comparison (greater than or equal to)
    template <typename T>
    bool operator>=(const typename ObjectProxy<T>::value_type& a,
            const ObjectProxy<T>& b) noexcept { return b <= a; }

    /// helper for std::swap for ObjectProxy<T>
    template <typename T>
    void swap(ObjectProxy<T> a, ObjectProxy<T> b) noexcept(
            noexcept(a.swap(b)))
    { a.swap(b); }
} // namespace SOA

#endif // SOAOBJECTPROXY_H

// vim: sw=4:tw=78:ft=cpp:et
