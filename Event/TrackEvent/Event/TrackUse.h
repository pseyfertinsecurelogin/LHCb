/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef EVENT_TRACKUSE_H
#define EVENT_TRACKUSE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
#include <type_traits>
#include "GaudiKernel/MsgStream.h"
// ============================================================================
// Event model
// ============================================================================
#include "Event/Track.h"
// ============================================================================
// forward declarations
// ============================================================================
#include "GaudiKernel/Algorithm.h" // Forward declaration broken by gaudi/Gaudi!678
class AlgTool   ;
// ============================================================================

/** @class TrackUse TrackUse.h Event/TrackUse.h
 *
 *  the helper class to feed the algorithms/services/tools with
 *  proper (and uniform) treatment of "Track properties" for
 *  selection (or) rejection of tracks
 *
 *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
 *  @date   2004-10-27
 */
class TrackUse final
{
public:
  typedef std::vector<int> Types ;

  /** declare 'own' properties for the Algorithm or AlgTool:
   *
   *  @code
   *
   *  // algorithm contructor
   *  MyAlg::MyAlg  ( const std::string& name ,
   *                  ISvcLocator*       svc  )
   *   : Algorithm  ( name , svc )
   *   , m_trackUse (*this)
   *   {
   *   };
   *
   *  @endcode
   *
   *  @param alg algorithm
   *  @return status code
   *
   *  // tool contructor
   *  MyTool::MyTool  ( const std::string& type ,
   *                    const std::string& name ,
   *                    const IInterface*  parent  )
   *   : AlgTool      ( type , name , parent )
   *   , m_trackUse (*this)
   *   {
   *   };
   *
   *  @endcode
   *
   *  @param tool tool
   *  @return status code
   */

private:
  struct tag {};
  template <typename T> TrackUse( T& parent, tag  );
public:
  explicit TrackUse( Algorithm& parent ) : TrackUse(parent, tag{}) {}
  explicit TrackUse( AlgTool& parent ) : TrackUse(parent, tag{}) {}

  /** the basic method which defines if track selected for further processing
   *  @param track pointer to Track object
   *  @return decsion
   */
  inline bool use  ( const LHCb::Track* track ) const ;
  /** the main method (STL functor interface)
   *  @param track pointer to Track object
   *  @return decsion
   */
  inline bool operator() ( const LHCb::Track* track ) const
  { return use ( track ) ; }

  //
  inline bool check        () const  { return m_check ; }
  inline bool skipClones   () const  { return m_skipClones   ; }
  inline bool skipInvalid  () const  { return m_skipInvalid  ; }
  inline bool skipBackward () const  { return m_skipBackward ; }
  /// get the list of accepted fit status
  size_t acceptedFitStatus ( std::vector<LHCb::Track::FitStatus>& s ) const ;
  /// get the list of accepted types
  size_t acceptedType      ( std::vector<LHCb::Track::Types>&     t ) const ;
  /// get the list of rejected history
  size_t rejectedHistory   ( std::vector<LHCb::Track::History>&   h ) const ;
public:
  inline bool acceptedFitStatus ( const LHCb::Track::FitStatus v ) const ;
  inline bool acceptedType      ( const LHCb::Track::Types     v ) const ;
  inline bool rejectedHistory   ( const LHCb::Track::History   v ) const ;
public:
  /// printout to MsgStream
  MsgStream&    fillStream ( MsgStream&    stream ) const ;
  /// printout of the track into the staream
  MsgStream&    print      ( MsgStream&         stream ,
                             const LHCb::Track* track  ) const ;
private:
  // check the track
  bool               m_check        = true;
  // reject clones
  bool               m_skipClones   = true;
  // reject invalid
  bool               m_skipInvalid  = true;
  // reject backward
  bool               m_skipBackward = true;
  //

  /// accepted fit status
  std::vector<int> m_fitstatus  = { LHCb::Track::FitStatus::Fitted };
  /// accepted type
  std::vector<int> m_type = {  LHCb::Track::Types::Long       ,
                               LHCb::Track::Types::Upstream   ,
                               LHCb::Track::Types::Downstream ,
                               LHCb::Track::Types::Ttrack     };
  /// rejected history
  std::vector<int> m_history ;
};
// ============================================================================
/// status to be accepted
// ============================================================================
inline bool TrackUse::acceptedFitStatus ( const LHCb::Track::FitStatus v ) const
{
  return
    m_fitstatus.end() != std::find( m_fitstatus.begin() , m_fitstatus.end() , v ) ;
}
// ============================================================================
/// type to be accepted
// ============================================================================
inline bool TrackUse::acceptedType    ( const LHCb::Track::Types  v ) const
{
  return
    m_type.end() != std::find( m_type.begin() , m_type.end() , v ) ;
}
// ============================================================================
/// history to be rejected
// ============================================================================
inline bool TrackUse::rejectedHistory ( const LHCb::Track::History v ) const
{
  return
    m_history.end() != std::find( m_history.begin() , m_history.end() , v ) ;
}
// ============================================================================
/** the basic method which defines if track selected for further processing
 *  @param track pointer to Track object
 *  @return decision
 */
// ============================================================================
inline bool
TrackUse::use  ( const LHCb::Track* track ) const
{
  if ( !track )   { return false ; }                              // RETURN
  if ( !check() ) { return true  ; }                              // RETURN
  /// check for flags
  if ( skipClones  ()  &&
       track->checkFlag ( LHCb::Track::Flags::Clone    ) )   { return false ; }
  /// check for flags
  if ( skipInvalid ()  &&
       track->checkFlag ( LHCb::Track::Flags::Invalid  ) )   { return false ; }
  /// check for flags
  if ( skipBackward()  &&
       track->checkFlag ( LHCb::Track::Flags::Backward ) )   { return false ; }
  /// accepted fit status ?
  if ( !acceptedFitStatus ( track -> fitStatus() )  ) { return false ; }
  /// accepted type   ?
  if ( !acceptedType    ( track -> type    () )   )   { return false ; }
  /// rejected history ?
  if (  rejectedHistory ( track -> history () )   )   { return false ; }
  // end!
  return true ;
}
// ============================================================================
inline MsgStream&
operator<<( MsgStream&      stream ,
            const TrackUse& trUse  )
{ return trUse.fillStream ( stream ) ; }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
#endif // EVENT_TRACKUSE_H
// ============================================================================
