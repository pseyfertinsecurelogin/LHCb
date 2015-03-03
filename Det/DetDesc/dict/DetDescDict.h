// $Id: DetDescDict.h,v 1.3 2007-12-20 10:44:14 cattanem Exp $
// ============================================================================
#ifndef DETSYS_DETDICT_H 
#define DETSYS_DETDICT_H 1
// ============================================================================
// Hack to get round gccxml parsing problem (SEAL bug 9704)
// ============================================================================
#ifdef _WIN32
#define LONG_LONG_MAX     0x7fffffffffffffffLL  /*maximum signed __int64 value */
#define LONG_LONG_MIN     0x8000000000000000LL  /*minimum signed __int64 value */
#define ULONG_LONG_MAX    0xffffffffffffffffLL  /*maximum unsigned __int64 value */
#endif
// ============================================================================
// DetDesc 
// ============================================================================
#include "DetDesc/IDetectorElement.h"
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ILVolume.h"
#include "DetDesc/IPVolume.h"
#include "DetDesc/ISolid.h"
#include "DetDesc/ITransportSvc.h"
#include "DetDesc/IGeometryErrorSvc.h"
// ============================================================================
#include "DetDesc/IAlignment.h"
#include "DetDesc/ICalibration.h"
#include "DetDesc/IConditionInfo.h"
#include "DetDesc/IFastControl.h"
#include "DetDesc/ISlowControl.h"
#include "DetDesc/IReadOut.h"
// ============================================================================
#include "DetDesc/Material.h"
#include "DetDesc/Mixture.h"
#include "DetDesc/Element.h"
#include "DetDesc/Isotope.h"
// ============================================================================
#include "DetDesc/LogVolBase.h"
#include "DetDesc/LVolume.h"
#include "DetDesc/LAssembly.h"
// ============================================================================
#include "DetDesc/Condition.h"
#include "DetDesc/AlignmentCondition.h"
#include "DetDesc/Surface.h"
#include "DetDesc/DetectorElement.h"
#include "DetDesc/TabulatedProperty.h"
#include "DetDesc/ValidDataObject.h"
#include "DetDesc/ParamValidDataObject.h"
// ============================================================================
#include "DetDesc/SolidBase.h"
#include "DetDesc/SolidBoolean.h"
#include "DetDesc/SolidBox.h"
#include "DetDesc/SolidChild.h"
#include "DetDesc/SolidCons.h"
#include "DetDesc/SolidIntersection.h"
#include "DetDesc/SolidPolycone.h"
#include "DetDesc/SolidPolyHedronHelper.h"
#include "DetDesc/SolidSphere.h"
#include "DetDesc/SolidSubtraction.h"
#include "DetDesc/SolidTrap.h"
#include "DetDesc/SolidTrd.h"
#include "DetDesc/SolidTubs.h"
#include "DetDesc/SolidUnion.h"
// ============================================================================
#include "DetDesc/PVolume.h"
#include "DetDesc/GeometryInfoPlus.h"
#include "DetDesc/AlignmentInfo.h"
#include "DetDesc/CalibrationInfo.h"
#include "DetDesc/ConditionInfo.h"
#include "DetDesc/FastControlInfo.h"
#include "DetDesc/ReadOutInfo.h"
#include "DetDesc/SlowControlInfo.h"
#include "DetDesc/SimpleValidity.h"
// ============================================================================
#include "DetDesc/3DTransformationFunctions.h"
#include "DetDesc/GlobalToLocalDelta.h"

/** @file 
 *   
 *  Helper file to generate LCG dictionaries for detector description classes
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2004-03-01
 *  @date   updated 2005-07-07
 */

namespace DetDescPython
{
  //--- Templace instantiations
  struct __Instantiations 
  {
    ILVolume::PVolumes      i1 ;
    ILVolume::ReplicaPath   i2 ;
    ILVolume::PVolumePath   i3 ;
    ILVolume::Interval      i4 ;
    ILVolume::Intersection  i5 ;
    ILVolume::Intersections i6 ;
    ISolid::Ticks           i7 ;
    
    SmartRef<Condition> sref_inst1;
  };
  
};

// ============================================================================
#endif // DETSYS_DETDICT_H
// ============================================================================

