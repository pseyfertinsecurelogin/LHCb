/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef DICT_RICHKERNELDICT_H
#define DICT_RICHKERNELDICT_H 1

#ifdef __INTEL_COMPILER           // Disable ICC remark from ROOT
#pragma warning( disable : 1572 ) // Floating-point comparisons are unreliable
#endif

// Interfaces
#include "RichInterfaces/IRichAddBackground.h"
#include "RichInterfaces/IRichDetParameters.h"
#include "RichInterfaces/IRichGenericHPDAnalysisTool.h"
#include "RichInterfaces/IRichHPDOccupancyTool.h"
#include "RichInterfaces/IRichMirrorSegFinder.h"
#include "RichInterfaces/IRichParticleProperties.h"
#include "RichInterfaces/IRichPixelClusteringTool.h"
#include "RichInterfaces/IRichPixelSuppressionTool.h"
#include "RichInterfaces/IRichRadiatorTool.h"
#include "RichInterfaces/IRichRawBufferToSmartIDsTool.h"
#include "RichInterfaces/IRichRawDataFormatTool.h"
#include "RichInterfaces/IRichRayTracing.h"
#include "RichInterfaces/IRichRefractiveIndex.h"
#include "RichInterfaces/IRichSmartIDTool.h"
#include "RichInterfaces/IRichSnellsLawRefraction.h"
#include "RichInterfaces/IRichToolRegistry.h"

// instanciate templated classes
namespace
{
  struct _Instantiations
  {
    Rich::IAddBackground::HPDBackgrounds   obj_1;
    Rich::IGenericHPDAnalysisTool::Result  obj_2;
    Rich::IGenericHPDAnalysisTool::Results obj_3;
  };
} // namespace

#endif // DICT_RICHKERNELDICT_H
