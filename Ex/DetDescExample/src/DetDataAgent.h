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
#ifndef DDEXAMPLE_DETDATAAGENT_H
#define DDEXAMPLE_DETDATAAGENT_H

#include "GaudiKernel/IDataStoreAgent.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/IRegistry.h"

class IMessageSvc;

/// Data agent class for the Detector Store
class DetDataAgent : virtual public IDataStoreAgent {
public:
  DetDataAgent( IMessageSvc* msgSvc ) : m_msgSvc( msgSvc ) {}
  virtual ~DetDataAgent() {}
  bool analyse(IRegistry* dir, int level ) override;
private:
  IMessageSvc* m_msgSvc;
};

inline bool DetDataAgent::analyse(IRegistry* dir, int /*level*/ ){
  MsgStream log( m_msgSvc, "detDataAgent" );
  log << MSG::INFO << dir->identifier()
      << ( dir->object() ? " [loaded] " : " " )
      << endmsg;
  return true;
}

#endif // DDEXAMPLE_DETDATAAGENT_H
