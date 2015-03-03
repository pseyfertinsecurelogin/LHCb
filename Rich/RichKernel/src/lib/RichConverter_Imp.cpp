
//-----------------------------------------------------------------------------
/** @file RichConverter_Imp.cpp
 *
 *  Implementation file for class : Rich::Converter_Imp
 *
 *  CVS Log :-
 *  $Id: RichConverter_Imp.cpp,v 1.3 2009-09-30 08:25:59 jonrob Exp $
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2009-07-07
 */
//-----------------------------------------------------------------------------

// gaudi
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IChronoStatSvc.h"
#include "GaudiKernel/IAlgTool.h"

// local
#include "RichKernel/RichConverter_Imp.h"

// print and count the error
StatusCode Rich::Converter_Imp::Error( const std::string& Message ,
                                       const StatusCode   Status ) const
{
  return this->Print( Message , MSG::ERROR  , Status  ) ;
}

// print and count the warning
StatusCode Rich::Converter_Imp::Warning( const std::string& Message ,
                                         const StatusCode   Status ) const
{
  return this->Print( Message , MSG::WARNING , Status ) ;
}

// print info message
StatusCode Rich::Converter_Imp::Info( const std::string& Message ,
                                      const StatusCode   Status ) const
{
  return this->Print( Message , MSG::INFO , Status ) ;
}

// print Debug message
StatusCode Rich::Converter_Imp::Debug( const std::string& Message ,
                                       const StatusCode   Status ) const
{
  return this->Print( Message , MSG::DEBUG , Status ) ;
}

// print verbose message
StatusCode Rich::Converter_Imp::Verbose( const std::string& Message ,
                                         const StatusCode   Status ) const
{
  return this->Print( Message , MSG::VERBOSE , Status ) ;
}

// print the message
StatusCode Rich::Converter_Imp::Print( const std::string& Message ,
                                       const MSG::Level   level   ,
                                       const StatusCode   Status  ) const
{
  this->msgStream(level) << Message << endmsg ;
  return Status;
}

IToolSvc* Rich::Converter_Imp::toolSvc() const
{
  if ( !m_toolSvc )
  {
    m_toolSvc = this -> svc<IToolSvc>( "ToolSvc" );
  }
  return m_toolSvc;
}

IDataProviderSvc* Rich::Converter_Imp::detSvc() const
{
  if ( !m_detSvc )
  {
    m_detSvc = this -> svc<IDataProviderSvc>( "DetectorDataSvc" );
  }
  return m_detSvc;
}

IChronoStatSvc* Rich::Converter_Imp::chronoSvc() const
{
  if ( !m_chronoSvc )
  {
    m_chronoSvc = this -> svc<IChronoStatSvc>( "ChronoStatSvc" );
  }
  return m_chronoSvc;
}

// initialize
StatusCode Rich::Converter_Imp::initialize()
{
  const StatusCode sc = ::Converter::initialize();
  if ( sc.isFailure() )
  { return Error( "Could not initialize base class Converter", sc); }

  return sc;
}

StatusCode Rich::Converter_Imp::finalize()
{
  // release used services
  if ( 0 != m_detSvc    ) { m_detSvc    ->release() ; m_detSvc    = 0 ; }
  if ( 0 != m_toolSvc   ) { m_toolSvc   ->release() ; m_toolSvc   = 0 ; }
  if ( 0 != m_chronoSvc ) { m_chronoSvc ->release() ; m_chronoSvc = 0 ; }
  // try to finalize the base class
  return ::Converter::finalize();
}

// ============================================================================
// manual forced (and 'safe') release of the active tool or service
// ============================================================================
StatusCode Rich::Converter_Imp::release ( const IInterface* interface ) const
{
  if ( 0 == interface )
  { return Error ( "release(IInterface):: IInterface* points to NULL!" ) ; }
  // dispatch between tools and services
  const IAlgTool* algTool = dynamic_cast<const IAlgTool*>( interface )  ;
  // perform the actual release
  return 0 != algTool ? releaseTool( algTool ) : releaseSvc( interface ) ;
}
// ============================================================================

// ============================================================================
// manual forced (and 'save') release of the tool
// ============================================================================
StatusCode Rich::Converter_Imp::releaseTool ( const IAlgTool* algTool ) const
{
  if( 0 == algTool   )
  { return Error ( "releaseTool(IAlgTool):: IAlgTool* points to NULL!" ) ; }
  if( this->toolSvc() == 0 )
  { return Error ( "releaseTool(IAlgTool):: IToolSvc* points to NULL!" ) ; }
  // find a tool in the list of active tools
  AlgTools::reverse_iterator it =
    std::find( m_tools.rbegin() , m_tools.rend() , algTool ) ;
  if(  m_tools.rend() == it )
  { return Warning("releaseTool(IAlgTool):: IAlgTool* is not active"   ) ; }
  // get the tool
  IAlgTool* t = *it ;
  // cache name
  const std::string name = t->name();
  if ( msgLevel(MSG::DEBUG) )
  { debug() << "Releasing tool '" << name << "'" << endmsg; }
  // remove the tool from the lists
  m_tools.erase( --it.base() ) ;
  // release tool
  const StatusCode sc = this->toolSvc()->releaseTool( t ) ;
  if ( sc.isFailure() )
  { return Warning ( "releaseTool(IAlgTool):: error from IToolSvc whilst releasing "+name , sc ) ; }
  // return final status code
  return sc ;
}
// ============================================================================

// ============================================================================
// manual forced (and 'safe') release of the service
// ============================================================================
StatusCode Rich::Converter_Imp::releaseSvc ( const IInterface* Svc  ) const
{
  if( 0 == Svc ) {
    return Error ( "releaseSvc(IInterface):: IInterface* points to NULL!" ) ;
  }
  SmartIF<IService> svc(const_cast<IInterface*>(Svc));
  if (svc.isValid()) {
    Services::iterator it = m_services.find(svc->name());
    if (it == m_services.end()) {
      return Warning( "releaseSvc(IInterface):: IInterface* is not active" );
    }
    if ( msgLevel(MSG::DEBUG) ) {
      debug() << "Releasing service '" << it->first << "'" << endmsg;
    }
    m_services.erase(it);
    return StatusCode::SUCCESS;
  }
  return Warning( "releaseSvc(IInterface):: IInterface* is not a service" );
}
// ============================================================================
