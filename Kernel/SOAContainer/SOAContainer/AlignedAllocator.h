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
/** @file AlignedAllocator.h
 *
 * @brief allocator class that allows classes like std::vector to make
 * guarantees about memory alignment.
 *
 * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
 * @date 2017-03-28
 */

#ifndef ALIGNEDALLOCATOR_H
#define ALIGNEDALLOCATOR_H

namespace SOA {
    /** @brief aligned allocator
     *
     * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
     * @date 2017-03-28
     *
     * This class allows for allocation of aligned memory for classes like
     * std::vector. It is used this way:
     *
     * @code
     * #include <vector>
     * #include "AlignedAllocator.h"
     * template <typename T> using aligned_vector =
     *     std::vector<T, AlignedAllocator<T, 64> >;
     * aligned_vector<int> v;
     * v.push_back(42); // more operations
     * @endcode
     *
     * The class has the following template parameters:
     *
     * @tparam T            type for which to allocate memory
     * @tparam ALIGN        alignment in bytes
     *
     * ALIGN must be a power of two between 1 and 128 bytes. (Larger alignment
     * requirements are better satisfied with a more complex allocator.)
     */
    template <typename T, std::size_t ALIGN>
    class AlignedAllocator {
    private:
        // a few sanity checks...
        static_assert(ALIGN > 0, "ALIGN must be positive.");
        static_assert(ALIGN <= 128, "ALIGN must be 128 or smaller.");
        static_assert(0 == (ALIGN & (ALIGN - 1)),
                      "ALIGN must be a power of 2.");
        static_assert(0 == (ALIGN % alignof(T)),
                      "ALIGN not suitable for type T");

        std::allocator<char> alloc; ///< underlying allocator

    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_copy_assignment = std::true_type;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_swap = std::true_type;
        using is_always_equal = std::true_type;
        constexpr AlignedAllocator<T, ALIGN>
        select_on_container_copy_construction() const noexcept
        {
            return {};
        }

        AlignedAllocator() = default;
        AlignedAllocator(const AlignedAllocator<T, ALIGN>&) = default;
        AlignedAllocator(AlignedAllocator<T, ALIGN>&&) = default;
        AlignedAllocator<T, ALIGN>&
        operator=(const AlignedAllocator<T, ALIGN>&) = default;
        AlignedAllocator<T, ALIGN>&
        operator=(AlignedAllocator<T, ALIGN>&&) = default;
        template <typename U>
        constexpr AlignedAllocator(const AlignedAllocator<U, ALIGN>&)
        {}


        template <typename U>
        struct rebind {
            using other = AlignedAllocator<U, ALIGN>;
        };

        pointer allocate(size_type n, const void* hint = nullptr)
        {
            char* p = alloc.allocate(n * sizeof(T) + ALIGN, hint);
            p += ALIGN;
            auto adj = std::ptrdiff_t(p) & (ALIGN - 1);
            p -= adj;
            *(p - 1) = adj;
            return (T*) p;
        }

        void deallocate(pointer p, std::size_t n)
        {
            auto adj = *(((unsigned char*) p) - 1);
            char* q = ((char*) p) + adj - ALIGN;
            alloc.deallocate(q, n * sizeof(T) + ALIGN);
        }

        constexpr size_type max_size() const noexcept
        {
            return std::numeric_limits<size_type>::max() / sizeof(T);
        }
        constexpr bool operator==(const AlignedAllocator<T, ALIGN>&) const
                noexcept
        {
            return true;
        }
        constexpr bool operator!=(const AlignedAllocator<T, ALIGN>&) const
                noexcept
        {
            return false;
        }

#if defined(__GNUC__) && !defined(__clang__) &&                              \
        !defined(__INTEL_COMPILER) && __GNUC__ < 5
        void construct(pointer p, const_reference val) const
        {
            new (p) T(val);
        }
        template <typename U, typename... ARGS>
        void construct(U* p, ARGS&&... args) const
        {
            new (p) U(std::forward<ARGS>(args)...);
        }
        void destroy(pointer p) const { p->~T(); }
        template <typename U>
        void destroy(U* p) const
        {
            p->~U();
        }
        constexpr pointer address(reference r) const noexcept { return &r; }
        constexpr const_pointer address(const_reference r) const noexcept
        {
            return &r;
        }
#endif
    };

    /// convenience typedef for 64 byte alignment
    template <typename T>
    using CacheLineAlignedAllocator = AlignedAllocator<T, 64>;
} // namespace SOA

#endif // ALIGNEDALLOCATOR_H

// vim: sw=4:tw=78:ft=cpp:et
