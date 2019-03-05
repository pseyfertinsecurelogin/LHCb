/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#ifndef SOAContainerSet_h
#define SOAContainerSet_h
#include "IncompatibleZipException.h" // for IncompatibleZipException
#include "SOAContainer/SOASkin.h"     // for is_skin
#include "SOAContainer/SOAUtils.h"    // for ALL, is_view
#include "SOAContainer/SOAView.h"     // for zip
#include "ZipTraits.h"
#include "ZipUtils.h"
#include <type_traits> // for remove_reference_t, enable_if_t
#include <utility>     // for forward

// for id initialization
#define BOOST_UUID_USE_SSE41 1
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

template <typename FIRST, typename... OTHERS>
int firstid( FIRST& first, OTHERS&... /*unused*/ ) {
  return first.ZipIdentifier();
}

template <typename CONTAINER>
class ZipContainer : public CONTAINER {
public:
  using view = CONTAINER;

  template <typename... Args>
  ZipContainer( int ID, Args&&... args ) : CONTAINER( args... ), m_identifier( ID ) {}

  ZipContainer() : CONTAINER() {
    boost::uuids::uuid tag{boost::uuids::random_generator()()};
    m_identifier =
        int{( tag.data[0] << 8 * 0 ) + ( tag.data[1] << 8 * 1 ) + ( tag.data[2] << 8 * 2 ) + ( tag.data[3] << 8 * 3 )};
  }

  template <typename T>
  auto push_back( T&& t )
      -> std::enable_if_t<view::self_type::fields_typelist::size() == 1 &&
                          std::is_same_v<std::remove_reference_t<T>,
                                         typename view::self_type::fields_typelist::template at<0>::type::type>> {
    CONTAINER::emplace_back( std::forward<T>( t ) );
  }
  template <typename T>
  auto push_back( const T& t )
      -> std::enable_if_t<view::self_type::fields_typelist::size() == 1 &&
                          std::is_same_v<std::remove_reference_t<T>,
                                         typename view::self_type::fields_typelist::template at<0>::type::type>> {
    CONTAINER::emplace_back( std::forward<const T>( t ) );
  }

private:
  int m_identifier;

public:
  int ZipIdentifier() const { return m_identifier; }
};

template <template <class> class SKIN, typename... IDeds,
          typename = typename std::enable_if_t<SOA::Utils::ALL(
              SOA::impl::is_skin<SKIN>(),
              has_semantic_zip<typename std::remove_cv_t<typename std::remove_reference_t<IDeds>>>::value... )>>
auto semantic_zip( IDeds&&... views ) {
#ifndef NDEBUG
  if ( !are_semantically_compatible( views... ) ) { throw IncompatibleZipException( "zipping from different sets" ); }
#endif

  using barezip = decltype(
      zip<SKIN,
          typename std::conditional_t<std::is_reference_v<IDeds>, const typename std::remove_reference_t<IDeds>::view&,
                                      const typename std::remove_reference_t<IDeds>::view>...>(
          std::forward<typename std::conditional_t<std::is_reference_v<IDeds>,
                                                   const typename std::remove_reference_t<IDeds>::view&,
                                                   const typename std::remove_reference_t<IDeds>::view>>(
              views )... ) );

  return ZipContainer<barezip>(
      firstid( views... ),
      zip<SKIN,
          typename std::conditional_t<std::is_reference_v<IDeds>, const typename std::remove_reference_t<IDeds>::view&,
                                      const typename std::remove_reference_t<IDeds>::view>...>(
          std::forward<typename std::conditional_t<std::is_reference_v<IDeds>,
                                                   const typename std::remove_reference_t<IDeds>::view&,
                                                   const typename std::remove_reference_t<IDeds>::view>>(
              views )... ) );
}

#endif
