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
struct UTDetTraits;

template <>
struct UTDetTraits<DeUTStation> {
  typedef DeUTDetector parent;
  typedef DeUTLayer    child;
};

template <>
struct UTDetTraits<DeUTLayer> {
  typedef DeUTStation parent;
  typedef DeUTModule  child;
};

template <>
struct UTDetTraits<DeUTModule> {
  typedef DeUTLayer  parent;
  typedef DeUTSector child;
};

template <>
struct UTDetTraits<DeUTSector> {
  typedef DeUTModule parent;
  typedef DeUTSensor child;
};

template <>
struct UTDetTraits<DeUTSensor> {
  typedef DeUTSector parent;
  typedef DeUTSensor child;
};

#endif
