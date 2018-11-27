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
// Include files

#include<iostream>
#include<string>
#include<sstream>

// local
#include "L0DUMultiConfigProvider.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0DUMultiConfigProvider
//
//  Provide  **ANY** predefined L0DU configuration according to
//  the Trigger Configuration Key
//  Configuration described in options file
//
// 2008-01-26 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( L0DUMultiConfigProvider )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0DUMultiConfigProvider::L0DUMultiConfigProvider( const std::string& type,
                                                  const std::string& name,
                                                  const IInterface* parent )
: base_class ( type, name , parent )
{
  declareInterface<IL0DUConfigProvider>(this);
  // preload Config in the initialization phase or load on-Demand
  m_preload = ( context().find("Check") != std::string::npos );
  declareProperty("Preload" , m_preload );
  declareProperty("registerTCK" , m_list );
}
//=============================================================================


//=============================================================================
// Initialize is the main method
//=============================================================================
StatusCode L0DUMultiConfigProvider::initialize(){
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Initialize L0DUMultiConfigProvider" << endmsg;
  StatusCode sc = GaudiTool::initialize();
  if(sc.isFailure())return sc;

  // check TCK is uniquely defined
  for( auto itck = m_list.cbegin() ; itck != m_list.cend() ; ++itck){

    if( itck->compare( 0, 2, "0x" ) !=0 ) {
      error() << "TCK value " << *itck << " MUST be registered in hexadecimal format '0x" << *itck << "'" << endmsg;
      return StatusCode::FAILURE;
    }

    auto jtck = std::find(next(itck),m_list.cend(),*itck);
    if( jtck != m_list.cend()){
        error() << "The Trigger Configuration Key " << *itck << " is not unique " << endmsg;
        return StatusCode::FAILURE;
    }
  }

  std::string  slot = (rootInTES().empty() ? "T0" : rootInTES());

  // pre-load 'template' configuration
  m_template[slot] = loadConfig( LHCb::L0DUTemplateConfig::Name ,slot );
  //  info() << "preloading template for slot " << slot << " " << m_template[slot] << endmsg;

  // pre-load TCK configurations if requested (pre-loading for rootInTES slot only)
  if( m_preload ){
    for( const auto& tck : m_list ) loadConfig( tck , slot );
  }

  return StatusCode::SUCCESS;
}
//============
// finalize
//============
StatusCode L0DUMultiConfigProvider::finalize(){
  m_configs.clear();
  return base_class::finalize();
}

//=============================================================================
LHCb::L0DUConfig*  L0DUMultiConfigProvider::config( long tck ,std::string slot){

  if( slot.empty() ) slot = "T0";

  // first : handle the 'template' configuration
  if( tck == LHCb::L0DUTemplateConfig::TCKValue ){
    if( m_template[slot]) return m_template[slot];
    LHCb::L0DUConfig* temp = loadConfig( LHCb::L0DUTemplateConfig::Name ,slot );
    m_template[slot] = temp;
    return temp;
  }

  if(tck < 0 || tck > 0xFFFF){
    warning() << "requested TCK " << tck << "is not a 16 bit word" << endmsg;
    return nullptr;
  }


  auto it = m_configs.find( slot );
  if( it != m_configs.end() && it->second->object(tck) ) return it->second->object(tck);

  std::stringstream s;
  s << "0x" << format("%04X", tck) ;

  return loadConfig( s.str() ,slot);
}

//=============================================================================
LHCb::L0DUConfig*  L0DUMultiConfigProvider::loadConfig( const std::string& tck , std::string slot){

  if ( slot.empty() ) slot = "T0";

  // first : handle the 'template' configuration
  if( tck == LHCb::L0DUTemplateConfig::Name ){
    info() << "Creating the TEMPLATE configuration tools : " <<name() << "." << LHCb::L0DUTemplateConfig::Name
           << " for time slot :  '" << slot << "'" << endmsg;
    m_provider = tool<IL0DUConfigProvider>("L0DUConfigProvider" , LHCb::L0DUTemplateConfig::Name ,this );
    LHCb::L0DUConfig* temp = m_provider->config( LHCb::L0DUTemplateConfig::TCKValue, slot ); // force template
    if (!temp) error() << "Failed to load the TEMPLATE configuration"    << endmsg;
    return temp;
  }

  if(std::find(m_list.begin(),m_list.end(),tck)==m_list.end()){
    Error("The requested TCK = " + tck + " is not registered ", StatusCode::SUCCESS ).ignore();
    return nullptr;
  }

  std::stringstream s;
  s <<  "TCK_" << tck ;
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Loading L0DUConfigProvider : " << s.str() << endmsg;
  m_provider = tool<IL0DUConfigProvider>("L0DUConfigProvider" , s.str(),this );


  auto index = tck.rfind("0x") + 2 ;
  int itck = std::stoi(tck.substr( index ),nullptr,16);

  if ( msgLevel(MSG::DEBUG) )
    debug() << "TCK = '"<<tck <<"' -> decimal value = " << itck << endmsg ;

  LHCb::L0DUConfig* config = m_provider->config( itck , slot);

  if( !config ){
    error() << "Failed to load configuration for TCK = " << tck
            << " ... return empty LODUConfig" << endmsg;
    return config;
  }

  if ( msgLevel(MSG::DEBUG) )
    debug() << "looking for configs for slot " << slot << " size = " << m_configs.size() << endmsg;
  auto ic = m_configs.find( slot );
  if(ic != m_configs.end())debug() << "configs for slot " << slot << " found : " << ic->second.get() << endmsg;
  if(ic == m_configs.end()){
    LHCb::L0DUConfigs* confs = new LHCb::L0DUConfigs();
    confs->addRef();
    auto ret = m_configs.emplace(slot, confs);
    assert(ret.second);
    ic = ret.first;
  }
  if ( msgLevel(MSG::DEBUG) )
    debug() << "inserting config " << config << " in container " << ic->second.get() << " for slot " << slot << endmsg;
  ic->second->insert( config );

  return  config;
}
