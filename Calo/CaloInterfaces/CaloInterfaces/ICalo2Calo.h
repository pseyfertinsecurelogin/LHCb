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
// ============================================================================
#ifndef ICALO2CALO_H
#define ICALO2CALO_H 1
// ============================================================================
// Include files
// ============================================================================
// STL
#include <string>
// ============================================================================
#include "Kernel/CaloCellID.h"
// ============================================================================
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
// ============================================================================
// from Gaudi
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// forward declaration
// ============================================================================
// ============================================================================
/** @class ICalo2Calo ICalo2Calo.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-05-29
 */
struct ICalo2Calo : public extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( ICalo2Calo, 4, 0 );

  virtual void setCalos( const std::string& from, const std::string& to ) = 0;

  virtual const std::vector<LHCb::CaloCellID>& cellIDs( const LHCb::CaloCluster& fromCluster,
                                                        const std::string&       toCalo )                                = 0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs( const LHCb::CaloCellID& fromId, const std::string& toCalo,
                                                        bool init = true )                                         = 0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs()                                                           = 0;
  virtual const std::vector<LHCb::CaloDigit*>& digits( const LHCb::CaloCellID& fromId, const std::string& toCalo ) = 0;
  virtual const std::vector<LHCb::CaloDigit*>& digits( const LHCb::CaloCluster& fromCluster,
                                                       const std::string&       toCalo )                                 = 0;
  virtual const std::vector<LHCb::CaloDigit*>& digits()                                                            = 0;
  //
  virtual double energy( const LHCb::CaloCellID& fromId, const std::string& toCalo )             = 0;
  virtual double energy( const LHCb::CaloCluster& fromCluster, const std::string& toCalo )       = 0;
  virtual double energy()                                                                        = 0;
  virtual int    multiplicity( const LHCb::CaloCellID& fromId, const std::string& toCalo )       = 0;
  virtual int    multiplicity( const LHCb::CaloCluster& fromCluster, const std::string& toCalo ) = 0;
  virtual int    multiplicity()                                                                  = 0;

  virtual bool isLocalMax( const LHCb::CaloDigit& digit ) = 0;
};
#endif // ICALO2CALO_H
