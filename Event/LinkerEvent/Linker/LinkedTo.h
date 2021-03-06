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
#ifndef LINKER_LINKEDTO_H
#define LINKER_LINKEDTO_H 1

// STL
#include <sstream>

// Include files
#include "Event/LinksByKey.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/LinkManager.h"
#include "GaudiKernel/ObjectContainerBase.h"
#include "GaudiKernel/SmartDataPtr.h"

/** @class LinkedTo LinkedTo.h Linker/LinkedTo.h
 *  Templated interface to the LinksByKey, direct version
 *
 *  @author Olivier Callot
 *  @date   2004-01-06
 */

template <class TARGET, class SOURCE = ContainedObject>
class LinkedTo final {
public:
  //== Typedefs to please Matt
  typedef typename std::vector<TARGET*>                 LRange;
  typedef typename std::vector<TARGET*>::const_iterator LRangeIt;

  /// Standard constructor
  LinkedTo( IDataProviderSvc* eventSvc, IMessageSvc* msgSvc, std::string containerName ) {
    m_eventSvc       = eventSvc;
    std::string name = "Link/" + containerName;
    if ( containerName.compare( 0, 7, "/Event/" ) == 0 ) { name = "Link/" + containerName.substr( 7 ); }
    SmartDataPtr<LHCb::LinksByKey> links( eventSvc, name );
    if ( !links ) {
      if ( msgSvc ) {
        MsgStream msg( msgSvc, "LinkedTo::" + containerName );
        msg << MSG::ERROR << "*** Link container " << name << " not found." << endmsg;
      }
    } else {
      //== Check proper template, only if specified.
      if ( links->sourceClassID() != SOURCE::classID() && CLID_ContainedObject != SOURCE::classID() ) {
        std::ostringstream message;
        message << "Incompatible SOURCE type for location " << containerName << " : Template classID is "
                << SOURCE::classID() << " expected " << links->sourceClassID();
        throw GaudiException( message.str(), "LinkedTo", StatusCode::FAILURE );
      }
      if ( links->targetClassID() != TARGET::classID() ) {
        std::ostringstream message;
        message << "Incompatible TARGET type for location " << containerName << " : Template classID is "
                << TARGET::classID() << " expected " << links->targetClassID();
        throw GaudiException( message.str(), "LinkedTo", StatusCode::FAILURE );
      }
    }
    m_links = links;
    m_curReference.setNextIndex( -1 );
    m_curReference.setWeight( 0. );
  };

  bool notFound() const { return ( 0 == m_links ); }

  /** returns the first target related to the specified source.
   */
  TARGET* first( const SOURCE* source ) {
    if ( !m_links ) return nullptr;
    bool status = m_links->firstReference( source->index(), source->parent(), m_curReference );
    if ( status ) return currentTarget();
    m_curReference.setNextIndex( -1 );
    return nullptr;
  }

  /** returns the first target related to the source specified as a key.
   */
  TARGET* first( int key ) {
    if ( !m_links ) return nullptr;
    bool status = m_links->firstReference( key, nullptr, m_curReference );
    if ( status ) return currentTarget();
    m_curReference.setNextIndex( -1 );
    return nullptr;
  }

  /** returns the next target. The source of the relation was specified by a
      previous call to 'first'
   */
  TARGET* next() {
    if ( !m_links ) return nullptr;
    bool status = m_links->nextReference( m_curReference );
    if ( status ) return currentTarget();
    m_curReference.setNextIndex( -1 );
    return nullptr;
  }
  /** returns the weight of the previously defined relation */
  double weight() { return m_curReference.weight(); }

  /** returns a vector of targets, onto which STL functions can be used.
   */
  LRange& range( const SOURCE* source ) {
    m_vect.clear();
    auto tmp = first( source );
    while ( tmp ) {
      m_vect.push_back( tmp );
      tmp = next();
    }
    return m_vect;
  }

  /** returns a vector of targets, onto which STL functions can be used.
   */
  LRange& range( int key ) {
    m_vect.clear();
    auto tmp = first( key );
    while ( tmp ) {
      m_vect.push_back( tmp );
      tmp = next();
    }
    return m_vect;
  }

private:
  TARGET* currentTarget() {
    if ( !m_links ) return nullptr;
    int                myLinkID = m_curReference.linkID();
    LinkManager::Link* link     = m_links->linkMgr()->link( myLinkID );
    if ( !link->object() ) {
      SmartDataPtr<DataObject> tmp( m_eventSvc, link->path() );
      link->setObject( tmp );
      if ( !tmp ) return nullptr;
    }
    ObjectContainerBase* parent = dynamic_cast<ObjectContainerBase*>( link->object() );
    return parent ? static_cast<TARGET*>( parent->containedObject( m_curReference.objectKey() ) ) : nullptr;
  }

  IDataProviderSvc*   m_eventSvc;
  LHCb::LinksByKey*   m_links;
  LHCb::LinkReference m_curReference;
  LRange              m_vect;
};
#endif // LINKER_LINKEDTO_H
