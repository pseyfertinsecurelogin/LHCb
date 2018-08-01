// Include files
#include <optional>

// from Gaudi

// local
#include "L0DUDecoder.h"
#include "Event/L0DUBase.h"
#include "Event/L0DUConfig.h"

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
  static const std::string MuonCU0_Status =  "MuonCU0(Status)";
  static const std::string MuonCU1_Status =  "MuonCU1(Status)";
  static const std::string MuonCU2_Status =  "MuonCU2(Status)";
  static const std::string MuonCU3_Status =  "MuonCU3(Status)";

  static const std::string PU_MoreInfo = "PU(MoreInfo)";

  static const std::string Sum_Et_Prev2 = "Sum(Et,Prev2)";
  static const std::string Sum_Et_Prev1 = "Sum(Et,Prev1)";

  static const std::string Sum_Et_Next1 = "Sum(Et,Next1)";
  static const std::string Sum_Et_Next2 = "Sum(Et,Next2)";


  void encode_( std::optional<std::map<std::string, std::pair<unsigned int, double>>>& map,
                LHCb::L0DUReport& report,
                const std::string& name, unsigned int data , unsigned int scale = 1 ) {
    if ( map ) (*map)[name] = { data, scale };
    if ( report.dataDigit(name) != data ) report.addToData( name, data, scale );
  }
}

//-----------------------------------------------------------------------------
// Implementation file for class : L0DUFromRawHlt1Tool
//
// 2012-02-13 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( L0DUDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0DUDecoder::L0DUDecoder( const std::string& name,
                          ISvcLocator* pSvcLocator )
: Transformer( name, pSvcLocator,
               KeyValue{ "RawEventLocations", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Trigger ,  LHCb::RawEventLocation::Default ) },
               KeyValue{ "L0DUReportLocation", LHCb::L0DUReportLocation::Default } )
{
}

//=========================================================================
//
//=========================================================================
StatusCode L0DUDecoder::initialize() {
  StatusCode sc = Transformer::initialize();
  if(sc.isFailure())return sc;

  m_confTool = tool<IL0DUConfigProvider>(m_configType , m_configName);
  m_conddb = tool<IL0CondDBProvider>("L0CondDBProvider");

  return sc;
}

