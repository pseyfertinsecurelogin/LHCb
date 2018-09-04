// $Id: UTKernelDict.h,v 1.17 2010-04-05 09:35:32 mneedham Exp $
#ifndef DICT_UTKERNELDICT_H 
#define DICT_UTKERNELDICT_H 1

#include "Kernel/IUTRawADCInfo.h"
#include "Kernel/IUTClusterSelector.h"
#include "Kernel/IUTChannelIDSelector.h"
#include "Kernel/IUTReadoutTool.h"
#include "Kernel/IUTNoiseCalculationTool.h"
#include "Kernel/UTTell1Board.h"
#include "Kernel/UTTell1ID.h"
#include "Kernel/UTBitsPolicy.h"
#include "Kernel/UTFun.h"
#include "Kernel/UTBeetleRepresentation.h"
#include "Kernel/UTPPRepresentation.h"
#include "Kernel/UTStripRepresentation.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTBoardMapping.h"
#include "Kernel/UTDetectorPlot.h"
#include "Kernel/UTXMLUtils.h"

struct _Instantiations{

  UTDAQ::UTBeetleRepresentation m_beetleRep;
  UTDAQ::UTPPRepresentation m_ppRep;
  UTDAQ::UTStripRepresentation m_stripRep;

  UTDAQ::chanPair m_chanPair;
  
  UT::UTDetectorPlot m_utdetPlot;

}; 

#endif // DICT_UTKERNELDICT_H
