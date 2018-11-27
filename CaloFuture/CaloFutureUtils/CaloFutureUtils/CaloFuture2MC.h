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
#ifndef CALOFUTURE2MC_H 
#define CALOFUTURE2MC_H 1
#include "Event/CaloHypo.h"

namespace LHCb {
  class MCParticle;
  class CaloCluster;
  class CaloDigit;
}

template <class FROM,class TO,class WEIGHT> class IRelationWeighted; 
template <class FROM,class TO> class LinkerWithKey;
template <class FROM,class TO> class LinkedTo;
namespace LHCb{ 
  template <class FROM,class TO,class WEIGHT> class RelationWeighted1D;
}

/** @class CaloFuture2MC CaloFuture2MC.h Event/CaloFuture2MC.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2006-02-21
 */
namespace LHCb { 
  namespace CaloFuture2MC {
    typedef IRelationWeighted< LHCb::CaloCluster , LHCb::MCParticle , float >  IClusterTable ; 
    typedef LHCb::RelationWeighted1D<LHCb::CaloCluster,LHCb::MCParticle,float> ClusterTable ;
    
    typedef IRelationWeighted<LHCb::CaloDigit,LHCb::MCParticle,float>          IDigitTable; 
    typedef LHCb::RelationWeighted1D<LHCb::CaloDigit,LHCb::MCParticle,float>   DigitTable ; 

    typedef LinkerWithKey<LHCb::MCParticle,LHCb::CaloDigit>                   DigitLink ;
    typedef LinkedTo<LHCb::MCParticle,LHCb::CaloDigit>                        DigitLinkTo;

    typedef LinkerWithKey<LHCb::MCParticle,LHCb::CaloHypo>                   HypoLink ;
    typedef LinkedTo<LHCb::MCParticle,LHCb::CaloHypo>                        HypoLinkTo;

    //    typedef RelationWeighted1D<TrgCaloFutureCluster,MCParticle,float> TrgClusterTable ;
    
  }
  
}

#endif // CALOFUTURE2MC_H 
