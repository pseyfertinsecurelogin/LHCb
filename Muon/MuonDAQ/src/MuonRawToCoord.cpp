/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files
#include <cstdio>

// local
#include "MuonRawToCoord.h"
using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : MuonRawToCoord
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( MuonRawToCoord )

namespace{
  struct Coord {
      LHCb::MuonTileID m_pad;
      unsigned m_tdc1;
      unsigned m_tdc2;
      const LHCb::MuonTileID& m_tile1;
      const LHCb::MuonTileID& m_tile2;
      Coord(const LHCb::MuonTileID& pad, unsigned tdc1, unsigned tdc2, const LHCb::MuonTileID& tile1, const LHCb::MuonTileID& tile2) :
          m_pad(pad), m_tdc1(tdc1), m_tdc2(tdc2), m_tile1(tile1), m_tile2(tile2) {}
  };
  using Coords = std::vector<Coord>;
}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonRawToCoord::MuonRawToCoord( const std::string& name,
                  ISvcLocator* pSvcLocator)
  : Transformer ( name , pSvcLocator ,
        KeyValue{"RawEventLocation" , LHCb::RawEventLocation::Default},
        KeyValue{"MuonCoordLocation", LHCb::MuonCoordLocation::MuonCoords})
{
  m_forceResetDAQ=(context()=="TAE");
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode MuonRawToCoord::initialize() {
  auto sc = Transformer::initialize();
  if(sc.isFailure()) return sc;

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) 
    debug() << "==> Initialise" << endmsg;
  m_muonDetector=getDet<DeMuonDetector>(DeMuonLocation::Default);
  if(!m_muonDetector)
    return Error("Could not read /dd/Structure/LHCb/DownstreamRegion/Muon from TDS");

  // unnecessary debug
  /* 
  MuonBasicGeometry basegeometry( detSvc(),msgSvc());
  m_NStation= basegeometry.getStations();
  m_NRegion = basegeometry.getRegions(); 
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug()<<" station number "<<m_NStation<<" "<<m_NRegion <<endmsg;
  */
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
LHCb::MuonCoords MuonRawToCoord::operator()(const LHCb::RawEvent &raw) const {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) 
    debug() << "==> Execute" << endmsg;

  // need to loop over input vector of MuonDigits
  // and make output vectors of MuonCoords one for each station
  //Digits decoding;
  //FIXME: should this decoding be used?
  
  std::array<std::vector<std::pair<LHCb::MuonTileID, unsigned int> >, 14>  m_decoding;
  std::vector<std::pair<LHCb::MuonTileID, unsigned int> >  decoding;

  LHCb::MuonCoords coords;

  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" start the real decoding "<<endmsg;

  const auto& mb = raw.banks(RawBank::Muon);
  if (mb.empty()){
    return coords;
  }

  //first decode data 
  for( const auto& r : mb ) {
    unsigned int tell1Number=r->sourceID();
    m_decoding[tell1Number] = decodeTileAndTDCV1(r);
      //TODO handle failure
//      if(sc.isFailure())return storage;
  }
  if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
    verbose()<<" the decoding is finished "<<endmsg;
    //compact data  in one container

  // flatten out the vector of vectors
  // this is quick and dirty
  
  decoding.reserve(m_decoding.size());
  for( const auto& r : mb ) {
    unsigned int tell1Number=r->sourceID();
    std::copy(m_decoding[tell1Number].begin(),
              m_decoding[tell1Number].end(),
              std::back_inserter(decoding) );
  }

  if(decoding.empty()) {
    error() << "Error in decoding the muon raw data ";
    return coords;
  }

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug()<<decoding.size()<<" digits in input "<<endmsg;

  std::sort(decoding.begin(), decoding.end(),
    [] (const Digit& a, const Digit& b) {
    if (a.first.station() < b.first.station()) return true;
      else if (b.first.station() < a.first.station()) return false;
      else return a.first.region() < b.first.region();
  });
  std::array<DigitsRange, 5 * 4> perStaReg;
  perStaReg.fill(boost::make_iterator_range(decoding.end(), decoding.end()));
  unsigned nStaReg = 0;
  
  auto it = decoding.begin();
  for (auto jt = it, kt = decoding.end(); kt != jt; ++jt) {
    if (jt->first.station() != it->first.station() ||
        jt->first.region() != it->first.region()) {
      perStaReg[nStaReg++] = boost::make_iterator_range(it, jt);
      it = jt;
    }
  }
  perStaReg[nStaReg++] = boost::make_iterator_range(it, decoding.end());

  Coords mycoords;
  mycoords.reserve(10000); // FIXME: increase if needed
  
  for (auto& coordsPerStaReg : boost::make_iterator_range(perStaReg.begin(), perStaReg.begin() + nStaReg)) {
    // return coords directly
    addCoordsCrossingMap(coordsPerStaReg, coords);
  }
  //return StatusCode::SUCCESS;
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "coords size = " << coords.size() << endmsg;
  return coords;
}

