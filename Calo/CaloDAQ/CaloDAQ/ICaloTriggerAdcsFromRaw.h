// $Id: ICaloTriggerAdcsFromRaw.h,v 1.3 2007-02-22 23:39:52 odescham Exp $
#ifndef CALODAQ_ICALOTRIGGERADCSFROMRAW_H 
#define CALODAQ_ICALOTRIGGERADCSFROMRAW_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Event/L0CaloAdc.h"
#include "Event/RawEvent.h"

static const InterfaceID IID_ICaloTriggerAdcsFromRaw ( "ICaloTriggerAdcsFromRaw", 1, 0 );

/** @class ICaloTriggerAdcsFromRaw ICaloTriggerAdcsFromRaw.h CaloDAQ/ICaloTriggerAdcsFromRaw.h
 *  INterface for trigger adc decoding
 *
 *  @author Olivier Callot
 *  @date   2005-01-05
 */
class ICaloTriggerAdcsFromRaw : virtual public IAlgTool {
public: 

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_ICaloTriggerAdcsFromRaw; }

  virtual std::vector<LHCb::L0CaloAdc>&    adcs( ) = 0;
  virtual std::vector<LHCb::L0CaloAdc>&    adcs( int source ) = 0;
  virtual std::vector<LHCb::L0CaloAdc>&    adcs( LHCb::RawBank* bank ) = 0;
  virtual std::vector<LHCb::L0CaloAdc>&    pinAdcs( ) = 0;
  //
  virtual StatusCode  getCaloBanks()=0;
  virtual void setBanks(const std::vector<LHCb::RawBank*>* bank ) = 0;

protected:

private:

};
#endif // CALODAQ_ICALOTRIGGERADCSFROMRAW_H
