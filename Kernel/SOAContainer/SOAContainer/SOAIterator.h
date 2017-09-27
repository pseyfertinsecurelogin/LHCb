/** @file SOAIterator.h
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-10-02
 */

#ifndef SOAITERATOR_H
#define SOAITERATOR_H

#include <tuple>
#include <iterator>
#include <ostream>

// forward decls
template <typename PROXY>
class SOAConstIterator;
template<typename T>
std::ostream& operator<<(std::ostream&, const SOAConstIterator<T>&);
template < template <typename...> class CONTAINER,
    template <typename> class SKIN, typename... FIELDS>
class _SOAContainer;

/** @brief class mimicking a const pointer to pointee inidicated by PROXY
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-05-03
 *
 * @tparam PROXY        proxy class
 */
template <typename PROXY>
class SOAConstIterator
{
    protected:
        /// parent type (underlying container)
        using parent_type = typename PROXY::parent_type;
        // parent container is a friend
        friend parent_type;
        /// corresponding _SOAContainers are friends
        template < template <typename...> class CONTAINER,
                 template <typename> class SKIN, typename... FIELDS>
        friend class _SOAContainer;
        /// parent's proxy type
        using proxy = PROXY;
        // underlying "dressed" proxy is friend as well
        friend proxy;
        /// parent's naked proxy type
        using naked_proxy = typename parent_type::naked_proxy;
        // underlying "naked" proxy is friend as well
        friend naked_proxy;

        proxy m_proxy; ///< pointee


    public:
        /// convenience using = for our own type
        using self_type = SOAConstIterator<proxy>;
        /// import value_type from proxy
        using value_type = typename proxy::value_type;
        /// import size_type from proxy
        using size_type = typename proxy::size_type;
        /// import difference_type from proxy
        using difference_type = typename proxy::difference_type;
        /// using = for reference to pointee
        using reference = const proxy;
        /// using = for const reference to pointee
        using const_reference = const proxy;
        /// using = for pointer
        using pointer = typename proxy::pointer;
        /// using = for const pointer
        using const_pointer = typename proxy::const_pointer;
        /// iterator category
        using iterator_category = std::random_access_iterator_tag;

    protected:
        /// constructor building proxy in place
        explicit SOAConstIterator(typename proxy::SOAStorage* storage,
                size_type index, typename proxy::parent_type::its_safe_tag
                safe) noexcept : m_proxy(storage, index, safe) { }

    public:
        /// default constructor (nullptr equivalent)
        SOAConstIterator() noexcept : SOAConstIterator(nullptr, 0) { }

        /// copy constructor
        SOAConstIterator(const self_type& other) noexcept = default;
        /// move constructor
        SOAConstIterator(self_type&& other) noexcept = default;

        /// assignment
        self_type& operator=(const self_type& other) noexcept
        { m_proxy.assign(other.m_proxy); return *this; }
        /// assignment (move semantics)
        self_type& operator=(self_type&& other) noexcept
        { m_proxy.assign(std::move(other.m_proxy)); return *this; }

        /// deference pointer (*p)
        reference operator*() noexcept
        { return m_proxy; }
        /// deference pointer (*p)
        const_reference operator*() const noexcept
        { return m_proxy; }
        /// deference pointer (p->blah)
        reference* operator->() noexcept
        { return std::addressof(m_proxy); }
        /// deference pointer (p->blah)
        const_reference* operator->() const noexcept
        { return std::addressof(m_proxy); }

        /// (pre-)increment
        self_type& operator++() noexcept
        { ++m_proxy.m_index; return *this; }
        /// (pre-)decrement
        self_type& operator--() noexcept
        { --m_proxy.m_index; return *this; }
        /// (post-)increment
        self_type operator++(int) noexcept
        { self_type retVal(*this); ++m_proxy.m_index; return retVal; }
        /// (post-)decrement
        self_type operator--(int) noexcept
        { self_type retVal(*this); --m_proxy.m_index; return retVal; }
        /// advance by dist elements
        self_type& operator+=(difference_type dist) noexcept
        { m_proxy.m_index += dist; return *this; }
        /// "retreat" by dist elements
        self_type& operator-=(difference_type dist) noexcept
        { m_proxy.m_index -= dist; return *this; }
        /// advance by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type&
                >::type operator+=(T dist) noexcept
        { m_proxy.m_index += dist; return *this; }
        /// "retreat" by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type&
                >::type operator-=(T dist) noexcept
        { m_proxy.m_index -= dist; return *this; }
        /// advance by dist elements
        self_type operator+(difference_type dist) const noexcept
        { return self_type(*this) += dist; }
        /// "retreat" by dist elements
        self_type operator-(difference_type dist) const noexcept
        { return self_type(*this) -= dist; }
        /// advance by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type
                >::type operator+(T dist) const noexcept
        { return self_type(*this) += dist; }
        /// "retreat" by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type
                >::type operator-(T dist) const noexcept
        { return self_type(*this) -= dist; }
        /// distance between two pointers
        difference_type operator-(const self_type& other) const noexcept
        {
            // give warning about buggy code subtracting pointers from
            // different containers (ill-defined operation on this pointer
            // class), use plain C style assert here
            assert(m_proxy.m_storage &&
                    m_proxy.m_storage == other.m_proxy.m_storage);
#if !defined(BREAKACTIVELY) && !defined(NDEBUG)
            return (m_proxy.m_index - other.m_proxy.m_index);
#else
            // return distance if pointers from same container, else return
            // ridiculously large value in the hopes of badly breaking
            // ill-behaved client code (when asserts are disabled)
            return (m_proxy.m_storage &&
                    m_proxy.m_storage == other.m_proxy.m_storage) ?
                (m_proxy.m_index - other.m_proxy.m_index) :
                std::numeric_limits<difference_type>::max();
#endif
        }

