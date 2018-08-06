// Include files

// local
#include "MuonDet/MuonBasicGeometry.h"
#include "MuonDet/MuonL1Board.h"
#include "MuonDet/MuonStationCabling.h"
#include "MuonDet/MuonODEBoard.h"
#include "MuonDet/MuonTSMap.h"
#include "Event/RawBank.h"
#include "Event/MuonBankVersion.h"
#include "MuonRawBuffer.h"
#include "GaudiKernel/IIncidentSvc.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MuonRawBuffer
//
// 2005-10-18 : Alessia Satta
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( MuonRawBuffer )

using namespace LHCb;

namespace {
  constexpr unsigned int c_NLink = 24;
  constexpr unsigned int c_ODEWord = 35;


  class TESOffsetGuard final {
    GaudiTool*    m_parent;
    std::string m_original;
    bool        m_active = true;
    int&        m_parentScope; // check whether we are contained with the scope of another guard
                               // if so, don't do anything!

  public:
    // we require parentScope to support recursive guards, where only the outermost
    // guard actually does something. If this is not needed, the code could be simplified
    // significantly!
    TESOffsetGuard( GaudiTool& parent, int& parentScope,  const std::string& offset)
    : m_parent(&parent), m_parentScope(parentScope)
    {
      if (++m_parentScope!=1) return; // not the outermost guard
      if( UNLIKELY( m_parent->msgLevel(MSG::DEBUG) ) ) m_parent->debug()<<" changing rootInTES "<<endmsg;
      m_original=m_parent->rootInTES();
      StatusCode sc = m_parent->setProperty( "RootInTES", offset );
      if(sc.isFailure()) {
          m_parent->warning()<<" unable to set property "<<endmsg;
          --m_parentScope;
          m_active = false;
      }
    }
    ~TESOffsetGuard() {
      if (!m_active || --m_parentScope!=0 ) return; // not active, or not the outermost guard
      StatusCode sc = m_parent->setProperty( "RootInTES", m_original );
      if(sc.isFailure()) m_parent->warning()<<" unable to set property "<<endmsg;
    }
  };

}
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonRawBuffer::MuonRawBuffer( const std::string& type,
                              const std::string& name,
                              const IInterface* parent )
  : base_class( type, name , parent )
{
  //new for decoders, initialize search path, and then call the base method
  m_rawEventLocations = {LHCb::RawEventLocation::Muon, LHCb::RawEventLocation::Default};
  initRawEventSearch();
}

//=============================================================================

StatusCode MuonRawBuffer::initialize()
{
  StatusCode sc = Decoder::ToolBase::initialize() ;
  if(sc.isFailure())return sc;

  m_muonDet=getDet<DeMuonDetector>(DeMuonLocation::Default);
  if(!m_muonDet) return Error("Could not locate Muon detector element");
  m_M1Tell1=(m_muonDet->getDAQInfo())->M1TellNumber();
  incSvc()->addListener( this, IncidentType::BeginEvent );
  m_counter_invalid_hit.fill(0);
  m_processed_bank.fill(0);
  m_NZScounter_invalid_hit.fill(0);
  m_NZSprocessed_bank.fill(0);
  m_pad_checkSize.fill(0);
  m_hit_checkSize.fill(0);
  m_tell1_header_error.fill(0);
  m_tell1_header_ORODE_error.fill(0);
  m_tell1_header_SYNCH_data_error.fill(0);
  m_tell1_header_SYNCH_BC_error.fill(0);
  m_tell1_header_SYNCH_Evt_error.fill(0);

  clearData();

  return StatusCode::SUCCESS ;
}


void MuonRawBuffer::handle ( const Incident& incident )
{
  if ( IncidentType::BeginEvent == incident.type() ) clearData() ;
}


void MuonRawBuffer::clearData(){
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug()<<" reset all buffers "<<endmsg;
  m_checkTell1HeaderPerformed=false;
  m_checkTell1HeaderResult=true;
  m_statusCreated=false;

  m_alreadyDecoded.reset();
  m_padAlreadyDecoded.reset();
  m_ODEAlreadyDecoded.reset();
  m_already_decoded_headerTell1.reset();
  m_hitNumInLink.fill(0);
  m_eventHeader.fill(0);
  for (unsigned int i=0;i<MuonDAQHelper_maxTell1Number;i++){
    m_storage[i].clear();
    m_padStorage[i].clear();
  }
  for (unsigned int i=0;i<MuonDAQHelper_maxODENumber;i++){
    m_ODEData[i].resetODEData();
  }
}


