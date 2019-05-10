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

template <typename T>
using VOC = Gaudi::Functional::details::vector_of_const_<T>;
template <typename T>
using SEL      = Zipping::ExportedSelection<>;
using KeyValue = std::pair<std::string, std::string>;

template <typename T>
struct warner;

struct ZipBarrierGatherer final : Gaudi::Functional::MergingTransformer<VOC<SEL*>( VOC<SEL*> const& )> {

  ZipBarrierGatherer( std::string const& name, ISvcLocator* pSvcLocator )
      : Gaudi::Functional::MergingTransformer<VOC<SEL*>( VOC<SEL*> const& )>(
            name, pSvcLocator, {"InputSelections", {"/Event/SelectAll"}},
            {"OutputSelection", {"/Event/GatheredSelects"}} ) {}

  VOC<SEL*> operator()( VOC<SEL*> const& vec ) const override { return vec; }
};

DECLARE_COMPONENT( ZipBarrierGatherer )
