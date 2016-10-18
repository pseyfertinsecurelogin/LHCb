
#ifndef RICHUTILS_MakeArray
#define RICHUTILS_MakeArray 1

// An implementation (cut down) of 'std::make_array' based on
// http://en.cppreference.com/w/cpp/experimental/make_array
// Once we target gcc 6.1 or newer only, this can be removed
// and <experimental/array> directly used instead.

#include <array>
#include <type_traits>

namespace Rich
{

  template <typename... T>
  constexpr decltype(auto) make_array( T && ... values )
  {
    return std::array< 
    typename std::decay< 
    typename std::common_type<T... >::type >::type, sizeof...(T) > 
    { std::forward<T>(values)... };
  }
  
}

#endif // RICHUTILS_MakeArray
