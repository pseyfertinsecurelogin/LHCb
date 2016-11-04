#ifndef DETDESC_CALIBRATIONINFO_H
#define DETDESC_CALIBRATIONINFO_H 1

// Base classes
#include "ConditionInfo.h"
#include "DetDesc/ICalibration.h"

///---------------------------------------------------------------------------
/** @class CalibrationInfo CalibrationInfo.h Det/DetDesc/CalibrationInfo.h
   The simplest implementation of the ICalibration abstract interface,
    derived from the generic class ConditionInfo.

    @author Andrea Valassi
    @date December 2001
*///--------------------------------------------------------------------------

struct CalibrationInfo: ConditionInfo, virtual ICalibration
{
  /// Constructor
  // ROOT dictionaries don't like
  // using ConditionInfo::ConditionInfo;
  CalibrationInfo( IDetectorElement* de, const std::string& condition ) 
  : ConditionInfo(de,condition) {}
};

#endif // DETDESC_CALIBRATIONINFO_H
