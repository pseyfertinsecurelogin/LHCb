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
#ifndef LINKER_LINKERWITHKEY_H
#define LINKER_LINKERWITHKEY_H 1

// Include files
#include "Event/LinksByKey.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartDataPtr.h"

/** @class LinkerWithKey LinkerWithKey.h Linker/LinkerWithKey.h
 *  This is a helper for the new type of relations
 *
 *  @author Olivier Callot
 *  @date   2004-01-06
 */
template <class TARGET, class SOURCE = ContainedObject>
class LinkerWithKey final {
public:
  /// Standard constructor
  LinkerWithKey( IDataProviderSvc* evtSvc, IMessageSvc* msgSvc, std::string containerName ) : m_links( nullptr ) {
    if ( !containerName.empty() ) {
      std::string name = "Link/" + containerName;
      if ( "/Event/" == containerName.substr( 0, 7 ) ) { name = "Link/" + containerName.substr( 7 ); }

      //== If it exists, just append to it.

      SmartDataPtr<LHCb::LinksByKey> links( evtSvc, name );
      if ( links ) {
        m_links = links;
      } else {
        m_links       = new LHCb::LinksByKey();
        StatusCode sc = evtSvc->registerObject( name, m_links );
        if ( !sc && msgSvc ) {
          MsgStream msg( msgSvc, "LinkerWithKey::" + containerName );
          msg << MSG::ERROR << "*** Link container " << name << " cannot be registered, Status " << sc << endmsg;
        }
      }

      m_links->setTargetClassID( TARGET::classID() );
      m_links->setSourceClassID( SOURCE::classID() );
    }
  };

  void reset() { m_links->reset(); }

  void link( const SOURCE* source, const TARGET* dest, double weight = 1. ) {
    if ( !source ) return;
    if ( !dest ) return;
    if ( !m_links ) return;
    int srcIndex   = source->index();
    int srcLinkID  = m_links->linkID( source->parent() );
    int destIndex  = dest->index();
    int destLinkID = m_links->linkID( dest->parent() );

    m_links->addReference( srcIndex, srcLinkID, destIndex, destLinkID, weight );
  }

  void link( int key, const TARGET* dest, double weight = 1. ) {
    if ( !dest ) return;
    if ( !m_links ) return;
    int destIndex  = dest->index();
    int destLinkID = m_links->linkID( dest->parent() );
    m_links->addReference( key, -1, destIndex, destLinkID, weight );
  }

  void setIncreasingWeight() {
    if ( m_links ) m_links->setIncreasing();
  }
  void setDecreasingWeight() {
    if ( m_links ) m_links->setDecreasing();
  }

private:
  LHCb::LinksByKey* m_links;
};
#endif // LINKER_LINKERWITHKEYNEW_H