StatusCode MuonRawBuffer::finalize() {

  auto any_error = [](const auto& container)
                   { return std::any_of(begin(container),end(container),
                                        [](const auto& i) { return i!=0; }); };
  bool print_ZS= any_error(m_counter_invalid_hit) ||
                 any_error(m_pad_checkSize)       ||
                 any_error(m_hit_checkSize)       ;

  bool print_NZS= any_error(m_NZScounter_invalid_hit);

  bool print_errors = any_error( m_tell1_header_error ) ||
                      any_error( m_tell1_header_ORODE_error ) ||
                      any_error( m_tell1_header_SYNCH_data_error ) ||
                      any_error( m_tell1_header_SYNCH_BC_error ) ||
                      any_error( m_tell1_header_SYNCH_Evt_error );

  if(print_ZS||print_NZS||print_errors){
    info()<<" Summary of Muon banks decoding errors "<<endmsg;
  }

  if(print_ZS)
  {
    info()<<" Zero suppressed part "<<endmsg;
    for (unsigned int i=0;i<MuonDAQHelper_maxTell1Number;i++){
      if(  m_counter_invalid_hit[i]>0){

        info()<<" in Tell1 " <<i<<" invalid hit address "<<
          m_counter_invalid_hit[i]<<" processed banks # "<<
          m_processed_bank[i]<<endmsg;
      }
      if(  m_pad_checkSize[i]>0){
        info()<<" in Tell1 pad decoding " <<i<<" invalid bank size "<<
          m_pad_checkSize[i]<<" processed banks # "<<
          m_processed_bank[i]<<endmsg;
      }
      if(  m_hit_checkSize[i]>0){
        info()<<" in Tell1 hit decoding " <<i<<" invalid bank size "<<
          m_hit_checkSize[i]<<" processed banks # "<<
          m_processed_bank[i]<<endmsg;
      }
    }
  }

  if(print_errors){
    for (unsigned int i=0;i<MuonDAQHelper_maxTell1Number;i++){
      if(m_tell1_header_error[i]>0){
//commented out waiting for a proper Tell1 firmware 2011-04-24
//        info()<<" in Tell1 "<<i<<" error inside Tell1 "<<m_tell1_header_error[i]<<endmsg;

      }

      if(m_tell1_header_ORODE_error[i]>0)
      {
        info()<<" in Tell1 "<<i<<" ODE reported error "<<m_tell1_header_ORODE_error[i]<<endmsg;
      }

//commented out waiting for a proper Tell1 firmware 2011-04-24
      if( m_tell1_header_SYNCH_data_error[i]>0){
        //info()<<" in Tell1 "<<i<<" SYNCH data consistency  error "
        //       <<m_tell1_header_SYNCH_data_error[i]<<endmsg;
      }

      if(m_tell1_header_SYNCH_BC_error[i]>0){
        info()<<" in Tell1 "<<i<<" SYNCH BC consistency  error "
              <<m_tell1_header_SYNCH_BC_error[i]<<endmsg;
      }
      if(m_tell1_header_SYNCH_Evt_error[i]>0){

        info()<<" in Tell1 "<<i<<" SYNCH L0Evt consistency  error "
              <<m_tell1_header_SYNCH_BC_error[i]<<endmsg;
      }

    }

  }



  if(print_NZS){

    info()<<" Non Zero suppressed part "<<endmsg;
    for (unsigned int i=0;i<MuonDAQHelper_maxTell1Number;i++){
      info()<<" in Tell1 " <<i<<" invalid hit address "<<
        m_NZScounter_invalid_hit[i]<<" processed banks # "<<
        m_NZSprocessed_bank[i]<<endmsg;

    }
  }

  clearData();
  return Decoder::ToolBase::finalize() ;
}

StatusCode MuonRawBuffer::getTile(std::vector<LHCb::MuonTileID>& storage)
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  return raw ? getTile(raw,storage,rootInTES())
             : Error("Failed to find raw data");
}

StatusCode MuonRawBuffer::getTileAndTDC(std::vector<std::pair<LHCb::MuonTileID,unsigned int>>& storage)
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  return raw ? getTileAndTDC(raw,storage,rootInTES())
             : Error("Failed to find raw data");
}

StatusCode  MuonRawBuffer::decodeTileAndTDCDC06(const RawBank* rawdata){

  if( RawBank::MagicPattern != rawdata->magic() ) {
    error()<<"magic pattern not correct in muon bank "<<endmsg;
    return StatusCode::FAILURE;
  }

  const unsigned char* it=rawdata->begin<unsigned char>();
  short skip=0;

  unsigned int tell1Number=rawdata->sourceID();
  if(tell1Number>=m_M1Tell1){
    //how many pads ?
    short nPads=*rawdata->begin<short>();
    skip=(nPads+2)/2 ;
  }
  std::advance(it,4*skip);

  // how many ODE in this tell1?

  unsigned int nODE=(m_muonDet->getDAQInfo())->ODEInTell1((rawdata)->sourceID());
  std::vector<unsigned int> firedInODE;
  firedInODE.resize(nODE) ;
  unsigned int itODE=0;
  unsigned int channelsInTell1=0;
  for(itODE=0; itODE < nODE ; itODE++) {
    //first decode the header
    firedInODE[itODE]=*it;
    channelsInTell1 += *it;
    it++;
  }
  for(itODE=0;itODE<nODE;itODE++){
    unsigned int channels=0;
    unsigned int odenumber=(m_muonDet->getDAQInfo())->
      getODENumberInTell1((rawdata)->sourceID(),itODE);
    for(channels=0;channels<firedInODE[itODE];channels++){
      unsigned int address = *it;
      MuonTileID tile=(m_muonDet->getDAQInfo())->
        getADDInODENoHole(odenumber-1,address);
      std::pair<MuonTileID,unsigned int> tileAndTDC;
      tileAndTDC.first=tile;
      m_storage[tell1Number].push_back(tileAndTDC);
      it++;
    }
  }

  //then decode the TDC info

  //first skip the byte required for padding
  //how many?
  if((channelsInTell1+nODE)%4) {
    std::advance( it, 4-(channelsInTell1+nODE)%4 );
  }

  unsigned int TDCword = (channelsInTell1+1)/2;
  unsigned int countChannels=0;
  unsigned int chIterator=0;
  for(unsigned int ch=0;ch<TDCword;ch++){
    unsigned int time1 = (*it)&((unsigned char)15 );
    unsigned int time2 = ((*it)>>4)&((unsigned char) 15);

    m_storage[tell1Number][chIterator].second=time1;
    chIterator++;

    if(countChannels+1<channelsInTell1) {
      m_storage[tell1Number][chIterator].second=time2;
      chIterator++;
    }
    countChannels=countChannels+2;
    it++;

  }
  return StatusCode::SUCCESS;
}



