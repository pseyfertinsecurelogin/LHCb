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
#ifndef ICALOFUTUREHYPO2CALOFUTURE_H
#define ICALOFUTUREHYPO2CALOFUTURE_H 1

// Include files
// from STL
#include <string>

#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/CaloHypo.h"
#include "Kernel/CaloCellID.h"
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// forward declaration

/** @class ICaloFutureHypo2CaloFuture ICaloFutureHypo2CaloFuture.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-09-09
 */

struct ICaloFutureHypo2CaloFuture : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureHypo2CaloFuture, 2, 0 );

  virtual void                                 setCalos( const std::string& from, const std::string& to ) = 0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs( const LHCb::CaloHypo& fromHypo,
                                                        const std::string&    toCaloFuture )                 = 0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs( const LHCb::CaloCluster& fromCluster,
                                                        const std::string&       toCaloFuture )                 = 0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs()                                                  = 0;

  virtual const std::vector<LHCb::CaloDigit*>& digits( const LHCb::CaloCluster& fromCluster,
                                                       const std::string&       toCaloFuture ) = 0;
  virtual const std::vector<LHCb::CaloDigit*>& digits( const LHCb::CaloHypo& fromHypo,
                                                       const std::string&    toCaloFuture ) = 0;
  virtual const std::vector<LHCb::CaloDigit*>& digits()                                  = 0;

  virtual double energy( const LHCb::CaloCluster& fromCluster, const std::string& toCaloFuture ) = 0;
  virtual double energy( const LHCb::CaloHypo& fromHypo, const std::string& toCaloFuture )       = 0;
  virtual double energy()                                                                        = 0;

  virtual int        multiplicity( const LHCb::CaloCluster& fromCluster, const std::string& toCaloFuture ) = 0;
  virtual int        multiplicity( const LHCb::CaloHypo& fromHypo, const std::string& toCaloFuture )       = 0;
  virtual int        multiplicity()                                                                        = 0;
  virtual StatusCode _setProperty( const std::string&, const std::string& )                                = 0;
};
#endif // ICALOFUTUREHYPO2CALOFUTURE_H
