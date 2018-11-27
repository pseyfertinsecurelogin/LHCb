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
#ifndef ICALOCOSMICSTRACKTOOL_H 
#define ICALOCOSMICSTRACKTOOL_H 1

// Include files

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "Event/Track.h"

// forward declarations
struct ICaloCosmicsTool;


/** @class ICaloCosmicsTrackTool ICaloCosmicsTrackTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2008-05-17
 */
struct ICaloCosmicsTrackTool : public extend_interfaces<IAlgTool> {

  DeclareInterfaceID( ICaloCosmicsTrackTool, 2, 0 );

  virtual StatusCode processing()=0;
  virtual StatusCode tupling(unsigned int unit)=0;
  //virtual  const double time()=0;
  virtual double phi()=0;
  virtual double phiVariance()=0;
  virtual double theta()=0;
  virtual double thetaVariance()=0;
  virtual const Gaudi::XYZVector slopes()=0;
  virtual const Gaudi::XYZPoint  referencePoint()=0;
  virtual const Gaudi::SymMatrix3x3 slopesCovariance()=0;
  virtual const Gaudi::SymMatrix3x3 referencePointCovariance()=0;
  virtual bool   forward()=0;
  virtual bool   tracked()=0;
  virtual double time()=0;
  virtual double timeVariance()=0;
  virtual bool   timed()=0;  
  virtual const LHCb::Track& track()=0;
  virtual ICaloCosmicsTool* ecal()=0;
  virtual ICaloCosmicsTool* hcal()=0;

  virtual StatusCode propagate(Gaudi::Plane3D plane)=0;
  virtual StatusCode propagate(double z )=0;


};
#endif // ICALOCOSMICSTRACKTOOL_H
