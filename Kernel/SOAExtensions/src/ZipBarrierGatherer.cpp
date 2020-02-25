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
#include "GaudiAlg/MergingTransformer.h"
#include "GaudiAlg/Transformer.h"
#include "SOAExtensions/ZipSelection.h"

/** @class ZipBarrierGatherer ZipBarrierGatherer.cpp
 *
 *  The Gatherer takes multiple optional input-containers (LHCb::Tracks for
 *  instance) (selections from different lines) and puts them into one big
 *  vector. This "only" creates vector of selection, i.e. not one big
 *  selection, this seemingly pointless step needs to be done before the
 *  ZipBarrierMerger because of the particularities of how the functional
 *  framework works.
 *
 *  This algorithm is globally usable. It is not for the purpose of the
 *  SOAExtensions barrier example. It can gather selections of integer, tracks,
 *  particles, ... as long as they are in the SOAExtensions framework.
 *
 *  @brief gather multiple Zipping::ExportedSelection for processing in ZipBarrierMerger
 */
struct ZipBarrierGatherer final
    : Gaudi::Functional::MergingTransformer<
          Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*>(
              Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*> const& )> {

  ZipBarrierGatherer( std::string const& name, ISvcLocator* pSvcLocator )
      : Gaudi::Functional::MergingTransformer<
            Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*>(
                Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*> const& )>(
            name, pSvcLocator, {"InputSelections", {"/Event/SelectAll"}},
            {"OutputSelection", {"/Event/GatheredSelects"}} ) {}

  Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*> operator()(
      Gaudi::Functional::details::vector_of_const_<const Zipping::ExportedSelection<>*> const& vec ) const override {
    return vec;
  }
};

DECLARE_COMPONENT( ZipBarrierGatherer )
