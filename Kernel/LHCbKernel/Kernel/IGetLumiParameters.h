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
#ifndef IGETLUMIPARAMETERS_H
#define IGETLUMIPARAMETERS_H 1

// Include files
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** @class IGetLumiParameters IGetLumiParameters.h
 *
 *  Interface for Luminosity parameters tool
 *
 *  @author Jaap Panman
 *  @date   2010-11-20
 */
struct IGetLumiParameters : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IGetLumiParameters, 2, 0 );

  [[nodiscard]] virtual unsigned int        getTCK() const            = 0; ///< retrieve TCK
  virtual long                              CollidingBunches()        = 0; ///< retrieve number of colliding bunches
  [[nodiscard]] virtual double              OdinFraction() const      = 0; ///< retrieve Odin Random BB fraction
  [[nodiscard]] virtual double              HLTRandomRate() const     = 0; ///< retrieve random rate in HLT
  [[nodiscard]] virtual double              LHCFrequency() const      = 0; ///< retrieve revolution frequency
  virtual double                            RandomRateBB()            = 0; ///< retrieve random bunch-bunch rate in HLT
  [[nodiscard]] virtual std::vector<double> CalibRelative() const     = 0; ///< relative calibration factors
  [[nodiscard]] virtual std::vector<double> CalibCoefficients() const = 0; ///< usage factors
  [[nodiscard]] virtual std::vector<double> CalibRelativeLog() const  = 0; ///< relative calibration factors
  [[nodiscard]] virtual std::vector<double> CalibCoefficientsLog() const = 0; ///< usage factors
  [[nodiscard]] virtual double              CalibScale() const           = 0; ///< absolute scale
  [[nodiscard]] virtual double              CalibScaleError() const      = 0; ///< absolute scale error
  [[nodiscard]] virtual double              StatusScale() const          = 0; ///< status scale flag
};
#endif // IGETLUMIPARAMETERS_H
