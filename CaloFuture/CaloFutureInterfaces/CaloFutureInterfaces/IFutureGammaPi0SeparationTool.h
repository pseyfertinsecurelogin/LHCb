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
#ifndef IFUTUREGAMMAPI0SEPARATIONTOOL_H
#define IFUTUREGAMMAPI0SEPARATIONTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "Event/CaloHypo.h"
#include "GaudiKernel/IAlgTool.h"

/** @class IFutureGammaPi0SeparationTool IFutureGammaPi0SeparationTool.h
 *
 *
 *  @author Miriam Calvo Gomez --
 *  @date   2019-03-28
 */
struct IFutureGammaPi0SeparationTool : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IFutureGammaPi0SeparationTool, 4, 0 );

  virtual double isPhoton( const LHCb::CaloHypo* hypo )                                     = 0;
  virtual double isPhoton( const double* v )                                                = 0;
  virtual bool ClusterVariables( const LHCb::CaloHypo* hypo, double& fr2, double& fasym, double& fkappa, double& fr2r4,
                                 double& etot, double& Eseed, double& E2, int& area )       = 0;

  virtual double                        inputData( std::string data ) = 0;
  virtual std::map<std::string, double> inputDataMap()                = 0;
};
#endif // IFUTUREGAMMAPI0SEPARATIONTOOL_H
