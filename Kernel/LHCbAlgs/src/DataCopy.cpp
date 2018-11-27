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
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SmartDataPtr.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
// ============================================================================
/** @file
 *
 *  A pair of useful algorithms to copy/move/link data in TES
 *
 *  @see Gaudi::DataCopy
 *  @see Gaudi::DataLink
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2011-10-05
 *
 */
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  /** @class DataCopy
   *  Trivial algorithm to "copy/move" data in TES
   *
   *  @code
   *
   *  from Configurables import Gaudi__DataCopy
   *
   *  rootInTes = '/Event/Leptonic/'
   *  rawEvt  = Gaudi_DataCopy ( 'CopyRawEvt' ,
   *                             What   =      '/Event/DAQ/RawEvent' ,
   *                             Target = rootInTes + 'DAQ/RawEvent' )
   *
   *  @endcode
   *
   *  @see Gaudi::DataLink
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-05
   */
  class DataCopy : public GaudiAlgorithm
  {
  public:
    /// standard initialization
    StatusCode initialize ()  override;
    /// the only one essential method
    StatusCode execute ()  override; // the only one essential method
    /// standard constructor
    DataCopy ( const std::string& name ,
               ISvcLocator*       pSvc ) ;
  protected:
    inline std::string _TESLocation
    ( const std::string & location     ,
      const bool          useRootInTES ) const
    {
      // already rootified name :
      if ( useRootInTES
           && 0 == location.find ("/Event/"    )
           && 0 == location.find ( rootInTES() ) ) { return location ; }

      // The logic is:
      // if no R.I.T., give back location
      // if R.I.T., this is the mapping:
      // (note that R.I.T. contains a trailing '/')
      //  location       -> result
      //  -------------------------------------------------
      //  ""             -> R.I.T.[:-1]      ("rit")
      //  "/Event"       -> R.I.T.[:-1]      ("rit")
      //  "/Event/MyObj" -> R.I.T. + "MyObj" ("rit/MyObj")
      //  "MyObj"        -> R.I.T. + "MyObj" ("rit/MyObj")
      return ( !useRootInTES || rootInTES().empty() ?
               location
               :
               location.empty() || ( location == "/Event" ) ?
               rootInTES().substr(0,rootInTES().size()-1)
               :
               0 == location.find("/Event/") ?
               rootInTES() + location.substr(7)
               :
               rootInTES() + location );
    }
    // ========================================================================
  private:
    // ========================================================================
    /// update handler for "What"
    void handler_1 ( ::Property&  p ) ;            // update handler for "What"
    /// update handler for "Target"
    void handler_2 ( ::Property&  p ) ;            // update handler for "Target"
    // ========================================================================
  protected:
    // ========================================================================
    /// copy data ?
    bool m_copy              ;                                    // copy data?
    // ========================================================================
  private:
    // ========================================================================
    /// never fail
    bool        m_never_fail ;                                 // never fail
    /// what  to copy
    std::string m_what       ;                                 //  what to copy
    /// where to copy
    std::string m_target     ;                                 // where to copy
    // ========================================================================
  } ; // end of class DataCopy
  // ==========================================================================
  /** @class DataLink
   *  Trivial algorithm to "link" data in TES
   *
   *  @code
   *
   *  from Configurables import Gaudi__DataLink
   *
   *  rootInTes = '/Event/Leptonic/'
   *  rawEvt  = Gaudi_DataLink ( 'CopyRawEvt' ,
   *                             What   =      '/Event/DAQ/RawEvent' ,
   *                             Target = rootInTes + 'DAQ/RawEvent' )
   *
   *  @endcode
   *
   *  @see Gaudi::DataCopy
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-05
   */
  class DataLink : public Gaudi::DataCopy
  {
  public:
    /// standard constructor
    DataLink ( const std::string& name ,
               ISvcLocator*       pSvc ) ;
  } ;
} //                                                     end of namespace Gaudi
// ============================================================================
Gaudi::DataCopy::DataCopy
( const std::string& name ,
  ISvcLocator*       pSvc )
  : GaudiAlgorithm ( name , pSvc )
