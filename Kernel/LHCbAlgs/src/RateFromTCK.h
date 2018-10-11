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
#ifndef RATEFROMTCK_H
#define RATEFROMTCK_H 1

// CondDB
#include "DetDesc/Condition.h"
#include "GaudiKernel/IDetDataSvc.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/IRateFromTCK.h"            // Interface

#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"
#include "GaudiKernel/SmartDataPtr.h"
/** @class RateFromTCK RateFromTCK.h
 *
 *  Get rate of a given rate limiter for the present TCK
 *
 *  @author Patrick Koppenburg
 *  @date   2011-08-10
 */

class RateFromTCK final : public extends<GaudiTool, IRateFromTCK>
{

public:

  /// Standard constructor
  using extends<GaudiTool, IRateFromTCK>::extends;

  StatusCode initialize() override;

 public:

  inline unsigned int getTCK() const override {
    return   m_triggerTCK;
  }                            ///< retrieve TCK
  /// Get rate of rate limiting algorithm given its instance name (See TCKsh)
  double rateFromTCK(const std::string& instanceName) const override
  {
    return parameterFromTCK(instanceName,"Code","RATE(");
  }

  /// Get prescale of algorithm given its instance name (See TCKsh)
  double prescaleFromTCK(const std::string& instanceName) const override
  {
    return parameterFromTCK(instanceName,"AcceptFraction","");
  }

  StatusCode runUpdate() override;

private:

  /// Get prescale of algorithm given its instance name (See TCKsh)
  double parameterFromTCK( const std::string& instanceName,
                           const std::string& code,
                           const std::string& sub ) const ;

  StatusCode i_cacheTriggerData();              ///< Function extracting data from Condition


  mutable IPropertyConfigSvc * m_propertyConfigSvc = nullptr;
  Gaudi::Property<std::string> m_propertyConfigSvcName
  { this, "IPropertyConfigSvcInstance", "PropertyConfigSvc", "Name of PropertyConfigSvc" };
  Condition *m_condTrigger = nullptr;           ///< Condition for sampling coefficients
  unsigned int m_triggerTCK{0};                 ///< tck for these data

};

#endif // RATEFROMTCK_H
