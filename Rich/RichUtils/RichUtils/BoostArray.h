
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
