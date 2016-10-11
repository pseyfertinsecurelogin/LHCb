#ifndef INEUTRALIDTOOL_H 
#define INEUTRALIDTOOL_H 1

// Include files
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Event/CaloHypo.h"
#include "ICaloHypoEstimator.h"

/** @class INeutralIDTool INeutralIDTool.h
 *  @author Mostafa HOBALLAH --
 *  @date   2013-07-25
 */
struct INeutralIDTool : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID(INeutralIDTool, 3, 0 );

  virtual double isNotE(const LHCb::CaloHypo* hypo,ICaloHypoEstimator* e=nullptr)=0;
  virtual double isNotH(const LHCb::CaloHypo* hypo,ICaloHypoEstimator* e=nullptr)=0;

  virtual double isNotE(const double* v)=0;
  virtual double isNotH(const double* v)=0;

  virtual void Variables(const LHCb::CaloHypo* hypo,  double& clmatch, double& prse, 
                         double& e19, double& hclecl, double& prse19,double& prse49, double& sprd, double& prse4mx, 
                         double& prsm, double& spdm)=0;

  
};
#endif // INEUTRALIDTOOL_H
