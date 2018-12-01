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
#ifndef DETCONDEXAMPLE_DETDATAAGENT_H
#define DETCONDEXAMPLE_DETDATAAGENT_H

#include "GaudiKernel/IDataStoreAgent.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/MsgStream.h"

#include "DetDesc/Condition.h"

class IMessageSvc;

///---------------------------------------------------------------------------
/** @class DetDataAgent DetDataAgent.h DetCondExample/DetDataAgent.h

    Simple agent to dump the contents of DetectorDataSvc data store.
    Copied from Ex/DetDescExample.

    @date December 2001
*///--------------------------------------------------------------------------

class DetDataAgent : virtual public IDataStoreAgent {

 public:

  DetDataAgent( IMessageSvc* msgSvc, bool dumpConditions = false ) :
    m_msgSvc( msgSvc ), m_dumpCond( dumpConditions ) {}

  virtual ~DetDataAgent() {}

  bool analyse(IRegistry* dir, int level ) override;

 private:

  IMessageSvc* m_msgSvc;
  bool m_dumpCond;

};

//---------------------------------------------------------------------------

bool DetDataAgent::analyse(IRegistry* dir, int /*level*/ ){
  MsgStream log( m_msgSvc, "detDataAgent" );
  log << MSG::INFO << dir->identifier();
  if ( dir->object() ) {
    log << " [loaded] (clId = " << dir->object()->clID() << ")";
    if (m_dumpCond) {
      ParamValidDataObject * pvdo = dynamic_cast<ParamValidDataObject *>(dir->object());
      if (pvdo) log << "\n" << pvdo->printParams();
    }
  }
  log << endmsg;
  return true;
}

//---------------------------------------------------------------------------

#endif // DETCONDEXAMPLE_DETDATAAGENT_H
