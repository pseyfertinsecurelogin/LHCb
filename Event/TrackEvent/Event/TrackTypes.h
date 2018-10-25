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
//------------------------------------------------------------------------------
/** @file TrackTypes.h
 *
 *  Track based typedefs
 *
 *  @author M. Needham
 *  @date   2005-1-09
 */

#ifndef EVENT_TRACKTYPES_H
#define EVENT_TRACKTYPES_H 1

// Include files
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"

namespace Gaudi
{
  // NB : Please remember to give a simple doxygen comment for each tyedef
  using TrackVector = Vector5 ;            ///< Vector type for Track
  using TrackSymMatrix = SymMatrix5x5 ;    ///< Symmetrix matrix type for Track
  using TrackMatrix = Matrix5x5 ;          ///< Matrix type for Track
  using TrackProjectionMatrix = Matrix1x5; ///< Projection matrix type for Track

}

#endif //  EVENT_TRACKTYPES_H
