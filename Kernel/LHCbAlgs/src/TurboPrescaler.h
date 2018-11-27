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
#ifndef TURBOPRESCALER_H
#define TURBOPRESCALER_H 1

// Include files
#include "DetDesc/Condition.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/HltDecReports.h"

/** @class TurboPrescaler TurboPrescaler.h
 *
 *
 *  @author Sean Benson
 *  @date   2016-06-13
 */
class TurboPrescaler final : public GaudiAlgorithm
{

 public:

  TurboPrescaler( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  StatusCode execute   () override;

 private:

  using StringMap = std::map<std::string, double>;

 private:

  void getPrescalesFromTCK( unsigned int, StringMap&, StringMap&, StringMap& );

  bool endedWith(const std::string &lineName, const std::string &ending);

  void setupPrescalers();
  void updatePrescalers();

 private:

  std::string m_scalerName;
  std::string m_postScalerName;
  std::string m_preScalerName;

  StringMap m_prescalesInput;
  StringMap m_postscalesInput;
  StringMap m_scaleProductsInput;

  std::map<std::string, Algorithm*> prescalers;

  IPropertyConfigSvc * m_propertyConfigSvc=nullptr;
  bool m_filter=false;
  std::string m_propertyConfigSvcName;
  std::string m_hltDecReportsLocation;
  unsigned int m_lastTCK=0;
  unsigned int m_outputTCK=0;
  std::map<std::string,double> m_outputPS;
  std::string m_outRepLoc;

};

#endif // TURBOPRESCALER_H
