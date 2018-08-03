#ifndef SOAITERATOR_H
#define SOAITERATOR_H

#include <ostream>
#include <iterator>

namespace SOA {
    template < template <typename...> class CONTAINER,
        template <typename> class SKIN, typename... FIELDS>
    class _Container;

    template <typename POSITION, bool ISCONST>
    class Iterator : protected POSITION {
    private:
        template < template <typename...> class CONTAINER,
            template <typename> class SKIN, typename... FIELDS>
        friend class _Container;
        using parent_type = typename POSITION::parent_type;

        using POSITION::stor;
        using POSITION::idx;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using reference = typename std::conditional<ISCONST,
              typename parent_type::const_reference,
              typename parent_type::reference>::type;
        using size_type = typename parent_type::size_type;
        using difference_type = typename parent_type::difference_type;
        using value_type = typename parent_type::value_type;
        using pointer = Iterator<POSITION, ISCONST>;
        using const_pointer = Iterator<POSITION, true>;

        // "magic" constructor to be used by SOA container classes only
        //
        // it's "magic" in the sense that it's kind of hard to get at the
        // POSITION type accidentally, since none of the SOA container classes
        // expose this type in their public interface, so there is little
        // danger of inadvertently creating an Iterator compatible with SOA
        // containers that is in a funky state because the user accidentally
        // passed funny values to the constructor.
        template <typename POS>
        constexpr explicit Iterator(POS&& pos,
            typename std::enable_if<std::is_base_of<POSITION,
            POS>::value>::type* = nullptr) noexcept :
            POSITION(std::forward<POS>(pos))
        {}

        // test for "nullness"
        constexpr operator bool() const noexcept
        { return stor() && (idx() < std::get<0>(*stor()).size()); }

        // deference/index/operator->
        constexpr reference operator*() const noexcept
        { return reference{ POSITION(*this) }; }
        reference* operator->() noexcept
        { return reinterpret_cast<reference*>(this); }
        constexpr reference operator[](size_type ofs) const noexcept
        { return *((*this) + ofs); }

        // pointers convert to const_pointers, but not vice-versa
        constexpr operator const_pointer() const noexcept
        { return const_pointer{ POSITION(*this) }; }

        /// pointer arithmetic
        pointer& operator++() noexcept { ++idx(); return *this; }
        pointer& operator--() noexcept { --idx(); return *this; }
        pointer operator++(int) noexcept
        { auto retVal(*this); ++idx(); return retVal; }
        pointer operator--(int) noexcept
        { auto retVal(*this); --idx(); return retVal; }

        pointer& operator+=(difference_type inc) noexcept
        { idx() += inc; return *this; }
        pointer& operator-=(difference_type inc) noexcept
        { idx() -= inc; return *this; }

        // distance between iterators
        difference_type operator-(const pointer& p) const noexcept
        { return idx() - p.idx(); }

        // pointer equality
        friend constexpr bool operator==(
                const pointer& p, const pointer& q) noexcept
        { return p.stor() == q.stor() && p.idx() == q.idx(); }
        friend constexpr bool operator!=(
                const pointer& p, const pointer& q) noexcept
        { return !(p == q); }

        // pointer comparisons - ordering works only if they point into same
        // container instance
        friend constexpr bool operator<(
                const pointer& p, const pointer& q) noexcept
        {
            return (p.stor() < q.stor()) ||
                (!(q.stor() < p.stor()) && p.idx() < q.idx());
        }
        friend constexpr bool operator>(
                const pointer& p, const pointer& q) noexcept
        { return q < p; }
        friend constexpr bool operator<=(
                const pointer& p, const pointer& q) noexcept
        { return !(p > q); }
        friend constexpr bool operator>=(
                const pointer& p, const pointer& q) noexcept
        { return !(p < q); }

        friend std::ostream& operator<<(std::ostream& os, const pointer& p)
        {
            os << "iterator{ stor=" << p.stor() << ", pos=" << p.idx() << " }";
            return os;
        }
    };

    /// iterator + integer constant (or similar)
    template <typename POSITION, bool CONST, typename INC>
    constexpr typename std::enable_if<std::is_convertible<
              typename Iterator<POSITION, CONST>::difference_type, INC>::value,
    Iterator<POSITION, CONST> >::type operator+(
            const Iterator<POSITION, CONST>& pos, INC inc) noexcept
    { return Iterator<POSITION, CONST>(pos) += inc; }

    /// integer constant (or similar) + iterator
    template <typename POSITION, bool CONST, typename INC>
    constexpr typename std::enable_if<std::is_convertible<
              typename Iterator<POSITION, CONST>::difference_type, INC>::value,
    Iterator<POSITION, CONST> >::type operator+(INC inc,
            const Iterator<POSITION, CONST>& pos) noexcept
    { return Iterator<POSITION, CONST>(pos) += inc; }

    /// iterator - integer constant (or similar)
    template <typename POSITION, bool CONST, typename INC>
    constexpr typename std::enable_if<std::is_convertible<
              typename Iterator<POSITION, CONST>::difference_type, INC>::value,
    Iterator<POSITION, CONST> >::type operator-(
            const Iterator<POSITION, CONST>& pos, INC inc) noexcept
    { return Iterator<POSITION, CONST>(pos) -= inc; }

} // namespace SOA

#endif // SOAITERATOR_H

// vim: sw=4:tw=78:ft=cpp:et
