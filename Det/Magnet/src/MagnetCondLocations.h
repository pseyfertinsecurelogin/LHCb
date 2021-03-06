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
#ifndef MAGNETCONDLOCATIONS_H
#define MAGNETCONDLOCATIONS_H 1

/** @namespace MagnetCondLocations
 *
 *  Namespace for the location of Magnet conditions in the TDS
 *
 *  @author Marco Cattaneo
 *  @date   2008-06-18
 *
 */

#include <string>

namespace MagnetCondLocations {
  /// Location of Field Map file names for "Up" polarity
  inline const std::string FieldMapFilesUp = "/dd/Conditions/HardwareProperties/LHCb/Magnet/FieldMapFilesUp";

  /// Location of Field Map file names for "Down" polarity
  inline const std::string FieldMapFilesDown = "/dd/Conditions/HardwareProperties/LHCb/Magnet/FieldMapFilesDown";

  /// Location of scaling factors for "Up" polarity
  inline const std::string ScaleUp = "/dd/Conditions/HardwareProperties/LHCb/Magnet/ScaleUp";

  /// Location of scaling factors for "Down" polarity
  inline const std::string ScaleDown = "/dd/Conditions/HardwareProperties/LHCb/Magnet/ScaleDown";

  /// Location of online measured current and polarity
  inline const std::string Measured = "/dd/Conditions/Online/LHCb/Magnet/Measured";

  /// Location of online set current and polarity
  inline const std::string Set = "/dd/Conditions/Online/LHCb/Magnet/Set";
} // namespace MagnetCondLocations

#endif // MAGNETCONDLOCATIONS_H
