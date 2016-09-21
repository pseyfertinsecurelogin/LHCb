#ifndef RICHFUTUREDAQ_RICHRAWBANKDECODER_H 
#define RICHFUTUREDAQ_RICHRAWBANKDECODER_H 1

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// Gaudi Functional
#include "GaudiAlg/Transformer.h"

// Rich Utils
#include "RichUtils/RichDecodedData.h"

// Event model
#include "Event/RawEvent.h"
#include "Event/ODIN.h"

namespace Rich
{
  namespace Future
  {

    // Use the functional framework
    using namespace Gaudi::Functional;

    /** @class RichRawBankDecoder RichRawBankDecoder.h
     *
     *  RICH Raw bank decoder.
     *
     *  @author Chris Jones
     *  @date   2016-09-21
     */
    class RawBankDecoder final : Transformer< Rich::DAQ::L1Map( const LHCb::RawEvent&, 
                                                                const LHCb::ODIN& ),
                                              Traits::BaseClass_t<AlgBase> >
    {
      
    public:

      /// Standard constructor
      RawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );
      
      /// Algorithm execution via transform
      Rich::DAQ::L1Map operator()( const LHCb::RawEvent&, const LHCb::ODIN& ) const final;

    private:

    };

  }
}

#endif // RICHFUTUREDAQ_RICHRAWBANKDECODER_H
