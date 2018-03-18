// Include files:
// from Gaudi
// from Event
#include "Event/Track.h"
#include "Event/State.h"
#include "Event/RecVertex.h"
#include "Event/MCTrackInfo.h"
// Local
#include "PGPrimaryVertex.h"



//-----------------------------------------------------------------------------
// Implementation file for class : PGPrimaryVertex
//
// 2008-12-07 : Marcin Kucharczyk
// 2014-08-01 : Improved by Dominik Mitzel
// 2018-05-03 : A.Davis. Made all hard coded numbers configurable, 
//              move to LHCb from Rec so that it can be used in Gauss
//-----------------------------------------------------------------------------
DECLARE_COMPONENT( PGPrimaryVertex )

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode PGPrimaryVertex::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;  
  if(msgLevel(MSG::DEBUG)) debug() << "Initialisation" << endmsg;

  if( m_usePVResFile ){
    // -- Open the file containing the PV resolution
    m_file = TFile::Open( m_inputFileName.value().c_str() );
    if( !m_file ){
      error() << "Could not find PV resolution template file!" << endmsg;
      return StatusCode::FAILURE;
    }
    m_hist = (TH3D*) m_file->Get( m_histoPath.value().c_str() );
    if( !m_hist ){
      error() << "Could not find PV resolution template histogram!" << endmsg;
      return StatusCode::FAILURE;
    }
  } else {
    // -- Initialize the random number generator
    sc = m_gaus.initialize(randSvc(), Rndm::Gauss(0., 1.));
    if (sc.isFailure()) {
      error() << "Cannot initialize Gaussian random number generator." << endmsg;
      return sc;
    }
    sc = m_flat.initialize(randSvc(), Rndm::Flat(0., 1.));
    if (sc.isFailure()) {
      error() << "Cannot initialize Uniform random number generator." << endmsg;
      return sc;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Execution
//=============================================================================
StatusCode PGPrimaryVertex::execute() {

  if(msgLevel(MSG::DEBUG)) debug() << "Execute" << endmsg;

  LHCb::RecVertices* outputVertices = new LHCb::RecVertices();

  put(outputVertices,m_outputVerticesName);

  // find a visible MC primary vertex
  double x(0.), y(0.), z(0.);
  bool found(false);
  LHCb::MCVertices* mcVertices = get<LHCb::MCVertices>(m_inputVerticesName);
  for(LHCb::MCVertices::const_iterator itMCV = mcVertices->begin();  mcVertices->end() != itMCV; itMCV++) {
     const LHCb::MCVertex* mcv = *itMCV ;
     if ( 0 == mcv || !mcv->isPrimary() ) continue;
     x = mcv->position().x();
     y = mcv->position().y();
     z = mcv->position().z();
     found = true;
     break;
  }
  if (!found) {
     error() << "Could not find any PV in the event" << endmsg;
     return StatusCode::SUCCESS;
  }

  // calculate smearing
  double smearX(0.), smearY(0.), smearZ(0.);
  if( m_usePVResFile ) {
     // -- sample components of PV resolution according to template in histogram
     m_hist->GetRandom3(smearX, smearY, smearZ);
  } else {
     // -- use parameterization of PV resolution
     double rx = m_flat();
     if (rx < m_fracx1)       
       smearX = m_gaus()*m_SmearX_width1 + m_SmearX_cv1;
     else if (rx < m_fracx2)
       smearX = m_gaus()*m_SmearX_width2 + m_SmearX_cv2;
     else
       smearX = m_gaus()*m_SmearX_width3 + m_SmearX_cv3;
     //y
     double ry = m_flat();
     if (ry < m_fracy1)
        smearY = m_gaus()*m_SmearY_width1 + m_SmearY_cv1;
     else if (ry < m_fracy2)
        smearY = m_gaus()*m_SmearY_width2 + m_SmearY_cv2;
     else
        smearY = m_gaus()*m_SmearY_width3 + m_SmearY_cv3;
     double rz = m_flat();
     if (rz < m_fracz1)
        smearZ = m_gaus()*m_SmearZ_width1 + m_SmearZ_cv1;
     else if (rz < m_fracz2)
        smearZ = m_gaus()*m_SmearZ_width2 + m_SmearZ_cv2;
     else
        smearZ = m_gaus()*m_SmearZ_width3 + m_SmearZ_cv3;
  }
  if(msgLevel(MSG::DEBUG)) debug()<<"Got smear XYZ("<<smearX<<", "<<smearY<<", "<<smearZ<<")"<<endmsg;
  
  // smear the MC primary vertex
  LHCb::RecVertex* cheatedPV = new LHCb::RecVertex();
  cheatedPV->clearTracks();
  Gaudi::XYZPoint cheatedPVposition(x+smearX, y+smearY, z+smearZ);
  cheatedPV->setPosition(cheatedPVposition);
  Gaudi::SymMatrix3x3 cheatedPVcov;
  cheatedPVcov(0,0) = m_covXX;
  cheatedPVcov(1,1) = m_covYY;
  cheatedPVcov(2,2) = m_covZZ;
  cheatedPV->setCovMatrix(cheatedPVcov);
  int nDoF(1); double chi2Vtx(1.);
  cheatedPV->setChi2(chi2Vtx);
  cheatedPV->setNDoF(nDoF);
  cheatedPV->setTechnique(LHCb::RecVertex::RecVertexType::Primary);

  // save it
  outputVertices->insert(cheatedPV);

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PGPrimaryVertex::finalize() {

  debug() << "==> Finalize" << endmsg;

  if( m_usePVResFile ) m_file->Close();

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}
