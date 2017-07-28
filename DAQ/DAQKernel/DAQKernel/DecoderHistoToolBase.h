//-----------------------------------------------------------------------------
/** @file DecoderHistoToolBase.h
 *
 *  Header file for tool base class : DecoderHistoToolBase
 *
 *  @author Rob Lambert   Rob.Lambert@cern.ch
 *  @date   02/10/2014
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERHISTOTOOLBASE_H
#define DAQKERNEL_DECODERHISTOTOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiHistoTool.h"

// local
#include "DAQKernel/DecoderCommonBase.h"

namespace Decoder
{
  //-----------------------------------------------------------------------------
  /** @class HistoToolBase DecoderHistoToolBase.h DAQKernel/DecoderHistoToolBase.h
   *
   *  Abstract base class for DAQ tools providing some basic functionality.
   *
   *  In addition, uses the histogramming
   *  functionality from the base class GaudiTupleAlg.
   *
   *  @author Rob Lambert   Rob.Lambert@cern.ch
   *  @date   10/02/2014
   */
  //-----------------------------------------------------------------------------

  using  HistoToolBase =  Decoder::CommonBase<GaudiHistoTool>;

}

#endif // DAQKERNEL_DECODERHISTOTOOLBASE_H
