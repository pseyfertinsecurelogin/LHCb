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
#ifndef TCKPRESCALEEMULATOR_H
#define TCKPRESCALEEMULATOR_H 1

// Include files
// from DaVinci, this is a specialized GaudiAlgorithm
#include "GaudiAlg/GaudiAlgorithm.h"
#include "DetDesc/Condition.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/HltDecReports.h"
using namespace LHCb;

/** @class TCKPrescaleEmulator TCKPrescaleEmulator.h
 *
 *
 *  @author Conor Fitzpatrick
 *  @date   2012-09-13
 */
class TCKPrescaleEmulator final : public GaudiAlgorithm
{

 public:

  /// Standard constructor
  TCKPrescaleEmulator( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~TCKPrescaleEmulator( ) = default; ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

 private:

  using StringMap = std::map<std::string,double>;

 private:

  /// Fill maps with prescale, postscale and post*prescale based on specificed TCK
  StatusCode getPrescalesFromTCK( unsigned int,
                                  StringMap &,
                                  StringMap &,
                                  StringMap &);

  const HltDecReports* getReports(); ///< get the DecReports for each event, calculate the ratio of prescales if the TCK has changed or first event and call updatePrescalers if so

  StatusCode getPrescalers(); ///< Initialise Prescalers
  StatusCode updatePrescalers(); ///< Set accept rates of Prescalers based on ratio of prescales from the user-specced TCK and the TCK in (the first, or changed) MC event TCK


  StatusCode i_cacheTriggerData();              ///< Function extracting data from Condition

  bool endedWith(const std::string &lineName, const std::string &ending);

 private:

  std::string m_scalerName; //< Name prepended to PropertyConfig
  std::string m_postScalerName; //< Name postpended to PropertyConfig if postscaled
  std::string m_preScalerName; //< Name postpended to PropertyConfig if prescaled

  //Pre, post scales we get from the MC data itself
  StringMap prescalesInMC; //name of prescalers
  StringMap postscalesInMC; //name of postscalers
  StringMap scaleProductsInMC; //pre*post-scales

  //Pre, post scales we get from the user-specified TCK
  StringMap prescalesToEmulate; //name of prescalers
  StringMap postscalesToEmulate; //name of postscalers
  StringMap scaleProductsToEmulate; //pre*post-scales

  StringMap scaleProductsToApply; ///< map of scaleProducts that will get applied to the MC, (ratio of MC and ToEmulate scaleProducts)

  std::map<std::string, Algorithm*> prescalers; ///< map of prescaler algorithms

  IPropertyConfigSvc * m_propertyConfigSvc = nullptr;
  unsigned int m_tck{0};         //
  std::string m_propertyConfigSvcName;          ///< Name of PropertyConfigSvc
  Condition *m_condTrigger = nullptr;                     ///< Condition for sampling coefficients
  unsigned int m_triggerTCK{0};                    ///< tck for these data
  std::string m_hltDecReportsLocation;       ///< Location of the DecReports
  std::vector<std::string> m_linesToKill;       ///< lines to prescale to zero in MC regardless of existence in TCK or not
  bool firstevent{true};
  unsigned int lasttck{0};

};

#endif // TCKPRESCALEEMULATOR_H
