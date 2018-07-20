// Include files

// from Gaudi
#include "L0Interfaces/IL0CondDBProvider.h"

// local
#include "L0DUFromRawHlt1Tool.h"

namespace {


  static const std::array<std::string,8> s_Muon_Add =
  { "M0(Add)", "M1(Add)", "M2(Add)", "M3(Add)", "M4(Add)", "M5(Add)", "M6(Add)", "M7(Add)" };

  static const std::array<std::string,8> s_Muon_Pt =
  { "M0(Pt)", "M1(Pt)", "M2(Pt)", "M3(Pt)", "M4(Pt)", "M5(Pt)", "M6(Pt)", "M7(Pt)" };

  static const std::array<std::string,8> s_Muon_Sgn =
  { "M0(Sgn)", "M1(Sgn)", "M2(Sgn)", "M3(Sgn)", "M4(Sgn)", "M5(Sgn)", "M6(Sgn)", "M7(Sgn)" };

  static const std::string Electron_Status = "Electron(Status)";
  static const std::string Photon_Status =   "Photon(Status)";
  static const std::string Hadron_Status =   "Hadron(Status)";
  static const std::string GlobalPi0_Status ="GlobalPi0(Status)";
  static const std::string LocalPi0_Status = "LocalPi0(Status)";
  static const std::string Sum_Status =      "Sum(Status)";
  static const std::string Spd_Status =      "Spd(Status)";
  static const std::string PU1_Status =      "PU1(Status)";
  static const std::string PU2_Status =      "PU2(Status)";

  static const std::string PU_MoreInfo = "PU(MoreInfo)";

  static const std::string Sum_Et_Prev2 = "Sum(Et,Prev2)";
  static const std::string Sum_Et_Prev1 = "Sum(Et,Prev1)";

  static const std::string Sum_Et_Next1 = "Sum(Et,Next1)";
  static const std::string Sum_Et_Next2 = "Sum(Et,Next2)";

  static const std::string MuonCU0_Status = "MuonCU0(Status)";
  static const std::string MuonCU1_Status = "MuonCU1(Status)";
  static const std::string MuonCU2_Status = "MuonCU2(Status)";
  static const std::string MuonCU3_Status = "MuonCU3(Status)";
}

//-----------------------------------------------------------------------------
// Implementation file for class : L0DUFromRawHlt1Tool
//
// 2012-02-13 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( L0DUFromRawHlt1Tool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0DUFromRawHlt1Tool::L0DUFromRawHlt1Tool( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
  : base_class ( type, name , parent )
{
  declareInterface<IL0DUFromRawTool>(this);
  declareProperty( "RawLocations"           , m_rawLocations  = { LHCb::RawEventLocation::Trigger ,  LHCb::RawEventLocation::Default } );
  declareProperty( "L0DUConfigProviderName"  , m_configName="L0DUConfig");
  declareProperty( "L0DUConfigProviderType"  , m_configType="L0DUMultiConfigProvider");
}

//=========================================================================
//
//=========================================================================
StatusCode L0DUFromRawHlt1Tool::initialize ( ) {
  if ( msgLevel(MSG::DEBUG) ) debug() << "Initialize" << endmsg;
  StatusCode sc = base_class::initialize();
  if(sc.isFailure())return sc;

  m_confTool = tool<IL0DUConfigProvider>(m_configType , m_configName);

  // Built L0ProcessorData : needed by the interface. no real use here.
  m_processorDatas.clear();
  for( int fiber = 0 ; fiber != (int) L0DUBase::Fiber::Empty ; fiber++){
    m_processorDatas.insert( new LHCb::L0ProcessorData ( (L0DUBase::Fiber::Type) fiber , L0DUBase::EmptyData ) );
  }

  m_oldTck = 0;

  //== Get the scales once and for all. No change during Hlt execution...
  IL0CondDBProvider* condb = tool<IL0CondDBProvider>("L0CondDBProvider");
  m_muonScale = condb->muonPtScale();
  m_caloScale = condb->caloEtScale();

  return sc;
}

StatusCode L0DUFromRawHlt1Tool::finalize ( ) {
  m_processorDatas.clear();
  return base_class::finalize();
}