//=============================================================================

void MuonRawToCoord::addCoordsCrossingMap(DigitsRange& digits, LHCb::MuonCoords& retVal) const {
  // need to calculate the shape of the horizontal and vertical logical strips
  //  if( 2 != m_muonDetector->getLogMapInRegion(station,region) ){
  // log << MSG::ERROR << "There are " << pRegion->numberLogicalMap()
  //     << " logical maps, I expect either 1 or 2" << endmsg;
  //}

  // get local MuonLayouts for strips
  MuonLayout layoutOne, layoutTwo;
  makeStripLayouts(digits.front().first.station(),
                   digits.front().first.region(), 
                   layoutOne, layoutTwo);
  // used flags
  std::vector<bool> used(digits.size(), false);

  // partition into the two directions of digits 
  // vertical and horizontal stripes
  const auto mid = std::partition(digits.begin(), digits.end(),
          [&layoutOne] (const Digit& digit) { 
          return digit.first.layout() == layoutOne; });
  auto digitsOne = boost::make_iterator_range(digits.begin(), mid);
  auto digitsTwo = boost::make_iterator_range(mid, digits.end());

  // check how many cross
  unsigned i = 0;
  retVal.reserve(digitsOne.size() * digitsTwo.size() + digits.size());
  for (const Digit& one: digitsOne) {
    unsigned j = mid - digits.begin();
    for (const Digit& two: digitsTwo) {
      LHCb::MuonTileID pad = one.first.intercept(two.first);
      if (pad.isValid()) {
        auto current = std::make_unique<MuonCoord>();
        current->setUncrossed(false);
        current->setDigitTile({one.first, two.first});
        current->setDigitTDC1(one.second);
        current->setDigitTDC2(two.second);
        retVal.insert(current.get(), pad);
        current.release();
        // set used flag
        used[i] = used[j] = true;
      }
    ++j;
    }
  ++i;
  }

  // copy over "uncrossed" digits

  unsigned m = 0;
  for (const Digit& digit: digits) {
    if (!used[m]) {
      LHCb::MuonTileID pad = digit.first;
      auto current = std::make_unique<MuonCoord>();
      current->setUncrossed(true);
      current->setDigitTile({digit.first});
      current->setDigitTDC1(digit.second);
      retVal.insert(current.get(), pad);
      current.release();
    }
    ++m;
  }
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "retVal size = " << retVal.size() << endmsg;

}

void MuonRawToCoord::makeStripLayouts(unsigned int station, unsigned int region,
                                     MuonLayout &layout1,
                                     MuonLayout &layout2) const {

  unsigned int x1 = m_muonDetector->getLayoutX(0,station,region);
  unsigned int y1 = m_muonDetector->getLayoutY(0,station,region);
  unsigned int x2 = m_muonDetector->getLayoutX(1,station,region);
  unsigned int y2 = m_muonDetector->getLayoutY(1,station,region);
  layout1= MuonLayout(x1,y1);
  layout2= MuonLayout(x2,y2);
}

