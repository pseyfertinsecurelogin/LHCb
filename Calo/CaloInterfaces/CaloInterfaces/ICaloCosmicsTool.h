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
#ifndef ICALOCOSMICSTOOL_H 
#define ICALOCOSMICSTOOL_H 1

// Include files
// from STL
#include <utility>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Point3DTypes.h"

// forward declarations
class DeCalorimeter;


/** @class ICaloCosmicsTool ICaloCosmicsTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2008-04-07
 */
struct ICaloCosmicsTool : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( ICaloCosmicsTool, 2, 0 );

  virtual StatusCode processing()=0;

  virtual Gaudi::XYZPoint referencePoint()=0;
  virtual Gaudi::XYZPoint referencePointVariance()=0;
  virtual const std::pair<Gaudi::XYZPoint,Gaudi::XYZPoint>& extrema()=0;
  virtual double deposit()=0;
  virtual double phi()=0;
  virtual double phiVariance()=0;
  virtual double asymmetry()=0;
  virtual double asymmetryVariance()=0;
  virtual double slot()=0;
  virtual double time()=0;
  virtual double timeVariance()=0;
  virtual double timeDispersion()=0;
  virtual DeCalorimeter* det()=0;
  virtual StatusCode tupling(unsigned int unit)=0;
  virtual bool tracked()=0;
  virtual bool timed()=0;
  virtual double kernel()=0;
 

};
#endif // ICALOCOSMICSTOOL_H
