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
#ifndef PGPRIMARYVERTEX_H
#define PGPRIMARYVERTEX_H 1
// Include files:
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/RndmGenerators.h"
// From MCEvent
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
// From ROOT
#include "TFile.h"
#include "TH3.h"

//#include "MCInterfaces/IForcedBDecayTool.h"

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

class PGPrimaryVertex : public GaudiAlgorithm {
 public:
  // Standard constructor

  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;
  StatusCode execute() override;
  StatusCode finalize() override;

  int fromMCVertex(const LHCb::MCParticle* mcParticle,const LHCb::MCVertex* mcVertex);

 private:
  //AD. Add input vertices as a configurable in case you want to pass something non-standard
  Gaudi::Property<std::string> m_inputVerticesName{this,
      "InputVerticesName",
      LHCb::MCVertexLocation::Default,
      "Input MC Vertices to use"};
  Gaudi::Property<std::string> m_outputVerticesName{this,
      "OutputVerticesName", 
      LHCb::RecVertexLocation::Primary, 
      "Location in TES of output"};
  Gaudi::Property<bool>m_usePVResFile{this,
      "UsePVResFile", 
      false,
      "external input of the possible PV parameterization"};
  Gaudi::Property<std::string>m_inputFileName{this,"InputFileName", ""};
  Gaudi::Property<std::string>m_histoPath{this,"HistogramPath", "h3_resovxvyvz"};

  Rndm::Numbers m_gaus;
  Rndm::Numbers m_flat;
  TFile *m_file{nullptr};
  TH3* m_hist{nullptr};
  
  
  //AD. make hard coded numbers configurable
  //central value of smearing, X, for each gaussian
  //triple gaussian resolution 
  //from Previous authors
  //The model is 3 gaussians with fractions f1, f2, 1-f1-f2. To sample from this model we do:
  //  - get r uniformly distributed in [0, 1[
  //  - if r<f1 then sample from the first gaussian
  //  - else if r<(f1+f2) then sample from the second gaussian
  //  - else sample from the third
  //x direction
  Gaudi::Property<double>m_SmearX_cv1{this,"GaussResolutionX1CentralValue",-1.64321e-04,"Triple Gaussian, G1 x mean"};
  Gaudi::Property<double>m_SmearX_width1{this,"GaussResolutionX1Width",2.14809e-02,"Triple Gaussian, G1 x width"};
  Gaudi::Property<double>m_fracx1{this,"GaussResolutionX1fraction",0.4086, "Triple Gaussian, G1 x fraction1"};
  //  
  Gaudi::Property<double>m_SmearX_cv2{this,"GaussResolutionX2CentralValue",- 2.07447e-05,"Triple Gaussian, G2 x mean"};
  Gaudi::Property<double>m_SmearX_width2{this,"GaussResolutionX2Width",9.28222e-03,"Triple Gaussian, G2 x width"};
  Gaudi::Property<double>m_fracx2{this,"GaussResolutionX2fraction",0.7969, "Triple Gaussian, G2 x fraction1+2"};
  //
  Gaudi::Property<double>m_SmearX_cv3{this,"GaussResolutionX3CentralValue", -1.59372e-04,"Triple Gaussian, G3 x mean"};
  Gaudi::Property<double>m_SmearX_width3{this,"GaussResolutionX3Width",5.53627e-02,"Triple Gaussian, G3 x width"};

  //y direction
  Gaudi::Property<double>m_SmearY_cv1{this,"GaussResolutionY1CentralValue",-3.54945e-05,"Triple Gaussian, G1 y mean"};
  Gaudi::Property<double>m_SmearY_width1{this,"GaussResolutionY1Width",2.12752e-02 ,"Triple Gaussian, G1 y width"};
  Gaudi::Property<double>m_fracy1{this,"GaussResolutionY1fraction",0.4035, "Triple Gaussian, G1 y fraction1"};
  //  
  Gaudi::Property<double>m_SmearY_cv2{this,"GaussResolutionY2CentralValue",-1.92725e-05,"Triple Gaussian, G2 y mean"};
  Gaudi::Property<double>m_SmearY_width2{this,"GaussResolutionY2Width",9.42666e-03,"Triple Gaussian, G2 y width"};
  Gaudi::Property<double>m_fracy2{this,"GaussResolutionY2fraction",0.8039, "Triple Gaussian, G2 y fraction1+2"};
  //
  Gaudi::Property<double>m_SmearY_cv3{this,"GaussResolutionY3CentralValue",-7.60356e-05,"Triple Gaussian, G3 y mean"};
  Gaudi::Property<double>m_SmearY_width3{this,"GaussResolutionY3Width",5.50516e-02,"Triple Gaussian, G3 y width"};

  //z direction
  Gaudi::Property<double>m_SmearZ_cv1{this,"GaussResolutionZ1CentralValue",1.92948e-03,"Triple Gaussian, G1 z mean"};
  Gaudi::Property<double>m_SmearZ_width1{this,"GaussResolutionZ1Width",2.93170e-01,"Triple Gaussian, G1 z width"};
  Gaudi::Property<double>m_fracz1{this,"GaussResolutionZ1fraction",0.1628, "Triple Gaussian, G1 z fraction1"};
  //  
  Gaudi::Property<double>m_SmearZ_cv2{this,"GaussResolutionZ2CentralValue",-6.39393e-05,"Triple Gaussian, G2 z mean"};
  Gaudi::Property<double>m_SmearZ_width2{this,"GaussResolutionZ2Width",4.58140e-02,"Triple Gaussian, G2 z width"};
  Gaudi::Property<double>m_fracz2{this,"GaussResolutionZ2fraction",0.5597, "Triple Gaussian, G2 z fraction1+2"};
  //
  Gaudi::Property<double>m_SmearZ_cv3{this,"GaussResolutionZ3CentralValue",4.43561e-04,"Triple Gaussian, G3 z mean"};
  Gaudi::Property<double>m_SmearZ_width3{this,"GaussResolutionZ3Width",1.10641e-01,"Triple Gaussian, G3 z width"};
  
  //covariance properties
  Gaudi::Property<double>m_covXX{this,"CovarianceXX",4.36969844565514146e-04, "Covariance Matrix XX term"};
  Gaudi::Property<double>m_covYY{this,"CovarianceYY",4.09792105425419665e-04, "Covariance Matrix YY term"};
  Gaudi::Property<double>m_covZZ{this,"CovarianceZZ",1.85588016359800601e-02, "Covariance Matrix ZZ term"};

  
  
    
};
#endif // PGPRIMARYVERTEX_H
