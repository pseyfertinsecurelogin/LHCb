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
#ifndef RateFromCondDB_H
#define RateFromCondDB_H 1

// CondDB
#include "DetDesc/Condition.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"

//#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/IRateFromTCK.h"
#include "Kernel/TCK.h"
static const InterfaceID IID_RateFromCondDB( "RateFromCondDB", 1, 0 ); // needed as there's no interface

/** @class RateFromCondDB RateFromCondDB.h
 *
 *  Get rate of a given rate limiter for the Condition Database
 *
 *  @sa OfflineRateLimiter
 *
 * @todo This could implement the same interface as @sa RateFromTCK
 *
 *  @author Patrick Koppenburg
 *  @date   2012-05-09
 */
class RateFromCondDB final : public GaudiTool
//, virtual public IRateFromCondDB
{

public:
  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_RateFromCondDB; }; // needed as there's no interface

  /// Standard constructor
  RateFromCondDB( const std::string& type, const std::string& name, const IInterface* parent );

  bool initializeCondDB();

  /// Get rate from Conditions Database
  double getRate() const;

private:
  StatusCode i_updateConditions(); ///< Function extracting data from Condition

private:
  Condition*  m_condTrigger = nullptr; ///< Condition for sampling coefficients
  double      m_rate;                  ///< rate in condDB
  std::string m_condition;             ///< ConditionName
  std::string m_lineRate;              ///< Line Rate name in CondDB
  double      m_unit;                  ///< Unit for rate
};

#endif // RateFromCondDB
