// $Id: ITsaCollector.h,v 1.1 2006-09-05 15:38:19 mneedham Exp $
#ifndef _ITsaCollector_H
#define _ITsaCollector_H

/** @class ITsaCollector ITsaCollector Tsa/ITsaCollector.h
 *
 *  Interface to isolation tool
 *
 *  @author M.Needham
 *  @date   30/04/2004
 */

#include "GaudiKernel/IAlgTool.h"
#include "TsaKernel/Cluster.h"
#include "TsaKernel/OTCluster.h"
#include "TsaKernel/STCluster.h"

namespace LHCb{
  class State;
}

static const InterfaceID IID_ITsaCollector( "ITsaCollector", 0, 0 );

class ITsaCollector: virtual public IAlgTool{

public:


  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITsaCollector ; }

  /// the method
  virtual StatusCode collect(const LHCb::State& aState,
                             Tsa::Clusters* clusters) const = 0;


 virtual StatusCode collect(const LHCb::State& aState, 
                            Tsa::STClusters* clusters) const = 0;


 virtual StatusCode collect(const LHCb::State& aState, 
                            Tsa::OTClusters* clusters) const = 0;


};

#endif