        /// indexing
        reference operator[](size_type idx) noexcept
        { return { m_proxy.m_storage, m_proxy.m_index + idx }; }
        /// indexing
        const_reference operator[](size_type idx) const noexcept
        { return { m_proxy.m_storage, m_proxy.m_index + idx }; }

        /// comparison (equality)
        bool operator==(const self_type& other) const noexcept
        {
            return m_proxy.m_index == other.m_proxy.m_index &&
                m_proxy.m_storage == other.m_proxy.m_storage;
        }
        /// comparison (inequality)
        bool operator!=(const self_type& other) const noexcept
        {
            return m_proxy.m_index != other.m_proxy.m_index ||
                m_proxy.m_storage != other.m_proxy.m_storage;
        }
        /// comparison (less than)
        bool operator<(const self_type& other) const noexcept
        {
            return m_proxy.m_storage < other.m_proxy.m_storage ? true :
                other.m_proxy.m_storage < m_proxy.m_storage ? false :
                m_proxy.m_index < other.m_proxy.m_index;
        }
        /// comparison (greater than)
        bool operator>(const self_type& other) const noexcept
        {
            return m_proxy.m_storage < other.m_proxy.m_storage ? false :
                other.m_proxy.m_storage < m_proxy.m_storage ? true :
                other.m_proxy.m_index < m_proxy.m_index;
        }
        /// comparison (less than or equal to)
        bool operator<=(const self_type& other) const noexcept
        {
            return m_proxy.m_storage < other.m_proxy.m_storage ? true :
                other.m_proxy.m_storage < m_proxy.m_storage ? false :
                m_proxy.m_index <= other.m_proxy.m_index;
        }
        /// comparison (greater than or equal to)
        bool operator>=(const self_type& other) const noexcept
        {
            return m_proxy.m_storage < other.m_proxy.m_storage ? false :
                other.m_proxy.m_storage < m_proxy.m_storage ? true :
                other.m_proxy.m_index <= m_proxy.m_index;
        }
        /// check for validity (if (ptr) or if (!ptr) idiom)
        operator bool() const noexcept
        {
            return m_proxy.m_storage &&
                m_proxy.m_index < std::get<0>(*m_proxy.m_storage).size();
        }

    protected:
        /// give access to underlying storage pointer
        auto storage() const noexcept -> decltype(&*m_proxy.m_storage)
        { return &*m_proxy.m_storage; }
        /// give access to index into storage
        auto index() const noexcept -> decltype(m_proxy.m_index)
        { return m_proxy.m_index; }
        /// make operator<< friend to allow calling storage() and index()
        template <typename T>
        friend std::ostream& operator<<(std::ostream&, const SOAConstIterator<T>&);
};

/** @brief class mimicking a pointer to pointee inidicated by PROXY
 *
 * @author Manuel Schiller <Manuel.Schiller@cern.ch>
 * @date 2015-05-03
 *
 * @tparam PROXY        proxy class
 */
template <typename PROXY>
class SOAIterator : public SOAConstIterator<PROXY>
{
    private:
        /// parent type (underlying container)
        using parent_type = typename PROXY::parent_type;
        // parent container is a friend
        friend parent_type;
        /// parent's proxy type
        using proxy = PROXY;
        // underlying "dressed" proxy is friend as well
        friend proxy;
        /// corresponding _SOAContainers are friends
        template < template <typename...> class CONTAINER,
                 template <typename> class SKIN, typename... FIELDS>
        friend class _SOAContainer;
        /// parent's naked proxy type
        using naked_proxy = typename parent_type::naked_proxy;
        // underlying "naked" proxy is friend as well
        friend naked_proxy;

    public:
        /// convenience using = for our own type
        using self_type = SOAIterator<proxy>;
        /// import value_type from proxy
        using value_type = typename proxy::value_type;
        /// import size_type from proxy
        using size_type = typename proxy::size_type;
        /// import difference_type from proxy
        using difference_type = typename proxy::difference_type;
        /// using = for reference to pointee
        using reference = proxy;
        /// using = for const reference to pointee
        using const_reference = const proxy;
        /// using = for pointer
        using pointer = SOAIterator<proxy>;
        /// using = for const pointer
        using const_pointer = SOAConstIterator<proxy>;
        /// iterator category
        using iterator_category = std::random_access_iterator_tag;

