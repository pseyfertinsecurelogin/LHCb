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
/** @file SOAIteratorRange.h
 *
 * @brief a range given by two iterators
 *
 * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
 * @date 2018-04-04
 */
#ifndef SOA_ITERATOR_RANGE_H
#define SOA_ITERATOR_RANGE_H

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>

#ifndef __GNUC__
#define __builtin_expect(c, val) ((c))
#else // __GNUC__
// gnu/clang/... and friends have __builtin_expect
#endif // __GNUC__

namespace SOA {
    /// @brief tag to mark iterator ranges
    struct iterator_range_tag {};

    /** @brief your standard, dumb iterator range
     *
     * This class models an iterator range, i.e. the range between two elements
     * first and last, where first is included in the range, but last is not.
     *
     * The class supports the usual typedefs (iterator_category,
     * difference_type, size_type, value_type, reference, pointer, iterator,
     * and, if IT is at least a bidirectional iterator, also reverse_iterator).
     *
     * Supported operations are empty(), size(), operator[], at(), begin(),
     * end(), front(), back(), and, if IT is at least a bidirectional iterator,
     * rbegin(), rend().
     *
     * @tparam IT       type of iterator
     *
     * IT must be at least a forward iterator.
     */
    template <typename IT>
    class iterator_range : public SOA::iterator_range_tag {
        private:
            IT m_first, m_last; ///< two iterators
        public:
            /// iterator_category
            using iterator_category = typename std::iterator_traits<IT>::iterator_category;
            // check constraints on IT
            static_assert(std::is_base_of<std::forward_iterator_tag,
                    iterator_category>::value,
                    "IT must be at least a forward iterator");
            /// all the remaining usual typedefs
            using difference_type = typename std::iterator_traits<IT>::difference_type;
            using size_type = typename std::make_unsigned<difference_type>::type;
            using value_type = typename std::iterator_traits<IT>::value_type;
            using reference = typename std::iterator_traits<IT>::reference;
            using pointer = typename std::iterator_traits<IT>::pointer;
            using iterator = IT;
            using reverse_iterator = std::reverse_iterator<iterator>;

            /// construct from a pair of iterators
            template <typename ITFWD1, typename ITFWD2,
                     typename DUMMY1 = ITFWD1, typename DUMMY2 = ITFWD2,
                     typename = typename std::enable_if<
                         (std::is_constructible<IT, DUMMY1>::value ||
                          std::is_convertible<DUMMY1, IT>::value) &&
                         (std::is_constructible<IT, DUMMY2>::value ||
                          std::is_convertible<DUMMY2, IT>::value)>::type>
            constexpr explicit iterator_range(ITFWD1&& first, ITFWD2&& last) :
                m_first(std::forward<ITFWD1>(first)),
                m_last(std::forward<ITFWD2>(last)) {}

            /// construct from another range (if iterators are convertible)
            template <typename JT, typename DUMMY = JT, typename = typename
                std::enable_if<std::is_constructible<IT, JT>::value ||
                std::is_convertible<JT, IT>::value>::type>
            constexpr iterator_range(const SOA::iterator_range<JT>& range) :
                iterator_range(range.begin(), range.end()) {}

            /// is range empty
            constexpr bool empty() const { return m_last == m_first; }
            /// size of range
            constexpr size_type size() const
            { return std::distance(m_first, m_last); }
            /// element at position idx
            template <typename SZ = std::size_t>
            constexpr typename std::enable_if<
                    std::is_integral<SZ>::value &&
                    std::is_base_of<std::random_access_iterator_tag,
                    iterator_category>::value,
            reference>::type operator[](SZ idx) const
            { return *(m_first + idx); }
            /// check access to element at idx (can throw!)
            template <typename SZ = std::size_t>
            typename std::enable_if<
                    std::is_integral<SZ>::value &&
                    std::is_base_of<std::random_access_iterator_tag,
                    iterator_category>::value,
            reference>::type at(SZ idx) const
            {
                if (__builtin_expect(std::size_t(idx) >= size(), false)) {
                    throw std::out_of_range("SOA::iterator_range::at("
                            "std::size_t): out of bounds");
                }
                return *(m_first + idx);
            }

            /// start of range
            constexpr iterator begin() const
            { return m_first; }
            /// end of range
            constexpr iterator end() const
            { return m_last; }
            /// start of reverse range
            template <typename DUMMY = typename std::enable_if<
                std::is_same<std::bidirectional_iterator_tag,
                iterator_category>::value>*>
            constexpr reverse_iterator rbegin(DUMMY = nullptr) const
            { return reverse_iterator{ m_last }; }
            /// end of reverse range
            template <typename DUMMY = typename std::enable_if<
                std::is_same<std::bidirectional_iterator_tag,
                iterator_category>::value>*>
            constexpr reverse_iterator rend(DUMMY = nullptr) const
            { return reverse_iterator{ m_first }; }
            /// access to first element
            constexpr reference front() const { return *begin(); }
            /// access to last element
            template <typename SZ = void*>
            typename std::enable_if<
                    std::is_same<SZ, void*>::value &&
                    !std::is_base_of<std::bidirectional_iterator_tag,
                    iterator_category>::value,
            reference>::type back(SZ = nullptr) const
            {
                auto p = begin();
                std::advance(p, size() - 1);
                return *p;
            }
            /// access to last element
            template <typename SZ = void*>
            typename std::enable_if<
                    std::is_same<SZ, void*>::value &&
                    std::is_base_of<std::bidirectional_iterator_tag,
                    iterator_category>::value &&
                    !std::is_base_of<std::random_access_iterator_tag,
                    iterator_category>::value,
            reference>::type back(SZ = nullptr) const
            { auto p = end(); --p; return *p; }
            /// access to last element
            template <typename SZ = void*>
            constexpr typename std::enable_if<
                    std::is_same<SZ, void*>::value &&
                    std::is_base_of<std::bidirectional_iterator_tag,
                    iterator_category>::value,
            reference>::type back(SZ = nullptr) const
            { return *(end() - 1); }
    };

    /** @brief build an iterator_range given two iterators
     *
     * @tparam IT       iterator type (at least forward iterators)
     *
     * @param first     start of range
     * @param last      one past end of range
     *
     * @returns iterator_range<IT> from [first, last(
     */
    template <typename IT, typename JT,
              typename = typename std::enable_if<std::is_same<
                      typename std::decay<IT>::type,
                      typename std::decay<JT>::type>::value>::type>
    constexpr iterator_range<typename std::remove_reference<IT>::type>
    make_iterator_range(IT&& first, JT&& last)
    {
        return iterator_range<typename std::remove_reference<IT>::type>{
            std::forward<IT>(first), std::forward<JT>(last) };
    }
} // namespace SOA

#ifndef __GNUC__
#undef __builtin_expect // clean up
#endif // __GNUC__

#endif // SOA_ITERATOR_RANGE_H

// vim: sw=4:tw=78:ft=cpp:et
