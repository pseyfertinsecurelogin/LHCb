#ifndef DICT_RICHUTILSDICT_H 
#define DICT_RICHUTILSDICT_H 1

#ifdef __INTEL_COMPILER       // Disable ICC remark from ROOT
#pragma warning(disable:1572) // Floating-point comparisons are unreliable
#endif

#include "RichUtils/RichPixelCluster.h"
#include "RichUtils/RichGeomPhoton.h"
#include "RichUtils/RichTrackSegment.h"
#include "RichUtils/RichDAQDefinitions.h"
#include "RichUtils/RichDecodedData.h"
#include "RichUtils/RichHypoData.h"
#include "RichUtils/RichObjPtn.h"
#include "RichUtils/RichSmartIDCnv.h"
#include "RichUtils/RichHPDIdentifier.h"
#include "RichUtils/RichDAQL0Header.h"
#include "RichUtils/RichRayTracingUtils.h"
#include "RichUtils/RichHypoData.h"
#include "RichUtils/RichObjPtn.h"

// instanciate templated classes
namespace 
{
  struct _Instantiations 
  {
    Rich::HypoData<float>                        obj_2;
    Rich::HypoData<double>                       obj_3;
    Rich::DAQ::Level0ID                          obj_4;
    Rich::DAQ::EventID                           obj_5;
    Rich::DAQ::BXID                              obj_6;
    Rich::DAQ::Level1LogicalID                   obj_7;
    Rich::DAQ::Level1HardwareID                  obj_8;
    Rich::DAQ::L1IngressID                       obj_9;
    Rich::DAQ::L1InputWithinIngress              obj_10;
    Rich::DAQ::Level1Input                       obj_11;
    Rich::DAQ::HPDHardwareID                     obj_12;
    Rich::DAQ::HPDL1InputID                      obj_13;
    Rich::DAQ::HPDCopyNumber                     obj_14;
    Rich::DAQ::Level1CopyNumber                  obj_15;
  };
}

#endif // DICT_RICHUTILSDICT_H

