// $Id: ITTDataSvc.h,v 1.2 2006-12-06 14:35:19 mneedham Exp $
#ifndef _ITTDataSvc_H
#define _ITTDataSvc_H

/** @class ITTDataSvc ITTDataSvc Tsa/ITTDataSvc.h
 *
 *  Interface to data svc
 *
 *  @author M.Needham
 *  @date   30/04/2004
 */

#include "GaudiKernel/IAlgTool.h"
#include "TsaKernel/Types.h"

#include "TsaKernel/STCluster.h"

static const InterfaceID IID_ITTDataSvc( "ITTDataSvc", 0, 0 );

class ITTDataSvc: virtual public IAlgTool 
{
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITTDataSvc ; }

  /// event-by-event init
  virtual StatusCode initializeEvent() = 0;

  // init event
  virtual StatusCode initializeEvent(Tsa::STClusters* clusters) = 0;

  virtual Tsa::STRange layer(const int& iStation, const int& iLayer) const = 0;
  
  virtual unsigned int dataSize() const = 0;                                        

};

#endif
