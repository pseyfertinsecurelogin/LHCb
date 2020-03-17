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
#include "Event/MCParticle.h"
#include "Event/MCTrackInfo.h"
#include "Event/MCVertex.h"
#include "Event/RecVertex.h"
#include "Event/State.h"
#include "Event/Track.h"
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/RndmGenerators.h"
#include "TFile.h"
#include "TH3.h"

/** @class PVEff PGPrimaryVertex.h
 *  Particle Gun Primary Vertex insertion
 *
 *  @author Marcin Kucharczyk
 *  @date   2008-12-07
 *  @author Domink Mitzel
 *  @date   2014-08-01
 *  @author Adam Davis
 *  @date   2018-05-03
 */

class PGPrimaryVertex : public Gaudi::Functional::Transformer<LHCb::RecVertices( const LHCb::MCVertices& )> {
public:
  // Standard constructor
  PGPrimaryVertex( const std::string& name, ISvcLocator* pSvcLocator )
      : Transformer{name,
                    pSvcLocator,
                    {"InputVerticesName", LHCb::MCVertexLocation::Default},
                    {"OutputVerticesName", LHCb::RecVertexLocation::Primary}} {}
  StatusCode        initialize() override;
  LHCb::RecVertices operator()( const LHCb::MCVertices& ) const override;
  StatusCode        finalize() override;

private:
  int                          fromMCVertex( const LHCb::MCParticle* mcParticle, const LHCb::MCVertex* mcVertex );
  Gaudi::Property<bool>        m_usePVResFile{this, "UsePVResFile", false,
                                       "external input of the possible PV parameterization"};
  Gaudi::Property<std::string> m_inputFileName{this, "InputFileName", ""};
  Gaudi::Property<std::string> m_histoPath{this, "HistogramPath", "h3_resovxvyvz"};

  mutable Rndm::Numbers m_gaus;
  mutable Rndm::Numbers m_flat;
  TFile*                m_file{nullptr};
  TH3*                  m_hist{nullptr};

  // AD. make hard coded numbers configurable
  // central value of smearing, X, for each gaussian
  // triple gaussian resolution
  // from Previous authors
  // The model is 3 gaussians with fractions f1, f2, 1-f1-f2. To sample from this model we do:
  //  - get r uniformly distributed in [0, 1[
  //  - if r<f1 then sample from the first gaussian
  //  - else if r<(f1+f2) then sample from the second gaussian
  //  - else sample from the third
  // x direction
  Gaudi::Property<double> m_SmearX_cv1{this, "GaussResolutionX1CentralValue", -1.64321e-04,
                                       "Triple Gaussian, G1 x mean"};
  Gaudi::Property<double> m_SmearX_width1{this, "GaussResolutionX1Width", 2.14809e-02, "Triple Gaussian, G1 x width"};
  Gaudi::Property<double> m_fracx1{this, "GaussResolutionX1fraction", 0.4086, "Triple Gaussian, G1 x fraction1"};
  //
  Gaudi::Property<double> m_SmearX_cv2{this, "GaussResolutionX2CentralValue", -2.07447e-05,
                                       "Triple Gaussian, G2 x mean"};
  Gaudi::Property<double> m_SmearX_width2{this, "GaussResolutionX2Width", 9.28222e-03, "Triple Gaussian, G2 x width"};
  Gaudi::Property<double> m_fracx2{this, "GaussResolutionX2fraction", 0.7969, "Triple Gaussian, G2 x fraction1+2"};
  //
  Gaudi::Property<double> m_SmearX_cv3{this, "GaussResolutionX3CentralValue", -1.59372e-04,
                                       "Triple Gaussian, G3 x mean"};
  Gaudi::Property<double> m_SmearX_width3{this, "GaussResolutionX3Width", 5.53627e-02, "Triple Gaussian, G3 x width"};