StatusCode MuonRawToCoord::checkBankSize(const LHCb::RawBank* rawdata) const {

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
    error()<< " muon bank "<<tell1Number<<" is too short "<< bank_size<<endmsg;
    return StatusCode::FAILURE;
  }
  //how many pads ?
  //const unsigned short * itPad=rawdata->begin<unsigned short>();
  unsigned short nPads=*it;
  //  if ( msgLevel(MSG::VERBOSE) ){
  //    verbose()<< " pad # "<<nPads<<endmsg;
  //  }

  int skip=(nPads+3)*0.5;

  if((bank_size-skip*4)<0){
    error()<<"bank_size "<<bank_size<<" pad size to read "<<nPads*4<<endmsg;
    error()<< "so muon bank "<<tell1Number<<" is too short in pad part "<<endmsg;
    return StatusCode::FAILURE;
  }

  it += 2*skip;
  read_data=read_data+skip*2;
  if(read_data<0)info()<<nPads<<" "<<skip<<" "<< bank_size<<" "<<read_data<<endmsg;

  for(int i=0;i<4;i++){
    //now go to the single pp counter
    int pp_cnt=*it++;
    read_data++;

    //check size before start looping
    if(bank_size-read_data*2<pp_cnt*2){
      error()<<"bank_size "<<bank_size<<"read data "<<read_data<<" hit size to read "<<pp_cnt*2<<endmsg;
      error()<< "so muon bank "<<tell1Number<<" is too short in hit part "<<endmsg;
      //break;
      return StatusCode::FAILURE;
    }
    it+=pp_cnt;
  }

  return StatusCode::SUCCESS;

}

std::vector<std::pair<LHCb::MuonTileID, unsigned int>> MuonRawToCoord::decodeTileAndTDCV1(const RawBank* rawdata) const {

  StatusCode sc=checkBankSize(rawdata);
  if(sc.isFailure()){
    return Digits();
  }

  unsigned int tell1Number=rawdata->sourceID();

  if(tell1Number>=MuonDAQHelper_maxTell1Number){
    error()<<" in muon data a Tell1 Source ID is gretare than maximum "<<endmsg;
    //TODO handle failure
    //return StatusCode::FAILURE;
  }

  const unsigned short* it=rawdata->begin<unsigned short>();

//  std::array<unsigned int,24> hit_link_cnt = { { 0 } };
  short skip=0;
  unsigned  short nPads=*it;
  skip=(nPads+3)*0.5;
  for(int k=0;k<2*skip;k++){
//    if (k==1) fillTell1Header(tell1Number,*it);
    it++;
  }
  std::vector<std::pair<LHCb::MuonTileID, unsigned int> >  storage;

  for(int i=0;i<4;i++){
    //now go to the single pp counter
    unsigned int pp_cnt=*it++;

    if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
      verbose()<<" hit in PP "<<pp_cnt<<endmsg;

    for (unsigned int loop=0;loop<pp_cnt;++loop) {
      unsigned int add=(*it)&(0x0FFF);
      unsigned int tdc_value=(((*it)&(0xF000))>>12);
      ++it;
      LHCb::MuonTileID tile = m_muonDetector->getDAQInfo()->getADDInTell1(tell1Number,add);
      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
        verbose()<<" add "<<add<<" "<<tile <<endmsg;
      if(tile.isValid()) {
        if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
          debug()<<" valid  add "<<add<<" "<<tile <<endmsg;
        storage.emplace_back(tile, tdc_value);
      } else {
        info() << "invalid add " << add << " " << tile << endmsg; 
      }
      //update the hitillink counter
//      unsigned int link=add/192;
//      hit_link_cnt[link]++;
    }
  }
  
  return storage;
}
