//-----------------------------------------------------------------------------
/** @file DecoderAlgBase.h
 *
 *  Header file for algorithm base class : Decoder::AlgBase
 *
 *  @author Rob Lambert   Rob.Lambert@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERALGBASE_H
#define DAQKERNEL_DECODERALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiAlgorithm.h"

// local
#include "DAQKernel/DecoderCommonBase.h"

namespace Decoder
{
  //-----------------------------------------------------------------------------
  /** @class AlgBase DAQAlgBase.h DAQKernel/DecoderAlgBase.h
   *
   *  Abstract base class for DAQ algorithms providing
   *  some basic functionality.
   *
   *  @author Rob Lambert   Rob.Lambert@cern.ch
   *  @date   02/10/2014
   */
  //-----------------------------------------------------------------------------

  using AlgBase = Decoder::CommonBase<GaudiAlgorithm>;

}

#endif // DAQKERNEL_DECODERALGBASE_H
