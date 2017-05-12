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
// Standard constructor, initializes variables
//=============================================================================
L0CondDBProvider::L0CondDBProvider( const std::string& type,
                    const std::string& name,
                    const IInterface* parent )
  : base_class ( type, name , parent )
{
  declareInterface<IL0CondDBProvider>(this);
  declareProperty("RAMBCID" , m_mapRam);
}

//=============================================================================
StatusCode L0CondDBProvider::initialize(){
  if ( msgLevel(MSG::DEBUG) ) debug() << "Initialize L0CondDBProvider" <<endmsg;
  StatusCode sc = base_class::initialize();
  if(sc.isFailure())return sc;

  m_ecal = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );

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



double L0CondDBProvider::scale(unsigned int base ){
  switch (base) {
    case L0DUBase::Type::Digit  : return 1;
    case L0DUBase::Type::CaloEt : return caloEtScale();
    case L0DUBase::Type::MuonPt : return muonPtScale();
  }
  Error("Unknown scale type", StatusCode::SUCCESS).ignore();
  counter("Unknown scale type")+=1;
  return 0.;
}


double L0CondDBProvider::caloEtScale(){
  m_gain = m_ecal->condition( "Gain" );
  double caloEtScale = 0.0;
  if ( !m_gain ){
    Error("Condition 'Gain' not found in Ecal",StatusCode::SUCCESS).ignore();
    counter("'Gain' condition not found for Ecal")+=1;
    return 0.0;
  }
  if ( m_gain->exists( "L0EtBin" ) ) {
    caloEtScale = m_gain->paramAsDouble( "L0EtBin" ) ;
  } else {
    Error("Parameter 'L0EtBin' not found in Ecal 'Gain'",StatusCode::SUCCESS).ignore();
    counter("'L0EtBin' parameter not found in 'Gain' condition")+=1;
  }
  if ( msgLevel(MSG::DEBUG) )
    debug() << "CaloEt scale set to " << caloEtScale << " MeV" << endmsg;
  return caloEtScale;
}


double L0CondDBProvider::muonPtScale(){
  double muonPtScale = 40.*Gaudi::Units::MeV ;// ADC to MeV (hardcoded -- to be extracted from CondDB)
  if( std::abs(caloEtScale() - 24.*Gaudi::Units::MeV)<1.e-3  ) muonPtScale = 50.*Gaudi::Units::MeV;  // patch for hardware change on 2015/06/02
  return muonPtScale;
}


const std::vector<int> L0CondDBProvider::RAMBCID(std::string vsn){
  const std::vector<int> empty;
  auto it = m_mapRam.find(vsn);
  if( it == m_mapRam.end() ){
    Warning("The RAM(BCID) with the requested version " + vsn + " has not been found" ).ignore();
    return empty;
  }
  return (*it).second;
}
int L0CondDBProvider::RAMBCID(std::string vsn,int bcid){
  const std::vector<int>& ram = RAMBCID( vsn );
  if( bcid+1 > (int)ram.size()){
    std::stringstream b("");
    b<<bcid;
    Warning("The requested BCID = " + b.str() + " exceed the RAM(BCID) size").ignore();
    return 0;
  }
  return ram[bcid];
}

