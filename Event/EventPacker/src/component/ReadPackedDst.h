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
#ifndef READPACKEDDST_H
#define READPACKEDDST_H 1

#include "Event/RawBank.h"
#include "Kernel/STLExtensions.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/IGenericTool.h"
#include "GaudiKernel/IAddressCreator.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/LinkManager.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/ToolHandle.h"

/** @class ReadPackedDst ReadPackedDst.h
 *  This is the unpacker of DAQEVent containing the DST data
 *
 *  @author Olivier Callot
 *  @date   2009-01-19
 */
class ReadPackedDst : public GaudiAlgorithm {

public:
  /// Standard constructor
  ReadPackedDst( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode finalize() override;   ///< Algorithm finalization
  StatusCode execute() override;    ///< Algorithm execution

protected:
  //== extract a string from the buffer
  std::string stringFromData() {
    unsigned int len = *m_data++;
    m_size--;
    if ( 0 == len ) return "";
    len = len / 4 + 1;
    std::string value( (char*)m_data );
    m_data += len;
    m_size -= len;
    return value;
  }

  //== extract an int from the buffer
  int nextInt() {
    m_size--;
    return *m_data++;
  }

  //== Extract the linkMgr information and set the version
  void processLinks( DataObject* obj, unsigned char version ) {
    DataObject*  dummy   = 0;
    unsigned int nbLinks = *m_data++;
    m_size--;
    for ( unsigned int kk = 0; nbLinks > kk; ++kk ) {
      std::string name = stringFromData();
      obj->linkMgr()->addLink( name, dummy );
    }
    obj->setVersion( version );
  }

  //== Fill a vector of objects CLASS from the buffer and the blobs.
  template <class CLASS>
  void getFromBlob( std::vector<CLASS>& vect, LHCb::span<const LHCb::RawBank*> blobs );

private:
  std::string              m_inputLocation;
  std::string              m_postfix;
  const unsigned int*      m_data = nullptr;
  int                      m_size{0};
  ToolHandle<IGenericTool> m_odinDecoder;
  SmartIF<IAddressCreator> m_addrCreator;
  SmartIF<IDataManagerSvc> m_evtMgr;
};
#endif // READPACKEDDST_H
