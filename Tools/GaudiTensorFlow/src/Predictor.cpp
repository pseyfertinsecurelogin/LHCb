/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// STL
#include <iostream>

// Boost
#include "boost/assert.hpp"

// Local
#include "GaudiTensorFlow/Predictor.h"

namespace GaudiTensorFlow {
  //============================================================================
  //   Constructor
  //============================================================================
  Predictor::Predictor( std::string input_dir, std::vector<std::string> input_tensors,
                        std::vector<std::string> output_tensors, unsigned int numberOfThreads )
      : m_input_dir( std::move( input_dir ) )
      , m_input_tensors( std::move( input_tensors ) )
      , m_output_tensors( std::move( output_tensors ) )
      , m_run_options( TF_NewBuffer() )
      , m_graph( TF_NewGraph() )
      , m_graph_def( TF_NewBuffer() )
      , m_input_ops( m_input_tensors.size() )
      , m_output_ops( m_output_tensors.size() ) {

    SmartStatus status( TF_NewStatus() );
    const char* tags  = "serve";
    m_session_options = makeSessionOptions( numberOfThreads );

    m_session =
        SmartSession( TF_LoadSessionFromSavedModel( m_session_options.get(), m_run_options.get(), m_input_dir.c_str(),
                                                    &tags, 1, m_graph.get(), m_graph_def.get(), status.get() ) );

    checkStatus( status );

    for ( size_t iIn = 0; iIn < m_input_tensors.size(); ++iIn )
      m_input_ops[iIn] = {TF_GraphOperationByName( m_graph.get(), m_input_tensors[iIn].c_str() ),
                          static_cast<int>( iIn )};

    for ( size_t iOut = 0; iOut < m_output_tensors.size(); ++iOut )
      m_output_ops[iOut] = {TF_GraphOperationByName( m_graph.get(), m_output_tensors[iOut].c_str() ),
                            static_cast<int>( iOut )};
  }

  //============================================================================
  //   Deleter
  //============================================================================
  Predictor::~Predictor() {
    SmartStatus status( TF_NewStatus() );

    TF_CloseSession( m_session.get(), status.get() );
    // checkStatus ( status );  // Exceptions in destructor should be avoided
    if ( TF_GetCode( status.get() ) != TF_OK ) std::cerr << "Error while closing the TensorFlow session" << std::endl;
  }

  //============================================================================
  //   Execute
  //============================================================================
  void Predictor::exec( std::initializer_list<TensorAbs*>&& inputs, std::initializer_list<TensorAbs*>&& outputs ) {
    SmartStatus status( TF_NewStatus() );

    if ( inputs.size() != m_input_ops.size() )
      throw std::runtime_error( "Number of operator() input tensors inconsistent with Predictor declaration." );
    std::vector<SmartTensor> input_tensors;
    input_tensors.reserve( inputs.size() );
    std::vector<TF_Tensor*> input_raw;
    input_raw.reserve( inputs.size() );

    for ( auto& input : inputs ) {
      input_tensors.push_back( input->get_TF_Tensor() );
      input_raw.push_back( input_tensors.back().get() );
    }

    std::vector<TF_Tensor*> output_raw( m_output_ops.size(), nullptr );

    TF_SessionRun( m_session.get(), nullptr, m_input_ops.data(), input_raw.data(), m_input_ops.size(),
                   m_output_ops.data(), output_raw.data(), m_output_ops.size(), nullptr, 0, nullptr, status.get() );

    checkStatus( status );

    int iTensor = 0;
    for ( auto& output : outputs ) {
      const auto shape = output->getShape();
      BOOST_ASSERT_MSG( static_cast<size_t>( TF_NumDims( output_raw[iTensor] ) ) == shape.size(),
                        "Allocated output tensor has wrong number of dimensions." );

      for ( size_t iDim = 0; iDim < shape.size(); ++iDim )
        BOOST_ASSERT_MSG( static_cast<size_t>( TF_Dim( output_raw[iTensor], iDim ) ) !=
                              static_cast<size_t>( shape[iDim] ),
                          "Dimensions of the allocated output tensor do not match the graph" );

      output->importData( TF_TensorData( output_raw[iTensor] ) );
      TF_DeleteTensor( output_raw[iTensor] );

      iTensor++;
    }
  }

  //============================================================================
  //  checkStatus (private)
  //   Throw an exception describing the error in case of any non-success status
  //============================================================================
  void Predictor::checkStatus( const SmartStatus& status ) {
    if ( TF_GetCode( status.get() ) != TF_OK )
      throw std::runtime_error( std::string( "Cannot load model" ) + TF_Message( status.get() ) );
  }

  //============================================================================
  //  makeSessionOptions (private)
  //    Creates (and return ownership) of TF_SessionOptions setting 1 thread.
  //============================================================================
  SmartSessionOptions Predictor::makeSessionOptions( unsigned int nThreads ) {
    TF_SessionOptions* opts = TF_NewSessionOptions();
    SmartStatus        status( TF_NewStatus() );

    const std::array<unsigned char, 4> protoConfig = {0x10, 0x1, 0x28, static_cast<unsigned char>( nThreads )};

    TF_SetConfig( opts, protoConfig.data(), protoConfig.size(), status.get() );
    checkStatus( status );

    return SmartSessionOptions( opts );
  }
} // namespace GaudiTensorFlow
