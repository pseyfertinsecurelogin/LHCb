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
#ifndef _GAUDITENSORFLOW_SMARTPOINTER_H_
#define _GAUDITENSORFLOW_SMARTPOINTER_H_
#include <iostream>

#include "tensorflow/c/c_api.h"

// STL
#include <memory>

namespace GaudiTensorFlow {
  struct TF_Deleter {
    //==========================================================================
    //  TF-object deleter
    //==========================================================================
    void operator()( TF_Graph* p ) { TF_DeleteGraph( p ); }
    void operator()( TF_Buffer* p ) { TF_DeleteBuffer( p ); }
    void operator()( TF_Tensor* p ) { TF_DeleteTensor( p ); }
    void operator()( TF_SessionOptions* p ) { TF_DeleteSessionOptions( p ); }
    void operator()( TF_Status* p ) { TF_DeleteStatus( p ); }
    void operator()( TF_Session* p ) {
      std::unique_ptr<TF_Status, TF_Deleter> status( TF_NewStatus() );
      TF_DeleteSession( p, status.get() );
    }
  };

  //============================================================================
  //  TF-object unique_ptr
  //============================================================================
  typedef std::unique_ptr<TF_Graph, TF_Deleter>          SmartGraph;
  typedef std::unique_ptr<TF_Buffer, TF_Deleter>         SmartBuffer;
  typedef std::unique_ptr<TF_Tensor, TF_Deleter>         SmartTensor;
  typedef std::unique_ptr<TF_SessionOptions, TF_Deleter> SmartSessionOptions;
  typedef std::unique_ptr<TF_Session, TF_Deleter>        SmartSession;
  typedef std::unique_ptr<TF_Status, TF_Deleter>         SmartStatus;

} // namespace GaudiTensorFlow

#endif // _GAUDITENSORFLOW_SMARTPOINTER_H_
