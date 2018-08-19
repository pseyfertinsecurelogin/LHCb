
//---------------------------------------------------------------------------------
/** @file IRichAddBackground.h
 *
 * Header file for tool interface : Rich::IAddBackground
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date   2008-10-10
 */
//---------------------------------------------------------------------------------

#pragma once

// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// RICH Utils
#include "RichUtils/RichHashMap.h"

namespace Rich
{

  /** @class IAddBackground RichKernel/IRichAddBackground.h
   *
   *  Interface for tools that generate RICH backgrounds
   *
   *  @author Chris Jones
   *  @date   2008-10-10
   */

  class IAddBackground : virtual public IAlgTool
  {

  public:
    /// Interface ID
    DeclareInterfaceID( IAddBackground, 1, 0 );

  public:
    /// Data structure for returned additional backgrounds
    using HPDBackgrounds = Rich::HashMap< LHCb::RichSmartID, LHCb::RichSmartID::Vector >;

  public:
    /// Compute a set of background hits
    virtual StatusCode createBackgrounds( HPDBackgrounds &backgrounds,
                                          const bool      aliceMode = false ) const = 0;
  };

} // namespace Rich
