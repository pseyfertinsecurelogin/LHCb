
#ifndef RICHFUTUREKERNEL_RICHCONVERTER_H
#define RICHFUTUREKERNEL_RICHCONVERTER_H 1

// local
#include "RichFutureKernel/RichConverter_Imp.h"
#include "RichFutureKernel/RichCommonBase.h"

namespace Rich
{
  namespace Future
  {

    /** @class Converter RichFutureKernel/RichConverter.h
     *
     *  Base class for RICH converters
     *
     *  @author Chris Jones
     *  @date   2009-07-07
     */
    class Converter : public CommonBase<Converter_Imp>
    {

    public:

      /// Standard constructor
      Converter ( long storage_type,
                  const CLID &class_type,
                  ISvcLocator *svc = nullptr )
        : CommonBase<Converter_Imp> ( storage_type, class_type, svc )
      { }

    };

  }
}

#endif // RICHFUTUREKERNEL_RICHCONVERTER_H
