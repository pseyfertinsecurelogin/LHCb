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
//-----------------------------------------------------------------------------
/** @file DecoderTupleAlgBase.h
 *
 *  Header file for algorithm base class : DecoderTupleAlgBase
 *
 *  @author Rob Lambert   Rob.Lambert@cern.ch
 *  @date   02/10/2014
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERTUPLEALGBASE_H
#define DAQKERNEL_DECODERTUPLEALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleAlg.h"

// local
#include "DAQKernel/DecoderCommonBase.h"

namespace Decoder
{
  //-----------------------------------------------------------------------------
  /** @class TupleAlgBase DecoderTupleAlgBase.h DAQKernel/DecoderTupleAlgBase.h
   *
   *  Abstract base class for DAQ algorithms providing some basic functionality.
   *
   *  In addition, uses the histogramming and ntupling
   *  functionality from the base class GaudiTupleAlg.
   *
   *  @author Rob Lambert   Rob.Lambert@cern.ch
   *  @date   10/02/2014
   */
  //-----------------------------------------------------------------------------

  using TupleAlgBase = Decoder::CommonBase<GaudiTupleAlg>;

}


#endif // DAQKERNEL_DECODERTUPLEALGBASE_H
