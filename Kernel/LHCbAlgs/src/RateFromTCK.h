// $Id: $
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

class RateFromTCK final : public GaudiTool, virtual public IRateFromTCK 
{

public: 

  /// Standard constructor
  RateFromTCK( const std::string& type, 
               const std::string& name,
               const IInterface* parent);

  virtual ~RateFromTCK( ) = default; ///< Destructor

  StatusCode initialize(); 

 public:

  inline unsigned int getTCK() const {
    return   m_triggerTCK;
  };                            ///< retrieve TCK
  /// Get rate of rate limiting algorithm given its instance name (See TCKsh)
  double rateFromTCK(const std::string& instanceName) const
  {
    return parameterFromTCK(instanceName,"Code","RATE(");
  }
  
  /// Get prescale of algorithm given its instance name (See TCKsh)
  double prescaleFromTCK(const std::string& instanceName) const
  {
    return parameterFromTCK(instanceName,"AcceptFraction","");
  }

  StatusCode runUpdate() ;

private:

  /// Get prescale of algorithm given its instance name (See TCKsh)
  double parameterFromTCK( const std::string& instanceName, 
                           const std::string& code,
                           const std::string& sub ) const ;

  StatusCode i_cacheTriggerData();              ///< Function extracting data from Condition

 private:

  mutable IPropertyConfigSvc * m_propertyConfigSvc = nullptr;
  std::string m_propertyConfigSvcName;          ///< Name of PropertyConfigSvc
  Condition *m_condTrigger = nullptr;           ///< Condition for sampling coefficients
  unsigned int m_triggerTCK{0};                 ///< tck for these data

};

#endif // RATEFROMTCK_H
