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
#ifndef REPORTCONVERTTOOL_H
#define REPORTCONVERTTOOL_H 1

// Include files
// from STL
#include <string>
#include <unordered_map>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// From PhysEvent
#include "Event/Particle.h"
#include "Event/ProtoParticle.h"
#include "Event/Track.h"
#include "Event/RichPID.h"
#include "Event/MuonPID.h"
#include "Event/CaloCluster.h"
#include "Event/RecVertex.h"
#include "Event/Vertex.h"
#include "Event/State.h"

#include "Event/HltDecReports.h"
#include "Event/HltSelReports.h"
#include "Event/HltVertexReports.h"
#include "Event/HltDecReports.h"

#include "HltDAQ/HltSelRepRBStdInfo.h"
#include "HltDAQ/IReportConvert.h"
#include "Event/HltObjectSummary.h"

using std::unordered_map;
using std::string;
using std::pair;
using GaudiUtils::VectorMap;
using LHCb::HltObjectSummary;
using LHCb::HltSelRepRBStdInfo;

/** @class ReportConvertTool ReportConvertTool.h
 *
 *  Tool to check if version number of the HltSelReports
 *
 *  @author Sean Benson
 *  @date   02/10/2014
 */
class ReportConvertTool : public extends<GaudiTool, IReportConvert>
{
  public:

    /// Standard constructor
    ReportConvertTool( const std::string& type, const std::string& name,
                       const IInterface* parent);

    void setReportVersion(int) override;
    void setReportVersionLatest() override;
    int getReportVersion() override;
    int getLatestVersion() override;
    int getSizeSelRepParticleLatest() override;


    //===========================================================================
    /// Convert the sub bank to a HltObjectSummary.
    void SummaryFromRaw( HltObjectSummary::Info*, HltSelRepRBStdInfo::StdInfo*, int ) override;

    /// Put the information in to the HltObjectSummary
    void ParticleObject2Summary( HltObjectSummary::Info*, const LHCb::Particle* , bool)  override;
    void ProtoParticleObject2Summary( HltObjectSummary::Info*, const LHCb::ProtoParticle*, bool)  override;
    void TrackObject2Summary( HltObjectSummary::Info*, const LHCb::Track*, bool)  override;
    void RichPIDObject2Summary( HltObjectSummary::Info*, const LHCb::RichPID*, bool)  override;
    void MuonPIDObject2Summary( HltObjectSummary::Info*, const LHCb::MuonPID*, bool)  override;
    void CaloClusterObject2Summary( HltObjectSummary::Info*, const LHCb::CaloCluster*, bool)  override;
    void CaloHypoObject2Summary( HltObjectSummary::Info*, const LHCb::CaloHypo*, bool)  override;
    void RecVertexObject2Summary( HltObjectSummary::Info*, const LHCb::RecVertex*, bool)  override;
    void VertexObject2Summary( HltObjectSummary::Info*, const LHCb::Vertex*, bool)  override;
    void RecSummaryObject2Summary( HltObjectSummary::Info*, const LHCb::RecSummary*)  override;
    void GenericMapObject2Summary( HltObjectSummary::Info*, const GaudiUtils::VectorMap<short,float>*)  override;
    //
    /// Put the information in the summary back in the object
    void ParticleObjectFromSummary( const HltObjectSummary::Info*, LHCb::Particle*,bool)  override;
    void ProtoParticleObjectFromSummary( const HltObjectSummary::Info*, LHCb::ProtoParticle*,bool)  override;
    void TrackObjectFromSummary( const HltObjectSummary::Info*, LHCb::Track*,bool)  override;
    void RichPIDObjectFromSummary( const HltObjectSummary::Info*, LHCb::RichPID*,bool)  override;
    void MuonPIDObjectFromSummary( const HltObjectSummary::Info*, LHCb::MuonPID*,bool)  override;
    void CaloClusterObjectFromSummary( const HltObjectSummary::Info*, LHCb::CaloCluster*,bool)  override;
    void CaloHypoObjectFromSummary( const HltObjectSummary::Info*, LHCb::CaloHypo*,bool)  override;
    void RecVertexObjectFromSummary( const HltObjectSummary::Info*, LHCb::RecVertex*,bool)  override;
    void VertexObjectFromSummary( const HltObjectSummary::Info*, LHCb::Vertex*,bool)  override;
    void RecSummaryObjectFromSummary( const HltObjectSummary::Info*, LHCb::RecSummary*)  override;
    void GenericMapObjectFromSummary( const HltObjectSummary::Info*, GaudiUtils::VectorMap<short,float>*)  override;

    int findBestPrevious(const unordered_map<int, unordered_map<string,pair<int,int> > >&,int) const;

  private:
    int m_version = -999;
    int m_LatestVersion = 1;

}; // End of class header.

#endif // REPORTCONVERTTOOL_H