  // y direction
  Gaudi::Property<double> m_SmearY_cv1{this, "GaussResolutionY1CentralValue", -3.54945e-05,
                                       "Triple Gaussian, G1 y mean"};
  Gaudi::Property<double> m_SmearY_width1{this, "GaussResolutionY1Width", 2.12752e-02, "Triple Gaussian, G1 y width"};
  Gaudi::Property<double> m_fracy1{this, "GaussResolutionY1fraction", 0.4035, "Triple Gaussian, G1 y fraction1"};
  //
  Gaudi::Property<double> m_SmearY_cv2{this, "GaussResolutionY2CentralValue", -1.92725e-05,
                                       "Triple Gaussian, G2 y mean"};
  Gaudi::Property<double> m_SmearY_width2{this, "GaussResolutionY2Width", 9.42666e-03, "Triple Gaussian, G2 y width"};
  Gaudi::Property<double> m_fracy2{this, "GaussResolutionY2fraction", 0.8039, "Triple Gaussian, G2 y fraction1+2"};
  //
  Gaudi::Property<double> m_SmearY_cv3{this, "GaussResolutionY3CentralValue", -7.60356e-05,
                                       "Triple Gaussian, G3 y mean"};
  Gaudi::Property<double> m_SmearY_width3{this, "GaussResolutionY3Width", 5.50516e-02, "Triple Gaussian, G3 y width"};

  // z direction
  Gaudi::Property<double> m_SmearZ_cv1{this, "GaussResolutionZ1CentralValue", 1.92948e-03,
                                       "Triple Gaussian, G1 z mean"};
  Gaudi::Property<double> m_SmearZ_width1{this, "GaussResolutionZ1Width", 2.93170e-01, "Triple Gaussian, G1 z width"};
  Gaudi::Property<double> m_fracz1{this, "GaussResolutionZ1fraction", 0.1628, "Triple Gaussian, G1 z fraction1"};
  //
  Gaudi::Property<double> m_SmearZ_cv2{this, "GaussResolutionZ2CentralValue", -6.39393e-05,
                                       "Triple Gaussian, G2 z mean"};
  Gaudi::Property<double> m_SmearZ_width2{this, "GaussResolutionZ2Width", 4.58140e-02, "Triple Gaussian, G2 z width"};
  Gaudi::Property<double> m_fracz2{this, "GaussResolutionZ2fraction", 0.5597, "Triple Gaussian, G2 z fraction1+2"};
  //
  Gaudi::Property<double> m_SmearZ_cv3{this, "GaussResolutionZ3CentralValue", 4.43561e-04,
                                       "Triple Gaussian, G3 z mean"};
  Gaudi::Property<double> m_SmearZ_width3{this, "GaussResolutionZ3Width", 1.10641e-01, "Triple Gaussian, G3 z width"};

  // covariance properties
  Gaudi::Property<double> m_covXX{this, "CovarianceXX", 4.36969844565514146e-04, "Covariance Matrix XX term"};
  Gaudi::Property<double> m_covYY{this, "CovarianceYY", 4.09792105425419665e-04, "Covariance Matrix YY term"};
  Gaudi::Property<double> m_covZZ{this, "CovarianceZZ", 1.85588016359800601e-02, "Covariance Matrix ZZ term"};
};

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
  StatusCode sc = Transformer::initialize();
  return m_usePVResFile ? sc.andThen( [&]() -> StatusCode {
                              // -- Open the file containing the PV resolution
                              m_file = TFile::Open( m_inputFileName.value().c_str() );
                              if ( !m_file ) {
                                error() << "Could not find PV resolution template file!" << endmsg;
                                return StatusCode::FAILURE;
                              }
                              return StatusCode::SUCCESS;
                            } )
                              .andThen( [&]() -> StatusCode {
                                m_hist = (TH3D*)m_file->Get( m_histoPath.value().c_str() );
                                if ( !m_hist ) {
                                  error() << "Could not find PV resolution template histogram!" << endmsg;
                                  return StatusCode::FAILURE;
                                }
                                return StatusCode::SUCCESS;
                              } )
                        : sc.andThen( [&] {
                              // -- Initialize the random number generator
                              return m_gaus.initialize( randSvc(), Rndm::Gauss( 0., 1. ) ).orElse( [&] {
                                error() << "Cannot initialize Gaussian random number generator." << endmsg;
                              } );
                            } )
                              .andThen( [&] {
                                return m_flat.initialize( randSvc(), Rndm::Flat( 0., 1. ) ).orElse( [&] {
                                  error() << "Cannot initialize Uniform random number generator." << endmsg;
                                } );
                              } );
}

