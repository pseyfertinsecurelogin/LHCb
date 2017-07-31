//-----------------------------------------------------------------------------
/** @file DecoderTupleToolBase.h
 *
 *  Header file for tool base class : DecoderTupleToolBase
 *
 *  @author Rob Lambert   Rob.Lambert@cern.ch
 *  @date  10/02/2014
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERTUPLETOOLBASE_H
#define DAQKERNEL_DECODERTUPLETOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleTool.h"

// local
#include "DAQKernel/DecoderCommonBase.h"

namespace Decoder
{
  //-----------------------------------------------------------------------------
  /** @class TupleToolBase DecoderTupleToolBase.h DAQKernel/DecoderTupleToolBase.h
   *
   *  Abstract base class for DAQ tools providing some basic functionality.
   *
   *  In addition, uses the histogramming and ntupling
   *  functionality from the base class GaudiTupleTool.
   *
   *  @author Rob Lambert   Rob.Lambert@cern.ch
   *  @date   02/10/2014
   */
  //-----------------------------------------------------------------------------

  using TupleToolBase = Decoder::CommonBase<GaudiTupleTool>;

}

#endif // DAQKERNEL_DECODERTUPLETOOLBASE_H
