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

//================================================================
/** @file BoostArray.h
 *
 *  Header file for boost::array + Additional Gaudi methods
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-07-31
 */
//================================================================

#pragma once

// STL
#include <iostream>

// Boost
//#define BOOST_DISABLE_ASSERTS
#include "boost/array.hpp"

// Gaudi
//#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ToStream.h"

/// Implement textual std::ostream << method for boost::array's
template < class TYPE, std::size_t N >
inline std::ostream &
operator<<( std::ostream &s, const boost::array< TYPE, N > &data )
{
  return Gaudi::Utils::toStream( data.begin(), data.end(), s, "[ ", " ]", " , " );
}
