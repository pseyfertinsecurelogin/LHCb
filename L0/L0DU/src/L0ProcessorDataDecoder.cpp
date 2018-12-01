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
// ============================================================================

// local
#include "L0ProcessorDataDecoder.h"

/** @file
 *  Implementation file for class : L0ProcessorDataDecoder
 *
 *  purpose : extract relevant information from L0ProcessorData
 *
 * 2006-02-17 : Olivier Deschamps
 */

DECLARE_COMPONENT( L0ProcessorDataDecoder )

// ============================================================================
L0ProcessorDataDecoder::L0ProcessorDataDecoder( const std::string& type   ,
                                                const std::string& name   ,
                                                const IInterface*  parent )
: base_class ( type, name , parent )
{
  declareInterface<IL0ProcessorDataDecoder> ( this ) ;
}
// ============================================================================
StatusCode L0ProcessorDataDecoder::initialize ()
{
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Initialize L0ProcessorDataDecoder" << endmsg;
  StatusCode sc = base_class::initialize();
  if(sc.isFailure())return sc;
  m_condDB = tool<IL0CondDBProvider>("L0CondDBProvider");
  m_dataContainer.clear();
  return StatusCode::SUCCESS;
}

// ============================================================================
StatusCode L0ProcessorDataDecoder::finalize ()
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "release L0ProcessoDataDecoder" << endmsg;
  m_dataContainer.clear();
  if ( msgLevel(MSG::DEBUG) ){    
    for(std::map<int,std::string>::iterator it=m_fiberSource.begin();m_fiberSource.end()!=it;++it){
      debug() << "Fiber "<< it->first << " data is provided by : " << it->second <<endmsg;
    }
  } 
  return base_class::finalize();
}

// ============================================================================
bool L0ProcessorDataDecoder::setL0ProcessorData(const std::vector<LHCb::L0ProcessorDatas*>& datass){
  m_dataContainer.clear();
  m_ok=true;
  for(LHCb::L0ProcessorDatas* datas : datass) {
    if(!datas){
      Warning("Data container points to NULL ",StatusCode::SUCCESS).ignore();
      m_ok = false;
      break;
    }
    for(auto& itt : *datas) {
      if( m_dataContainer.object( itt->key() ) != nullptr ){
        Warning("Fiber "+Gaudi::Utils::toString(itt->key())+" is used twice - Data not inserted", StatusCode::SUCCESS).ignore();
        continue;
      }
      m_dataContainer.insert(itt);
    }
  }
  return m_ok;
}

// ============================================================================
bool L0ProcessorDataDecoder::setL0ProcessorData(LHCb::L0ProcessorDatas* datas){
  return setL0ProcessorData( std::vector<LHCb::L0ProcessorDatas*>{ datas } );
}

bool L0ProcessorDataDecoder::setL0ProcessorData(const std::vector<std::string>& dataLocs){
  m_dataContainer.clear();
  m_ok=true;
  // check whether the fiber receives HC data
  m_hasHC=false;
  for ( const auto& loc : dataLocs ) {
    if( loc == LHCb::L0ProcessorDataLocation::HC || loc == LHCb::L0ProcessorDataLocation::L0HC ){
      const LHCb::L0ProcessorDatas* datas = getIfExists<LHCb::L0ProcessorDatas>( loc ) ;
      if ( datas && datas->size() != 0 ) m_hasHC=true;
      if ( !m_hasHC )Warning("Expected Herschel L0ProcessorData container is not found or is empty",StatusCode::SUCCESS).ignore();
      break;
    }
  }
  // collect the data
  for ( const auto& loc : dataLocs ) {
    bool isCalo = ( loc == LHCb::L0ProcessorDataLocation::Calo || loc == LHCb::L0ProcessorDataLocation::L0Calo );
    const LHCb::L0ProcessorDatas* datas = getIfExists<LHCb::L0ProcessorDatas>( loc ) ;
    if( !datas ){
      Warning("L0ProcessorData container not found at " + loc , StatusCode::SUCCESS).ignore();
      m_ok=false;
      continue;
    }
    if( msgLevel(MSG::VERBOSE) ) verbose() << "inserting data from " << loc << endmsg;
    for(const auto&  itt : *datas) {
      if( isCalo && m_hasHC &&
          ( itt->key() == L0DUBase::Fiber::CaloPi0Global ||  itt->key() == L0DUBase::Fiber::CaloPi0Local) ){
        if( msgLevel(MSG::VERBOSE) ) verbose() << "CaloPI0 fibers to be replaced by HC fibers " << loc << endmsg;
      }else{
        if( !m_dataContainer.object( itt->key() ) ){
          m_fiberSource[itt->key()]=loc;
          m_dataContainer.insert(itt);
        } else {
          std::ostringstream ss;
          ss << "L0ProcessorData for fiber " << itt->key() << " already exists. Ignoring data from " << loc;
          Warning(ss.str(), StatusCode::SUCCESS).ignore();
        }
      }
    }
  }
  if( msgLevel(MSG::VERBOSE) ) verbose() << "DATA INSERTED" << endmsg;
  return m_ok;
}

