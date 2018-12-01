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

// STL
#include <algorithm>

// Gaudi
#include "GaudiKernel/GaudiException.h"

// local
#include "Event/ProtoParticle.h"


//-----------------------------------------------------------------------------

// Fillstream method
std::ostream& LHCb::ProtoParticle::fillStream( std::ostream & s ) const
{
  s << "{"
    << " Track "     << this->track()
    << " CaloHypos " << this->calo()
    << " RichPID "   << this->richPID()
    << " MuonPID "   << this->muonPID()
    << " ExtraInfo [";
  for ( const auto & i : extraInfo() )
  {
    const auto info = static_cast<LHCb::ProtoParticle::additionalInfo>(i.first);
    s << " " << info << "=" << i.second;
  }
  return s << " ] }";
}

LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::clearCalo( const LHCb::CaloHypo::Hypothesis & hypo )
{

  // number of hypos removed
  LHCb::ProtoParticle::ExtraInfo::size_type removed = 0;

  // remove NULL hypos
  const auto it = std::remove ( m_calo.begin(), m_calo.end(), 0 );
  if ( m_calo.end() != it )
  {
    removed += m_calo.end() - it;
    m_calo.erase ( it , m_calo.end() );
  }

  // Find hypos to remove
  const auto iHypoRemove =
    std::remove_if ( m_calo.begin(), m_calo.end(),
                     [&](const LHCb::CaloHypo* h) { return h->hypothesis() == hypo ; } );
  if ( m_calo.end() != iHypoRemove )
  {
    removed += m_calo.end() - iHypoRemove;
    m_calo.erase ( iHypoRemove , m_calo.end() );
  }

  // return number of hypos removed
  return removed;
}

// Remove all Combined DLL information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeCombinedInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CombDLLe  );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CombDLLmu );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CombDLLpi );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CombDLLk  );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CombDLLp  );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::NoPID     );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CombDLLd  );
  return erased;
}

// Remove all RICH information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeRichInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichDLLe );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichDLLmu );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichDLLpi );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichDLLk );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichDLLp );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichDLLbt );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichDLLd );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::RichPIDStatus );
  // Set RichPID pointer to NULL
  this->setRichPID(nullptr);
  // Invalidate Combined DLL information since information has changed
  erased += this->removeCombinedInfo();
  return erased;
}

// Remove all MUON information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeMuonInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::InAccMuon );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonMuLL );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonBkgLL );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonNShared );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonChi2Corr);
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA1);
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA2);
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA3);
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonMVA4);
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::MuonPIDStatus );
  // Set MuonPID pointer to NULL
  this->setMuonPID(nullptr);
  // Invalidate Combined DLL information since information has changed
  erased += this->removeCombinedInfo();
  return erased;
}

// Remove all CALO-ECAL information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeCaloEcalInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::InAccEcal );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloTrajectoryL );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloChargedSpd );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloChargedPrs );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloChargedEcal );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloElectronMatch );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloTrMatch );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloEcalE );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloEcalChi2 );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloClusChi2 );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::EcalPIDe );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::EcalPIDmu );
  // removed associated CaloHypos
  erased += this->clearCalo( LHCb::CaloHypo::Hypothesis::EmCharged );
  // Invalidate Combined DLL information since information has changed
  erased += this->removeCombinedInfo();
  return erased;
}

// Remove all CALO-BREM information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeCaloBremInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::InAccBrem );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloNeutralSpd );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloNeutralPrs );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloNeutralEcal );
  erased += this->eraseInfo( LHCb::ProtoParticle::additionalInfo::CaloBremMatch );
  // remove associated Hypos
  erased += this->clearCalo( LHCb::CaloHypo::Hypothesis::Photon );
  // Invalidate Combined DLL information since information has changed
  erased += this->removeCombinedInfo();
  return erased;
}

// Remove all CALO-SPD information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeCaloSpdInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::InAccSpd);
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::CaloSpdE);
  // Invalidate Combined DLL information since information has changed
  erased += this->removeCombinedInfo();
  return erased;
}

// Remove all CALO-PRS information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeCaloPrsInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::InAccPrs);
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::CaloPrsE);
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::PrsPIDe);
  // Invalidate Combined DLL information since information has changed
  erased += this->removeCombinedInfo();
  return erased;
}

// Remove all CALO-HCAL information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeCaloHcalInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::InAccHcal);
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::CaloHcalE);
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::HcalPIDe);
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::HcalPIDmu);
  // Invalidate Combined DLL information since information has changed
  erased += this->removeCombinedInfo();
  return erased;
}

// Remove all VELO information stored in this ProtoParticle
LHCb::ProtoParticle::ExtraInfo::size_type
LHCb::ProtoParticle::removeVeloInfo()
{
  LHCb::ProtoParticle::ExtraInfo::size_type erased = 0;
  erased += this->eraseInfo(LHCb::ProtoParticle::additionalInfo::VeloCharge);
  return erased;
}

LHCb::ProtoParticle::additionalInfo
LHCb::ProtoParticle::convertExtraInfo(const std::string& name)
{
  auto enumval = LHCb::ProtoParticle::additionalInfoToType(name);
  if(enumval == LHCb::ProtoParticle::additionalInfo::LastGlobal) {
    // 'name' wasn't known
    throw GaudiException( "Unknown enum name " + name,
        "*LHCb::ProtoParticle*", StatusCode::FAILURE );
  }
  return enumval;
}
