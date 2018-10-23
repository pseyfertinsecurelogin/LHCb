/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef ICALOHYPO2CALO_H 
#define ICALOHYPO2CALO_H 1

// Include files
// from STL
#include <string>

#include "Kernel/CaloCellID.h"
#include "Event/CaloDigit.h"
#include "Event/CaloCluster.h"
#include "Event/CaloHypo.h"
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// forward declaration


/** @class ICaloHypo2Calo ICaloHypo2Calo.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2008-09-09
 */


struct ICaloHypo2Calo : extend_interfaces<IAlgTool> 
{

  // Return the interface ID
  DeclareInterfaceID( ICaloHypo2Calo, 2, 0 );

  virtual void setCalos(const std::string &from, const std::string &to)=0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs(const LHCb::CaloHypo     &fromHypo,    const std::string   &toCalo)=0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs(const LHCb::CaloCluster  &fromCluster, const std::string   &toCalo)=0;
  virtual const std::vector<LHCb::CaloCellID>& cellIDs()=0;

  virtual const std::vector<LHCb::CaloDigit*>& digits(const LHCb::CaloCluster &fromCluster, const std::string   &toCalo)=0;
  virtual const std::vector<LHCb::CaloDigit*>& digits(const LHCb::CaloHypo    &fromHypo,    const std::string   &toCalo)=0;
  virtual const std::vector<LHCb::CaloDigit*>& digits()=0;

  virtual double energy(const LHCb::CaloCluster  &fromCluster, const std::string   &toCalo)=0;
  virtual double energy(const LHCb::CaloHypo     &fromHypo,    const std::string   &toCalo)=0;
  virtual double energy()=0;

  virtual int multiplicity(const LHCb::CaloCluster  &fromCluster, const std::string   &toCalo)=0;
  virtual int multiplicity(const LHCb::CaloHypo     &fromHypo,    const std::string   &toCalo)=0;
  virtual int multiplicity()=0;
  virtual StatusCode _setProperty(const std::string&, const std::string&)=0;

};
#endif // ICALOHYPO2CALO_H
