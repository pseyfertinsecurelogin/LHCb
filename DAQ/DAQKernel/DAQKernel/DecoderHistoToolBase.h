
//-----------------------------------------------------------------------------
/** @file DecoderHistoToolBase.h
 *
 *  Header file for tool base class : DecoderHistoToolBase
 *
 *  CVS Log :-
 *  $Id: DecoderHistoToolBase.h,v 1.6 2009-07-27 20:29:01 jonrob Exp $
 *
 *  @author Rob Lambert   Rob.Lambert@cern.ch
 *  @date   02/10/2014
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERHISTOTOOLBASE_H
#define DAQKERNEL_DECODERHISTOTOOLBASE_H 1

#include <functional>
#include <string>

#include "/tmp/foobarpath/LHCb/DAQ/DAQKernel/src/lib/DecoderCommonBase.icpp"
// local
#include "DAQKernel/DecoderCommonBase.h"
#include "Event/RawEvent.h"
// GaudiAlg
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"

class IInterface;

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

  class HistoToolBase : public Decoder::CommonBase<GaudiHistoTool>
  {

  public:

    /// Standard constructor
    HistoToolBase( const std::string& type,
                   const std::string& name,
                   const IInterface* parent )
      : Decoder::CommonBase<GaudiHistoTool> ( type, name, parent )
    { }
  protected:
      ///avoid getIfExists name resolution for now
      inline LHCb::RawEvent* getIfExistsRaw(const std::string loc, bool rootOnTes) const override
      {
      	return GaudiHistoTool::getIfExists<LHCb::RawEvent>(loc,rootOnTes);
      }
  };

}

#endif // DAQKERNEL_DECODERHISTOTOOLBASE_H
