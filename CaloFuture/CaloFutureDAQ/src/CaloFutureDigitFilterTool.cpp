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
// Include files
// local
#include "CaloFutureDigitFilterTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureDigitFilterTool
//
// 2010-12-13 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( CaloFutureDigitFilterTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloFutureDigitFilterTool::CaloFutureDigitFilterTool( const std::string& type, const std::string& name,
                                                      const IInterface* parent )
    : base_class( type, name, parent ) {
  declareInterface<ICaloFutureDigitFilterTool>( this );
  declareProperty( "MaskMap", m_maskMap );
  declareProperty( "UseCondDB", m_useCondDB = true );
  declareProperty( "EcalOffset", m_ecalMaps.offsets );
  declareProperty( "HcalOffset", m_hcalMaps.offsets );
  declareProperty( "PrsOffset", m_prsMaps.offsets );
  declareProperty( "EcalOffsetRMS", m_ecalMaps.offsetsRMS );
  declareProperty( "HcalOffsetRMS", m_hcalMaps.offsetsRMS );
  declareProperty( "PrsOffsetRMS", m_prsMaps.offsetsRMS );
  declareProperty( "ScalingMethod", m_scalingMethod = 0 ); // 0 : SpdMult ; 1 = nPV  (+10 for clusters)
  declareProperty( "ScalingBin", m_scalingBin = 50 );
  declareProperty( "ScalingMin", m_scalingMin = 150. );
  m_maskMap["Default"] = CaloCellQuality::OfflineMask;
  declareProperty( "VertexLoc", m_vertLoc = "" );
  declareProperty( "UsePV3D", m_usePV3D = false );
  declareProperty( "SetCounterLevel", m_setCounters = 1 );
}
//=============================================================================

StatusCode CaloFutureDigitFilterTool::initialize() {
  StatusCode sc = base_class::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;          // error printed already by CaloFuture2Dview
  if ( m_vertLoc.empty() ) {
    m_vertLoc = ( m_usePV3D ) ? LHCb::RecVertexLocation::Velo3D : LHCb::RecVertexLocation::Primary;
  }

  // check
  for ( const auto& i : m_maskMap ) {
    if ( ( i.second & CaloCellQuality::OfflineMask ) == 0 )
      Warning( "OfflineMask flag is disabled for " + i.first + " - Are you sure ?", StatusCode::SUCCESS ).ignore();
  }
  // subscribe to incident Svc
  incSvc()->addListener( this, IncidentType::BeginEvent );

  return sc;
}

//-----------------------
StatusCode CaloFutureDigitFilterTool::caloUpdate() {
  // loop over all current maps and recreate
  for ( auto& maps : m_offsetMap ) { createMaps( maps.first, false ); }
  const auto offsets = m_offsetMap.find( m_calo );
  setMaps( offsets != m_offsetMap.end() ? *( offsets->second ) : createMaps( m_calo ) );
  return StatusCode::SUCCESS;
}

//-----------------------
const CaloFutureDigitFilterTool::CondMaps& CaloFutureDigitFilterTool::createMaps( DeCalorimeter* calo,
                                                                                  const bool     regUpdate ) {
  // Map new set of maps for this calo
  m_offsetMap[calo].reset( new CondMaps() );
  auto* maps = m_offsetMap[calo].get();
  // clear, just in case..
  maps->offsets.clear();
  maps->offsetsRMS.clear();
  // fill the maps from the CaloFuture DetElem
  for ( const auto& c : calo->cellParams() ) {
    const auto id = c.cellID();
    if ( !calo->valid( id ) || id.isPin() ) continue;
    maps->offsets[id]    = c.pileUpOffset();
    maps->offsetsRMS[id] = c.pileUpOffsetRMS();
  }
  // if first time, register callback for future updates
  if ( regUpdate ) { updMgrSvc()->registerCondition( this, calo, &CaloFutureDigitFilterTool::caloUpdate ); }
  // return the new maps
  return *maps;
}