//=========================================================================
//
//=========================================================================
LHCb::L0DUReport L0DUDecoder::operator()( const LHCb::RawEvent& rawEvent ) const {

  using namespace L0DUBase::PredefinedData;

  LHCb::L0DUReport report;

  std::optional<std::map<std::string, std::pair<unsigned int,double>>> dataMap;
  if (msgLevel(MSG::DEBUG)) dataMap.emplace();

  auto encode = [&](auto&&... args) {
      encode_(dataMap,report,std::forward<decltype(args)>(args)...);
  };

  //== Get the normal data bank. Check that it exists and is alone.
  const std::vector<LHCb::RawBank*> l0Banks = rawEvent.banks( LHCb::RawBank::L0DU );
  if( l0Banks.size() != 1 ){
    // Note the StatusCode SUCCESS here despite the exception thrown. Details are given
    // in https://gitlab.cern.ch/lhcb/Rec/merge_requests/557 on why we need to do this
    // kind of trick here. To sketch the main lines : the fact that the raw bank is missing
    // can be normal in case of simulated data, so the program should go on in this case.
    // A better solution would be to introduce a filter. It would be clearer and thread
    // safe, but as this code will never be used in multi-threaded environment and won't
    // survive the Run 3 upgrade, it was not implemented or the moment
    throw GaudiException( l0Banks.empty() ? "Missing raw bank" : "More than one raw bank",
                          name(), StatusCode::SUCCESS );
  }
  const LHCb::RawBank* bank = l0Banks.front();
  // Check Magic pattern
  if( LHCb::RawBank::MagicPattern != bank->magic() ) {
    throw GaudiException("Bad MagicPattern, corrupt bank", name(), StatusCode::FAILURE );
  }
  // rawBank header :
  const unsigned int* data = bank->data();
  unsigned int size   = bank->size();  // Bank size is in bytes
  unsigned int vsn    = bank->version();

  report.setBankVersion( vsn );
  if ( 2 != vsn ) {
    throw GaudiException( "== Unsupported version " + std::to_string(vsn) + " L0DU bank",
                          name(), StatusCode::FAILURE );
  }

  unsigned int itc    = (*data & 0x00000003) >>  0;
  unsigned int iec    = (*data & 0x0000000C) >>  2;
  //unsigned int pgaVsn = (*data & 0x00000FF0) >>  4; // unused...
  int nm              = (*data & 0x00003000) >> 12;
  int np              = (*data & 0x0000C000) >> 14;
  unsigned int tck    = (*data & 0xFFFF0000) >> 16;
  if (m_ensureKnownTCK.value() && !tck) {
      throw GaudiException("TCK not recognized. Run with L0Conf().EnsureKnownTCK=False to ignore this error",
                           name(),StatusCode::FAILURE);
  }
  report.setTck( tck );

  const LHCb::L0DUConfig *config = m_config;
  if ( !config || config->tck()!= tck ) {
    config = m_confTool->config( tck , "T0");
    m_config = config;
  }

  if ( UNLIKELY(!config) ) {
    std::stringstream s;
    s << " Unable to load the configuration for tck = " <<  format("0x%04X", tck) << " --> Incomplete L0DUReport" ;
    Warning(s.str(), StatusCode::SUCCESS).ignore();
  } else {
    report.setConfiguration( config );
  }

  //== PGA3-block header
  ++data;
  unsigned long pga3Status   = (*data & 0x001FFFFF ) >>  0;
  unsigned int bcid3         = (*data & 0x0FE00000 ) >> 21;
  unsigned int  nmu          = (*data & 0xF0000000 ) >> 28;
  unsigned int pga3Size = 4 * ( 4 + ( int((nmu+3)/4) ) + ( int((nmu+1)/2) ) );

  encode( MuonCU0_Status,(pga3Status >> 0) & 0xF );
  encode( MuonCU1_Status,(pga3Status >> 4) & 0xF );
  encode( MuonCU2_Status,(pga3Status >> 8) & 0xF );
  encode( MuonCU3_Status,(pga3Status >> 12)& 0xF );

  ++data;
  int mu1 = (*data & 0x0000007F ) >> 0;
  auto muonScale = m_conddb->muonPtScale();
  encode( Name[Muon1Pt] , mu1, muonScale);
  encode( Name[Muon1Sgn],(*data & 0x00000080 ) >> 7  );
  int mu2 = (*data & 0x00007F00 ) >> 8;
  encode( Name[Muon2Pt] ,mu2  , muonScale);
  encode( Name[Muon2Sgn],(*data & 0x00008000 ) >> 15 );
  encode( Name[Muon3Pt] ,(*data & 0x007F0000 ) >> 16 , muonScale);
  encode( Name[Muon3Sgn],(*data & 0x00800000 ) >> 23 );
  encode( Name[DiMuonPt],(*data & 0xFF000000 ) >> 24, muonScale);
  encode( Name[DiMuonProdPt], mu1*mu2, muonScale);

  ++data;
  encode( Name[Muon1Add],(*data & 0x0000FFFF ) >>0   );
  encode( Name[Muon2Add],(*data & 0xFFFF0000 ) >>16  );

  ++data;
  encode( Name[Muon3Add],(*data & 0x0000FFFF)  >> 0  );
  //unsigned int muCleanPattern = (*data & 0x00FF0000)  >>16; // unused...

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
    encode( s_Muon_Pt[n[imu]], pt, muonScale );
    encode( s_Muon_Sgn[n[imu]], sgn );
  }

  // PGA2-block header
  ++data;
  unsigned int rsda      = (*data & 0x0000FFFF );
  unsigned int pga2Status= (*data & 0x0FFF0000 ) >> 16;
  //unsigned int status    = ( pga2Status & 0xE00 ) >> 12;  // unused...

  unsigned int bcid2     = (rsda & 0x3FF)  | ( (*data & 0x30000000 ) >> 18);
  unsigned int beam1     = (rsda >> 10) & 0x1;
  unsigned int beam2     = (rsda >> 11) & 0x1;

  unsigned int decision  = (rsda >> 12) & 1;
  unsigned int fb        = (rsda >> 13) & 1;
  unsigned int ttb       = (rsda >> 14) & 1;

  int decisionValue = 0x0;
  if( decision == 1) decisionValue |= LHCb::L0DUDecision::Physics;
  if( beam1    == 1) decisionValue |= LHCb::L0DUDecision::Beam1;
  if( beam2    == 1) decisionValue |= LHCb::L0DUDecision::Beam2;

  // update L0DUReport
  report.setDecisionValue( decisionValue );
  report.setTimingTriggerBit( ttb == 1 );
  report.setForceBit( fb == 1 );
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
  if ( size != allSize ){
    info() << "READOUTSTATUS : the total expected size " << allSize
           << " does NOT match the bank size " << size << " <** POSSIBLE DATA CORRUPTION **>"
           << endmsg;
    throw GaudiException("Corrupt L0DU RawBank", name(), StatusCode::FAILURE );
  }
  counter("L0DU RawBank Size (Bytes)") += size;

  if ( (0x7F & bcid2) != bcid3){
    info() << " BCIDs PGA2(LSB)/PGA3= " << (bcid2 & 0x7F) << " /"  << bcid3 << " NOT ALIGNED " << endmsg;
  }


  // PGA2 processing : maps
  for(unsigned int i = 0 ; i < itc ; i++){
    report.setChannelsPreDecisionSummary( *(++data), i, 0 );
  }
  for(unsigned int i = 0 ; i < itc ; i++){
    report.setChannelsDecisionSummary( *(++data), i, 0 );
  }
  for(unsigned int i = 0 ; i < iec ; i++){
    report.setConditionsValueSummary( *(++data), i, 0 );
  }

  //PGA2 input data
  ++data;
  int sumEt0 = ( *data & 0x00003FFF );
  std::map<int , unsigned int> sumEt;
  sumEt[0] = sumEt0;
  auto caloScale = m_conddb->caloEtScale();
  encode( Name[SumEt], sumEt0, caloScale );
  encode( Name[SpdMult], (*data & 0x0FFFC000 ) >> 14 );
  encode( PU_MoreInfo,   (*data & 0xF0000000 ) >> 28 );

  ++data;
  encode( Name[ElectronEt] ,(*data & 0x000000FF )      , caloScale );
  encode( Name[PhotonEt]   ,(*data & 0x0000FF00 ) >>  8, caloScale );
  encode( Name[GlobalPi0Et],(*data & 0x00FF0000 ) >> 16, caloScale );
  encode( Name[LocalPi0Et] ,(*data & 0xFF000000 ) >> 24, caloScale );

  ++data;
  encode( Name[HadronEt],(*data & 0x000000FF ) , caloScale );
  encode( Name[PuPeak1], (*data & 0x0000FF00 ) >>  8 );
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
      report.setChannelsDecisionSummary( *(++data), i, -im );
    }
    for(unsigned int i = 0 ; i < iec ; i++){
      report.setConditionsValueSummary( *(++data), i, -im );
    }
  }

  // SumEt
  for(int im = nm ; im > 0 ; im--){
    int odd = (nm-im)%2;
    if(odd == 0 )++data;
    sumEt[-im]= (*data >> 16*odd) & 0x3FFF ;
  }

  encode( Sum_Et_Prev2, sumEt[-2], caloScale);
  encode( Sum_Et_Prev1, sumEt[-1], caloScale);

  //== Same fo rnext crossings

  for(int ip = 0 ; ip < np ; ip++){
    for(unsigned int i = 0 ; i < itc ; i++){
      report.setChannelsDecisionSummary( *(++data), i, ip+1 );
    }
    for(unsigned int i = 0 ; i < iec ; i++){
      report.setConditionsValueSummary( *(++data), i, ip+1 );
    }
  }

  for(int ip = 0 ; ip < np ; ip++){
    int odd = ip%2;
    if(odd == 0 ) ++data;
    sumEt[ip+1] = (*data >> 16*odd) & 0x3FFF ;
  }

  encode( Sum_Et_Next1, sumEt[1], caloScale);
  encode( Sum_Et_Next2, sumEt[2], caloScale);

  report.setSumEt( std::move(sumEt) );
  report.setBcid( bcid2 );

  // Print all data values extracted from rawBank
  if ( dataMap ) {
    for(const auto& imap : *dataMap ) {
      debug() << "   --> Data = (value,scale) : " << imap.first << " = " <<  imap.second << endmsg;
    }
  }
   return report;
}
//=============================================================================
