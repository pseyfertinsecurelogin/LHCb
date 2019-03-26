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
#include "ConfigurableDummy.h"

DECLARE_COMPONENT( ConfigurableDummy )

#define ON_DEBUG if ( msgLevel( MSG::DEBUG ) )
#define DEBUG_MSG ON_DEBUG debug()

#define ON_VERBOSE if ( msgLevel( MSG::VERBOSE ) )
#define VERBOSE_MSG ON_VERBOSE verbose()

StatusCode ConfigurableDummy::initialize() {
  auto sc = Algorithm::initialize();
  if ( !sc ) return sc;

  // This is a bit ugly. There is no way to declare a vector of DataObjectHandles, so
  // we need to wait until initialize when we've read in the input and output key
  // properties, and know their size, and then turn them
  // into Handles and register them with the framework by calling declareProperty. We
  // could call declareInput/declareOutput on them too.

  int i = 0;
  for ( auto k : m_inpKeys ) {
    DEBUG_MSG << "adding input key " << k << endmsg;
    m_inputHandles.push_back( std::make_unique<DataObjectReadHandle<DataObject>>( k, this ) );
    declareProperty( "dummy_in_" + std::to_string( i ), *( m_inputHandles.back() ) );
    i++;
  }

  i = 0;
  for ( auto k : m_outKeys ) {
    DEBUG_MSG << "adding output key " << k << endmsg;
    m_outputHandles.push_back( std::make_unique<DataObjectWriteHandle<DataObject>>( k, this ) );
    declareProperty( "dummy_out_" + std::to_string( i ), *( m_outputHandles.back() ) );
    i++;
  }

  return sc;
}

StatusCode ConfigurableDummy::execute() // the execution of the algorithm
{

  VERBOSE_MSG << "inputs number: " << m_inputHandles.size() << endmsg;
  for ( auto& inputHandle : m_inputHandles ) {
    if ( !inputHandle->isValid() ) continue;

    VERBOSE_MSG << "get from TS: " << inputHandle->objKey() << endmsg;
    DataObject const* obj = inputHandle->get();
    if ( obj == nullptr ) error() << "A read object was a null pointer." << endmsg;
  }

  VERBOSE_MSG << "outputs number: " << m_outputHandles.size() << endmsg;
  for ( auto& outputHandle : m_outputHandles ) {
    if ( !outputHandle->isValid() ) continue;

    VERBOSE_MSG << "put to TS: " << outputHandle->objKey() << endmsg;
    outputHandle->put( std::make_unique<DataObject>() );
  }

  setFilterPassed( m_CFD );

  return StatusCode::SUCCESS;
}
