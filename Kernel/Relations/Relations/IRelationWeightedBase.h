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
#ifndef RELATIONS_IRELATIONWEIGHTEDBASE_H
#define RELATIONS_IRELATIONWEIGHTEDBASE_H 1
// ============================================================================
// Include files
// local
#include "Relations/IRelationBase.h"

/** @class IRelationWeightedBase Relations/IRelationWeightedBase.h
 *
 *  The helper base class for weighted relations
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2004-01-14
 */
class IRelationWeightedBase : public IRelationBase {
public:
  /** the unique interface ID (static)
   *  @return the unique interface identifier
   */
  static const InterfaceID& interfaceID();
};

// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_IRELATIONWEIGHTEDBASE_H