//=========================================================================
//
//=========================================================================
bool L0DUFromRawHlt1Tool::decodeBank( int iBank ) {
  using namespace L0DUBase::PredefinedData;

  m_report.clear();

  m_roStatus = LHCb::RawBankReadoutStatus( LHCb::RawBank::L0DU );
  m_roStatus.addStatus( 0, LHCb::RawBankReadoutStatus::Status::OK);

  LHCb::RawEvent* raw = nullptr;
  for ( const auto& loc : m_rawLocations ) {
    raw = getIfExists<LHCb::RawEvent>( loc );
    if( raw ) break;
  }
  if( !raw ){
    Warning("rawEvent not found in  " + Gaudi::Utils::toString(m_rawLocations) +" locations ",StatusCode::SUCCESS).ignore();
    return false;
  }


  //== Any error bank?
  if ( raw->banks( LHCb::RawBank::L0DUError ).size() != 0 ) {
    m_roStatus.addStatus( 0, LHCb::RawBankReadoutStatus::Status::ErrorBank );
  }
  //== Get the normal data bank. Check that it exists and is alone.
  const auto& l0Banks = raw->banks( LHCb::RawBank::L0DU );
  if ( l0Banks.empty() ) {
    m_roStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::Missing);
    return false;
  }
  if( 1 < l0Banks.size() ){
    m_roStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::NonUnique);
    return false;
  }
  const LHCb::RawBank* bank = l0Banks[iBank];
  // Check Magic pattern
  if( LHCb::RawBank::MagicPattern != bank->magic() ) {
    Error("Bad MagicPattern",StatusCode::SUCCESS).ignore();
    m_roStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::BadMagicPattern);
    return false;
  }
  // rawBank header :
  const unsigned int* data = bank->data();
  m_size   = bank->size();  // Bank size is in bytes
  m_vsn    = bank->version();
  m_roStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::OK);

  m_report.setBankVersion( m_vsn );
  if ( 2 != m_vsn ) {
    warning() << "== Unsupported version " << m_vsn << " for L0DU bank" << endmsg;
    return false;
  }

  unsigned int itc      = (*data & 0x00000003)  >>  0;
  unsigned int iec      = (*data & 0x0000000C)  >>  2;
  m_pgaVsn              = (*data & 0x00000FF0)  >>  4;
  int nm                = (*data & 0x00003000 ) >> 12;
  int np                = (*data & 0x0000C000 ) >> 14;
  m_tck                 = (*data & 0xFFFF0000)  >> 16;
  m_report.setTck( m_tck );

  if ( m_tck != m_oldTck ) {
    m_config = m_confTool->config( m_tck , "T0");
    m_oldTck = m_tck;
  }
  m_report.setConfiguration( m_config );

  //== PGA3-block header
  ++data;

  unsigned long pga3Status   = (*data & 0x001FFFFF ) >>  0;
  m_bcid3                    = (*data & 0x0FE00000 ) >> 21;
  unsigned int  nmu          = (*data & 0xF0000000 ) >> 28;
  unsigned int pga3Size = 4 * ( 4 + ( int((nmu+3)/4) ) + ( int((nmu+1)/2) ) );

  encode(MuonCU0_Status,(pga3Status >> 0) & 0xF );
  encode(MuonCU1_Status,(pga3Status >> 4) & 0xF );
  encode(MuonCU2_Status,(pga3Status >> 8) & 0xF );
  encode(MuonCU3_Status,(pga3Status >> 12)& 0xF );

  ++data;
  int mu1 = (*data & 0x0000007F ) >> 0;
  encode( Name[Muon1Pt] , mu1, m_muonScale);
  encode( Name[Muon1Sgn],(*data & 0x00000080 ) >> 7  );
  int mu2 = (*data & 0x00007F00 ) >> 8;
  encode( Name[Muon2Pt] ,mu2  , m_muonScale);
  encode( Name[Muon2Sgn],(*data & 0x00008000 ) >> 15 );
  encode( Name[Muon3Pt] ,(*data & 0x007F0000 ) >> 16 , m_muonScale);
  encode( Name[Muon3Sgn],(*data & 0x00800000 ) >> 23 );
  encode( Name[DiMuonPt],(*data & 0xFF000000 ) >> 24, m_muonScale);
  encode( Name[DiMuonProdPt], mu1*mu2, m_muonScale);

  ++data;
  encode( Name[Muon1Add],(*data & 0x0000FFFF ) >>0   );
  encode( Name[Muon2Add],(*data & 0xFFFF0000 ) >>16  );

  ++data;
  encode( Name[Muon3Add],(*data & 0x0000FFFF)  >> 0  );
  m_muCleanPattern = (*data & 0x00FF0000)  >>16;

  // PGA3 Input data
  int n[8];
  for(unsigned int imu =0 ; imu < nmu ; imu++){
    int odd = imu%2;
    if ( 0 == odd ) ++data;
    n[imu]          = ((*data  >> 16*odd) & 0x0000E000 ) >> 13;
    unsigned int val = (*data  >> 16*odd) & 0x0000FFFF;
    encode( s_Muon_Add[n[imu]], val );
  }

  for(unsigned int imu =0 ; imu < nmu ; imu++){
    int biodd = imu%4;
    if( 0 == biodd ) ++data;
    unsigned int pt = (*data >> 8*biodd) & 0x0000007F ;
    unsigned int sgn=((*data >> 8*biodd) & 0x00000080) >> 7;
    encode( s_Muon_Pt[n[imu]], pt, m_muonScale );
    encode( s_Muon_Sgn[n[imu]], sgn );
  }

  // PGA2-block header
  ++data;
  m_rsda       = (*data & 0x0000FFFF );
  unsigned int pga2Status= (*data & 0x0FFF0000 ) >> 16;
  m_status               = (pga2Status & 0xE00   )  >> 12;

  unsigned int beam1 = 0;
  unsigned int beam2 = 0;
  m_bcid2   = (m_rsda & 0x3FF)  | ( (*data & 0x30000000 ) >> 18);
  beam1     = (m_rsda >> 10) & 0x1;
  beam2     = (m_rsda >> 11) & 0x1;

  unsigned int decision  = (m_rsda >> 12) & 1;
  unsigned int fb        = (m_rsda >> 13) & 1;
  unsigned int ttb       = (m_rsda >> 14) & 1;

  int decisionValue = 0x0;
  if( decision == 1)decisionValue |= LHCb::L0DUDecision::Physics;
  if( beam1    == 1)decisionValue |= LHCb::L0DUDecision::Beam1;
  if( beam2    == 1)decisionValue |= LHCb::L0DUDecision::Beam2;


  // update L0DUReport
  m_report.setDecisionValue( decisionValue );

  m_report.setTimingTriggerBit( (ttb == 1) );
  m_report.setForceBit( (fb == 1) );
  //
  encode( Electron_Status,  (pga2Status>>L0DUBase::Fiber::CaloElectron)  & 0x1 );
  encode( Photon_Status,    (pga2Status>>L0DUBase::Fiber::CaloPhoton)    & 0x1 );
  encode( Hadron_Status,    (pga2Status>>L0DUBase::Fiber::CaloHadron)    & 0x1 );
  encode( GlobalPi0_Status, (pga2Status>>L0DUBase::Fiber::CaloPi0Local)  & 0x1 );
  encode( LocalPi0_Status,  (pga2Status>>L0DUBase::Fiber::CaloPi0Global) & 0x1 );
  encode( Sum_Status,       (pga2Status>>L0DUBase::Fiber::CaloSumEt)     & 0x1 );
  encode( Spd_Status,       (pga2Status>>L0DUBase::Fiber::CaloSpdMult)   & 0x1 );
  encode( PU1_Status,       (pga2Status>>L0DUBase::Fiber::Pu1)           & 0x1 );
  encode( PU2_Status,       (pga2Status>>L0DUBase::Fiber::Pu2)           & 0x1 );

  // check the bank size is consistent

  unsigned int pga2Size = 4 * ( 7 + iec + 2*itc +  (itc+iec)*(nm+np)
                                + (int( (nm+1)/2) + int ((np+1)/2)));
  unsigned int allSize = 4 + pga2Size + pga3Size;
  if ( m_size != allSize ){
    info() << "READOUTSTATUS : the total expected size " << allSize
           << " does NOT match the bank size " << m_size << " <** POSSIBLE DATA CORRUPTION **>"
           << endmsg;
    m_roStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::Corrupted );
  }

  if ( (0x7F & m_bcid2) != m_bcid3){
    info() << " BCIDs PGA2(LSB)/PGA3= " << (m_bcid2 & 0x7F) << " /"  << m_bcid3 << " NOT ALIGNED " << endmsg;
  }


  // PGA2 processing : maps
  for(unsigned int i = 0 ; i < itc ; i++){
    m_report.setChannelsPreDecisionSummary( *(++data), i, 0 );
  }
  for(unsigned int i = 0 ; i < itc ; i++){
    m_report.setChannelsDecisionSummary( *(++data), i, 0 );
  }
  for(unsigned int i = 0 ; i < iec ; i++){
    m_report.setConditionsValueSummary( *(++data), i, 0 );
  }

  //PGA2 input data
  ++data;
  int sumEt0 = (*data & 0x00003FFF );
  m_sumEt[0] = sumEt0;
  encode( Name[SumEt]        , sumEt0, m_caloScale );

  encode( Name[SpdMult], (*data & 0x0FFFC000 ) >> 14 );
  encode( PU_MoreInfo,   (*data & 0xF0000000 ) >> 28 );

  ++data;
  encode( Name[ElectronEt] ,(*data & 0x000000FF )      , m_caloScale );
  encode( Name[PhotonEt]   ,(*data & 0x0000FF00 ) >> 8 , m_caloScale );
  encode( Name[GlobalPi0Et],(*data & 0x00FF0000 ) >> 16, m_caloScale );
  encode( Name[LocalPi0Et] ,(*data & 0xFF000000 ) >> 24, m_caloScale );

  ++data;
  encode( Name[HadronEt],(*data & 0x000000FF ) , m_caloScale );
  encode( Name[PuPeak1], (*data & 0x0000FF00 ) >> 8  );
  encode( Name[PuPeak2], (*data & 0x00FF0000 ) >> 16 );
  encode( Name[PuHits] , (*data & 0xFF000000 ) >> 24 );

  ++data;
  encode( Name[ElectronAdd] ,(*data & 0x0000FFFF )       );
  encode( Name[PhotonAdd]   ,(*data & 0xFFFF0000 ) >> 16 );

  ++data;
  encode( Name[GlobalPi0Add],(*data & 0x0000FFFF )       );
  encode( Name[LocalPi0Add] ,(*data & 0xFFFF0000 ) >> 16 );

  ++data;
  encode( Name[HadronAdd]  ,(*data & 0x0000FFFF )       );
  encode( Name[PuPeak1Pos] ,(*data & 0x00FF0000 ) >> 16 );
  encode( Name[PuPeak2Pos] ,(*data & 0xFF000000 ) >> 24 );

  //== Handle previous corssing information

  for(int im = nm ; im > 0 ; im--){
    for(unsigned int i = 0 ; i < itc ; i++){
      m_report.setChannelsDecisionSummary( *(++data), i, -im );
    }
    for(unsigned int i = 0 ; i < iec ; i++){
      m_report.setConditionsValueSummary( *(++data), i, -im );
    }
  }

  // SumEt
  for(int im = nm ; im > 0 ; im--){
    int odd = (nm-im)%2;
    if(odd == 0 )++data;
    m_sumEt[-im]= (*data >> 16*odd) & 0x3FFF ;
  }

  encode( Sum_Et_Prev2, m_sumEt[-2], m_caloScale);
  encode( Sum_Et_Prev1, m_sumEt[-1], m_caloScale);

  //== Same fo rnext crossings

  for(int ip = 0 ; ip < np ; ip++){
    for(unsigned int i = 0 ; i < itc ; i++){
      m_report.setChannelsDecisionSummary( *(++data), i, ip+1 );
    }
    for(unsigned int i = 0 ; i < iec ; i++){
      m_report.setConditionsValueSummary( *(++data), i, ip+1 );
    }
  }

  for(int ip = 0 ; ip < np ; ip++){
    int odd = ip%2;
    if(odd == 0 ) ++data;
    m_sumEt[ip+1] = (*data >> 16*odd) & 0x3FFF ;
  }

  encode( Sum_Et_Next1, m_sumEt[1], m_caloScale);
  encode( Sum_Et_Next2, m_sumEt[2], m_caloScale);

  m_report.setSumEt( m_sumEt );
  m_report.setBcid( m_bcid2 );

  // Print all data values extracted from rawBank
  if ( msgLevel( MSG::DEBUG) ){
    for(const auto& imap : m_dataMap ) {
      debug() << "   --> Data = (value,scale) : " << imap.first << " = " <<  imap.second << endmsg;
    }
  }

   return true;
}
//=============================================================================