//-----------------------
bool CaloFutureDigitFilterTool::setDet( const std::string& det ) {
  if ( m_caloName == det ) return true;
  m_caloName = LHCb::CaloFutureAlgUtils::CaloFutureNameFromAlg( det );
  m_calo     = getDet<DeCalorimeter>( LHCb::CaloFutureAlgUtils::DeCaloFutureLocation( m_caloName ) );
  if ( nullptr == m_calo ) return false;
  m_mask = getMask( m_caloName );
  getOffsetMap( m_caloName );
  return true;
}

//-----------------------
void CaloFutureDigitFilterTool::getOffsetMap( const std::string& det ) {
  if ( m_useCondDB ) {
    m_scalingMethod = m_calo->pileUpSubstractionMethod();
    m_scalingMin    = m_calo->pileUpSubstractionMin();
    m_scalingBin    = m_calo->pileUpSubstractionBin();
    if ( m_scalingMethod < 0 ) {
      setMaps( m_nullMaps );
    } else {
      const auto caloOffs = m_offsetMap.find( m_calo );
      setMaps( caloOffs != m_offsetMap.end() ? *( caloOffs->second ) : createMaps( m_calo ) );
    }
  } else {
    setMaps( det == "Ecal" ? m_ecalMaps : det == "Hcal" ? m_hcalMaps : det == "Prs" ? m_prsMaps : m_nullMaps );
  }
}

//-----------------------
double CaloFutureDigitFilterTool::getOffset( LHCb::CaloCellID id, int scale ) {
  if ( 0 == scale ) return 0;
  if ( scale <= m_scalingMin ) return 0;
  const auto& table = offsets();
  const auto  it    = table.find( id );
  if ( it == table.end() ) return 0.;
  const auto& ref = it->second;
  //  overlap probabilty (approximate)
  constexpr double ncells = 6016.;
  double           rscale = scale / ( 1. - scale / ncells );
  const double     aa     = 4 * scale / ncells;
  if ( aa < 1 ) { rscale = 0.5 * ncells * ( 1. - std::sqrt( 1. - aa ) ); }
  const double step   = double( m_scalingBin );
  const double bin    = double( rscale - m_scalingMin );
  const double aaa    = bin / step;
  const double offset = aaa * ref;
  m_offsetRMS         = std::sqrt( aaa ) * ref;
  return offset;
}

//-----------------------
int CaloFutureDigitFilterTool::getMask( const std::string& det ) {
  auto it = m_maskMap.find( det );
  if ( it != m_maskMap.end() ) return it->second;
  it = m_maskMap.find( "Default" );
  return it != m_maskMap.end() ? it->second : 0;
}

//-----------------------
void CaloFutureDigitFilterTool::setMaskMap( const std::map<std::string, int>& maskMap ) { m_maskMap = maskMap; }

//-----------------------
int CaloFutureDigitFilterTool::getScale() {
  if ( !m_reset ) return m_scale;
  m_reset   = false;
  m_scale   = 0;
  m_scaling = "None";
  if ( m_scalingMethod == 1 || m_scalingMethod == 11 ) { // nPV
    m_scaling = "nPV";
    m_scale   = nVertices();
  }
  return m_scale;
}

double CaloFutureDigitFilterTool::offset( LHCb::CaloCellID id ) {
  if ( id.caloName() != m_caloName ) {
    if ( !setDet( id.caloName() ) ) return 0.;
  }
  int scale = getScale();
  return getOffset( id, scale );
}

double CaloFutureDigitFilterTool::offsetRMS( LHCb::CaloCellID id ) {
  offset( id );
  return m_offsetRMS;
}

//================= Scale accessors

unsigned int CaloFutureDigitFilterTool::nVertices() {
  int nVert = 0;
  if ( !m_usePV3D ) {
    LHCb::RecVertices* verts = getIfExists<LHCb::RecVertices>( evtSvc(), m_vertLoc );
    if ( verts ) return verts->size();
  }
  // try PV3D if explicitely requested or if RecVertices not found
  if ( !m_usePV3D ) m_vertLoc = LHCb::RecVertexLocation::Velo3D;
  LHCb::VertexBases* verts = getIfExists<LHCb::VertexBases>( m_vertLoc );
  if ( verts ) nVert = verts->size();
  return nVert;
}

StatusCode CaloFutureDigitFilterTool::finalize() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;
  incSvc()->removeListener( this );
  return base_class::finalize(); // must be called after all other actions
}
