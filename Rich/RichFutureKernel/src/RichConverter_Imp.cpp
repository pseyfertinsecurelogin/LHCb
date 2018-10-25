/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//-----------------------------------------------------------------------------
/** @file RichConverter_Imp.cpp
 *
 *  Implementation file for class : Rich::Converter_Imp
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2009-07-07
 */
//-----------------------------------------------------------------------------

// gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/IChronoStatSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"

// local
#include "RichFutureKernel/RichConverter_Imp.h"

// print and count the error
StatusCode
Rich::Future::Converter_Imp::Error( const std::string &Message, const StatusCode Status ) const
{
  return this->Print( Message, MSG::ERROR, Status );
}

// print and count the warning
StatusCode
Rich::Future::Converter_Imp::Warning( const std::string &Message, const StatusCode Status ) const
{
  return this->Print( Message, MSG::WARNING, Status );
}

// print info message
StatusCode
Rich::Future::Converter_Imp::Info( const std::string &Message, const StatusCode Status ) const
{
  return this->Print( Message, MSG::INFO, Status );
}

// print Debug message
StatusCode
Rich::Future::Converter_Imp::Debug( const std::string &Message, const StatusCode Status ) const
{
  return this->Print( Message, MSG::DEBUG, Status );
}

// print verbose message
StatusCode
Rich::Future::Converter_Imp::Verbose( const std::string &Message, const StatusCode Status ) const
{
  return this->Print( Message, MSG::VERBOSE, Status );
}

// print the message
StatusCode
Rich::Future::Converter_Imp::Print( const std::string &Message,
                                    const MSG::Level   level,
                                    const StatusCode   Status ) const
{
  this->msgStream( level ) << Message << endmsg;
  return Status;
}

IToolSvc *
Rich::Future::Converter_Imp::toolSvc() const
{
  return m_toolSvc;
}

IDataProviderSvc *
Rich::Future::Converter_Imp::detSvc() const
{
  return m_detSvc;
}

IChronoStatSvc *
Rich::Future::Converter_Imp::chronoSvc() const
{
  return m_chronoSvc;
}

// initialize
StatusCode
Rich::Future::Converter_Imp::initialize()
{
  const StatusCode sc = ::Converter::initialize();
  if ( sc.isFailure() ) { return Error( "Could not initialize base class Converter", sc ); }

  // load common tools and services
  m_detSvc    = this->svc< IDataProviderSvc >( "DetectorDataSvc" );
  m_toolSvc   = this->svc< IToolSvc >( "ToolSvc" );
  m_chronoSvc = this->svc< IChronoStatSvc >( "ChronoStatSvc" );

  // return
  return sc;
}

StatusCode
Rich::Future::Converter_Imp::finalize()
{
  // release used services
  if ( m_detSvc )
  {
    m_detSvc->release();
    m_detSvc = nullptr;
  }
  if ( m_toolSvc )
  {
    m_toolSvc->release();
    m_toolSvc = nullptr;
  }
  if ( m_chronoSvc )
  {
    m_chronoSvc->release();
    m_chronoSvc = nullptr;
  }
  // try to finalize the base class
  return ::Converter::finalize();
}

// ============================================================================
// manual forced (and 'safe') release of the active tool or service
// ============================================================================
StatusCode
Rich::Future::Converter_Imp::release( const IInterface *interface ) const
{
  if ( nullptr == interface )
  { return Error( "release(IInterface):: IInterface* points to NULL!" ); }
  // dispatch between tools and services
  const IAlgTool *algTool = dynamic_cast< const IAlgTool * >( interface );
  // perform the actual release
  return nullptr != algTool ? releaseTool( algTool ) : releaseSvc( interface );
}
// ============================================================================

// ============================================================================
// manual forced (and 'save') release of the tool
// ============================================================================
StatusCode
Rich::Future::Converter_Imp::releaseTool( const IAlgTool *algTool ) const
{
  if ( nullptr == algTool ) { return Error( "releaseTool(IAlgTool):: IAlgTool* points to NULL!" ); }
  if ( this->toolSvc() == nullptr )
  { return Error( "releaseTool(IAlgTool):: IToolSvc* points to NULL!" ); }
  // find a tool in the list of active tools
  auto it = std::find( m_tools.rbegin(), m_tools.rend(), algTool );
  if ( m_tools.rend() == it )
  { return Warning( "releaseTool(IAlgTool):: IAlgTool* is not active" ); } // get the tool
  IAlgTool *t = *it;
  // cache name
  const auto &name = t->name();
  if ( msgLevel( MSG::DEBUG ) ) { debug() << "Releasing tool '" << name << "'" << endmsg; }
  // remove the tool from the lists
  m_tools.erase( --it.base() );
  // release tool
  const StatusCode sc = this->toolSvc()->releaseTool( t );
  if ( sc.isFailure() )
  {
    return Warning( "releaseTool(IAlgTool):: error from IToolSvc whilst releasing " + name, sc );
  }
  // return final status code
  return sc;
}
// ============================================================================

// ============================================================================
// manual forced (and 'safe') release of the service
// ============================================================================
StatusCode
Rich::Future::Converter_Imp::releaseSvc( const IInterface *Svc ) const
{
  if ( nullptr == Svc ) { return Error( "releaseSvc(IInterface):: IInterface* points to NULL!" ); }
  SmartIF< IService > svc( const_cast< IInterface * >( Svc ) );
  if ( svc.isValid() )
  {
    auto it = m_services.find( svc->name() );
    if ( it == m_services.end() )
    { return Warning( "releaseSvc(IInterface):: IInterface* is not active" ); }
    if ( msgLevel( MSG::DEBUG ) )
    { debug() << "Releasing service '" << it->first << "'" << endmsg; }
    m_services.erase( it );
    return StatusCode::SUCCESS;
  }
  return Warning( "releaseSvc(IInterface):: IInterface* is not a service" );
}
// ============================================================================
