
//=============================================================================
/** @file DeRichPD.cpp
 *
 *  Implementation file for class : DeRichPD
 *
 *  @author Chris Jones  christopher.rob.jones@cern.ch
 *  @date   2011-09-23
 */
//=============================================================================

// local
#include "RichDet/DeRichPD.h"

//=============================================================================

StatusCode
DeRichPD::initialize()
{

  // Which RICH are we in ?
  {
    const auto atestGP = geometry()->toGlobalMatrix() * Gaudi::XYZPoint { 0, 0, 0 };
    m_rich             = ( atestGP.z() > 6000.0 ? Rich::Rich2 : Rich::Rich1 );
    _ri_debug << "In " << rich() << endmsg;
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
