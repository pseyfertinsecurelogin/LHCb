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

#ifndef ZipContainer_h
#define ZipContainer_h
#include "IncompatibleZipException.h" // for IncompatibleZipException
#include "SOAContainer/SOASkin.h"     // for is_skin
#include "SOAContainer/SOAUtils.h"    // for ALL, is_view
#include "SOAContainer/SOAView.h"     // for zip
#include "ZipTraits.h"
#include "ZipUtils.h"
#include <atomic>
#include <tuple>
#include <type_traits> // for remove_reference_t, enable_if_t
#include <utility>     // for forward

namespace Zipping {
  struct ZipID {
    explicit ZipID( uint64_t t ) : value( t ) {}
    uint64_t value;
    // assuming most of what can be done with uint64_t doesn't make sense as a family identifier
    explicit operator uint64_t() const { return value; }
    bool     operator==( const ZipID other ) { return value == other.value; }
    bool     operator!=( const ZipID other ) { return value != other.value; }
  };

  namespace details {
    class ZipIdProvider {
    private:
      inline static std::atomic_uint64_t generator;

    public:
      static ZipID getId() { return ZipID( generator.fetch_add( 1 ) ); }
    };
  } // namespace details

  template <typename FIRST, typename... OTHERS>
  ZipID firstid( FIRST& first, OTHERS&... /*unused*/ ) {
    return first.zipIdentifier();
  }

  template <typename CONTAINER>
  class ZipContainer : public CONTAINER {
  public:
    using view = CONTAINER;

    template <typename... Args>
    ZipContainer( ZipID ID, Args&&... args ) : CONTAINER( args... ), m_identifier( ID ) {}

    template <typename Arg, typename... Args,
              typename = typename std::enable_if_t<!std::is_same_v<ZipID, std::decay_t<Arg>>>>
    ZipContainer( Arg&& arg, Args&&... args )
        : CONTAINER( arg, args... ), m_identifier( details::ZipIdProvider::getId() ) {}

    ZipContainer() : CONTAINER(), m_identifier( details::ZipIdProvider::getId() ) {}

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
    ZipID m_identifier;

  public:
    ZipID zipIdentifier() const { return m_identifier; }
  };

  template <template <class> class SKIN, typename... IDeds,
            typename = typename std::enable_if_t<SOA::Utils::ALL(
                SOA::impl::is_skin<SKIN>(),
                has_semantic_zip<typename std::remove_cv_t<typename std::remove_reference_t<IDeds>>>::value... )>>
  auto semantic_zip( IDeds&&... views ) {
#ifndef NDEBUG
    if ( !are_semantically_compatible( views... ) ) {
      throw IncompatibleZipException( "zipping from different container family" );
    }
#endif

    using barezip =
        decltype( zip<SKIN, typename std::conditional_t<std::is_reference_v<IDeds>,
                                                        const typename std::remove_reference_t<IDeds>::view&,
                                                        const typename std::remove_reference_t<IDeds>::view>...>(
            std::forward<typename std::conditional_t<std::is_reference_v<IDeds>,
                                                     const typename std::remove_reference_t<IDeds>::view&,
                                                     const typename std::remove_reference_t<IDeds>::view>>(
                views )... ) );

    return ZipContainer<barezip>(
        firstid( views... ),
        zip<SKIN, typename std::conditional_t<std::is_reference_v<IDeds>,
                                              const typename std::remove_reference_t<IDeds>::view&,
                                              const typename std::remove_reference_t<IDeds>::view>...>(
            std::forward<typename std::conditional_t<std::is_reference_v<IDeds>,
                                                     const typename std::remove_reference_t<IDeds>::view&,
                                                     const typename std::remove_reference_t<IDeds>::view>>(
                views )... ) );
  }
} // namespace Zipping

#endif
