// $Id: IOTDataSvc.h,v 1.3 2006-09-05 15:38:19 mneedham Exp $
#ifndef _IOTDataSvc_H
#define _IOTDataSvc_H

/** @class IOTDataSvc IOTDataSvc Tsa/IOTDataSvc.h
 *
 *  Interface to data svc
 *
 *  @author M.Needham
 *  @date   30/04/2004
 */

#include "GaudiKernel/IAlgTool.h"
#include "TsaKernel/Types.h"

static const InterfaceID IID_IOTDataSvc( "IOTDataSvc", 0, 0 );

class IOTDataSvc: virtual public IAlgTool 
{
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_IOTDataSvc ; }

  /// event-by-event init
  virtual StatusCode initializeEvent() = 0;

  virtual  Tsa::OTRange layer(const int& iStation, const int& iLayer) const = 0;
  
  virtual  Tsa::OTRange partition(const int& iStation, const int& iLayer,
                                  const int& iSector) const = 0;
 

  virtual unsigned int dataSize() const  = 0; 

};

#endif
