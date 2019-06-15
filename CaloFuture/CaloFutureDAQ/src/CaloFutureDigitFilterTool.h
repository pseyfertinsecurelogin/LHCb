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
#ifndef CALOFUTUREDIGITFILTERTOOL_H
#define CALOFUTUREDIGITFILTERTOOL_H 1

class StatusCode;
#include "CaloDet/DeCalorimeter.h"
#include "CaloFutureDAQ/ICaloFutureDigitFilterTool.h" // Interface
#include "CaloFutureUtils/CaloFutureAlgUtils.h"
#include "CaloFutureUtils/CaloFutureCellIDAsProperty.h"
#include "Event/CaloDigit.h"
#include "Event/RecVertex.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"
#include "Kernel/CaloCellID.h"

#include <memory>
#include <unordered_map>

/** @class CaloFutureDigitFilterTool CaloFutureDigitFilterTool.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2010-12-13
 */
class CaloFutureDigitFilterTool final : public extends<GaudiTool, ICaloFutureDigitFilterTool, IIncidentListener> {
public:
  /// Standard constructor
  CaloFutureDigitFilterTool( const std::string& type, const std::string& name, const IInterface* parent );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode finalize() override;   ///< Algorithm finalization

  //
  bool   setDet( const std::string& det );
  void   getOffsetMap( const std::string& det );
  int    getMask( const std::string& det );
  double getOffset( LHCb::CaloCellID id, int scale );
  void   setMaskMap( const std::map<std::string, int>& maskMap );
  int    getScale() override;
  int    method( const std::string& det ) override {
    if ( det != m_caloName ) { setDet( det ); }
    return m_scalingMethod;
  }
  unsigned int nVertices();
  double       offset( LHCb::CaloCellID id ) override;
  double       offsetRMS( LHCb::CaloCellID id ) override;
  /// Triggered by calo updates
  StatusCode caloUpdate();

private:
  void handle( const Incident& /* inc */ ) override {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "IIncident Svc reset" << endmsg;
    m_reset = true;
  }

  using Offsets = std::map<LHCb::CaloCellID, double>;
  inline const Offsets& offsets() const noexcept { return *m_offsets; }
  inline const Offsets& offsetsRMS() const noexcept { return *m_offsetsRMS; }
  struct CondMaps {
    Offsets offsets, offsetsRMS;
  };
  const CondMaps& createMaps( DeCalorimeter* calo, const bool regUpdate = true );
  inline void     setMaps( const CondMaps& maps ) const {
    m_offsets    = &maps.offsets;
    m_offsetsRMS = &maps.offsetsRMS;
  }

private:
  std::map<std::string, int>                          m_maskMap;
  int                                                 m_mask{0};
  std::map<DeCalorimeter*, std::unique_ptr<CondMaps>> m_offsetMap;
  mutable const Offsets*                              m_offsets    = nullptr;
  mutable const Offsets*                              m_offsetsRMS = nullptr;

  DeCalorimeter* m_calo = nullptr;
  std::string    m_caloName{"None"};
  int            m_scalingMethod{0};
  bool           m_useCondDB{true};

  CondMaps m_ecalMaps;
  CondMaps m_hcalMaps;
  CondMaps m_prsMaps;
  CondMaps m_nullMaps;

  int         m_scalingBin;
  double      m_scalingMin;
  std::string m_vertLoc;
  bool        m_usePV3D;
  std::string m_scaling{"None"};
  double      m_offsetRMS{0.0};
  bool        m_reset{true};
  int         m_scale{0};

  int m_setCounters;
};

#endif // CALOFUTUREDIGITFILTERTOOL_H
