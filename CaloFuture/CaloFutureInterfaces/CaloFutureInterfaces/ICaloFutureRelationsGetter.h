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
#ifndef ICALOFUTURERELATIONSGETTER_H
#define ICALOFUTURERELATIONSGETTER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

#include "Event/Track.h"
// Forward dec'
template <class FROM, class TO, class WEIGHT>
class IRelationWeighted;
template <class FROM, class TO, class WEIGHT>
class IRelationWeighted2D;
template <class FROM, class TO>
class IRelation;

namespace LHCb {
  class CaloHypo;
} // namespace LHCb

/** @class ICaloFutureRelationsGetter ICaloFutureRelationsGetter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2013-10-04
 */
struct ICaloFutureRelationsGetter : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureRelationsGetter, 3, 0 );

  virtual const IRelationWeighted2D<LHCb::Track, LHCb::CaloHypo, float>*
  getTrHypoTable2D( std::string location ) const = 0;
};
#endif // ICALOFUTURERELATIONSGETTER_H
