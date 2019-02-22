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
class STDetTraits {
public:
  typedef TYPE parent;
  typedef TYPE child;
};

template <>
class STDetTraits<DeITStation> {
public:
  typedef DeITDetector parent;
  typedef DeITBox      child;
};

template <>
class STDetTraits<DeTTStation> {
public:
  typedef DeTTDetector parent;
  typedef DeTTLayer    child;
};

template <>
class STDetTraits<DeITBox> {
public:
  typedef DeITStation parent;
  typedef DeITLayer   child;
};

template <>
class STDetTraits<DeITLayer> {
public:
  typedef DeITBox    parent;
  typedef DeITLadder child;
};

template <>
class STDetTraits<DeTTLayer> {
public:
  typedef DeTTStation    parent;
  typedef DeTTHalfModule child;
};

template <>
class STDetTraits<DeITLadder> {
public:
  typedef DeITLayer  parent;
  typedef DeITSector child;
};

template <>
class STDetTraits<DeTTHalfModule> {
public:
  typedef DeTTLayer  parent;
  typedef DeTTSector child;
};

template <>
class STDetTraits<DeITSector> {
public:
  typedef DeITLadder parent;
  typedef DeITSensor child;
};

template <>
class STDetTraits<DeTTSector> {
public:
  typedef DeTTHalfModule parent;
  typedef DeTTSensor     child;
};

template <>
class STDetTraits<DeITSensor> {
public:
  typedef DeITSector parent;
  typedef DeITSensor child;
};

template <>
class STDetTraits<DeTTSensor> {
public:
  typedef DeTTSector parent;
  typedef DeTTSensor child;
};

#endif
