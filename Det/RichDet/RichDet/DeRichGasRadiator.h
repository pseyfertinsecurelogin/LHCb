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

//----------------------------------------------------------------------------
/** @file DeRichGasRadiator.h
 *
 *  Header file for detector description class : DeRichGasRadiator
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2006-03-02
 */
//----------------------------------------------------------------------------

#pragma once

// Include files
#include "RichDet/DeRichSingleSolidRadiator.h"

// External declarations
extern const CLID CLID_DeRichGasRadiator;

/** @class DeRichGasRadiator RichDet/DeRichGasRadiator.h
 *
 *  Detector element for gaseous radiator volumes
 *
 *  @author Antonis Papanestis
 *  @date   2006-03-02
 */
class DeRichGasRadiator : public DeRichSingleSolidRadiator
{

public:

  /// Standard constructor
  DeRichGasRadiator( const std::string &name = "" );

  virtual ~DeRichGasRadiator() = default; ///< Destructor

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  inline const CLID &clID() const override final { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID &classID();

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  StatusCode initialize() override final;

private:

  /// method to update the refractive index of the radiator
  StatusCode updateProperties();

  /// method to update the refractive index of the radiator for the HLT
  StatusCode updateHltProperties();

  /**
   * Generates and returns the refractive index of the radiator for use by the HLT
   * @return A pointer to the HLT refractive index interpolated function of the radiator
   * @retval nullptr No interpolation function
   */
  StatusCode generateHltRefIndex() override;

  /** Method for the calculation of the refractive index from the Sellmeir
   *  coeficients and update of the Tabulated Property */
  StatusCode calcSellmeirRefIndex( const std::vector< double > &momVect,
                                   TabulatedProperty *          tabProp,
                                   const SmartRef< Condition > &gasParamCond ) const;

  /// method to use the old separate temperature and pressure conditions
  StatusCode setupOldGasConditions();

private: // data

  /// Condition holding the current temperature of radiator
  SmartRef< Condition > m_temperatureCond;

  /// Condition holding the current pressure of radiator
  SmartRef< Condition > m_pressureCond;

  /// Condition holding the current pressure of radiator for use Offline
  SmartRef< Condition > m_gasParametersCond;

  /// Condition holding the current pressure of radiator for use in the HLT
  SmartRef< Condition > m_hltGasParametersCond;

  /// Condition holding the scale factor for the refractivity
  /// This conditions scales n-1 NOT n
  SmartRef< Condition > m_scaleFactorCond;
};
