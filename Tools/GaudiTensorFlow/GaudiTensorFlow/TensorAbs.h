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
#ifndef _GAUDITENSORFLOW_TENSORABS_H_
#define _GAUDITENSORFLOW_TENSORABS_H_

#include "GaudiTensorFlow/SmartPointers.h"
#include <vector>

namespace GaudiTensorFlow {
  //============================================================================
  //  Non-templated, abstract interface to a tensor object.
  //============================================================================
  class TensorAbs {
  public:
    virtual ~TensorAbs(){};

    virtual SmartTensor         get_TF_Tensor()                   = 0;
    virtual void                importData( void* )               = 0;
    virtual TF_DataType         getTfType() const                 = 0;
    virtual size_t              getAllocatedNumberOfBytes() const = 0;
    virtual std::vector<size_t> getShape() const                  = 0;
  };
} // namespace GaudiTensorFlow

#endif // _GAUDITENSORFLOW_TENSORABS_H_
