
#ifndef RICHUTILS_ZipRange_H
#define RICHUTILS_ZipRange_H 1

#include "boost/iterator/zip_iterator.hpp"

namespace Rich
{
  /// Zips mulitple containers together to form a single range
  template<class... Conts>
  decltype(auto) ZipRange( Conts&... conts )
  {
    return boost::make_iterator_range
      ( boost::make_zip_iterator(boost::make_tuple(conts.begin()...)),
        boost::make_zip_iterator(boost::make_tuple(conts.end()...  )) );
  } 
}

#endif // RICHUTILS_ZipRange_H