StatusCode  MuonRawBuffer::decodeTileAndTDCV1(const RawBank* rawdata){


  StatusCode sc=checkBankSize(rawdata);
  unsigned int tell1Number=(rawdata)->sourceID();
  if(sc.isFailure()){
    m_hit_checkSize[tell1Number]++;

    return StatusCode::SUCCESS;
  }

  bool print_bank=false;
  bool muon_spec_header=true;

  //printout
  if(print_bank) {
    info()<<"Tell1 "<<tell1Number<<" "<<"bank length "<<rawdata->size()<<endmsg;
    std::for_each( rawdata->begin<unsigned short>(), rawdata->end<unsigned short>(),
                   [&,count_word=0](unsigned short i) mutable {
      info()<<count_word++ <<' '<<(i&(0xFFF))<<' '<<((i>>12)&(0xF))<<endmsg;
    } );
  }
  const unsigned short* it=rawdata->begin<unsigned short>();
  short skip=0;

  m_processed_bank[tell1Number]++;

  if(muon_spec_header) {
    //how many pads ?
    unsigned  short nPads=*it;
    skip=(nPads+3)/2;
    for(int k=0;k<2*skip;k++){
      if (k==1) fillTell1Header(tell1Number,*it);
      it++;
    }
  }

  std::array<unsigned int,24> hit_link_cnt = { { 0 } };

  for(int i=0;i<4;i++){
    //now go to the single pp counter
    unsigned int pp_cnt=*it++;
    m_hitNumInPP[tell1Number*4+i]=pp_cnt;

    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" hit in PP "<<pp_cnt<<endmsg;

    for (unsigned int loop=0;loop<pp_cnt;++loop) {
      unsigned int add=(*it)&(0x0FFF);
      unsigned int tdc_value=(((*it)&(0xF000))>>12);
      ++it;
      MuonTileID tile=m_muonDet->getDAQInfo()->getADDInTell1(tell1Number,add);
      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
        verbose()<<" add "<<add<<' '<<tile <<endmsg;
      if(tile.isValid()) {
        if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
          debug()<<" valid  add "<<add<<' '<<tile <<endmsg;
        m_storage[tell1Number].emplace_back(tile, tdc_value);
      } else {
        m_counter_invalid_hit[tell1Number]++;
      }
      //update the hitillink counter
      unsigned int link=add/192;
      hit_link_cnt[link]++;
    }
  }
  int active_link_counter=0;
  for(int i=0;i<24;++i){
    if(m_muonDet->getDAQInfo()->getODENumberInLink(tell1Number,i)==0){
      m_hitNumInLink[tell1Number*24+i] = 0;
    }else{
      m_hitNumInLink[tell1Number*24+i] = hit_link_cnt[active_link_counter];
      ++active_link_counter;
    }
  }
  return StatusCode::SUCCESS;
}


StatusCode MuonRawBuffer::getPads(std::vector<LHCb::MuonTileID>& storage)
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  if( !raw ) return Error("Failed to find raw data");

  storage.clear();

  return getPads(raw,storage,rootInTES());

}


StatusCode MuonRawBuffer::decodePadsV1(const LHCb::RawBank* r)
{
  unsigned int tell = r->sourceID();
  StatusCode sc=checkBankSize(r);
  if (sc.isFailure()) {
    m_pad_checkSize[tell]++;
    return StatusCode::SUCCESS;;
  }

  if((unsigned int)r->sourceID()>=m_M1Tell1){
    const auto* di = m_muonDet->getDAQInfo();
    const short* first=r->begin<short>();
    unsigned int npads=*first++;
    m_eventHeader[tell]=*first++;
    std::transform(first,first+npads,
                   std::back_inserter(m_padStorage[tell]),
                   [&](short addr) -> MuonTileID
                   { return di->getPadInTell1V1(tell, addr); } );
  }
  return StatusCode::SUCCESS;
}

StatusCode MuonRawBuffer::decodePadsDC06(const LHCb::RawBank* r)
{
  if( RawBank::MagicPattern != r->magic() ) {
    error()<<"magic pattern not correct in muon bank "<<endmsg;
    return StatusCode::FAILURE;
  }
  unsigned int tell=r->sourceID();
  if(tell>=m_M1Tell1){
    const auto* di = m_muonDet->getDAQInfo();
    const short* first=r->begin<short>();
    unsigned int npads=*first++;
    std::transform( first, first+npads,
                    std::back_inserter(m_padStorage[tell]),
                    [&](const short s) -> MuonTileID
                    { return di->getPadInTell1DC06(tell, s); } );
  }
  return StatusCode::SUCCESS;
}

