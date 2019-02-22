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
#ifndef _UTDetTraits_H
#define _UTDetTraits_H

/** @class UTDetTraits UTDetTraits.h "UTDet/UTDetTraits.h"
 *
 *  Traits info
 *
 *  @author Andy Beiter (based on code by Matthew Needham)
 */

class DeUTDetector;
class DeUTStation;
class DeUTLayer;
class DeUTModule;
class DeUTSector;
class DeUTSensor;

template <typename TYPE>
class UTDetTraits {
public:
  typedef TYPE parent;
  typedef TYPE child;
};

template <>
class UTDetTraits<DeUTStation> {
public:
  typedef DeUTDetector parent;
  typedef DeUTLayer    child;
};

template <>
class UTDetTraits<DeUTLayer> {
public:
  typedef DeUTStation parent;
  typedef DeUTModule  child;
};

template <>
class UTDetTraits<DeUTModule> {
public:
  typedef DeUTLayer  parent;
  typedef DeUTSector child;
};

template <>
class UTDetTraits<DeUTSector> {
public:
  typedef DeUTModule parent;
  typedef DeUTSensor child;
};

template <>
class UTDetTraits<DeUTSensor> {
public:
  typedef DeUTSector parent;
  typedef DeUTSensor child;
};

#endif
