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

/*  ZipBarrier algorithms:
 *
 *  - The Gatherer takes multiple optional input-containers (LHCb::Tracks for
 *    instance) (selections from different lines) and puts them into one big
 *    vector
 */

struct ZipBarrierGatherer final
    : Gaudi::Functional::MergingTransformer<Gaudi::Functional::details::vector_of_const_<Zipping::ExportedSelection<>*>(
          Gaudi::Functional::details::vector_of_const_<Zipping::ExportedSelection<>*> const& )> {

  ZipBarrierGatherer( std::string const& name, ISvcLocator* pSvcLocator )
      : Gaudi::Functional::MergingTransformer<
            Gaudi::Functional::details::vector_of_const_<Zipping::ExportedSelection<>*>(
                Gaudi::Functional::details::vector_of_const_<Zipping::ExportedSelection<>*> const& )>(
            name, pSvcLocator, {"InputSelections", {"/Event/SelectAll"}},
            {"OutputSelection", {"/Event/GatheredSelects"}} ) {}

  Gaudi::Functional::details::vector_of_const_<Zipping::ExportedSelection<>*>
  operator()( Gaudi::Functional::details::vector_of_const_<Zipping::ExportedSelection<>*> const& vec ) const override {
    return vec;
  }
};

DECLARE_COMPONENT( ZipBarrierGatherer )
