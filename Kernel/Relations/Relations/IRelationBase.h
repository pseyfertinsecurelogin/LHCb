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
#ifndef RELATIONS_IRELATIONBASE_H
#define RELATIONS_IRELATIONBASE_H 1
// ============================================================================
// Include files
// from STL
#include <string>
// from Gaudi
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/System.h"

/** @class IRelationBase IRelationBase.h Relations/IRelationBase.h
 *
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2004-01-14
 */
class IRelationBase : public IInterface {
public:
  /** rebuild/resort  existing relations
   *
   *  @return status code
   *
   */
  virtual StatusCode rebuild() = 0;

  /** remove ALL relations from ALL to ALL objects
   *
   *  @return status code
   */
  virtual StatusCode clear() = 0;

  /** the unique interface ID (static)
   *  @return the unique interface identifier
   */
  static const InterfaceID& interfaceID();
};

// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_IRELATIONBASE_H
