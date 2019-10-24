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
#pragma once

/** @class UTDetTraits UTDetTraits.h "UTDet/UTDetTraits.h"
 *
 *  Traits info
 *
 *  @author Andy Beiter (based on code by Matthew Needham)
 */

class DeUTDetector;
class DeUTStation;
class DeUTLayer;
class DeUTStave;
class DeUTSector;
class DeUTSensor;

template <typename TYPE>
struct UTDetTraits;

template <>
struct UTDetTraits<DeUTDetector> {
  using child = DeUTStation;
};

template <>
struct UTDetTraits<DeUTStation> {
  using parent = DeUTDetector;
  using child  = DeUTLayer;
};

template <>
struct UTDetTraits<DeUTLayer> {
  using parent = DeUTStation;
  using child  = DeUTStave;
};

template <>
struct UTDetTraits<DeUTStave> {
  using parent = DeUTLayer;
  using child  = DeUTSector;
};

template <>
struct UTDetTraits<DeUTSector> {
  using parent = DeUTStave;
  using child  = DeUTSensor;
};

template <>
struct UTDetTraits<DeUTSensor> {
  using parent = DeUTSector;
};
