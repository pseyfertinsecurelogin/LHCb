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
#ifndef CALOFUTUREDAQ_CALOFUTURE2DVIEW_H 
#define CALOFUTUREDAQ_CALOFUTURE2DVIEW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiHistoAlg.h"
// from LHCb
#include "CaloDet/DeCalorimeter.h"
#include "Kernel/CaloCellID.h"
// from Event
#include "Event/MCCaloHit.h"
#include "Event/MCCaloDigit.h"
#include "Event/CaloDigit.h"
#include "Event/CaloAdc.h"
#include "Event/L0CaloAdc.h"
#include "Event/L0PrsSpdHit.h"
#include "Event/L0CaloCandidate.h"
#include "Event/CaloCluster.h"
// from ROOT
#include <TH2.h> 
#include <TProfile2D.h>
#include <TH1.h>

//==============================================================================

/** @class CaloFuture2Dview CaloFuture2Dview.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2008-01-18
 */
class CaloFuture2Dview : public GaudiHistoAlg {
public: 
  /// Standard constructor
  CaloFuture2Dview( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization

  void bookCaloFuture2D(const HistoID& unit, const std::string title, std::string name, int area =-1) const;
  void bookCaloFuture2D(const HistoID& unit, const std::string title, unsigned int calo, int area = -1) const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::MCCaloHit mchit, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::MCCaloDigit mcdigit, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::CaloDigit digit, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::CaloAdc adc, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::L0CaloAdc l0adc, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::L0PrsSpdHit hit, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::L0CaloCandidate l0calo, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, LHCb::CaloCluster cluster, const std::string title="") const;
  void fillCaloFuture2D(const HistoID& unit, const LHCb::CaloCellID& id, double value, const std::string title="") const;
  void fillCaloFuturePin2D(const HistoID& unit,const LHCb::CaloCellID& id , double value, const std::string title="") const;
  
  void reset(const HistoID& unit, std::string title="") const;
  void resetTitle(const HistoID& unit, std::string title) const;

  void setThreshold(double threshold){m_threshold = threshold;};
  void setPinView(bool pin){m_pin = pin;};
  void setActualSize(bool dim){m_dim = dim;};
  void setL0ClusterView(bool l0) const {m_l0 = l0;};
  void setOffset(double offset){m_offset = offset;};
  void setFlux(bool flux){m_flux = flux;}
  void setEnergyWeighted(bool energyWeighted){m_energyWeighted = energyWeighted;}
  void setGeometricalView(bool geo){m_geo = geo;}
  void setOneDimension(bool dim){m_1d = dim;}
  void setSplit(bool split){m_split = split;}
  bool split() const {return m_split;}
  const HistoID getUnit(const HistoID& unit, int calo, int area) const;  

  // Container of the subdetector info
  struct CaloFutureParams {
    DeCalorimeter* calo;
    int nChan;
    int centre;
    int reg;
    int fCard;
    int lCard;
    std::vector<LHCb::CaloCellID> refCell;
    // these are calculated (cached) after init
    std::vector<double> xsize;
    std::vector<double> ysize;
  };
  
protected:
  mutable bool m_l0;    // due to some fillCaloFuture2D method enforcing m_l0=true
  bool m_dim;
  bool m_pin; 
  bool m_energyWeighted;
  bool m_flux;
  bool m_geo;
  bool m_split;
  bool m_1d;
  bool m_profile;
  int m_bin1d;
  double m_threshold;
  double m_offset;
  std::string m_lab;
  std::string m_prof;

private:
  std::string getTitle(std::string title, int calo, int area) const;
  // Container of 4 subdectector params
  std::array<CaloFutureParams,4> m_caloParams;
  mutable std::map<HistoID,unsigned int> m_caloViewMap;

};
#endif // CALOFUTUREDAQ_CALOFUTURE2DVIEW_H

