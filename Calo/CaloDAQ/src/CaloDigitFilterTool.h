#ifndef CALODIGITFILTERTOOL_H
#define CALODIGITFILTERTOOL_H 1

class StatusCode;
#include "Kernel/CaloCellID.h"
#include "CaloUtils/CaloCellIDAsProperty.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloDigit.h"
#include "CaloDAQ/ICaloDigitFilterTool.h"            // Interface
#include "CaloUtils/CaloAlgUtils.h"
#include "Event/RecVertex.h"

#include <memory>
#include <unordered_map>

/** @class CaloDigitFilterTool CaloDigitFilterTool.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2010-12-13
 */
class CaloDigitFilterTool final : public extends<GaudiTool, ICaloDigitFilterTool, IIncidentListener>{
public:
  /// Standard constructor
  CaloDigitFilterTool( const std::string& type,
                       const std::string& name,
                       const IInterface* parent);

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode finalize  () override;    ///< Algorithm finalization

  //
  bool setDet(const std::string& det);
  void getOffsetMap(const std::string& det);
  int getMask(const std::string & det);
  double getOffset(LHCb::CaloCellID id, int scale,bool spd=false);
  void setMaskMap(const std::map<std::string,int> & maskMap);
  int getScale() override;
  bool cleanDigits(const std::string & det, bool substr=true, bool mask = true,bool spd=false) override;
  int method(const std::string & det) override{
    if ( det != m_caloName ) { setDet( det ); }
    return m_scalingMethod;
  }
  unsigned int nVertices();
  unsigned int nSpd();
  double offset(LHCb::CaloCellID id,bool spd=false) override;
  double offsetRMS(LHCb::CaloCellID id,bool spd=false) override;
  /// Triggered by calo updates
  StatusCode caloUpdate();

private:
  void handle(const Incident& /* inc */ ) override {
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << "IIncident Svc reset" << endmsg;
    m_reset = true ;
  }

  using Offsets = std::map<LHCb::CaloCellID,double>;
  inline const Offsets & offsets()       const noexcept { return *m_offsets; }
  inline const Offsets & offsetsRMS()    const noexcept { return *m_offsetsRMS; }
  inline const Offsets & offsetsSPD()    const noexcept { return *m_offsetsSPD; }
  inline const Offsets & offsetsSPDRMS() const noexcept { return *m_offsetsSPDRMS; }
  struct CondMaps{Offsets offsets, offsetsRMS, offsetsSPD, offsetsSPDRMS;};
  const CondMaps & createMaps( DeCalorimeter* calo, const bool regUpdate = true );
  inline void setMaps( const CondMaps & maps ) const{
    m_offsets       = &maps.offsets;
    m_offsetsRMS    = &maps.offsetsRMS;
    m_offsetsSPD    = &maps.offsetsSPD;
    m_offsetsSPDRMS = &maps.offsetsSPDRMS;
  }

protected:
  void cleanDigit(LHCb::CaloDigit* digit, bool substr=true,int scale = -1, bool mask=true,bool spd=false);

private:
  std::map<std::string,int> m_maskMap;
  int m_mask{0};
  std::map< DeCalorimeter*, std::unique_ptr<CondMaps> > m_offsetMap;
  mutable const Offsets * m_offsets       = nullptr;
  mutable const Offsets * m_offsetsRMS    = nullptr;
  mutable const Offsets * m_offsetsSPD    = nullptr;
  mutable const Offsets * m_offsetsSPDRMS = nullptr;

  LHCb::CaloDigits* m_digits = nullptr;
  DeCalorimeter* m_calo = nullptr;
  std::string m_caloName{"None"};
  int m_scalingMethod{0};
  bool m_useCondDB{true};

  CondMaps m_ecalMaps;
  CondMaps m_hcalMaps;
  CondMaps m_prsMaps;
  CondMaps m_nullMaps;

  int    m_scalingBin;
  double m_scalingMin;
  double m_mOffs{0.0};
  int m_nMask{0};
  std::string m_vertLoc;
  bool m_usePV3D;
  std::string m_scaling{"None"};
  double m_offsetRMS{0.0};
  bool m_reset{true};
  int m_scale{0};

  int m_setCounters;

};

#endif // CALODIGITFILTERTOOL_H
