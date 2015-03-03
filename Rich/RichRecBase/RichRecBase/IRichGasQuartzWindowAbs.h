
//-----------------------------------------------------------------------------
/** @file IRichGasQuartzWindowAbs.h
 *
 *  Header file for RICH reconstruction tool interface : Rich::Rec::IGasQuartzWindowAbs
 *
 *  CVS Log :-
 *  $Id: IRichGasQuartzWindowAbs.h,v 1.5 2007-02-01 17:26:21 jonrob Exp $
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHRECTOOLS_IRICHGASQUARTZWINDOWABS_H
#define RICHRECTOOLS_IRICHGASQUARTZWINDOWABS_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Event model
namespace LHCb
{
  class RichRecSegment;
}

/// Static Interface Identification
static const InterfaceID IID_IRichGasQuartzWindowAbs( "Rich::Rec::IGasQuartzWindowAbs", 1, 0 );

//-----------------------------------------------------------------------------
/** @namespace Rich
 *
 *  General namespace for RICH software
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   08/07/2004
 */
//-----------------------------------------------------------------------------
namespace Rich
{

  /** @namespace Rich::Rec
   *
   *  General namespace for RICH reconstruction software
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  namespace Rec
  {

    //-----------------------------------------------------------------------------
    /** @class IGasQuartzWindowAbs IRichGasQuartzWindowAbs.h
     *
     *  Interface for tool to calculate transmission properties for the
     *  gas quartz window
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   15/03/2002
     */
    //-----------------------------------------------------------------------------

    class IGasQuartzWindowAbs : public virtual IAlgTool
    {

    public:

      /** static interface identification
       *  @return unique interface identifier
       */
      static const InterfaceID& interfaceID() { return IID_IRichGasQuartzWindowAbs; }

      /** Calculates the transmission probability through the gas quartz window for
       *  a given pathlength (segment) and photon energy
       *
       *  @param segment Pointer to a RichRecSegment
       *  @param energy photon energy
       *
       *  @return The transmission probability through the quartz window
       */
      virtual double photonTransProb( const LHCb::RichRecSegment * segment,
                                      const double energy ) const = 0;

    };

  }
}

#endif // RICHRECTOOLS_IRICHGASQUARTZWINDOWABS_H
