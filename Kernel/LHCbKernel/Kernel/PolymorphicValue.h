#ifndef LHCB_CXX_POLYMORPHIC_VALUE_H
#define LHCB_CXX_POLYMORPHIC_VALUE_H
#include <memory>
#include <type_traits>

/** @class LHCb::cxxx::PolymorphicValue PolymorphicValue.h
 *
 * This class template PolymorphicValue<T> is intended as an almost
 * drop-in replacement for std::unique_ptr<T>, i.e. it manages a heap
 * allocated object, which (at least) derives from T.
 *
 * Where std::unique_ptr<T> is a move-only, i.e. non-copyable type,
 * PolymorphicValue<T> is copyable -- copies are created by calling T::clone().
 * As a result, PolymorpicValue<T> can only be instantiated in case
 * `T::clone() const` exists, and returns a (unique) pointer to a
 * heap-allocated (derived) instance of T
 *
 * Warning: as it is 'trivial' to copy a PolymorphicValue<T> (eg. pass one
 * to a function by value instead of doing an std::move, or by const reference),
 * and this is potentially an 'expensive' operation, please make sure that no
 * unnecessary copies are made, eg. always check whether a move would be more
 * appropriate. Basically, avoid in interfaces, and only use 'behind the
 * scenes' in implementations.
 *
 * The idea for this class is based on the more general, non-intrusive
 * `polymorphic_type` which is proposed for standardization,
 * see https://github.com/jbcoe/polymorphic_value.
 *
 * @author Gerhard Raven
 * @date 2018/10/16
 *
 */

namespace LHCb {
namespace cxx {

namespace details {

    template <typename Interface, typename Ret>
    constexpr bool has_clone_v = std::is_convertible_v< decltype( std::declval<Interface const&>().clone() ), Ret>;

    template <typename Interface,typename T = void>
    using require_clone_t = std::enable_if_t< has_clone_v<Interface,Interface*> ||
                                              has_clone_v<Interface,std::unique_ptr<Interface>>,
                                              T >;

    template <typename Interface>
    require_clone_t<Interface,std::unique_ptr<Interface>> clone(Interface const* i) {
      if constexpr ( has_clone_v<Interface,std::unique_ptr<Interface>> ) {
        return i ? i->clone() : std::unique_ptr<Interface>{};
      } else if constexpr ( has_clone_v<Interface,Interface*> ) {
        return std::unique_ptr<Interface>{ i ? i->clone() : nullptr };
      }
    }
}

// (almost) drop-in replacement for a `std::unique_ptr<Interface>`
// which is copyable, _provided_ that `Interface` has a `clone()` function

template <typename Interface, typename = details::require_clone_t<Interface>>
class PolymorphicValue final {
    std::unique_ptr<Interface> m_ptr;
public:
    PolymorphicValue() = default;
    explicit PolymorphicValue(std::unique_ptr<Interface> ptr) noexcept : m_ptr{ std::move(ptr) } {}
    PolymorphicValue(PolymorphicValue const& other) : m_ptr{ details::clone(other.m_ptr.get())  } {}
    PolymorphicValue(PolymorphicValue&& other) noexcept : m_ptr{ std::move(other.m_ptr) } {}
    PolymorphicValue& operator=(PolymorphicValue const& other) { m_ptr = details::clone(other.m_ptr.get()); return *this; }
    PolymorphicValue& operator=(PolymorphicValue&& other) noexcept { m_ptr = std::move(other.m_ptr ); return *this; }

    Interface* ptr() noexcept { return m_ptr.get(); }
    Interface const* ptr() const noexcept { return m_ptr.get(); }

    Interface* operator->() noexcept { assert(m_ptr.get()); return m_ptr.get(); }
    Interface const* operator->() const noexcept { assert(m_ptr.get()); return m_ptr.get(); }

    Interface& operator*() noexcept(noexcept(*m_ptr)) { assert(m_ptr.get()); return *m_ptr; }
    Interface const& operator*() const noexcept(noexcept(*m_ptr)) { assert(m_ptr.get()); return *m_ptr; }

    friend void swap(PolymorphicValue& lhs, PolymorphicValue& rhs) noexcept { swap(lhs.m_ptr,rhs.m_ptr); }

    explicit operator bool() const noexcept { return static_cast<bool>(m_ptr); }

    template <typename T,typename U>
    friend bool operator==(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept;
    template <typename T,typename U>
    friend bool operator!=(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept;
    template <typename T,typename U>
    friend bool operator<(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept;
    template <typename T,typename U>
    friend bool operator>(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept;
    template <typename T,typename U>
    friend bool operator<=(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept;
    template <typename T,typename U>
    friend bool operator>=(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept;

};

template <typename T,typename U>
bool operator==(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept
{ return lhs.m_ptr==rhs.m_ptr; }

template <typename T,typename U>
bool operator!=(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept
{ return lhs.m_ptr!=rhs.m_ptr; }

template <typename T,typename U>
bool operator<(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept
{ return lhs.m_ptr<rhs.m_ptr; }

template <typename T,typename U>
bool operator>(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept
{ return lhs.m_ptr>rhs.m_ptr; }

template <typename T,typename U>
bool operator<=(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept
{ return lhs.m_ptr<=rhs.m_ptr; }

template <typename T,typename U>
bool operator>=(const PolymorphicValue<T>& lhs, const PolymorphicValue<U>& rhs) noexcept
{ return lhs.m_ptr>=rhs.m_ptr; }

}
}

namespace std {
    template <typename IFace> struct hash<LHCb::cxx::PolymorphicValue<IFace>> {
      std::size_t operator()(const LHCb::cxx::PolymorphicValue<IFace>& t) const { return hash<const IFace*>{}(t.ptr()); };
    };
}

#endif
