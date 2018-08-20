
#pragma once

// local
#include "RichKernel/RichCommonBase.h"
#include "RichKernel/RichConverter_Imp.h"

namespace Rich
{

  /** @class Converter RichKernel/RichConverter.h
   *
   *  Base class for RICH converters
   *
   *  @author Chris Jones
   *  @date   2009-07-07
   */
  class Converter : public Rich::CommonBase< Rich::Converter_Imp >
  {

  public:
    /// Standard constructor
    Converter( long storage_type, const CLID &class_type, ISvcLocator *svc = nullptr )
      : Rich::CommonBase< Rich::Converter_Imp >( storage_type, class_type, svc )
    {}
  };

} // namespace Rich
