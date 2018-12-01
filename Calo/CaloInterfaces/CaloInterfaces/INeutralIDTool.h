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
#ifndef INEUTRALIDTOOL_H 
#define INEUTRALIDTOOL_H 1

// Include files
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Event/CaloHypo.h"
#include "ICaloHypoEstimator.h"

/** @class INeutralIDTool INeutralIDTool.h
 *  @author Mostafa HOBALLAH --
 *  @date   2013-07-25
 */
struct INeutralIDTool : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID(INeutralIDTool, 3, 0 );

  virtual double isNotE(const LHCb::CaloHypo* hypo,ICaloHypoEstimator* e=nullptr)=0;
  virtual double isNotH(const LHCb::CaloHypo* hypo,ICaloHypoEstimator* e=nullptr)=0;

  virtual double isNotE(const double* v)=0;
  virtual double isNotH(const double* v)=0;

  virtual void Variables(const LHCb::CaloHypo* hypo,  double& clmatch, double& prse, 
                         double& e19, double& hclecl, double& prse19,double& prse49, double& sprd, double& prse4mx, 
                         double& prsm, double& spdm)=0;

  
};
#endif // INEUTRALIDTOOL_H
