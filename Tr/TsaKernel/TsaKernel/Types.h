// $Id: Types.h,v 1.1.1.1 2006-07-24 14:56:20 mneedham Exp $
#ifndef TSA_TYPES_H
#define TSA_TYPES_H 1

#include "LoKi/Range.h"

#include "TsaKernel/STCluster.h"
#include "TsaKernel/SpacePoint.h"
#include "TsaKernel/OTCluster.h"

namespace Tsa{

 typedef LoKi::Range_<Tsa::STClusters> STRange;
 typedef LoKi::Range_<Tsa::OTClusters> OTRange;
 typedef LoKi::Range_<Tsa::SpacePoints> SpacePointRange;

};

#endif
