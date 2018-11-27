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
#ifndef L0DUFROMRAWHLT1TOOL_H
#define L0DUFROMRAWHLT1TOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "L0Interfaces/IL0DUFromRawTool.h"
#include "L0Interfaces/IL0DUConfigProvider.h"

// from Event
#include "Event/RawEvent.h"
#include "Event/RawBankReadoutStatus.h"

/** @class L0DUFromRawHlt1Tool L0DUFromRawHlt1Tool.h
 *
 *
 *  @author Olivier Callot
 *  @date   2012-02-13
 */
class L0DUFromRawHlt1Tool : public extends<GaudiTool, IL0DUFromRawTool> {
public:
  /// Standard constructor
  L0DUFromRawHlt1Tool( const std::string& type,
                       const std::string& name,
                       const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;

  bool decodeBank( int iBank ) override;
  std::string dump() const override { return "== not implemented =="; }

  unsigned int data(const std::string& name) const override { return m_dataMap.at(name).first; }
  unsigned int version() const  override             { return m_vsn;};
  unsigned int tck() const      override             { return m_tck; }
  unsigned int firmware() const override             { return m_pgaVsn; }
  unsigned int rsda() const     override             { return m_rsda; }
  unsigned int muonCleaningPattern() const override  { return m_muCleanPattern; }
  unsigned int status() const   override             { return m_status; }
  unsigned int size() const     override             { return m_size;  }
  unsigned long roStatus() const override            { return m_roStatus.status();  }
  void fillDataMap(bool fill = true) override { m_fill = fill;}
  LHCb::L0DUReport report( ) const  override         { return m_report;}
  LHCb::L0ProcessorDatas* L0ProcessorDatas() override { return &m_processorDatas;}
  const std::pair<unsigned int,unsigned int> bcid() const override { return {m_bcid2,m_bcid3}; }
  const std::map<std::string, std::pair<unsigned int,double> >& datas() const override { return m_dataMap;}
  StatusCode  _setProperty(const std::string& p,const std::string& v) override   { return  setProperty(p,v);};

private:
  void encode(const std::string& name, unsigned int data , unsigned int scale = 1 ) {
    m_dataMap[name] = { data, scale };
    if ( m_report.dataDigit(name) != data ) m_report.addToData( name, data, scale );
  }

  void compareReports();

private:
  std::string m_configName;
  std::string m_configType;

  std::map<std::string, std::pair<unsigned int,double> > m_dataMap;
  unsigned int m_vsn;
  unsigned int m_status;
  unsigned int m_pgaVsn;
  unsigned int m_bcid2;
  unsigned int m_bcid3;
  unsigned int m_rsda;
  unsigned int m_muCleanPattern;
  std::map<int , unsigned int> m_sumEt;
  LHCb::L0DUReport m_report;
  LHCb::L0ProcessorDatas m_processorDatas;
  unsigned int m_tck;
  unsigned int m_oldTck;
  unsigned int m_size;
  LHCb::RawBankReadoutStatus m_roStatus;
  bool m_fill;
  IL0DUConfigProvider*   m_confTool = nullptr;
  int m_caloScale;
  int m_muonScale;
  LHCb::L0DUConfig* m_config = nullptr;
  std::vector<std::string> m_rawLocations;
};
#endif // L0DUFROMRAWHLT1TOOL_H
