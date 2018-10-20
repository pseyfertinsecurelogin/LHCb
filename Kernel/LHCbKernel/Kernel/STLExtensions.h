#ifndef LHCBKERNEL_STLExtensions
#define LHCBKERNEL_STLExtensions

// This header contains methods that are likely to become parts of the STL
// at some point in the future, but currently are not available.

#include <array>
#include <type_traits>
#include <functional>
#include <memory>

// add a macro state assumptions
#ifdef NDEBUG
#define ASSUME(COND) static_cast<void>((COND) ? void(0) : __builtin_unreachable())
#else
#include <cassert>
#define ASSUME(COND) assert(COND)
#endif

#ifdef NDEBUG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define GSL_UNENFORCED_ON_CONTRACT_VIOLATION
#endif
#include "gsl/span"
#ifdef NDEBUG
#pragma GCC diagnostic pop
#endif

namespace LHCb
{
  /// An implementation (cut down) of 'std::make_array' based on
  /// http://en.cppreference.com/w/cpp/experimental/make_array
  /// Once we target gcc 6.1 or newer only, this can be removed
  /// and <experimental/array> directly used instead.
  template <typename... T>
  [[deprecated("use class template argument deduction instead, i.e. replace LHCb::make_array(...) with std::array{...}")]]
  constexpr decltype(auto) make_array( T && ... values )
  {
    return std::array{ std::forward<T>(values)... };
  }

  template <typename F, typename ... Args >
  [[deprecated("use std::invoke instead")]]
  decltype(auto) invoke( F&& f, Args&&... args )
  {
      return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
  }

  // TODO: when we use a more recent version of range-v3, switch to its version of span
  using gsl::span;

  // TODO: remove with a more recent version of GSL:
  //
  template <class ElementType>
  auto make_span(ElementType* ptr, typename span<ElementType>::index_type count)
  {
      return span<ElementType>(ptr, count);
  }

  template <class ElementType>
  auto make_span(ElementType* firstElem, ElementType* lastElem)
  {
      return span<ElementType>(firstElem, lastElem);
  }

  template <class ElementType, std::size_t N>
  auto make_span(ElementType (&arr)[N])
  {
      return span<ElementType, N>(arr);
  }

  template <class ElementType, std::size_t N>
  auto make_span(std::array<ElementType,N>& arr)
  {
      return span<ElementType,N>(arr);
  }

  template <class ElementType, std::size_t N>
  auto make_span(const std::array<ElementType,N>& arr)
  {
      return span<const ElementType,N>(arr);
  }

  template <class Container>
  auto make_span(Container& cont)
  {
      return span<typename Container::value_type>(cont);
  }

  template <class Container>
  auto make_span(const Container& cont)
  {
      return span<const typename Container::value_type>(cont);
  }

  template <class Ptr>
  auto make_span(Ptr& cont, std::ptrdiff_t count)
  {
      return span<typename Ptr::element_type>(cont, count);
  }

  namespace details_se {
    // see https://en.cppreference.com/w/cpp/named_req/ContiguousIterator
    // 1) array::iterator is typically a pointer, so no need to support it here
    // 2) string_view::iterator: why would ever want to turn that into a span?
    // 3) valarray: if you really need it, then feel free to add it
    template <typename Iterator>
    constexpr bool isContiguous() {
        using Value = typename std::iterator_traits<Iterator>::value_type;
        return ( std::is_same<typename std::vector<Value>::iterator,
                            Iterator>::value
                 || std::is_same<typename std::vector<Value>::const_iterator,
                           Iterator>::value )
           && !std::is_same<bool,Value>::value;
    }
  }

  template <typename Iterator, typename = std::enable_if_t<details_se::isContiguous<Iterator>()> >
  auto make_span(Iterator firstElem, Iterator lastElem)
  {
      return make_span(std::addressof(*firstElem),std::distance(firstElem,lastElem));
  }

}

// gsl::span has no `front` or `back`
// whereas range::span does -- so until we get a version of range::v3 which
// includes range::span, we provide some free standing functions to provide
// `front` and `back`...
namespace gsl {
   template <typename T, std::ptrdiff_t N>
   decltype(auto) front(span<T,N> s) { return *s.begin(); }

   template <typename T, std::ptrdiff_t N>
   decltype(auto) back (span<T,N> s) { return *s.rbegin(); }
}


#endif // LHCBKERNEL_STLExtensions
