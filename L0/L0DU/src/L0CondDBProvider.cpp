// Include files

// from Gaudi
#include "GaudiKernel/SystemOfUnits.h"

// local
#include "L0CondDBProvider.h"
#include "Event/L0DUBase.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0CondDBProvider
//
// 2007-10-12 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( L0CondDBProvider )

//=============================================================================
StatusCode L0CondDBProvider::initialize(){
  if ( msgLevel(MSG::DEBUG) ) debug() << "Initialize L0CondDBProvider" <<endmsg;
  StatusCode sc = base_class::initialize();
  if(sc.isFailure())return sc;

  m_ecal = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );

  m_gain = m_ecal->condition( "Gain" );

  // RAM(BCID)
  m_rams += "{";
  if(m_mapRam.size() > 1)m_rams += "|";
  for(const auto& mp : m_mapRam) {
    const std::string& vsn = mp.first;
    const std::vector<int>& rMap = mp.second;
    if( rMap.size() != m_cycle){
      fatal() << " The RAM(BCID) vsn = '" << vsn << "' is badly defined (" << rMap.size() << "/" << m_cycle
              << ") elements" << endmsg;
      return StatusCode::FAILURE;
    }
    m_rams += vsn ;
    if(m_mapRam.size() > 1)m_rams += "|";
  }
  m_rams += "}";
  info() << "Registered RAM(BCID) versions = " << m_rams << endmsg;
  return StatusCode::SUCCESS;
}

double L0CondDBProvider::scale(unsigned int base ) const {
  switch (base) {
    case L0DUBase::Type::Digit  : return 1;
    case L0DUBase::Type::CaloEt : return caloEtScale();
    case L0DUBase::Type::MuonPt : return muonPtScale();
  }
  Error("Unknown scale type", StatusCode::SUCCESS).ignore();
  ++m_unknownScale;
  return 0.;
}

double L0CondDBProvider::caloEtScale() const {
  double caloEtScale = 0.0;
  if ( !m_gain ){
    Error("Condition 'Gain' not found in Ecal",StatusCode::SUCCESS).ignore();
    ++m_gainNotFound;
    return 0.0;
  }
  if ( m_gain->exists( "L0EtBin" ) ) {
    caloEtScale = m_gain->paramAsDouble( "L0EtBin" ) ;
  } else {
    Error("Parameter 'L0EtBin' not found in Ecal 'Gain'",StatusCode::SUCCESS).ignore();
    ++m_L0EtBinNotFound;
  }
  if ( msgLevel(MSG::DEBUG) )
    debug() << "CaloEt scale set to " << caloEtScale << " MeV" << endmsg;
  return caloEtScale;
}

double L0CondDBProvider::muonPtScale() const {
  double muonPtScale = 40.*Gaudi::Units::MeV ;// ADC to MeV (hardcoded -- to be extracted from CondDB)
  if( std::abs(caloEtScale() - 24.*Gaudi::Units::MeV)<1.e-3  ) muonPtScale = 50.*Gaudi::Units::MeV;  // patch for hardware change on 2015/06/02
  return muonPtScale;
}

const std::vector<int> L0CondDBProvider::RAMBCID(std::string vsn) const {
  auto it = m_mapRam.find(vsn);
  if( it == m_mapRam.end() ){
    Warning("The RAM(BCID) with the requested version " + vsn + " has not been found" ).ignore();
    return {};
  }
  return it->second;
}
int L0CondDBProvider::RAMBCID(std::string vsn,int bcid) const {
  const std::vector<int>& ram = RAMBCID( vsn );
  if ( bcid+1 > (int)ram.size()){
    Warning("The requested BCID = " + std::to_string(bcid) + " exceed the RAM(BCID) size").ignore();
    return 0;
  }
  return ram[bcid];
}