//
  , m_copy         ( true )
  , m_never_fail   ( true )
  , m_what         ()
  , m_target       ()
{
  //
  declareProperty
    ( "What" ,
      m_what ,
      "Data to be copied/linked" ) ->
    declareUpdateHandler ( &Gaudi::DataCopy::handler_1 , this ) ;
  //
  declareProperty
    ( "Target" ,
      m_target ,
      "Target for copy/link"     ) ->
    declareUpdateHandler ( &Gaudi::DataCopy::handler_2 , this ) ;
  //
  declareProperty
    ( "NeverFail" , m_never_fail , "Never fail" );
}
// ============================================================================
// update handler for property "What"
// ============================================================================
void Gaudi::DataCopy::handler_1 ( ::Property&  /* p */ )
{
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return ; }
  m_what = _TESLocation ( m_what     , true ) ;
  Info ( "Change property 'What'   :" + m_what   , StatusCode::SUCCESS ).ignore() ;
}
// ========================================================================
void Gaudi::DataCopy::handler_2 ( ::Property&  /* p */ )
{
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return ; }
  m_target = _TESLocation ( m_target , true ) ;
  Info ( "Change property 'Target' :" + m_target , StatusCode::SUCCESS ).ignore() ;
}
// ========================================================================
// standard initialization
// ========================================================================
StatusCode Gaudi::DataCopy::initialize ()
{
  //
  StatusCode sc = GaudiAlgorithm::initialize () ;
  if ( sc.isFailure() ) { return sc ; }                 // RETURN
  //
  m_what   = _TESLocation ( m_what   , true ) ;
  m_target = _TESLocation ( m_target , true ) ;
  //
  if ( m_what.empty()      )
  { Warning ( "Nothing to be copied"    , StatusCode::SUCCESS ).ignore() ; }
  if ( m_target.empty()    )
  { Warning ( "Target is not specified" , StatusCode::SUCCESS ).ignore() ; }
  if ( m_what  == m_target )
  { return Warning ( "No need to copy"  , StatusCode::SUCCESS ) ; }
  //
  return StatusCode::SUCCESS ;
}
// ========================================================================
// the main method
// ========================================================================
StatusCode Gaudi::DataCopy::execute()
{
  StatusCode ok = StatusCode::SUCCESS ;
  //
  if ( m_what   . empty () )
  { return Warning ( "Nothing to be copied"    , ok ) ; }
  if ( m_target . empty () )
  { return Warning ( "Target is not specified" , ok ) ; }
  if ( m_what  == m_target )
  { return Warning ( "No need to copy"         , ok ) ; }
  //
  //
  SmartDataPtr<DataObject> obj ( evtSvc() , m_what );
  if ( !obj )
  {
    setFilterPassed ( false ) ;
    return Warning ( "No valid data is found at '" + m_what + "'" ,
                     m_never_fail ? StatusCode::SUCCESS : StatusCode::FAILURE ) ;
  }
  //
  DataObject* object = obj ;
  //
  StatusCode sc =
    m_copy ?
    evtSvc () -> registerObject ( m_target , object ) :
    evtSvc () -> linkObject     ( m_target , object ) ;
  //
  if ( sc.isFailure() )
  {
    setFilterPassed ( false ) ;
    return Warning  ( "Unable copy/link " + m_what + " -> " + m_target ,
                      m_never_fail ? ok : sc ) ;
  }
  //
  setFilterPassed ( true ) ;
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
Gaudi::DataLink::DataLink
( const std::string& name ,
  ISvcLocator*       pSvc )
  : Gaudi::DataCopy ( name , pSvc )
{
  m_copy = false ;
}
// ============================================================================
DECLARE_COMPONENT( Gaudi::DataCopy )
DECLARE_COMPONENT( Gaudi::DataLink )
// ============================================================================
// The END
// ============================================================================
