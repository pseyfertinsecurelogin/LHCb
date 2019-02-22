/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef DICT_RICHUTILSDICT_H
#define DICT_RICHUTILSDICT_H 1

#ifdef __INTEL_COMPILER             // Disable ICC remark from ROOT
#  pragma warning( disable : 1572 ) // Floating-point comparisons are unreliable
#endif

#include "RichUtils/RichDAQDefinitions.h"
#include "RichUtils/RichDAQL0Header.h"
#include "RichUtils/RichDecodedData.h"
#include "RichUtils/RichGeomPhoton.h"
#include "RichUtils/RichHypoData.h"
#include "RichUtils/RichObjPtn.h"
#include "RichUtils/RichPDIdentifier.h"
#include "RichUtils/RichPixelCluster.h"
#include "RichUtils/RichRayTracingUtils.h"
#include "RichUtils/RichSIMDTypes.h"
#include "RichUtils/RichSmartIDCnv.h"
#include "RichUtils/RichTrackSegment.h"

// instanciate types
namespace {
  struct _Instantiations {
    Rich::HypoData<float>                                obj_2;
    Rich::HypoData<double>                               obj_3;
    Rich::DAQ::Level0ID                                  obj_4;
    Rich::DAQ::EventID                                   obj_5;
    Rich::DAQ::BXID                                      obj_6;
    Rich::DAQ::Level1LogicalID                           obj_7;
    Rich::DAQ::Level1HardwareID                          obj_8;
    Rich::DAQ::L1IngressID                               obj_9;
    Rich::DAQ::L1InputWithinIngress                      obj_10;
    Rich::DAQ::HPD::Level1Input                          obj_11;
    Rich::DAQ::PDHardwareID                              obj_12;
    Rich::DAQ::HPD::L1InputID                            obj_13;
    Rich::DAQ::PDCopyNumber                              obj_14;
    Rich::DAQ::Level1CopyNumber                          obj_15;
    Rich::DAQ::PDPanelIndex                              obj_16;
    LHCb::RichTrackSegment                               obj_17;
    LHCb::RichTrackSegment::SIMDFP                       obj_18;
    LHCb::RichTrackSegment::SIMDPoint                    obj_19;
    LHCb::RichTrackSegment::SIMDVector                   obj_20;
    Rich::SIMD::FP<Rich::SIMD::DefaultScalarFP>          obj_21;
    Rich::SIMD::Point<Rich::SIMD::DefaultScalarFP>       obj_22;
    Rich::SIMD::Vector<Rich::SIMD::DefaultScalarFP>      obj_23;
    Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP>       obj_24;
    Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> obj_25;
  };
} // namespace

#endif // DICT_RICHUTILSDICT_H
