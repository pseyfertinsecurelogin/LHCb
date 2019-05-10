/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#include "GaudiAlg/FunctionalDetails.h" // VOC
#include "GaudiAlg/Transformer.h"
#include "SOAExtensions/ZipSelection.h"
#include <iterator> // std::iterator_traits

/*  ZipBarrier algorithms:
 *
 *  - The Gatherer takes multiple optional input-containers (LHCb::Tracks for
 *    instance) (selections from different lines) and puts them into one big
 *    vector
 *
 *  - The Merger creates the union of all those containers to have one container
 *    where some shared algorithm can loop over (implemented for
 *    Keyed(SharedObject)Containers and Masks (with integral types of values 0
 *    and 1)
 *
 *  - The next ZipBarrier step is some shared algorithm, taking the output of the
 *    merger (and in case of masks some other data) as input
 *
 *  - The scatterer takes the output of the shared algorithm + the gatherer
 *    output + merger output how to scatter the inputs to the different lines
 *    again
 */

struct ZipBarrierMerger final
    : public Gaudi::Functional::Transformer<Zipping::ExportedSelection<>(
          Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*> const& )> {

  ZipBarrierMerger( std::string const& name, ISvcLocator* pSvcLocator )
      : Gaudi::Functional::Transformer<Zipping::ExportedSelection<>(
            Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*> const& )>(
            name, pSvcLocator, {"InputSelection", "/Event/GatheredSelects"},
            {"OutputSelection", "/Event/MergedSelects"} ) {}

  Zipping::ExportedSelection<> operator()(
      Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*> const& vec ) const override {
    // this only works thanks to the above iterator_traits
    auto iter = std::find_if( vec.begin(), vec.end(), []( auto* entry ) { return entry; } );

    if ( iter == vec.end() )
      throw GaudiException( std::string( "No single input into ZipBarrierMerger, "
                                         "shouldn't have been scheduled." ),
                            Zipping::details::typename_v<decltype( *this )>, StatusCode::FAILURE );
    auto init = **iter;
    return std::accumulate( ++iter, vec.end(), init,
                            []( Zipping::ExportedSelection<> const& a,
                                Zipping::ExportedSelection<> const* b ) -> Zipping::ExportedSelection<> {
                              if ( b ) return set_union( a, *b );
                              return a;
                            } );
  }
};

DECLARE_COMPONENT( ZipBarrierMerger )
