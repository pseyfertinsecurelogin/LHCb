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
#include <cassert>
#include <climits>
#include <cstdint>
#include <type_traits>

class stack_allocator_base {
protected:
    template <typename T, std::size_t BITS = CHAR_BIT * sizeof(T)>
    struct uint_bits_pair { using type = T; enum { bits = BITS }; };

    template <std::size_t BITS, typename... ARGS>
    struct find_first_matching_uint;
    template <std::size_t BITS>
    struct find_first_matching_uint<BITS> { using type = std::size_t; };
    template <std::size_t BITS, typename ARG, typename... ARGS>
    struct find_first_matching_uint<BITS, ARG, ARGS...>
        : std::conditional<BITS <= ARG::bits, ARG,
        find_first_matching_uint<BITS, ARGS...> >::type {};

    template <std::size_t BITS>
    struct sufficient_uint : find_first_matching_uint<BITS,
        uint_bits_pair<unsigned char>, uint_bits_pair<unsigned short>,
        uint_bits_pair<unsigned>, uint_bits_pair<unsigned long>,
        uint_bits_pair<unsigned long long>, uint_bits_pair<uint8_t, 8>,
        uint_bits_pair<uint16_t, 16>, uint_bits_pair<uint32_t, 32>,
        uint_bits_pair<uint64_t, 64> > {};

    constexpr static std::size_t log2(std::size_t n) noexcept
    { return (n <= 1) ? 0 : 1 + log2(n / 2); }
    constexpr static std::size_t ceil_log2(std::size_t n) noexcept
    { return log2(n) + bool(n & (n - 1)); }
};

template <typename T, std::size_t N, std::size_t ALIGN = 64>
class stack_allocator : protected stack_allocator_base {
#if defined(__GNUC__) && !defined(__clang__) && !defined(_INTEL_COMPILER) && __GNUC__ < 5
    // gcc versions before gcc 5.0 don't have std::is_trivially_copyable and
    // friends - work around using the following hack - this may be somewhat
    // more restrictive than desirable, but should be safe
    template <typename U>
    using is_trivially_copyable = std::integral_constant<bool, __has_trivial_copy(U)>;
    template <typename U>
    using is_trivially_move_assignable = std::integral_constant<bool,
          __has_trivial_assign(U) && __has_trivial_copy(U)>;
#else
    template <typename U>
    using is_trivially_copyable = std::is_trivially_copyable<U>;
    template <typename U>
    using is_trivially_move_assignable = std::is_trivially_move_assignable<U>;
#endif
public:
    using size_type = typename stack_allocator_base::sufficient_uint<
        stack_allocator_base::ceil_log2(N + 1)>::type;
    using difference_type = typename std::make_signed<size_type>::type;
    using value_type = T;
    using propagate_on_container_copy_assignment = is_trivially_copyable<T>;
    using propagate_on_container_move_assignment = is_trivially_move_assignable<T>;
    using propagate_on_container_swap = std::integral_constant<bool,
          is_trivially_move_assignable<T>::value || is_trivially_copyable<T>::value>;
    using is_always_equal = std::false_type;
    template <typename U>
    struct rebind { using other = stack_allocator<U, N, ALIGN>; };

private:
    static_assert(N, "stack_allocator: N must be positive");
    static_assert(ALIGN, "stack_allocator: ALIGN must be positive");
    static_assert(0 == (ALIGN % alignof(T)),
            "stack_allocator: T needs different alignment");
    alignas(ALIGN) unsigned char m_mem[N * sizeof(T)];
    size_type m_used = 0;

public:
    constexpr size_type max_size() const noexcept { return N; }

    T* allocate(std::size_t n, const void* /* hint */ = nullptr)
    {
        assert(n <= N);
        return ((m_used + n) <= max_size()) ?
            (reinterpret_cast<T*>(&m_mem[sizeof(T) * ((m_used += n) - n)])) :
            (throw std::bad_alloc(), nullptr);
    }

    void deallocate(T* p, size_type n) noexcept
    {
        assert(n <= N);
        assert(&m_mem[0] <= reinterpret_cast<unsigned char*>(p) &&
                reinterpret_cast<unsigned char*>(p) <= &m_mem[sizeof(T) * N]);
        // [NOTE]: If memory is not deallocated in reverse order of
        // allocations (i.e. LIFO), the "holes" do not become available for
        // subsequent allocation. That memory is freed eventually when the
        // allocator goes out of scope (no need for a free list or similar on
        // a stack-based allocator, and the typical growth pattern of
        // std::vector would not allow that space to be used efficiently).
        // Users should just reserve the right amount on construction of their
        // container.
        m_used -= n * bool(reinterpret_cast<unsigned char*>(p) ==
                &m_mem[sizeof(T) * (m_used - n)]);
    }

    constexpr stack_allocator select_on_container_copy_construction() const noexcept
    { return stack_allocator{}; }

    constexpr bool operator==(const stack_allocator<T, N, ALIGN>& other) const noexcept
    { return this == &other; }
    constexpr bool operator!=(const stack_allocator<T, N, ALIGN>& other) const noexcept
    { return this != &other; }
};


// vim: sw=4:tw=78:ft=cpp:et
