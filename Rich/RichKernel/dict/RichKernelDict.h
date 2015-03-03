#ifndef DICT_RICHKERNELDICT_H 
#define DICT_RICHKERNELDICT_H 1

#include "RichKernel/RichPixelCluster.h"
#include "RichKernel/RichGeomPhoton.h"
#include "RichKernel/RichTrackSegment.h"
#include "RichKernel/RichDAQDefinitions.h"
#include "RichKernel/RichDecodedData.h"
#include "RichKernel/RichHypoData.h"
#include "RichKernel/RichObjPtn.h"
#include "RichKernel/RichSmartIDCnv.h"
#include "RichKernel/RichHPDIdentifier.h"
#include "RichKernel/RichDAQL0Header.h"

// Interfaces
#include "RichKernel/IHpdUkL1DisableTool.h"
#include "RichKernel/IRichDetParameters.h"
#include "RichKernel/IRichMirrorSegFinder.h"
#include "RichKernel/IRichParticleProperties.h"
#include "RichKernel/IRichPixelClusteringTool.h"
#include "RichKernel/IRichPixelSuppressionTool.h"
#include "RichKernel/IRichRadiatorTool.h"
#include "RichKernel/IRichRawBufferToSmartIDsTool.h"
#include "RichKernel/IRichRawDataFormatTool.h"
#include "RichKernel/IRichRayTracing.h"
#include "RichKernel/IRichRefractiveIndex.h"
#include "RichKernel/IRichSmartIDTool.h"
#include "RichKernel/IRichToolRegistry.h"
#include "RichKernel/IRichSnellsLawRefraction.h"
#include "RichKernel/IRichAddBackground.h"
#include "RichKernel/IRichHPDOccupancyTool.h"
#include "RichKernel/IRichGenericHPDAnalysisTool.h"

// instanciate templated classes
namespace 
{
  struct _Instantiations 
  {
    Rich::IAddBackground::HPDBackgrounds   obj_1;
    Rich::IGenericHPDAnalysisTool::Result  obj_2;
    Rich::IGenericHPDAnalysisTool::Results obj_3;

  };
}

#endif // DICT_RICHKERNELDICT_H

