
//-----------------------------------------------------------------------------
/** @file IRichMirrorSegFinder.h
 *
 *  Header file for tool interface : Rich::IMirrorSegFinder
 *
 *  @author Antonis Papanestis
 *  @date   2003-11-04
 */
//-----------------------------------------------------------------------------

#pragma once

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// from LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"

// from MathCore
#include "GaudiKernel/Point3DTypes.h"

// RichDet
class DeRichSphMirror;

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class IMirrorSegFinder RichInterfaces/IRichMirrorSegFinder.h
   *
   *  Interface to a tool to find the appropriate mirror segment for a given reflection point
   *
   *  @author Antonis Papanestis
   *  @date   2003-11-04
   */
  //-----------------------------------------------------------------------------

  class IMirrorSegFinder : public virtual IAlgTool
  {

  public:

    /// Interface ID
    DeclareInterfaceID( IMirrorSegFinder, 1, 0 );

    /** Locates the spherical mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  @param rich       The RICH detector
     *  @param side       The RICH HPD panel side
     *  @param reflPoint  The reflection point on the spherical mirror
     *
     *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
     */
    virtual const DeRichSphMirror *findSphMirror( const Rich::DetectorType rich,
                                                  const Rich::Side         side,
                                                  const Gaudi::XYZPoint &  reflPoint ) const = 0;

    /** Locates the secondary (spherical) mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  @param rich       The RICH detector
     *  @param side       The RICH HPD panel side
     *  @param reflPoint  The reflection point on the secondary mirror
     *
     *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
     */
    virtual const DeRichSphMirror *findSecMirror( const Rich::DetectorType rich,
                                                  const Rich::Side         side,
                                                  const Gaudi::XYZPoint &  reflPoint ) const = 0;
  };

} // namespace Rich