StatusCode MuonRawBuffer::getTile(const LHCb::RawBank* r,std::vector<LHCb::MuonTileID>& tile)
{
  tile.clear();

  unsigned int tell1Number=(r)->sourceID();
  StatusCode sc=DecodeData(r);
  if(sc.isFailure()) return StatusCode::SUCCESS;

  //copy in output container
  tile.reserve(m_storage[tell1Number].size());
  std::transform(m_storage[tell1Number].begin(),m_storage[tell1Number].end(),
                 std::back_inserter(tile),
                 [](const std::pair<LHCb::MuonTileID,unsigned int>& p )
                 { return p.first; } );
  return StatusCode::SUCCESS;
}

StatusCode MuonRawBuffer::getTile( const LHCb::RawEvent* raw,std::vector<LHCb::MuonTileID>& storage,const std::string& offset)
{
  TESOffsetGuard guard{*this,m_TESChanged,offset};
  initStatus();

  StatusCode sc=checkAllHeaders(raw);

  storage.clear();
  if(!sc.isFailure()){

    const auto& b = raw->banks(RawBank::Muon);
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" tell1 "<<b.size()<<endmsg;

    //first decode data and insert in buffer
    for(const auto & r : b ) {
      StatusCode sc=DecodeData(r);
      if(sc.isFailure())return sc;
    }
    //compact storage in one container
    for(const auto&  bank : b ) {
      unsigned int tell1Number=bank->sourceID();
      std::transform( m_storage[tell1Number].begin(),
                      m_storage[tell1Number].end(),
                      std::back_inserter(storage),
                      [](const std::pair<LHCb::MuonTileID,unsigned int>& p)
                      { return p.first; } );
    }
  }
  putStatusOnTES();
  return StatusCode::SUCCESS;
}


StatusCode MuonRawBuffer::getTileAndTDC(const LHCb::RawEvent* raw,
                                        std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & storage,
                                        const std::string& offset)
{
  TESOffsetGuard guard{ *this,m_TESChanged, offset};
  initStatus();

  StatusCode sc=checkAllHeaders(raw);
  if(!sc.isFailure()){

    storage.clear();
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" start the real decoding "<<endmsg;

    const auto& mb = raw->banks(RawBank::Muon);

    //first decode data and insert in buffer
    for( const auto& r : mb ) {
      StatusCode sc=DecodeData(r);
      if(sc.isFailure())return sc;
    }
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" the decoding is finished "<<endmsg;
    //compact data  in one container

    for( const auto& r : mb ) {
      unsigned int tell1Number=r->sourceID();
      std::copy(m_storage[tell1Number].begin(),
                m_storage[tell1Number].end(),
                std::back_inserter(storage) );
    }
  }

  putStatusOnTES();
  return StatusCode::SUCCESS;
}


StatusCode MuonRawBuffer::getTileAndTDC(const LHCb::RawBank* r,std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tile)

{
  tile.clear();

  StatusCode sc=DecodeData(r);
  if(sc.isFailure()) return StatusCode::SUCCESS;

  unsigned int tell1Number=r->sourceID();

  //copy in output container
  tile.reserve(m_storage[tell1Number].size());
  std::copy( m_storage[tell1Number].begin(),
             m_storage[tell1Number].end(),
             std::back_inserter(tile) );
  return StatusCode::SUCCESS;

}


StatusCode MuonRawBuffer::getPads(const LHCb::RawEvent* raw,std::vector<LHCb::MuonTileID> & pads, const std::string& offset)
{
  TESOffsetGuard{ *this, m_TESChanged, offset };
  initStatus();

  StatusCode sc=checkAllHeaders(raw);
  pads.clear();
  if(!sc.isFailure()){
    pads.clear();

    const auto& b = raw->banks(RawBank::Muon);

    //first decode data and insert in buffer
    for(const auto& rb : b ) {
      StatusCode sc=DecodeDataPad(rb);
      if(sc.isFailure())return sc;
    }

    //compact data  in one container
    for(const auto& rb : b ) {
      unsigned int tell1Number=rb->sourceID();
      std::copy( m_padStorage[tell1Number].begin(),
                 m_padStorage[tell1Number].end(),
                 std::back_inserter(pads) );
    }
  }

  putStatusOnTES();
  return StatusCode::SUCCESS;
}





StatusCode MuonRawBuffer::getPads(const LHCb::RawBank* r,std::vector<LHCb::MuonTileID>& pads)
{
  pads.clear();
  StatusCode sc=DecodeDataPad(r);
  if(sc.isFailure())return sc;

  unsigned int tell=r->sourceID();
  std::copy(m_padStorage[tell].begin(),m_padStorage[tell].end(),
            std::back_inserter(pads));
  return StatusCode::SUCCESS;
}



