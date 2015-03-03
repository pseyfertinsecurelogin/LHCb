// $Id: IITDataSvc.h,v 1.4 2006-12-06 14:35:19 mneedham Exp $
#ifndef _IITDataSvc_H
#define _IITDataSvc_H

/** @class IITDataSvc IITDataSvc Tsa/IITDataSvc.h
 *
 *  Interface to data svc
 *
 *  @author M.Needham
 *  @date   30/04/2004
 */

#include "GaudiKernel/IAlgTool.h"
#include "TsaKernel/Types.h"

#include "TsaKernel/STCluster.h"

static const InterfaceID IID_IITDataSvc( "IITDataSvc", 0, 0 );

class IITDataSvc: virtual public IAlgTool 
{
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_IITDataSvc ; }

  /// event-by-event init
  virtual StatusCode initializeEvent() = 0;

  // init event
  virtual StatusCode initializeEvent(Tsa::STClusters* clusters) = 0;

  virtual Tsa::STRange layer(const int& iStation, const int& iLayer) const = 0;
  
  virtual  Tsa::STRange partition(const int& iStation, const int& iLayer,
                                  const int& iBox) const = 0;

  virtual unsigned int dataSize() const = 0;                                        

};

#endif