//=============================================================================
// Execution
//=============================================================================
LHCb::RecVertices PGPrimaryVertex::operator()( const LHCb::MCVertices& mcVertices ) const {

  LHCb::RecVertices outputVertices;
  // find a visible MC primary vertex
  auto imcv = std::find_if( mcVertices.begin(), mcVertices.end(),
                            []( const LHCb::MCVertex* mcv ) { return mcv && mcv->isPrimary(); } );
  if ( imcv == mcVertices.end() ) {
    error() << "Could not find any PV in the event" << endmsg;
    return outputVertices;
  }

  double x = ( *imcv )->position().x();
  double y = ( *imcv )->position().y();
  double z = ( *imcv )->position().z();

  // calculate smearing
  double smearX( 0. ), smearY( 0. ), smearZ( 0. );
  if ( m_usePVResFile ) {
    // -- sample components of PV resolution according to template in histogram
    m_hist->GetRandom3( smearX, smearY, smearZ );
  } else {
    // -- use parameterization of PV resolution
    double rx = m_flat();
    if ( rx < m_fracx1 )
      smearX = m_gaus() * m_SmearX_width1 + m_SmearX_cv1;
    else if ( rx < m_fracx2 )
      smearX = m_gaus() * m_SmearX_width2 + m_SmearX_cv2;
    else
      smearX = m_gaus() * m_SmearX_width3 + m_SmearX_cv3;
    // y
    double ry = m_flat();
    if ( ry < m_fracy1 )
      smearY = m_gaus() * m_SmearY_width1 + m_SmearY_cv1;
    else if ( ry < m_fracy2 )
      smearY = m_gaus() * m_SmearY_width2 + m_SmearY_cv2;
    else
      smearY = m_gaus() * m_SmearY_width3 + m_SmearY_cv3;
    double rz = m_flat();
    if ( rz < m_fracz1 )
      smearZ = m_gaus() * m_SmearZ_width1 + m_SmearZ_cv1;
    else if ( rz < m_fracz2 )
      smearZ = m_gaus() * m_SmearZ_width2 + m_SmearZ_cv2;
    else
      smearZ = m_gaus() * m_SmearZ_width3 + m_SmearZ_cv3;
  }
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Got smear XYZ(" << smearX << ", " << smearY << ", " << smearZ << ")" << endmsg;

  // smear the MC primary vertex
  auto cheatedPV = std::make_unique<LHCb::RecVertex>();
  cheatedPV->clearTracks();
  cheatedPV->setPosition( {x + smearX, y + smearY, z + smearZ} );
  Gaudi::SymMatrix3x3 cheatedPVcov;
  cheatedPVcov( 0, 0 ) = m_covXX;
  cheatedPVcov( 1, 1 ) = m_covYY;
  cheatedPVcov( 2, 2 ) = m_covZZ;
  cheatedPV->setCovMatrix( cheatedPVcov );
  int    nDoF( 1 );
  double chi2Vtx( 1. );
  cheatedPV->setChi2( chi2Vtx );
  cheatedPV->setNDoF( nDoF );
  cheatedPV->setTechnique( LHCb::RecVertex::RecVertexType::Primary );

  // save it
  outputVertices.insert( cheatedPV.release() );
  return outputVertices;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PGPrimaryVertex::finalize() {
  if ( m_file ) m_file->Close();
  return Transformer::finalize(); // must be called after all other actions
}
