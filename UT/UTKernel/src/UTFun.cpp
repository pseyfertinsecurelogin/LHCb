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

std::pair<double, unsigned int> UTFun::position( const SmartRefVector<LHCb::UTDigit>& digits, double trim ) {
  double                                        maxCharge = 0.0;
  SmartRefVector<LHCb::UTDigit>::const_iterator iterDigit = digits.begin();
  for ( ; iterDigit != digits.end(); ++iterDigit ) {
    if ( ( *iterDigit )->depositedCharge() > maxCharge ) maxCharge = ( *iterDigit )->depositedCharge();
  } // iterDigit

  const double trimVal = trim * maxCharge;

  unsigned int nUsed         = 0;
  double       trimmedCharge = 0.0;
  double       firstMoment   = 0.0;
  iterDigit                  = digits.begin();
  for ( ; iterDigit != digits.end(); ++iterDigit ) {
    if ( ( *iterDigit )->depositedCharge() >= trimVal ) {
      firstMoment += ( ( *iterDigit )->depositedCharge() * (double)( *iterDigit )->channelID().strip() );
      trimmedCharge += ( *iterDigit )->depositedCharge();
      ++nUsed;
    }
  } // iterDigit

  // mean u
  return std::make_pair( firstMoment / trimmedCharge, nUsed );
}

std::pair<double, unsigned int> UTFun::position( const LHCb::UTCluster::ADCVector& strips, double trim ) {
  // mean u - weighted according to charge dep
  double firstMoment   = 0.0;
  double trimmedCharge = 0;

  // mean
  double                                     maxCharge = 0.0;
  LHCb::UTCluster::ADCVector::const_iterator iter      = strips.begin();
  while ( iter != strips.end() ) {
    if ( iter->second > maxCharge ) maxCharge = iter->second;
    ++iter;
  }
  const unsigned int trimVal = int( trim * maxCharge );

  // calc trimmed mean
  unsigned int i     = 0;
  unsigned int nUsed = 0;
  iter               = strips.begin();
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
  return std::make_pair( firstMoment / trimmedCharge, nUsed );
}
