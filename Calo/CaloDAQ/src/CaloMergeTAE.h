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
#ifndef CALOMERGETAE_H
#define CALOMERGETAE_H 1

// Include files
#include <iomanip>
// from Gaudi
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
#include "GaudiAlg/GaudiAlgorithm.h"

#include "details.h"

/** @class CaloMergeTAE CaloMergeTAE.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-10-24
 */
class CaloMergeTAE : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloMergeTAE( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  void                                      mergeDigits();
  void                                      mergeAdcs();
  Gaudi::Property<std::vector<std::string>> m_slots = {this, "MergeSlots", {"T0", "Prev1", "Next1"}};
  Gaudi::Property<std::string>              m_detectorName{this, "Detector"};
  Gaudi::Property<double>                   m_threshold{this, "SumThreshold", -256};
  Gaudi::Property<double>                   m_slotcut{this, "SlotThreshold", -256};
  std::string                               m_locDigit;
  std::string                               m_outDigit;
  std::string                               m_locAdc;
  std::string                               m_outAdc;
  Gaudi::Property<details::source_t>        m_data{this, "OutputType", details::source_t::from_digit};

  Gaudi::Property<std::string> m_outputDataLoc{this, "OutputDataLocation", ""};
  Gaudi::Property<std::string> m_inExt{this, "inputExtension", ""};

  DeCalorimeter* m_calo = nullptr;
};

#endif // CALOMERGETAE_H
