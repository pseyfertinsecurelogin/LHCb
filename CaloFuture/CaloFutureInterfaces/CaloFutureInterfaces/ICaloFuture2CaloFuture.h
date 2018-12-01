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
#ifndef ICALOFUTURE2CALOFUTURE_H 
#define ICALOFUTURE2CALOFUTURE_H 1
// ============================================================================
// Include files
// ============================================================================
// STL
#include <string>
// ============================================================================
#include "Kernel/CaloCellID.h"
// ============================================================================
#include "Event/CaloDigit.h"
#include "Event/CaloCluster.h"
// ============================================================================
// from Gaudi
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// forward declaration
// ============================================================================
// ============================================================================
/** @class ICaloFuture2CaloFuture ICaloFuture2CaloFuture.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2007-05-29
 */
struct ICaloFuture2CaloFuture : public extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFuture2CaloFuture, 4, 0 );

  virtual void setCalos( const std::string& from , 
                         const std::string& to   ) = 0 ;
  
  virtual const std::vector<LHCb::CaloCellID>& cellIDs
  ( const LHCb::CaloCluster& fromCluster, const std::string& toCaloFuture)=0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs
  ( const LHCb::CaloCellID&  fromId     , const std::string& toCaloFuture, bool init=true)=0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs()=0;
  virtual const std::vector<LHCb::CaloDigit*>& digits 
  ( const LHCb::CaloCellID&  fromId     , const std::string& toCaloFuture)=0;
  virtual const std::vector<LHCb::CaloDigit*>& digits 
  ( const LHCb::CaloCluster& fromCluster, const std::string& toCaloFuture)=0;
  virtual const std::vector<LHCb::CaloDigit*>& digits ()=0;
  //
  virtual double energy
  ( const LHCb::CaloCellID&  fromId     , const std::string& toCaloFuture ) =0 ;
  virtual double energy
  ( const LHCb::CaloCluster& fromCluster, const std::string& toCaloFuture)=0;
  virtual double energy()=0;
  virtual int multiplicity
  ( const LHCb::CaloCellID&  fromId      , const std::string& toCaloFuture)=0;
  virtual int multiplicity
  ( const LHCb::CaloCluster& fromCluster , const std::string& toCaloFuture)=0;
  virtual int multiplicity()=0;


  virtual bool isLocalMax( const LHCb::CaloDigit& digit)=0;

};
#endif // ICALOFUTURE2CALOFUTURE_H