    private:
        /// constructor building proxy in place
        explicit SOAIterator(typename proxy::parent_type::SOAStorage* storage,
                size_type index, typename proxy::parent_type::its_safe_tag safe) noexcept :
            SOAConstIterator<proxy>(storage, index, safe) { }

    public:
        /// default constructor (nullptr equivalent)
        SOAIterator() noexcept : SOAIterator(nullptr, 0) { }

        /// copy constructor
        SOAIterator(const self_type& other) noexcept = default;
        /// move constructor
        SOAIterator(self_type&& other) noexcept = default;

        /// assignment
        self_type& operator=(const self_type& other) noexcept
        { SOAConstIterator<proxy>::operator=(other); return *this; }
        /// assignment (move semantics)
        self_type& operator=(self_type&& other) noexcept
        { SOAConstIterator<proxy>::operator=(std::move(other)); return *this; }

        /// deference pointer (*p)
        reference operator*() noexcept
        { return SOAConstIterator<proxy>::m_proxy; }
        /// deference pointer (*p)
        const_reference operator*() const noexcept
        { return SOAConstIterator<proxy>::m_proxy; }
        /// deference pointer (p->blah)
        reference* operator->() noexcept
        { return std::addressof(SOAConstIterator<proxy>::m_proxy); }
        /// deference pointer (p->blah)
        const_reference* operator->() const noexcept
        { return std::addressof(SOAConstIterator<proxy>::m_proxy); }

        /// (pre-)increment
        self_type& operator++() noexcept
        { SOAConstIterator<proxy>::operator++(); return *this; }
        /// (pre-)decrement
        self_type& operator--() noexcept
        { SOAConstIterator<proxy>::operator--(); return *this; }
        /// (post-)increment
        self_type operator++(int) noexcept
        { self_type retVal(*this); operator++(); return retVal; }
        /// (post-)decrement
        self_type operator--(int) noexcept
        { self_type retVal(*this); operator--(); return retVal; }
        /// advance by dist elements
        self_type& operator+=(difference_type dist) noexcept
        { SOAConstIterator<proxy>::operator+=(dist); return *this; }
        /// "retreat" by dist elements
        self_type& operator-=(difference_type dist) noexcept
        { SOAConstIterator<proxy>::operator-=(dist); return *this; }
        /// advance by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type
                >::type operator+=(T dist) noexcept
        { SOAConstIterator<proxy>::operator+=(dist); return *this; }
        /// "retreat" by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type
                >::type operator-=(T dist) noexcept
        { SOAConstIterator<proxy>::operator-=(dist); return *this; }
        /// advance by dist elements
        self_type operator+(difference_type dist) const noexcept
        { return self_type(*this) += dist; }
        /// "retreat" by dist elements
        self_type operator-(difference_type dist) const noexcept
        { return self_type(*this) -= dist; }
        /// advance by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type
                >::type operator+(T dist) const noexcept
        { return self_type(*this) += dist; }
        /// "retreat" by dist elements
        template <typename T>
        typename std::enable_if<
                std::is_integral<T>::value &&
                std::is_convertible<T, difference_type>::value, self_type
                >::type operator-(T dist) const noexcept
        { return self_type(*this) -= dist; }
        /// return distance between two pointers
        difference_type operator-(
                const SOAConstIterator<proxy>& other) const noexcept
        { return SOAConstIterator<proxy>::operator-(other); }

        /// indexing
        reference operator[](size_type idx) noexcept
        { return { SOAConstIterator<proxy>::m_proxy.m_storage,
                     SOAIterator<proxy>::m_proxy.m_index + idx }; }
        /// indexing
        const_reference operator[](size_type idx) const noexcept
        { return { SOAConstIterator<proxy>::m_proxy.m_storage,
                     SOAIterator<proxy>::m_proxy.m_index + idx }; }
};

/// implement integer + SOAIterator
template <typename PROXY, typename T>
typename std::enable_if<
    std::is_integral<T>::value && std::is_convertible<T,
        typename SOAIterator<PROXY>::difference_type>::value,
    SOAIterator<PROXY> >::type
    operator+(T dist, const SOAIterator<PROXY>& other) noexcept
{ return other + dist; }

/// implement integer + SOAConstIterator
template <typename PROXY, typename T>
typename std::enable_if<
    std::is_integral<T>::value && std::is_convertible<T,
        typename SOAConstIterator<PROXY>::difference_type>::value,
    SOAConstIterator<PROXY> >::type
    operator+(T dist, const SOAConstIterator<PROXY>& other) noexcept
{ return other + dist; }

/// operator<< for supporting idioms like "std::cout << it" (mostly debugging)
template<typename T>
std::ostream& operator<<(std::ostream& os, const SOAConstIterator<T>& it) {
    os << "(" << it.storage() << ", " << it.index() << ")";
    return os;
}

#endif // SOAITERATOR_H

// vim: sw=4:tw=78:ft=cpp:et
