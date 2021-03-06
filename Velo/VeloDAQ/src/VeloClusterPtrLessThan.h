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
#ifndef VELODAQ_VELOCLUSTERPTRLESSTHAN_H
#define VELODAQ_VELOCLUSTERPTRLESSTHAN_H 1

#include "Event/VeloCluster.h"
#include "VeloEvent/InternalVeloCluster.h"

namespace VeloDAQ {
  /**  Compare two VeloClusters or two InternalVeloClusters
   *   via pointers.
   *
   *   This function defines a less-than relation between two
   *   clusters. The lhs cluster is less than the rhs cluster
   *   if the lhs sensor number is lower than the rhs sensor number.
   *   In case of equal sensor numbers, the first strip number
   *   decides.
   *
   *   I'd very much prefer to template this. But that requires a
   *   change in the InternalVeloCluster interface.
   *
   *   @see VeloCluster
   *   @see InternalVeloCluster
   *   @author Kurt Rinnert
   *   @date   2011-09-20
   */
  class veloClusterPtrLessThan {

  public:
    inline bool operator()( const LHCb::VeloCluster* lhs, const LHCb::VeloCluster* rhs ) const;

    inline bool operator()( const LHCb::InternalVeloCluster* lhs, const LHCb::InternalVeloCluster* rhs ) const;
  };

  bool veloClusterPtrLessThan::operator()( const LHCb::InternalVeloCluster* lhs,
                                           const LHCb::InternalVeloCluster* rhs ) const {
    const LHCb::VeloChannelID& lhsId = lhs->channelID( 0 );
    const LHCb::VeloChannelID& rhsId = rhs->channelID( 0 );

    return ( lhsId.sensor() == rhsId.sensor() ? lhsId.strip() < rhsId.strip() : lhsId.sensor() < rhsId.sensor() );
  }

  bool veloClusterPtrLessThan::operator()( const LHCb::VeloCluster* lhs, const LHCb::VeloCluster* rhs ) const {
    const LHCb::VeloChannelID& lhsId = lhs->channelID();
    const LHCb::VeloChannelID& rhsId = rhs->channelID();

    return ( lhsId.sensor() == rhsId.sensor() ? lhsId.strip() < rhsId.strip() : lhsId.sensor() < rhsId.sensor() );
  }
} // namespace VeloDAQ
#endif // VELOCLUSTERPTRLESSTHAN_H
