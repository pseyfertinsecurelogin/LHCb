
#ifndef RICHKERNEL_RICHCONVERTER_H
#define RICHKERNEL_RICHCONVERTER_H 1

#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/MsgStream.h"
#include "RichKernel/RichCommonBase.h"
// local
#include "RichKernel/RichConverter_Imp.h"

class ISvcLocator;

namespace Rich
{

  /** @class Converter RichKernel/RichConverter.h
   *
   *  Base class for RICH converters
   *
   *  @author Chris Jones
   *  @date   2009-07-07
   */
  class Converter : public Rich::CommonBase<Rich::Converter_Imp>
  {

  public:

    /// Standard constructor
    Converter ( long storage_type,
                const CLID &class_type,
                ISvcLocator *svc = nullptr )
      : Rich::CommonBase<Rich::Converter_Imp> ( storage_type, class_type, svc )
    { }

  };

}

#endif // RICHKERNEL_RICHCONVERTER_H
