
#ifndef LHCBKERNEL_STLExtensions
#define LHCBKERNEL_STLExtensions 1

// This header contains methods that are likely to become parts of the STL
// at some point in the future, but currently are not available.

#include <array>
#include <type_traits>

namespace LHCb
{

  /// An implementation (cut down) of 'std::make_array' based on
  /// http://en.cppreference.com/w/cpp/experimental/make_array
  /// Once we target gcc 6.1 or newer only, this can be removed
  /// and <experimental/array> directly used instead.
  template <typename... T>
  constexpr decltype(auto) make_array( T && ... values )
  {
    return std::array< 
    typename std::decay< 
    typename std::common_type<T... >::type >::type, sizeof...(T) > 
    { std::forward<T>(values)... };
  }
  

  // An implemention of std::invoke based on
  // http://en.cppreference.com/w/cpp/utility/functional/invoke
  // Once we target c++1z as a compile mode, this can be
  // replaced with std::invoke.
  namespace details {
      template <class T>
      struct is_reference_wrapper : std::false_type {};
      template <class U>
      struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

      template <class Base, class T, class Derived, class... Args>
      auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
          noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
       -> std::enable_if_t<std::is_function<T>::value &&
                           std::is_base_of<Base, std::decay_t<Derived>>::value,
          decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>
      {
            return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
      }

      template <class Base, class T, class RefWrap, class... Args>
      auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
          noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
       -> std::enable_if_t<std::is_function<T>::value &&
                           is_reference_wrapper<std::decay_t<RefWrap>>::value,
          decltype((ref.get().*pmf)(std::forward<Args>(args)...))>

      {
            return (ref.get().*pmf)(std::forward<Args>(args)...);
      }

      template <class Base, class T, class Pointer, class... Args>
      auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
          noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
       -> std::enable_if_t<std::is_function<T>::value &&
                           !is_reference_wrapper<std::decay_t<Pointer>>::value &&
                           !std::is_base_of<Base, std::decay_t<Pointer>>::value,
          decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>
      {
            return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
      }

      template <class Base, class T, class Derived>
      auto INVOKE(T Base::*pmd, Derived&& ref)
          noexcept(noexcept(std::forward<Derived>(ref).*pmd))
       -> std::enable_if_t<!std::is_function<T>::value &&
                           std::is_base_of<Base, std::decay_t<Derived>>::value,
          decltype(std::forward<Derived>(ref).*pmd)>
      {
            return std::forward<Derived>(ref).*pmd;
      }

      template <class Base, class T, class RefWrap>
      auto INVOKE(T Base::*pmd, RefWrap&& ref)
          noexcept(noexcept(ref.get().*pmd))
       -> std::enable_if_t<!std::is_function<T>::value &&
                           is_reference_wrapper<std::decay_t<RefWrap>>::value,
          decltype(ref.get().*pmd)>
      {
            return ref.get().*pmd;
      }

      template <class Base, class T, class Pointer>
      auto INVOKE(T Base::*pmd, Pointer&& ptr)
          noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
       -> std::enable_if_t<!std::is_function<T>::value &&
                           !is_reference_wrapper<std::decay_t<Pointer>>::value &&
                           !std::is_base_of<Base, std::decay_t<Pointer>>::value,
          decltype((*std::forward<Pointer>(ptr)).*pmd)>
      {
            return (*std::forward<Pointer>(ptr)).*pmd;
      }

      template <class F, class... Args>
      auto INVOKE(F&& f, Args&&... args)
          noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
       -> std::enable_if_t<!std::is_member_pointer<std::decay_t<F>>::value,
          decltype(std::forward<F>(f)(std::forward<Args>(args)...))>
      {
            return std::forward<F>(f)(std::forward<Args>(args)...);
      }
  } // namespace details


  template< class F, class... ArgTypes >
  auto invoke(F&& f, ArgTypes&&... args)
      // exception specification for QoI
      noexcept(noexcept(details::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...)))
   -> decltype(details::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...))
  {
      return details::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
  }
}

#endif // LHCBKERNEL_STLExtensions