StatusCode MuonRawBuffer::DecodeData(const LHCb::RawBank* r)
{
  StatusCode sc=StatusCode::FAILURE;
  unsigned int tell1Number=(r)->sourceID();
  if(tell1Number>=MuonDAQHelper_maxTell1Number){
    error()<<" in muon data a Tell1 Source ID is gretare than maximum "<<endmsg;
    return StatusCode::FAILURE;
  }

  if(!m_alreadyDecoded[tell1Number]){
    switch (r->version()) {
        case MuonBankVersion::DC06: sc=decodeTileAndTDCDC06(r); break;
        case MuonBankVersion::v1: sc=decodeTileAndTDCV1(r); break;
        default: sc = StatusCode::FAILURE; break;
    }
    m_alreadyDecoded[tell1Number]=true;
    if(sc.isFailure())return sc;
  }
  return StatusCode::SUCCESS;
}

StatusCode MuonRawBuffer::DecodeDataPad(const LHCb::RawBank* r)
{
  StatusCode sc=StatusCode::FAILURE;
  sc.ignore();
  unsigned int tell=r->sourceID();
  if(tell>=MuonDAQHelper_maxTell1Number){
    error()<<" in raw data there is a muon bank with source ID greater than maximum "<<endmsg;
    return StatusCode::FAILURE;
  }

  //  debug()<<tell<<" "<<m_padAlreadyDecoded[tell]<<endmsg;
  if((unsigned int)r->sourceID()>=m_M1Tell1){
    if(!m_padAlreadyDecoded[tell]){
      switch(r->version()) {
          case MuonBankVersion::DC06: sc=decodePadsDC06(r); break;
          case MuonBankVersion::v1: sc=decodePadsV1(r); break;
          default: sc = StatusCode::FAILURE; break;
      }
      m_padAlreadyDecoded[tell]=true;
      if(sc.isFailure())return sc;
    }
  } else m_padAlreadyDecoded[tell]=true;
  return StatusCode::SUCCESS;
}


StatusCode MuonRawBuffer::decodeNZSupp(int tell1Number)
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  if( !raw ) return Error("Failed to find raw data");

  const auto& b = raw->banks(RawBank::MuonFull);
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" tell1 "<<b.size()<<endmsg;
  if( b.empty() ) {
    info()<<" you have request NZS bank for muon detectorct ---> "<<
      "such bank are not presente in input file "<<endmsg;
  }
  auto i = std::find_if(b.begin(),b.end(),
                        [&](const RawBank* rb)
                        { return rb->sourceID() == tell1Number; } );
  return i!=b.end() ? decodeNZSupp(*i) : StatusCode::SUCCESS;
}

StatusCode  MuonRawBuffer::decodeNZSupp(const LHCb::RawBank* r){
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" start decoding "<<endmsg;
  unsigned int tell1Num=(r)->sourceID();
  if( RawBank::MagicPattern != r->magic() ) {
    error()<<"magic pattern not correct in muon bank "<<endmsg;
    return StatusCode::FAILURE;
  }
  if(tell1Num>=MuonDAQHelper_maxTell1Number) {
    return StatusCode::FAILURE;
  }

  std::array< std::array<unsigned int,35>, 24 > data; const unsigned int * it=r->begin<unsigned int>();

  //loop on PP
  for (int pp_num=0;pp_num<4;pp_num++){
    unsigned int off=pp_num*6;
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" pp 0 "<<pp_num<<endmsg;

    //skip first 6 words ... always set to zero
    std::advance(it,6);

    //now the  data
    for(unsigned int i=0;i<c_ODEWord*6;i++){
      unsigned int data1 = (*it)&0xFFFF;
      unsigned int data2 = ((*it)>>16)&0xFFFF;
      unsigned int ODE=i%3;
      unsigned int wordNum=i/6;
      //first or second data part?
      if(i%6<3){
        data[off+ODE*2  ][wordNum]=data1&0xFFFF;
        data[off+ODE*2+1][wordNum]=data2&0xFFFF;
      }else{
        data[off+ODE*2  ][wordNum]|=(data1<<16)&0xFFFF0000;
        data[off+ODE*2+1][wordNum]|=(data2<<16)&0xFFFF0000;
      }
      ++it;
    }

    //now the trailer
    for(int i=0;i<8;i++){
      m_PPEventInfo[tell1Num*4+pp_num].setWord(i,*it);
      it++;
    }

  }
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" now fill the buffer "<<endmsg;
  //now copy the data to the buffer
  for(int i=0;i<24;i++){
    m_ODEData[tell1Num*24+i].setData(data[i]);
  }
  return StatusCode::SUCCESS;
}

StatusCode MuonRawBuffer::dumpNZSupp(const LHCb::RawBank* r,unsigned int ode_num)
{
  unsigned int tell1Number=(r)->sourceID();
  if(tell1Number>=MuonDAQHelper_maxTell1Number)return StatusCode::FAILURE;
  if(!m_ODEAlreadyDecoded[tell1Number]) decodeNZSupp( r);
  for(int iLink=0;iLink<24;iLink++) {
    unsigned int ODE_num= m_muonDet->getDAQInfo()->getODENumberInLink(tell1Number,iLink);
    if(ODE_num==ode_num) {
      info()<<" ODE words for link # "<<iLink<<endmsg;
      for( unsigned int w=0;w<c_ODEWord;w++){
        info()<<(m_ODEData[tell1Number*24+iLink]).getWord(w)<<endmsg;
      }
    }
  }
  return StatusCode::SUCCESS;
}


