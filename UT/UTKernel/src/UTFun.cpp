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
#include "Kernel/UTFun.h"
#include <numeric>

std::pair<double, unsigned int> UTFun::position( const SmartRefVector<LHCb::UTDigit>& digits, double trim ) {
  const double maxCharge = std::accumulate( digits.begin(), digits.end(), 0.0, []( double mC, const auto& digi ) {
    return std::max( mC, digi->depositedCharge() );
  } );

  struct Sum {
    unsigned int nUsed         = 0;
    double       trimmedCharge = 0.0;
    double       firstMoment   = 0.0;
  };
  auto sum =
      std::accumulate( digits.begin(), digits.end(), Sum{}, [trimVal = trim * maxCharge]( Sum s, const auto& digit ) {
        if ( digit->depositedCharge() >= trimVal ) {
          s.firstMoment += ( digit->depositedCharge() * (double)digit->channelID().strip() );
          s.trimmedCharge += digit->depositedCharge();
          ++s.nUsed;
        }
        return s;
      } );

  // mean u
  return {sum.firstMoment / sum.trimmedCharge, sum.nUsed};
}

std::pair<double, unsigned int> UTFun::position( const LHCb::UTCluster::ADCVector& strips, double trim ) {

  // mean
  const unsigned int maxADC = std::accumulate(
      strips.begin(), strips.end(), 0u, []( unsigned int mC, const auto& adc ) { return std::max( mC, adc.second ); } );
  const unsigned int trimVal = int( trim * maxADC );

  // mean u - weighted according to charge dep
  double firstMoment   = 0.0;
  double trimmedCharge = 0;
  // calc trimmed mean
  unsigned int i     = 0;
  unsigned int nUsed = 0;
  auto         iter  = strips.begin();
  while ( iter != strips.end() ) {
    if ( iter->second >= trimVal ) {
      trimmedCharge += iter->second;
      firstMoment += iter->second * (double)i;
      ++nUsed;
    }
    ++iter;
    ++i;
  }

  // mean u
  return {firstMoment / trimmedCharge, nUsed};
}
