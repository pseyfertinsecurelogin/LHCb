
//-----------------------------------------------------------------------------
/** @file IRichBaseTrackSelector.h
 *
 *  Header file for RICH reconstruction tool interface : Rich::Rec::IBaseTrackSelector
 *
 *  CVS Log :-
 *  $Id: IRichBaseTrackSelector.h,v 1.2 2007-02-01 17:26:21 jonrob Exp $
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   12/08/2006
 */
//-----------------------------------------------------------------------------

#ifndef RICHRECBASE_IRichBaseTrackSelector_H
#define RICHRECBASE_IRichBaseTrackSelector_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Event model
namespace LHCb
{
  class RichRecTrack;
  class Track;
}

static const InterfaceID IID_IRichBaseTrackSelector ( "Rich::Rec::IBaseTrackSelector", 1, 0 );

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

  //-----------------------------------------------------------------------------
  /** @namespace Rich::Rec
   *
   *  General namespace for RICH reconstruction software
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  //-----------------------------------------------------------------------------
  namespace Rec
  {

    //---------------------------------------------------------------------------------
    /** @class IBaseTrackSelector IRichBaseTrackSelector.h
     *
     *  Interface to a utility tool used by RichBaseTrackSelector. Users should not use directly.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   2006-08-12
     */
    //---------------------------------------------------------------------------------

    class IBaseTrackSelector : virtual public IAlgTool
    {

    public:

      /** static interface identification
       *  @return unique interface identifier
       */
      static const InterfaceID & interfaceID() { return IID_IRichBaseTrackSelector; }

      /** Test if the given Track is selected
       *
       *  @param track Pointer to a Track
       *
       *  @return Boolean indicating if the track is selected
       *  @retval true  Track is selected
       *  @retval false Track is rejected
       */
      virtual bool trackSelected( const LHCb::Track * track ) const = 0;

      /** Test it the given RichRecTrack is selected
       *
       *  @param track Pointer to a RichRecTrack
       *
       *  @return Boolean indicating if the track is selected
       *  @retval true  Track is selected
       *  @retval false Track is rejected
       */
      virtual bool trackSelected( const LHCb::RichRecTrack * track ) const = 0;

      /// Returns the minimum momentum cut value
      virtual double minPCut() const = 0;

      /// Returns the maximum momentum cut value
      virtual double maxPCut() const = 0;

      /// Returns the minimum momentum cut value
      virtual double minPtCut() const = 0;

      /// Returns the maximum momentum cut value
      virtual double maxPtCut() const = 0;

      /// Returns the minimum chi^2 cut
      virtual double minChi2Cut() const = 0;

      /// Returns the maximum chi^2 cut
      virtual double maxChi2Cut() const = 0;

      /// Returns the charge selection
      virtual int chargeSel() const = 0;

    };

  }
}

#endif // RICHRECBASE_IRichBaseTrackSelector_H
