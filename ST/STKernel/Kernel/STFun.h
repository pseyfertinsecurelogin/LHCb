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
#ifndef _STKernel_STFun_H
#define _STKernel_STFun_H

#include "Event/STCluster.h"
#include "Event/STDigit.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/PPRepresentation.h"
#include "Kernel/StripRepresentation.h"
#include "STBitsPolicy.h"

#include <utility>

namespace STFun {

  /** quantize the interstrip fraction  from 'float' */
  double stripFraction( const double interStripPos );

  /**
   * calculate mean strip
   * @param  digits vector of digits
   * @return mean strip
   */
  std::pair<double, unsigned int> position( const SmartRefVector<LHCb::STDigit>& digits, double trim = 0 );

  /**
   * calculate mean strip
   * @param  strips vector of strip values
   * @return mean strip
   */
  std::pair<double, unsigned int> position( const LHCb::STCluster::ADCVector& strips, double trim = 0 );

} // namespace STFun

inline double STFun::stripFraction( const double interStripPos ) {
  return ( LHCb::Math::round( ( 1 << STBitsPolicy::interStripPrecision ) * interStripPos ) ) /
         double( 1 << STBitsPolicy::interStripPrecision );
}

#endif //_STKernel_STFun_H
