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
#ifndef TrackInterfaces_IUTClusterPosition_H
#define TrackInterfaces_IUTClusterPosition_H

#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/SmartRefVector.h"

#include "Kernel/SiPositionInfo.h"

namespace LHCb{
class StateVector;
class UTChannelID;
class UTCluster;
class UTDigit;
}

/** @class IUTClusterPosition TrackInterfaces/IUTClusterPosition.h
 *
 *  Interface Class for charge sharing IT strip
 *
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */


struct  IUTClusterPosition: extend_interfaces<IAlgTool> {

  // Declaration of the interface ID ( interface id, major version, minor version)
  DeclareInterfaceID( IUTClusterPosition, 1 , 0);

  typedef LHCb::SiPositionInfo<LHCb::UTChannelID> Info;

  /** calc position
  * @param aCluster cluster
  * @return Info (simple struct)
  * <br> strip = floored nearest channel
  * <br> fractionStrip = interstrip position (in fraction of strip)
  * <br> error = estimate of the error
  */
  virtual Info estimate(const LHCb::UTCluster* aCluster) const=0;


  /** calc position
  * @param aCluster cluster
  * @param refVector Reference vector from the track
  * @return Info (simple struct)
  * <br> strip = floored nearest channel
  * <br> fractionStrip = interstrip position (in fraction of strip)
  * <br> error = estimate of the error
  */
  virtual Info estimate(const LHCb::UTCluster* aCluster,
                        const LHCb::StateVector& refVector) const=0;

  /** calc position
  * @param digits vector of digits
  * @return Measurement (pair of pairs)
  * <br> first.first = floored nearest channel
  * <br> first.second = interstrip position (in fraction of strip)
  * <br> second = estimate of the error
  */
  virtual Info estimate(const SmartRefVector<LHCb::UTDigit>& digits) const=0;

  /** error parameterized as cluster size
  * @param nStrips number of strips
  * @return error estimate
  */
  virtual double error(const unsigned int nStrips) const =0;

  /** error parameterized as cluster size
  * @param nStrips number of strips
  * @param refVector Reference vector from the track
  * @return error estimate
  */
  virtual double error(const unsigned int nStrips,
                       const LHCb::StateVector& refVector) const =0;

};

#endif // TrackInterfaces_IUTClusterPosition_H
