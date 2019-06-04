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
#ifndef _STDetTraits_H
#define _STDetTraits_H

/** @class STDetTraits STDetTraits.h "STDet/STDetTraits.h"
 *
 *  Traits info
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 */

class DeITDetector;
class DeITStation;
class DeITBox;
class DeITLayer;
class DeITLadder;
class DeITSector;
class DeITSensor;

class DeTTDetector;
class DeTTStation;
class DeTTLayer;
class DeTTHalfModule;
class DeTTSector;
class DeTTSensor;

template <typename TYPE>
struct STDetTraits;

template <>
struct STDetTraits<DeITStation> {
  typedef DeITDetector parent;
  typedef DeITBox      child;
};

template <>
struct STDetTraits<DeTTStation> {
  typedef DeTTDetector parent;
  typedef DeTTLayer    child;
};

template <>
struct STDetTraits<DeITBox> {
  typedef DeITStation parent;
  typedef DeITLayer   child;
};

template <>
struct STDetTraits<DeITLayer> {
  typedef DeITBox    parent;
  typedef DeITLadder child;
};

template <>
struct STDetTraits<DeTTLayer> {
  typedef DeTTStation    parent;
  typedef DeTTHalfModule child;
};

template <>
struct STDetTraits<DeITLadder> {
  typedef DeITLayer  parent;
  typedef DeITSector child;
};

template <>
struct STDetTraits<DeTTHalfModule> {
  typedef DeTTLayer  parent;
  typedef DeTTSector child;
};

template <>
struct STDetTraits<DeITSector> {
  typedef DeITLadder parent;
  typedef DeITSensor child;
};

template <>
struct STDetTraits<DeTTSector> {
  typedef DeTTHalfModule parent;
  typedef DeTTSensor     child;
};

template <>
struct STDetTraits<DeITSensor> {
  typedef DeITSector parent;
  typedef DeITSensor child;
};

template <>
struct STDetTraits<DeTTSensor> {
  typedef DeTTSector parent;
  typedef DeTTSensor child;
};

#endif