StatusCode MuonRawBuffer::getNZSupp(const LHCb::RawBank* r,std::vector<std::pair<LHCb::MuonTileID,unsigned int> > & tileAndTDC)
{
  unsigned int tell1Number=(r)->sourceID();
  m_NZSprocessed_bank[tell1Number]++;
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug()<<" t1 "<<tell1Number<<endmsg;
  int fix_size=(c_NLink*36+8*4)*4;
   if(r->size()!=fix_size)return StatusCode::FAILURE;

   if(!m_ODEAlreadyDecoded[tell1Number]) decodeNZSupp( r);

   //now copy the ODE info in a more suitable format
   //first loop on ODE
   for (unsigned int iLink=0;iLink<c_NLink;iLink++){

     unsigned int ODE_num=(m_muonDet->getDAQInfo())
       ->getODENumberInLink(tell1Number,iLink);
     if(ODE_num>0){
       if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug()<<" ODE "<<ODE_num<<endmsg;

       unsigned int ch=0;
       for(unsigned int iData=1;iData<c_NLink+1;iData++){
         for (int iTDC=0;iTDC<8;iTDC++){
           unsigned int TDC_value=(m_ODEData[c_NLink*tell1Number+iLink]).
             getTDC(iData,iTDC);
           if(TDC_value>0){
             MuonTileID tile=(m_muonDet->getDAQInfo())->
               getADDInODE(ODE_num-1, ch);
             if(tile.isValid()){
               tileAndTDC.emplace_back( tile, TDC_value);
             }else{
               m_NZScounter_invalid_hit[tell1Number]++;
             }
           }
           ch++;
         }
       }
     }
   }
   return StatusCode::SUCCESS;

}