// ============================================================================
bool L0ProcessorDataDecoder::setL0ProcessorData( const std::string& dataLoc ){
  return setL0ProcessorData( std::vector<std::string>{ dataLoc } );
}

// ============================================================================
double L0ProcessorDataDecoder::value( const std::array<unsigned int,L0DUBase::Index::Size>& base,int bx){
  return ((double) digit(base,bx))*m_condDB->scale(base[L0DUBase::Index::Scale]);
}

std::vector<int> L0ProcessorDataDecoder::bxList( const std::array<unsigned int,L0DUBase::Index::Size>& base){
  std::vector<int> def;
  LHCb::L0ProcessorData* fiber = m_dataContainer.object( base[ L0DUBase::Index::Fiber ]  )  ;
  if( !fiber ){
    Warning("Fiber "+ Gaudi::Utils::toString(base[ L0DUBase::Index::Fiber ]) +" not found ",StatusCode::SUCCESS).ignore();
    m_ok=false;
    return def;
  }
  return fiber->bxList();
}

unsigned long L0ProcessorDataDecoder::digit( const std::array<unsigned int,L0DUBase::Index::Size>& base, int bx){

  //  if(!m_ok)return 0;

  LHCb::L0ProcessorData* fiber = m_dataContainer.object( base[ L0DUBase::Index::Fiber ]  )  ;
  if( !fiber ){
    Warning("Fiber "+ Gaudi::Utils::toString(base[ L0DUBase::Index::Fiber ]) +" not found ",StatusCode::SUCCESS).ignore();
    m_ok=false;
    return 0;
  }

  if( ! fiber->hasData(bx))
    Warning("Fiber "+ Gaudi::Utils::toString(base[ L0DUBase::Index::Fiber ]) +
            " has no defined data for BX="+Gaudi::Utils::toString(bx),StatusCode::SUCCESS,1).ignore();
  unsigned long val = ( ( fiber->word(bx)   &  base[L0DUBase::Index::Mask]  ) >> base[L0DUBase::Index::Shift]  ) ;

  if( L0DUBase::Fiber::Empty != base[ L0DUBase::Index::Fiber2 ]  ) {
    LHCb::L0ProcessorData* fiber2= m_dataContainer.object( base[ L0DUBase::Index::Fiber2 ]  )  ;
    if( 0 == fiber2 ){
      Warning("Data ( " + Gaudi::Utils::toString(base) + " ) not found in the container ",StatusCode::SUCCESS).ignore();
      m_ok=false;
      return 0;
    }
    if( ! fiber2->hasData(bx))
      Warning("Fiber "+ Gaudi::Utils::toString(base[ L0DUBase::Index::Fiber2 ]) +
              " has no defined data for BX="+Gaudi::Utils::toString(bx),StatusCode::SUCCESS,1).ignore();
    unsigned long val2 =( ( fiber2->word(bx) & base[ L0DUBase::Index::Mask2 ]  )  >> base[ L0DUBase::Index::Shift2 ]) ;
    val |=  (val2  << base[ L0DUBase::Index::Offset ]  );
  }
  m_ok=true;
  return val;
}
