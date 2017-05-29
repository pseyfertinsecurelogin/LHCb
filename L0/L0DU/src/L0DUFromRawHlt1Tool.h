#ifndef L0DUFROMRAWHLT1TOOL_H
#define L0DUFROMRAWHLT1TOOL_H 1

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Event/L0DUReport.h"
#include "Event/L0ProcessorData.h"
#include "Event/RawBankReadoutStatus.h"
// from Event
#include "Event/RawEvent.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/StatusCode.h"
#include "L0Interfaces/IL0DUConfigProvider.h"
#include "L0Interfaces/IL0DUFromRawTool.h"

class IInterface;
namespace LHCb {
class L0DUConfig;
}  // namespace LHCb
struct IL0DUConfigProvider;

/** @class L0DUFromRawHlt1Tool L0DUFromRawHlt1Tool.h
 *
 *
 *  @author Olivier Callot
 *  @date   2012-02-13
 */
class L0DUFromRawHlt1Tool : public GaudiTool, virtual public IL0DUFromRawTool {
public:
  /// Standard constructor
  L0DUFromRawHlt1Tool( const std::string& type,
                       const std::string& name,
                       const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;

  bool decodeBank( int iBank ) override;
  std::string dump() const override { return "== not implemented =="; }

  virtual unsigned int data(const std::string& name) const override { return m_dataMap.at(name).first; }
  virtual unsigned int version() const  override             { return m_vsn;};
  virtual unsigned int tck() const      override             { return m_tck; }
  virtual unsigned int firmware() const override             { return m_pgaVsn; }
  virtual unsigned int rsda() const     override             { return m_rsda; }
  virtual unsigned int muonCleaningPattern() const override  { return m_muCleanPattern; }
  virtual unsigned int status() const   override             { return m_status; }
  virtual unsigned int size() const     override             { return m_size;  }
  virtual unsigned long roStatus() const override            { return m_roStatus.status();  }
  virtual void fillDataMap(bool fill = true) override { m_fill = fill;}
  virtual LHCb::L0DUReport report( ) const  override         { return m_report;}
  virtual LHCb::L0ProcessorDatas* L0ProcessorDatas() override { return &m_processorDatas;}
  virtual const std::pair<unsigned int,unsigned int> bcid() const override { return {m_bcid2,m_bcid3}; }
  virtual const std::map<std::string, std::pair<unsigned int,double> >& datas() const override { return m_dataMap;}
  virtual StatusCode  _setProperty(const std::string& p,const std::string& v) override   { return  setProperty(p,v);};

private:
  void encode(const std::string& name, unsigned int data , unsigned int scale = 1 ) {
    m_dataMap[name] = std::make_pair( data, scale );
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
