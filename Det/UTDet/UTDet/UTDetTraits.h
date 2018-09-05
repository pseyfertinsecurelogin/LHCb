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

template<>
class UTDetTraits<DeUTStation>{
 public:
  typedef DeUTDetector parent;
  typedef DeUTLayer child;
};

template<>
class UTDetTraits<DeUTLayer>{
 public:
  typedef DeUTStation parent;
  typedef DeUTModule child;
};

template<>
class UTDetTraits<DeUTModule>{
 public:
  typedef DeUTLayer parent;
  typedef DeUTSector child;
};

template<>
class UTDetTraits<DeUTSector>{
 public:
  typedef DeUTModule parent;
  typedef DeUTSensor child;
};

template<>
class UTDetTraits<DeUTSensor>{
 public:
  typedef DeUTSector parent;
  typedef DeUTSensor child;
};

#endif
