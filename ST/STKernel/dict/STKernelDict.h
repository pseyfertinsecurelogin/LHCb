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
#ifndef DICT_STKERNELDICT_H
#define DICT_STKERNELDICT_H 1

#include "Kernel/BeetleRepresentation.h"
#include "Kernel/ISTChannelIDSelector.h"
#include "Kernel/ISTClusterSelector.h"
#include "Kernel/ISTNoiseCalculationTool.h"
#include "Kernel/ISTRawADCInfo.h"
#include "Kernel/ISTReadoutTool.h"
#include "Kernel/ITDetectorPlot.h"
#include "Kernel/PPRepresentation.h"
#include "Kernel/STBitsPolicy.h"
#include "Kernel/STBoardMapping.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STFun.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/STXMLUtils.h"
#include "Kernel/StripRepresentation.h"
#include "Kernel/TTDetectorPlot.h"

struct _Instantiations {

  STDAQ::BeetleRepresentation m_beetleRep;
  STDAQ::PPRepresentation     m_ppRep;
  STDAQ::StripRepresentation  m_stripRep;

  STDAQ::chanPair m_chanPair;

  ST::ITDetectorPlot m_itdetPlot;
  ST::TTDetectorPlot m_ttdetPlot;
};

#endif // DICT_STKERNELDICT_H
