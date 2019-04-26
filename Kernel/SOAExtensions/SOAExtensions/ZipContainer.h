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

/** @file
 * ZipContainer framework. Contains wrappers around SOA::Container and
 * SOA::View types. This provides semantics for zipping that encodes if two
 * containers are intended to be zipped together. E.g. RICH PID for downstream
 * tracks shall not be zipped with forward tracks, even though this is valid
 * from the point of view of C++ types and if accidentially there are as many
 * forward tracks and downstream tracks in an event.
 *
 * The validation is done based on the ZipFamilyNumber type. Each ZipContainer
 * has one ZipFamilyNumber as data member and ZipContainer with the same
 * ZipFamilyNumber are meant to be zipped. This property is called
 * 'semantically_compatible' in the following and can be checked with the
 * function Zipping::areSemanticallyCompatible. Semantic compatibility is
 * transitive (if A and B are compatible and A and C are compatible, then B and
 * C are compatible, too).
 *
 * The term 'container family' refers to the set of all ZipContainers that are
 * semantically compatible.  Each ZipContainer belongs to exactly one container
 * family.
 *
 * Semantic validation is only done in debug builds (more precisely based on
 * DNDEBUG) since it introduces a runtime overhead. Data members are not
 * optimized out in the optimized build to provide ABI compatibility and ensure
 * persistency formats do not depend on the architecture.
 */

#ifndef ZipContainer_h
#define ZipContainer_h
#include "IncompatibleZipException.h"
#include "SOAContainer/SOASkin.h"
#include "SOAContainer/SOAUtils.h"
#include "SOAContainer/SOAView.h"
#include "ZipTraits.h"
#include "ZipUtils.h"
#include <tuple> // IWYU pragma: keep
#include <type_traits>
#include <utility>

namespace Zipping {
  /** @class ZipContainer ZipContainer.h
   *
   * @brief Container class for Zipping.
   *
   * ZipContainer wrap SOA::View types to add a ZipFamilyNumber data member.
   * The templated base class SOA::View may be an owning SOA::Container or a
   * non-owning SOA::View.
   *
   * @see Kernel/SOAExtensions/SOAExtensions/ZipContainer.h for the overall idea.
   *
   * @tparam CONTAINER wrapped SOA::View type
   */
  template <typename CONTAINER>
  class ZipContainer : public CONTAINER {
    // types and usings
  public:
    using view_t = CONTAINER;

    // Zip functionality methods
  public:
    /// get number of container's container family
    ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

  private:
    ZipFamilyNumber m_zipIdentifier = details::ZipFamilyNumberGenerator::generate(); ///< container's family membership

    // constructors
  public:
    /**
     * @brief General constructor when the container family already exists.
     *
     * ZipContainer can be constructed with a specific ZipFamilyNumber, all
     * other constructor arguments are forwarded to the underlying
     * SOAContainer-type. (Sorry for the cumbersome phrasing:
     * `SOAContainer-type` is to make explicit that this may not be an
     * SOA::Container, but also an SOA::View).
     *
     * @tparam ARGS SOA::View constructor argument types
     * @param id    container family number
     * @param args  SOA::View constructor arguments
     */
    template <typename... ARGS>
    ZipContainer( ZipFamilyNumber id, ARGS&&... args )
        : CONTAINER( std::forward<ARGS>( args )... ), m_zipIdentifier( id ) {}

    // If the first constructor argument is not a ZipFamilyNumber, generate a new ZipFamilyNumber and consider
    // all constructor arguments as SOAContainer-type constructor arguments.

    // clang-format off
    /**
     * @brief General constructor when the container family does not yet exist.
     *
     * All constructor arguments are forwarded to the underlying SOAContainer-type.
     *
     * @tparam ARG    First SOA::View constructor argument types (to validate that it's not a ZipFamilyNumber and not a copy constructor)
     * @tparam ARGS   Remaining SOA::View constructor argument types
     * @param arg     First SOA::View constructor argument
     * @param args    Remaining SOA::View constructor argument
     */
    // clang-format on
    template <typename ARG, typename... ARGS,
              typename = std::enable_if_t<!std::is_same_v<ZipFamilyNumber, std::decay_t<ARG>> &&
                                          !std::is_same_v<ZipContainer, std::decay_t<ARG>>>>
    ZipContainer( ARG&& arg, ARGS&&... args ) : CONTAINER( std::forward<ARG>( arg ), std::forward<ARGS>( args )... ) {}

    /// Default constructor generates a new ZipFamilyNumber and default constructs the underlying SOAContainer-type.
    ZipContainer() = default;

    /// Copy constructor
    ZipContainer( const ZipContainer& other ) = default;

    // FIXME: review! This does slicing.
    /// Move constructor
    ZipContainer( ZipContainer&& other ) = default;

    // convenience helpers
    //
    // FIXME: they remove CONTAINER::push_back from the overload set. This is
    // not such a big issue for single column SOA::Container because there
    // push_back isn't so great to begin with, and the enable_if should keep
    // multi-field container push_backs intact.
    //
    // FIXME2: I'd prefer to enable/disable them here depending on the
    // View/Container type of CONTAINER.  In practice that's not a big issue
    // because push_back would only get instantiated if called and then
    // immediately fails in the function body.
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
      CONTAINER::emplace_back( t );
    }
  };

  template <typename CONTAINER>
  ZipContainer( ZipFamilyNumber, CONTAINER && )->ZipContainer<CONTAINER>;

  /**
   * @brief zip multiple ZipContainer to one, with semantic checks in debug build.
   *
   * @see Kernel/SOAExtensions/SOAExtensions/ZipContainer.h for the overall idea.
   *
   * @tparam SKIN         SOAContainer skin of the resulting zip (user provided)
   * @tparam ZIPCONTAINER input ZipContainer types (deduced)
   * @param views         input ZipContainer
   *
   * @return the zipped view - SOAContainer's zip output with ZipFamilyNumber
   */
  template <template <class> class SKIN, typename... ZIPCONTAINER,
            typename = std::enable_if_t<SOA::Utils::ALL( SOA::impl::is_skin<SKIN>(),
                                                         has_semantic_zip_v<std::decay_t<ZIPCONTAINER>>... )>>
  auto semantic_zip( ZIPCONTAINER&&... views ) {
#ifndef NDEBUG
    if ( !areSemanticallyCompatible( views... ) ) {
      throw IncompatibleZipException( "zipping from different container families" );
    }
#endif

    return ZipContainer{familyNumber( views... ), zip<SKIN>( std::forward<ZIPCONTAINER>( views )... )};
  }

  template <typename... ZIPCONTAINER,
            typename = std::enable_if_t<SOA::Utils::ALL( has_semantic_zip_v<std::decay_t<ZIPCONTAINER>>... )>>
  auto semantic_zip( ZIPCONTAINER&&... views ) {
#ifndef NDEBUG
    if ( !areSemanticallyCompatible( views... ) ) {
      throw IncompatibleZipException( "zipping from different container families" );
    }
#endif

    return ZipContainer{familyNumber( views... ), zip( std::forward<ZIPCONTAINER>( views )... )};
  }
} // namespace Zipping

#endif
