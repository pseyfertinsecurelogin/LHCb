
//-----------------------------------------------------------------------------
/** @file IRichRecGeomTool.h
 *
 *  Header file for RICH reconstruction tool interface : IRichRecGeomTool
 *
 *  CVS Log :-
 *  $Id: IRichRecGeomTool.h,v 1.6 2006-03-12 16:13:47 jonrob Exp $
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHRECTOOLS_IRICHRECGEOMTOOL_H
#define RICHRECTOOLS_IRICHRECGEOMTOOL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Event
namespace LHCb
{
  class RichRecPixel;
  class RichRecSegment;
}

/// Static Interface Identification
static const InterfaceID IID_IRichRecGeomTool( "IRichRecGeomTool" , 1 , 0 );

//-----------------------------------------------------------------------------
/** @class IRichRecGeomTool IRichRecGeomTool.h
 *
 *  Interface for a tool to answer simple geometrical questions
 *  using the reconstruction event model
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

class IRichRecGeomTool : public virtual IAlgTool {

public:

  /** static interface identification
   *  @return unique interface identifier
   */
  static const InterfaceID& interfaceID() { return IID_IRichRecGeomTool; }

  /** Computes the square of the distance separating the pixel hit and track hit
   *  position extrapolated using the RichRecSegment direction in local
   *  HPD panel coordinates
   *
   *  @param segment Pointer to the RichRecSegment
   *  @param pixel   Pointer to the RichRecPixel
   *
   *  @return The separation squared in local HPD panel coordinates
   */
  virtual double trackPixelHitSep2( const LHCb::RichRecSegment * segment,
                                    const LHCb::RichRecPixel * pixel ) const = 0;

  /** Computes the fraction of the Cherenkov cone for a given segment and mass hypothesis
   *  that lies within the average HPD panel acceptance
   *
   *  @param segment Pointer the a RichRecSegment
   *  @param id      The mass hypothesis
   *
   *  @return The fraction of the cherenkov ring that is within the averge HPD acceptance
   */
  virtual double hpdPanelAcceptance( LHCb::RichRecSegment * segment,
                                     const Rich::ParticleIDType id ) const = 0;


  /** Correct the given position (in local HPD coordinates) for the average
   *  optical distortion for given radiator
   *
   *  @param point The local position coordinate to correct
   *  @param rad   The radiator type to correct for
   *
   *  @return The corrected local coordinate
   */
  virtual Gaudi::XYZPoint 
  correctAvRadiatorDistortion( const Gaudi::XYZPoint & point,
                               const Rich::RadiatorType rad ) const = 0;
  

};


#endif // RICHRECTOOLS_IRICHRECGEOMTOOL_H
