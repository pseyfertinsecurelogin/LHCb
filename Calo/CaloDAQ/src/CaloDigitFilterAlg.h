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
#ifndef CALODIGITFILTERALG_H
#define CALODIGITFILTERALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "CaloDAQ/ICaloDigitFilterTool.h"


/** @class CaloDigitFilterAlg CaloDigitFilterAlg.h
 *
 *  @author Olivier Deschamps
 *  @date   2010-12-21
 */


class CaloDigitFilterAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  Gaudi::Property<int> m_ecal { this, "EcalFilter" , 0x3 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  Gaudi::Property<int> m_hcal { this, "HcalFilter" , 0x3 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  Gaudi::Property<int> m_prs  { this, "PrsFilter"  , 0x3 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  Gaudi::Property<int> m_spd  { this, "SpdFilter"  , 0x1 }; // 1 = Mask , 2=Offset , 3 = both, 0 = none
  ICaloDigitFilterTool* m_filter = nullptr;
};

#endif // CALODIGITFILTERALG_H