MuonPPEventInfo MuonRawBuffer::getPPInfo(const LHCb::RawBank* r,unsigned int pp_num){

  unsigned int tell1Number=r->sourceID();
  if(!m_ODEAlreadyDecoded[tell1Number]){
    StatusCode sc= decodeNZSupp( r);
    if( sc.isFailure() && UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug()<<"error in decoding NZSP bank"<<endmsg;
  }
  return m_PPEventInfo[tell1Number*4+pp_num];

}

MuonPPEventInfo MuonRawBuffer::getPPInfo(unsigned int tell1Number,unsigned int pp_num){
  if(!m_ODEAlreadyDecoded[tell1Number]){
    //serach for bank and decode it
    StatusCode sc= decodeNZSupp( tell1Number);
    if( sc.isFailure() && UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug()<<"error in decoding NZSP bank"<<endmsg;
  }
  return m_PPEventInfo[tell1Number*4+pp_num];
}

MuonODEData MuonRawBuffer::getODEData(const LHCb::RawBank* r,
                                      unsigned int link){
  if(link>23) return {};
  unsigned int tell1Number=(r)->sourceID();
  if(!m_ODEAlreadyDecoded[tell1Number]) decodeNZSupp( r);
  return (m_ODEData[tell1Number*24+link]);
}



unsigned int  MuonRawBuffer::BXCounter(unsigned int tell1Number)
{
  if( m_alreadyDecoded[ tell1Number]){
    return m_eventHeader[tell1Number]&(0xFF);
  }else{
    info()<<" the bank has not been decode yet "<<endmsg;
    return 0;
  }
}

StatusCode MuonRawBuffer::getNZSupp(std::vector<std::pair<LHCb::MuonTileID,
                                    unsigned int> > & tileAndTDC)
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  return raw ? getNZSupp(raw,tileAndTDC,rootInTES())
             : Error("Failed to find raw data");
}


StatusCode MuonRawBuffer::getNZSupp( const LHCb::RawEvent* raw,
                                     std::vector<std::pair<LHCb::MuonTileID,
                                     unsigned int> > & tileAndTDC,
                                     const std::string& offset)
{
  TESOffsetGuard guard{ *this, m_TESChanged, offset };

  initStatusNZS();

  const auto& b = raw->banks(RawBank::MuonFull);
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" tell1 "<<b.size()<<endmsg;
  if( b.empty()) {
    info()<<" you have request NZS bank for muon detectorct ---> "<<
      "such bank ar enot presente in input file "<<endmsg;
  }

  for(const auto& mb : b ) {
    std::vector<std::pair<LHCb::MuonTileID,unsigned int> > bank_container;
    StatusCode sc=getNZSupp(mb,bank_container);
    std::copy(bank_container.begin(),bank_container.end(),
              std::back_inserter(tileAndTDC));
  }
  putStatusOnTES();
  return StatusCode::SUCCESS;
}


StatusCode MuonRawBuffer::checkBankSize(const LHCb::RawBank* rawdata)
{
  if( RawBank::MagicPattern != rawdata->magic() ) {
    error()<<"magic pattern not correct in muon bank "<<endmsg;
    return StatusCode::FAILURE;
  }

  const unsigned short * it=rawdata->begin<unsigned short>();
  unsigned int tell1Number=(rawdata)->sourceID();
  int bank_size=rawdata->size();
  int read_data=0;
  //minimum length is 3 words --> 12 bytes
  if(bank_size<12){
    err()<< " muon bank "<<tell1Number<<" is too short "<<
      bank_size<<endmsg;
    m_status.addStatus(tell1Number,RawBankReadoutStatus::Status::Incomplete);
    return StatusCode::FAILURE;
  }
  //how many pads ?
  //const unsigned short * itPad=rawdata->begin<unsigned short>();
  unsigned short nPads=*it;
  //  if ( msgLevel(MSG::VERBOSE) ){
  //    verbose()<< " pad # "<<nPads<<endmsg;
  //  }

  int skip=(nPads+3)/2;

  if((bank_size-skip*4)<0){
    err()<<"bank_size "<<bank_size<<" pad size to read "<<nPads*4<<endmsg;
    err()<< "so muon bank "<<tell1Number<<" is too short in pad part "<<endmsg;
    m_status.addStatus(tell1Number,RawBankReadoutStatus::Status::Incomplete);
    return StatusCode::FAILURE;
  }

  it += 2*skip;
  read_data=read_data+skip*2;
  if(read_data<0)info()<<nPads<<" "<<skip<<" "<<
                   bank_size<<" "<<read_data<<endmsg;

  for(int i=0;i<4;i++){
    //now go to the single pp counter
    int pp_cnt=*it++;
    read_data++;

    //check size before start looping
    if(bank_size-read_data*2<pp_cnt*2){
      err()<<"bank_size "<<bank_size<<"read data "<<read_data<<" hit size to read "<<pp_cnt*2<<endmsg;
      err()<< "so muon bank "<<tell1Number<<" is too short in hit part "<<endmsg;
      m_status.addStatus(tell1Number,RawBankReadoutStatus::Status::Incomplete);
      //break;

      return StatusCode::FAILURE;
    }
    it+=pp_cnt;
  }

  return StatusCode::SUCCESS;

}


bool MuonRawBuffer::PPReachedHitLimit(unsigned int Tell1Number,int pp_num)
{
  if(!m_alreadyDecoded[Tell1Number]){
    std::vector<LHCb::MuonTileID> tile;
    StatusCode sc=getTile(tile);
    if(sc.isFailure())return true;
  }
  return m_hitNumInPP[Tell1Number*4+pp_num]>262;
}



bool MuonRawBuffer::LinkReachedHitLimit(unsigned int Tell1Number,int link_num)
{
  if(!m_alreadyDecoded[Tell1Number]){
    std::vector<LHCb::MuonTileID> tile;
    StatusCode sc=getTile(tile);
    if(sc.isFailure())return true;
  }
  return m_hitNumInLink[Tell1Number*24+link_num]>62;
}


StatusCode MuonRawBuffer::getPadsInStation(int station,std::vector<std::vector<LHCb::MuonTileID>* > & pads)
{
  for(const auto& tell1 : m_muonDet->getDAQInfo()->getTell1InStation(station)) {
    StatusCode sc=getPads(tell1);
    if(sc.isFailure()) return sc;
    pads.push_back(&m_padStorage[tell1]);
  }
  return StatusCode::SUCCESS;
}


StatusCode MuonRawBuffer::getPads( int tell1,std::vector<LHCb::MuonTileID>& pads)
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  if( !raw ) return Error("Failed to find raw data");

  pads.clear();
 //first decode data and insert in buffer
  for(const auto& r : raw->banks(RawBank::Muon) ) {
    if( r->sourceID()==tell1){
      StatusCode sc=DecodeDataPad(r);
      if(sc.isFailure())return sc;
      pads=m_padStorage[tell1];
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode MuonRawBuffer::getPads( int tell1)
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  if( !raw ) return Error("Failed to find raw data");

 //first decode data and insert in buffer
  for(const auto& r : raw->banks(RawBank::Muon) ) {
    if( r->sourceID()==tell1){
      StatusCode sc=DecodeDataPad(r);
      if(sc.isFailure())return sc;
    }
  }
  return StatusCode::SUCCESS;
}

MuonTell1Header MuonRawBuffer::getHeader(const LHCb::RawBank* r)
{
  StatusCode sc=DecodeData(r);
  if(sc.isFailure()) return  { };
  return MuonTell1Header(m_eventHeader[r->sourceID()]);
}

std::vector<std::pair<MuonTell1Header, unsigned int> > MuonRawBuffer::getHeaders(LHCb::RawEvent* raw)
{
  std::vector<std::pair<MuonTell1Header, unsigned int> > ret;
  const auto& b = raw->banks(RawBank::Muon);
  //first decode data and insert in buffer
  ret.reserve(b.size());
  std::transform(b.begin(),b.end(),
                 std::back_inserter(ret),
                 [&](const RawBank* rb) {
                     return std::make_pair(getHeader(rb),rb->sourceID());
  });
  return  ret;
}

std::vector<std::pair<MuonTell1Header, unsigned int> > MuonRawBuffer::getHeaders()
{
  LHCb::RawEvent* raw = findFirstRawEvent();
  if (raw) return getHeaders(raw);
  Error("Failed to find raw data").ignore();
  return { };
}


StatusCode MuonRawBuffer::checkAllHeaders(const LHCb::RawEvent* raw)
{
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" check headers consistency "<<endmsg;

  if( m_checkTell1HeaderPerformed ) return StatusCode{m_checkTell1HeaderResult};
  m_checkTell1HeaderPerformed=true;
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" check headers consistency not yet done"<<endmsg;


  const auto& b = raw->banks(RawBank::Muon);

  if(b.empty()){
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" no muon banks in event"<<endmsg;
    for(int i=0;i<  static_cast<int>(MuonDAQHelper_maxTell1Number);i++){
      m_status.addStatus( i,RawBankReadoutStatus::Status::Missing);
    }
    m_checkTell1HeaderResult=false;
    return StatusCode::SUCCESS;
  }
  //first decode data and insert in buffer
  bool foundError=false;

  std::vector<unsigned int> tell1InEvent;
  for(const auto& mb : b) {

    unsigned int tell1Number=mb->sourceID();
    m_status.addStatus( tell1Number,RawBankReadoutStatus::Status::OK);

    if (mb->size()==0)  m_status.addStatus( tell1Number,RawBankReadoutStatus::Status::Empty);
    auto iList=std::find(tell1InEvent.begin(), tell1InEvent.end(),tell1Number );
    if(iList!=tell1InEvent.end()){
      m_checkTell1HeaderResult=false;
      m_checkTell1HeaderPerformed=true;
      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) ) verbose()<<" failed "<<endmsg;
      foundError=true;
      m_status.addStatus( tell1Number,RawBankReadoutStatus::Status::NonUnique);
      break;
      //return StatusCode::FAILURE;
    }
    tell1InEvent.push_back(tell1Number);
  }

  //set missing bank readout status
  for(int i=0;i< static_cast<int>( MuonDAQHelper_maxTell1Number);i++){
    if(m_status.status(i)==LHCb::RawBankReadoutStatus::Status::Unknown){
      m_status.addStatus( i,RawBankReadoutStatus::Status::Missing);
    }
  }
  if(foundError){
    m_checkTell1HeaderResult=false;
    return StatusCode::FAILURE;
  }

  //there is repeated Tell1
  //now check the fw version

  //compact data  in one container
  auto ibad = std::find_if( b.begin(), b.end(),
                   [ReferenceVersion=b[0]->version()](const RawBank* rb)
                   { return rb->version()!=ReferenceVersion; } );
  if (ibad!=b.end()) {
      error()<<
        " The muon Tell1 boards: not all the same version so  skip the event"
             << endmsg;
      m_checkTell1HeaderResult=false;
      m_checkTell1HeaderPerformed=true;
      m_status.addStatus( (*ibad)->sourceID(), RawBankReadoutStatus::Status::Tell1Error );
      return StatusCode::FAILURE; // return m_checkTell1HeaderResult;
  }
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" test successeful "<<endmsg;
  return StatusCode::SUCCESS;
}

