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
#ifndef CALO2TRACK_H
#define CALO2TRACK_H 1

#include <string>

#include "Event/Track.h"
//Forward dec'
template <class FROM,class TO,class WEIGHT> class IRelationWeighted;
template <class FROM,class TO,class WEIGHT> class IRelationWeighted2D;
template <class FROM,class TO> class IRelation;

namespace LHCb{
  class CaloHypo;
  class CaloCluster;
}

/** @class Calo2Track Calo2Track.h Event/Calo2Track.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2006-02-21
 */
namespace LHCb {
  namespace CaloIdLocation {
    // Calo-Track matching tables   : IRelationWeighted< CaloCluster/Hypo , Track , float >
    inline const std::string ClusterMatch  = "Rec/Calo/ClusterMatch";
    inline const std::string PhotonMatch   = "Rec/Calo/PhotonMatch";
    inline const std::string BremMatch     = "Rec/Calo/BremMatch";
    inline const std::string ElectronMatch = "Rec/Calo/ElectronMatch";
    // Acceptance tables       : IRelation< Track , bool  >
    inline const std::string InSpd         =  "Rec/Calo/InAccSpd";
    inline const std::string InPrs         =  "Rec/Calo/InAccPrs";
    inline const std::string InBrem        =  "Rec/Calo/InAccBrem";
    inline const std::string InEcal        =  "Rec/Calo/InAccEcal";
    inline const std::string InHcal        =  "Rec/Calo/InAccHcal";
    // Intermediate estimators : IRelation< Track , float >
    inline const std::string SpdE          = "Rec/Calo/SpdE";
    inline const std::string PrsE          = "Rec/Calo/PrsE";
    inline const std::string EcalE         = "Rec/Calo/EcalE";
    inline const std::string HcalE         = "Rec/Calo/HcalE";
    inline const std::string EcalChi2      = "Rec/Calo/EcalChi2";
    inline const std::string BremChi2      = "Rec/Calo/BremChi2";
    inline const std::string ClusChi2      = "Rec/Calo/ClusChi2";
    // CaloPID DLLs            : IRelation< Track , float >
    inline const std::string PrsPIDe       = "Rec/Calo/PrsPIDe";
    inline const std::string EcalPIDe      = "Rec/Calo/EcalPIDe";
    inline const std::string BremPIDe      = "Rec/Calo/BremPIDe";
    inline const std::string HcalPIDe      = "Rec/Calo/HcalPIDe";
    inline const std::string EcalPIDmu     = "Rec/Calo/EcalPIDmu";
    inline const std::string HcalPIDmu     = "Rec/Calo/HcalPIDmu";
    // Neutral ID DLLs
    inline const std::string PhotonID      = "Rec/Calo/PhotonID" ;
    inline const std::string MergedID      = "Rec/Calo/MergedID" ;
    inline const std::string PhotonFromMergedID      = "Rec/Calo/PhotonFromMergedID" ;



    // =================== Hlt locations ============== DEPRECATED : use CaloAlgUtils::pathFromContext instead !!

    // Calo-Track matching tables   : IRelationWeighted< CaloCluster/Hypo , Track , float >
    inline const std::string ClusterMatchHlt  = "Hlt/Calo/ClusterMatch";
    inline const std::string PhotonMatchHlt   = "Hlt/Calo/PhotonMatch";
    inline const std::string BremMatchHlt     = "Hlt/Calo/BremMatch";
    inline const std::string ElectronMatchHlt = "Hlt/Calo/ElectronMatch";
    // Acceptance tables       : IRelation< Track , bool  >
    inline const std::string InSpdHlt        =  "Hlt/Calo/InAccSpd";
    inline const std::string InPrsHlt         =  "Hlt/Calo/InAccPrs";
    inline const std::string InBremHlt        =  "Hlt/Calo/InAccBrem";
    inline const std::string InEcalHlt        =  "Hlt/Calo/InAccEcal";
    inline const std::string InHcalHlt        =  "Hlt/Calo/InAccHcal";
    // Intermediate estimators : IRelation< Track , float >
    inline const std::string SpdEHlt          = "Hlt/Calo/SpdE";
    inline const std::string PrsEHlt         = "Hlt/Calo/PrsE";
    inline const std::string EcalEHlt         = "Hlt/Calo/EcalE";
    inline const std::string HcalEHlt         = "Hlt/Calo/HcalE";
    inline const std::string EcalChi2Hlt      = "Hlt/Calo/EcalChi2";
    inline const std::string BremChi2Hlt      = "Hlt/Calo/BremChi2";
    inline const std::string ClusChi2Hlt      = "Hlt/Calo/ClusChi2";
    // CaloPID DLLs            : IRelation< Track , float >
    inline const std::string PrsPIDeHlt       = "Hlt/Calo/PrsPIDe";
    inline const std::string EcalPIDeHlt      = "Hlt/Calo/EcalPIDe";
    inline const std::string BremPIDeHlt      = "Hlt/Calo/BremPIDe";
    inline const std::string HcalPIDeHlt      = "Hlt/Calo/HcalPIDe";
    inline const std::string EcalPIDmuHlt     = "Hlt/Calo/EcalPIDmu";
    inline const std::string HcalPIDmuHlt     = "Hlt/Calo/HcalPIDmu";
    // PhotonID DLL
    inline const std::string HltPhotonID         = "Hlt/Calo/PhotonID" ;
    inline const std::string HltMergedID         = "Hlt/Calo/MergedID" ;
    inline const std::string HltPhotonFromMergedID      = "Hlt/Calo/PhotonFromMergedID" ;
  }
  namespace Calo2Track {
    typedef IRelationWeighted< LHCb::CaloCluster , LHCb::Track , float >             IClusTrTable;
    typedef IRelationWeighted< LHCb::Track , LHCb::CaloCluster , float >             ITrClusTable;
    typedef IRelationWeighted< LHCb::CaloHypo , LHCb::Track , float >                IHypoTrTable;
    typedef IRelationWeighted< LHCb::Track , LHCb::CaloHypo , float >                ITrHypoTable;

    typedef IRelationWeighted2D< LHCb::CaloCluster , LHCb::Track , float >           IClusTrTable2D;
    typedef IRelationWeighted2D< LHCb::Track , LHCb::CaloCluster , float >           ITrClusTable2D;
    typedef IRelationWeighted2D< LHCb::CaloHypo , LHCb::Track , float >              IHypoTrTable2D;
    typedef IRelationWeighted2D< LHCb::Track , LHCb::CaloHypo , float >              ITrHypoTable2D;
    typedef IRelation< LHCb::CaloHypo , float >                                      IHypoEvalTable;

    typedef IRelation< LHCb::Track , float >                                         ITrEvalTable;
    typedef IRelation< LHCb::Track , bool  >                                         ITrAccTable;
  }
}

#endif // CALO2TRACK_H
