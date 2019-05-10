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

#ifndef ZipAlgorithms_h
#define ZipAlgorithms_h

#include "SOAContainer/SOAContainer.h"
#include "SOAExtensions/ZipContainer.h"
#include "SOAExtensions/ZipSelection.h"
#include <algorithm>

namespace Zipping {

  /**
   * @brief Create a new container that can be zipped to an input container with a per-element operation
   *
   * This overload version takes an owning SOA::Container, not a view.
   * The operation is performed on every element of the input container and the return of the operation
   * is added to the output container with emplace_back.
   * The operation will usually be a lambda, but can be any valid input for std::invoke.
   * The operation is applied to all inputs.
   *
   * @code
   * Zipping::ZipContainer<SOA::Container<std::vector, TrackSkin>> tracks = ...
   * auto muonids = Zipping::transform<MuonPIDSkin>( tracks, [](auto track) { MuonPID pid = ... ; return pid; } );
   * @endcode
   *
   * @tparam OUTSKIN    SOA-skin for the output container
   * @tparam CONTAINER  underlying storage template (auto deduced, typically std::vector)
   * @tparam INSKIN     SOA-skin of the input container (auto deduced)
   * @tparam Operation  type of the operation (auto deduced)
   * @param input       input container
   * @param operation   the per-element operation for the transformation
   *
   * @return a Zipping::ZipContainer that can be zipped to the input container
   */
  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER,
            template <typename> typename INSKIN, typename Operation>
  auto transform( const Zipping::ZipContainer<SOA::Container<CONTAINER, INSKIN>>& input, Operation&& operation ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.zipIdentifier() );
    retval.reserve( input.size() );
    for ( auto in : input ) { retval.emplace_back( std::invoke( operation, in ) ); }
    assert( retval.size() == input.size() );
    assert( Zipping::areSemanticallyCompatible( retval, input ) );
    return retval;
  }

  /**
   * @brief Create a new container that can be zipped to an input container with a per-element operation
   *
   * This overload version takes a non-owning SOA::View.
   * The operation is performed on every element of the input container and the return of the operation
   * is added to the output container with emplace_back.
   * The operation will usually be a lambda, but can be any valid input for std::invoke.
   * The operation is applied to all inputs.
   *
   * @code
   * Zipping::ZipContainer<SOA::Container<std::vector, TrackSkin>> tracks = ...
   * Zipping::ZipContainer<SOA::Container<std::vector, MuonPIDSkin>> muonids = ...
   * auto proto_particles = Zipping::semantic_zip<ProtoParticleSkin>( tracks, muonids );
   *
   * auto muon_isolations = Zipping::transform<MuonIsolationSkin, std::vector>( proto_particles, [](auto protop) {
   * MuonIsolation iso = ... ; return iso; } );
   * @endcode
   *
   * @tparam OUTSKIN    SOA-skin for the output container
   * @tparam CONTAINER  underlying storage template (not deduced, when in doubt probably std::vector)
   * @tparam INVIEW     type of the input ZipContainer<SOA::View ...
   * @tparam Operation  type of the operation (auto deduced)
   * @param input       input container
   * @param operation   the per-element operation for the transformation
   *
   * @return a Zipping::ZipContainer that can be zipped to the input container
   */
  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER, typename INVIEW,
            typename Operation, typename = std::enable_if_t<!SOA::Utils::is_container<INVIEW>::value>>
  auto transform( const Zipping::ZipContainer<INVIEW>& input, Operation&& operation ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.zipIdentifier() );
    retval.reserve( input.size() );
    for ( auto in : input ) { retval.emplace_back( std::invoke( operation, in ) ); }
    assert( retval.size() == input.size() );
    assert( Zipping::areSemanticallyCompatible( retval, input ) );
    return retval;
  }

  namespace details {
    template <typename IndexSize, typename INPUT, typename OUTPUT, typename Operation, typename SELECTION>
    auto transform( const INPUT& input, Operation&& operation, const SELECTION& sel, OUTPUT& retval ) {
      retval.reserve( input.size() );
      assert( std::is_sorted( sel.begin(), sel.end() ) );

      IndexSize i                 = 0;
      auto      selectionIterator = sel.begin();
      for ( ; i < input.size(); ++i ) {
        if ( selectionIterator != sel.end() && *selectionIterator == i ) {
          retval.emplace_back( std::invoke( operation, input[i] ) );
          ++selectionIterator;
        } else {
          retval.emplace_back(); // default / invalid construction
        }
      }

      assert( retval.size() == input.size() );
      assert( Zipping::areSemanticallyCompatible( retval, input ) );

      return retval;
    }
  } // namespace details

  /**
   * @brief Create a new container that can be zipped to an input container with a per-element operation
   *
   * This overload version takes an owning SOA::Container, not a view.
   * The operation is applied to selected elements, for others the default constructed output object is created.
   * The operation is performed on every element of the input container and the return of the operation
   * is added to the output container with emplace_back.
   * The operation will usually be a lambda, but can be any valid input for std::invoke.
   *
   * @code
   * Zipping::ZipContainer<SOA::Container<std::vector, TrackSkin>> tracks = ...
   * ExportedSelection<> large_ip_selection = ...
   * auto muonids = Zipping::transform<MuonPIDSkin>( tracks, [](auto track) { MuonPID pid = ... ; return pid; },
   * large_ip_selection );
   * @endcode
   *
   * @tparam OUTSKIN    SOA-skin for the output container
   * @tparam CONTAINER  underlying storage template (auto deduced, typically std::vector)
   * @tparam INSKIN     SOA-skin of the input container (auto deduced)
   * @tparam Operation  type of the operation (auto deduced)
   * @tparam IndexSize  Type for indices (auto deduced)
   * @param input       input container
   * @param operation   the per-element operation for the transformation
   * @param sel         selection of to-be-processed elements
   *
   * @return a Zipping::ZipContainer that can be zipped to the input container
   */
  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER,
            template <typename> typename INSKIN, typename Operation, typename IndexSize>
  auto transform( const Zipping::ZipContainer<SOA::Container<CONTAINER, INSKIN>>& input, Operation&& operation,
                  const ExportedSelection<IndexSize>& sel ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.zipIdentifier() );
    return details::transform<IndexSize>( input, operation, sel.m_indices, retval );
  }

  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER,
            template <typename> typename INSKIN, typename Operation, typename IndexSize>
  auto
  transform( const Zipping::SelectionView<Zipping::ZipContainer<SOA::Container<CONTAINER, INSKIN>>, IndexSize>& input,
             Operation&& operation ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.m_container->zipIdentifier() );
    return details::transform<IndexSize>( *input.m_container, operation, *input.m_indices, retval );
  }

  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER,
            template <typename> typename INSKIN, typename Operation, typename IndexSize>
  auto transform(
      const Zipping::SelectionView<const Zipping::ZipContainer<SOA::Container<CONTAINER, INSKIN>>, IndexSize>& input,
      Operation&& operation ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.m_container->zipIdentifier() );
    return details::transform<IndexSize>( *input.m_container, operation, *input.m_indices, retval );
  }

  /**
   * @brief Create a new container that can be zipped to an input container with a per-element operation
   *
   * This overload version takes a non-owning SOA::View.
   * The operation is applied to selected elements, for others the default constructed output object is created.
   * The operation is performed on every element of the input container and the return of the operation
   * is added to the output container with emplace_back.
   * The operation will usually be a lambda, but can be any valid input for std::invoke.
   *
   * @code
   * Zipping::ZipContainer<SOA::Container<std::vector, TrackSkin>> tracks = ...
   * Zipping::ZipContainer<SOA::Container<std::vector, MuonPIDSkin>> muonids = ...
   * auto proto_particles = Zipping::semantic_zip<ProtoParticleSkin>( tracks, muonids );
   * ExportedSelection<> large_ip_selection = ...
   *
   * auto muon_isolations = Zipping::transform<MuonIsolationSkin, std::vector>( proto_particles, [](auto protop) {
   * MuonIsolation iso = ... ; return iso; }, large_ip_selection );
   * @endcode
   *
   * @tparam OUTSKIN    SOA-skin for the output container
   * @tparam CONTAINER  underlying storage template (not deduced, when in doubt probably std::vector)
   * @tparam INVIEW     type of the input ZipContainer<SOA::View ...
   * @tparam Operation  type of the operation (auto deduced)
   * @tparam IndexSize  Type for indices (auto deduced)
   * @param input       input container
   * @param operation   the per-element operation for the transformation
   * @param sel         selection of to-be-processed elements
   *
   * @return a Zipping::ZipContainer that can be zipped to the input container
   */
  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER, typename INVIEW,
            typename Operation, typename = std::enable_if_t<!SOA::Utils::is_container<INVIEW>::value>,
            typename IndexSize>
  auto transform( const Zipping::ZipContainer<INVIEW>& input, Operation&& operation,
                  const ExportedSelection<IndexSize>& sel ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.zipIdentifier() );
    return details::transform<IndexSize>( input, operation, sel.m_indices, retval );
  }

  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER, typename INVIEW,
            typename Operation, typename = std::enable_if_t<!SOA::Utils::is_container<INVIEW>::value>,
            typename IndexSize>
  auto transform( const Zipping::SelectionView<Zipping::ZipContainer<INVIEW>, IndexSize>& input,
                  Operation&&                                                             operation ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.m_container->zipIdentifier() );
    return details::transform<IndexSize>( input, operation, *input.m_indices, retval );
  }

  template <template <typename> typename OUTSKIN, template <typename...> class CONTAINER, typename INVIEW,
            typename Operation, typename = std::enable_if_t<!SOA::Utils::is_container<INVIEW>::value>,
            typename IndexSize>
  auto transform( const Zipping::SelectionView<const Zipping::ZipContainer<INVIEW>, IndexSize>& input,
                  Operation&&                                                                   operation ) {
    Zipping::ZipContainer<SOA::Container<CONTAINER, OUTSKIN>> retval( input.m_container->zipIdentifier() );
    return details::transform<IndexSize>( input, operation, *input.m_indices, retval );
  }
} // namespace Zipping

#endif // ZipAlgorithms_h
