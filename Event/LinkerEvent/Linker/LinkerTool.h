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
#ifndef LINKER_LINKERTOOL_H
#define LINKER_LINKERTOOL_H 1

// STL
#include <sstream>

// Include files
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/LinkManager.h"
#include "GaudiKernel/ObjectContainerBase.h"
#include "GaudiKernel/SmartIF.h"

#include "Event/LinksByKey.h"

#include "Linker/LinkerTable.h"
#include "Linker/LinkerWithKey.h"

/** @class LinkerTool LinkerTool.h Linker/LinkerTool.h
 *  Mimic the relation tool as much as possible
 *
 *  @author Olivier Callot
 *  @date   2005-01-19
 */
template <class SOURCE, class TARGET>
class LinkerTool final {

public:
  typedef LinkerTable<SOURCE, TARGET>   DirectType;
  typedef LinkerTable<TARGET, SOURCE>   InverseType;
  typedef typename DirectType::Range    Range;
  typedef typename DirectType::iterator iterator;

  /** Standard constructor
   *  @param  svc             event service
   *  @param  containerName   name of the container of links.
   */
  LinkerTool( IDataProviderSvc* svc, std::string containerName ) {
    m_evtSvc         = svc;
    std::string name = "Link/" + containerName;
    if ( "/Event/" == containerName.substr( 0, 7 ) ) { name = "Link/" + containerName.substr( 7 ); }
    m_location    = name;
    m_invLocation = name + "Inv";
  };

  /** retrieve the direct relation
   *  @return     The direct table of relation.
   */

  DirectType* direct() {
    SmartDataPtr<LHCb::LinksByKey> links( m_evtSvc.get(), m_location );
    if ( links ) {
      links->resolveLinks( m_evtSvc.get() );

      if ( links->sourceClassID() != SOURCE::classID() ) {
        std::ostringstream message;
        message << "Incompatible SOURCE type for location " << m_location
                << " : Template classID is : " << SOURCE::classID() << " expected " << links->sourceClassID();
        throw GaudiException( message.str(), "LinkerTool", StatusCode::FAILURE );
      }
      if ( links->targetClassID() != TARGET::classID() ) {
        std::ostringstream message;
        message << "Incompatible TARGET type for location " << m_location << " : Template classID is "
                << TARGET::classID() << " expected " << links->targetClassID();
        throw GaudiException( message.str(), "LinkerTool", StatusCode::FAILURE );
      }
    }

    const LHCb::LinksByKey* linkPtr = links;
    m_table.load( linkPtr );
    return linkPtr ? &m_table : nullptr;
  }

  /** retrieve the inverse relation, build it if not yet done
   *  @return     The inverse table of relation.
   */

  InverseType* inverse() {
    SmartDataPtr<LHCb::LinksByKey> links( m_evtSvc.get(), m_invLocation );
    LHCb::LinksByKey*              linkPtr = links;
    if ( 0 == linkPtr ) {
      //== Invert the table...
      const DirectType* tmp = direct();
      if ( 0 != tmp ) {
        // Create, with name shortened to remove the "Link/" prefix
        LinkerWithKey<SOURCE, TARGET> makeLink( m_evtSvc.get(), 0, m_invLocation.substr( 5 ) );
        Range                         rd = tmp->relations();
        for ( iterator it = rd.begin(); rd.end() != it; ++it ) { makeLink.link( it->to(), it->from(), it->weight() ); }
        SmartDataPtr<LHCb::LinksByKey> newLinks( m_evtSvc.get(), m_invLocation );
        linkPtr = newLinks;
      }
    }
    if ( linkPtr ) linkPtr->resolveLinks( m_evtSvc.get() );
    m_invTable.load( linkPtr );
    if ( !linkPtr ) return nullptr;

    //== TARGET and SOURCE are exchanged for the inverse table
    if ( linkPtr->targetClassID() != SOURCE::classID() ) {
      throw GaudiException( "Incompatible SOURCE type for location " + m_location + " : Template classID is " +
                                std::to_string( SOURCE::classID() ) + " expected " +
                                std::to_string( links->targetClassID() ),
                            "LinkerTool", StatusCode::FAILURE );
    }
    if ( linkPtr->sourceClassID() != TARGET::classID() ) {
      throw GaudiException( "Incompatible TARGET type for location " + m_location + " : Template classID is " +
                                std::to_string( TARGET::classID() ) + " expected " +
                                std::to_string( links->sourceClassID() ),
                            "LinkerTool", StatusCode::FAILURE );
    }
    return &m_invTable;
  }

private:
  SmartIF<IDataProviderSvc> m_evtSvc;
  std::string               m_location;
  std::string               m_invLocation;
  DirectType                m_table;
  InverseType               m_invTable;
};
#endif // LINKER_LINKERTOOL_H
