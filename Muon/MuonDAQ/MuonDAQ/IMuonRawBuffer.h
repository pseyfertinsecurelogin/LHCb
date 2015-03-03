// $Id: IMuonRawBuffer.h,v 1.5 2007-11-28 14:48:15 cattanem Exp $
#ifndef MUONDAQ_IMUONRAWBUFFER_H 
#define MUONDAQ_IMUONRAWBUFFER_H 1

// Include files
#include "Kernel/MuonTileID.h"

// GaudiKernel
#include "GaudiKernel/IAlgTool.h"

#include <vector>
#include <utility>


/** @class IMuonRawBuffer IMuonRawBuffer.h MuonTools/IMuonRawBuffer.h
 *  
 *  Interface for the tools to convert MuonTileID to coordinates 
 *
 *  @author David Hutchcroft
 *  @date   11/03/2002
 */
static const InterfaceID IID_IMuonRawBuffer( "IMuonRawBuffer" , 1 , 0 );

class IMuonRawBuffer : public virtual IAlgTool {
public:

  /** static interface identification
   *  @return unique interface identifier
   */
  static const InterfaceID& interfaceID() { 
    return  IID_IMuonRawBuffer; 
  }
  virtual std::vector<LHCb::MuonTileID> getTile()=0;
  virtual std::vector<std::pair<LHCb::MuonTileID,unsigned int> > getTileAndTDC()=0;
  virtual std::vector<LHCb::MuonTileID> getPads()=0;
  

protected:

private:

};
#endif // MUONDAQ_IMUONRAWBUFFER_H
