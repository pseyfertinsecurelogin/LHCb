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
#include "IncompatibleZipException.h"
#include "SOAContainer/SOASkin.h"
#include "SOAContainer/SOAUtils.h"
#include "SOAContainer/SOAView.h"
#include "ZipTraits.h"
#include "ZipUtils.h"
#include <tuple>
#include <type_traits>
#include <utility>

namespace Zipping {
  template <typename CONTAINER>
  class ZipContainer : public CONTAINER {
    // types and usings
  public:
    using view_t = CONTAINER;

    // Zip functionality methods
  public:
    ZipFamilyNumber zipIdentifier() const { return m_family_id; }

  private:
    ZipFamilyNumber m_family_id;

    // constructors
  public:
    // ZipContainer can be constructed with a specific ZipFamilyNumber, all other constructor arguments
    // are forwarded to the underlying SOAContainer-type. (Sorry for the cumbersome phrasing `SOAContainer-type` is to
    // make explicit that this may not be an SOA::Container, but also an SOA::View.
    template <typename... ARGS>
    ZipContainer( ZipFamilyNumber id, ARGS&&... args ) : CONTAINER( std::forward<ARGS>(args)... ), m_family_id( id ) {}

    // If the first constructor argument is not a ZipFamilyNumber, generate a new ZipFamilyNumber and consider
    // all constructor arguments as SOAContainer-type constructor arguments.
    template <typename ARG, typename... ARGS,
              typename = typename std::enable_if_t<!std::is_same_v<ZipFamilyNumber, std::decay_t<ARG>>>>
    ZipContainer( ARG&& arg, ARGS&&... args )
        : CONTAINER( arg, args... ), m_family_id( details::ZipFamilyNumberGenerator::generate() ) {}

    // Default constructor generates a new ZipFamilyNumber and default constructs the underlying SOAContainer-type.
    ZipContainer() : CONTAINER(), m_family_id( details::ZipFamilyNumberGenerator::generate() ) {}

    // convenience helpers
  public:
    template <typename T>
    auto push_back( T&& t )
        -> std::enable_if_t<view_t::self_type::fields_typelist::size() == 1 &&
                            std::is_same_v<std::remove_reference_t<T>,
                                           typename view_t::self_type::fields_typelist::template at<0>::type::type>> {
      CONTAINER::emplace_back( std::forward<T>( t ) );
    }
    template <typename T>
    auto push_back( const T& t )
        -> std::enable_if_t<view_t::self_type::fields_typelist::size() == 1 &&
                            std::is_same_v<std::remove_reference_t<T>,
                                           typename view_t::self_type::fields_typelist::template at<0>::type::type>> {
      CONTAINER::emplace_back( std::forward<const T>( t ) );
    }
  };

  template <
      template <class> class SKIN, typename... ZIPCONTAINER,
      typename = typename std::enable_if_t<SOA::Utils::ALL(
          SOA::impl::is_skin<SKIN>(),
          has_semantic_zip<typename std::remove_cv_t<typename std::remove_reference_t<ZIPCONTAINER>>>::value... )>>
  auto semantic_zip( ZIPCONTAINER&&... views ) {
#ifndef NDEBUG
    if ( !areSemanticallyCompatible( views... ) ) {
      throw IncompatibleZipException( "zipping from different container families" );
    }
#endif

    using barezip = decltype(
        zip<SKIN, typename std::conditional_t<std::is_reference_v<ZIPCONTAINER>,
                                              const typename std::remove_reference_t<ZIPCONTAINER>::view_t&,
                                              const typename std::remove_reference_t<ZIPCONTAINER>::view_t>...>(
            std::forward<typename std::conditional_t<std::is_reference_v<ZIPCONTAINER>,
                                                     const typename std::remove_reference_t<ZIPCONTAINER>::view_t&,
                                                     const typename std::remove_reference_t<ZIPCONTAINER>::view_t>>(
                views )... ) );

    return ZipContainer<barezip>(
        familyNumber( views... ),
        zip<SKIN, typename std::conditional_t<std::is_reference_v<ZIPCONTAINER>,
                                              const typename std::remove_reference_t<ZIPCONTAINER>::view_t&,
                                              const typename std::remove_reference_t<ZIPCONTAINER>::view_t>...>(
            std::forward<typename std::conditional_t<std::is_reference_v<ZIPCONTAINER>,
                                                     const typename std::remove_reference_t<ZIPCONTAINER>::view_t&,
                                                     const typename std::remove_reference_t<ZIPCONTAINER>::view_t>>(
                views )... ) );
  }
} // namespace Zipping

#endif
