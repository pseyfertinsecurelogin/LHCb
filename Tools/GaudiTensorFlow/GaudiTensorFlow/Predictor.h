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
#ifndef _GAUDITENSORFLOW_PREDICTOR_H_
#define _GAUDITENSORFLOW_PREDICTOR_H_

// Local
#include "GaudiTensorFlow/SmartPointers.h"
#include "GaudiTensorFlow/TensorAbs.h"
#include "tensorflow/c/c_api.h"

// STL
#include <initializer_list>
#include <string>
#include <vector>

namespace GaudiTensorFlow {
  class Predictor {
  public:
    Predictor( std::string input_dir, std::vector<std::string> input_tensors, std::vector<std::string> output_tensors,
               unsigned int numberOfThreads = 1 );

    ~Predictor();

    void exec( std::initializer_list<TensorAbs*>&& input, std::initializer_list<TensorAbs*>&& output );

  private:
    std::string              m_input_dir;
    std::vector<std::string> m_input_tensors, m_output_tensors;

    SmartSessionOptions m_session_options;
    SmartBuffer         m_run_options;
    SmartGraph          m_graph;
    SmartBuffer         m_graph_def;
    SmartSession        m_session;

    std::vector<TF_Output> m_input_ops, m_output_ops;

  private: // methods
    static void                checkStatus( const SmartStatus& status );
    static SmartSessionOptions makeSessionOptions( unsigned int nThreads );
  };
} // namespace GaudiTensorFlow

#endif // _GAUDITENSORFLOW_PREDICTOR_H_