void MuonRawBuffer::fillTell1Header(unsigned int tell1Number,unsigned int data)
{
  m_eventHeader[tell1Number]=data;
  if(m_already_decoded_headerTell1[tell1Number])return;

  MuonTell1Header dataWord(data);
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<dataWord.getError()<<endmsg;

  if(dataWord.getError())m_tell1_header_error[tell1Number]++;
  if(dataWord.getOROfODEErrors())m_tell1_header_ORODE_error[tell1Number]++;
  if(dataWord.getSYNCHDataErrorInODE())m_tell1_header_SYNCH_data_error[tell1Number]++;
  if(dataWord.getSYNCHBCNCntErrorInODE())m_tell1_header_SYNCH_BC_error[tell1Number]++;
  if(dataWord.getSYNCHEventCntErrorInODE())m_tell1_header_SYNCH_Evt_error[tell1Number]++;

}


void MuonRawBuffer::putStatusOnTES(){
  // Readout Status
  typedef LHCb::RawBankReadoutStatus Status;
  typedef LHCb::RawBankReadoutStatuss Statuss;


  Statuss* statuss = getOrCreate<Statuss,Statuss>( LHCb::RawBankReadoutStatusLocation::Default );
  Status* status = statuss->object ( m_status.key() );

  if( !status ){
    std::stringstream type;
    type << LHCb::RawBank::typeName(m_status.key()) ;
    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose() << "Status for bankType " <<  type.str()  << " created now" << endmsg;
    status = new Status( m_status  );
    statuss->insert( status );

  } else {
    std::stringstream type;
    type << LHCb::RawBank::typeName(m_status.key()) ;

    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "Status for bankType " <<  type.str()  << " already exists" << endmsg;
    if( status->status() != m_status.status() ){
      Warning("Status for bankType " +  type.str() + " already exists  with different status value -> merge both"
              , StatusCode::SUCCESS).ignore();
      for( const auto& i : m_status.statusMap() ) {
        status->addStatus(i.first , i.second);
      }
    }
  }
}

void  MuonRawBuffer::initStatus()
{
  if(!m_statusCreated){
    m_statusCreated=true;
    m_status= LHCb::RawBankReadoutStatus(RawBank::Muon );
  }
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" init "<<m_status.key()<<" "<<m_status<<endmsg;
}

void  MuonRawBuffer::initStatusNZS()
{
    m_statusFull= LHCb::RawBankReadoutStatus(RawBank::MuonFull );
}
