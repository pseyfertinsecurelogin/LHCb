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

//----------------------------------------------------------------------------
/** @file DeRichAerogelRadiator.h
 *
 *  Header file for detector description class : DeRichAerogelRadiator
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2006-03-02
 */
//----------------------------------------------------------------------------

#pragma once

// Include files
#include "RichDet/DeRichSingleSolidRadiator.h"

/** @class DeRichAerogelRadiator RichDet/DeRichAerogelRadiator.h
 *
 *  Detector element for aerogel radiator volumes
 *
 *  @author Antonis Papanestis
 *  @date   2006-03-02
 */
class DeRichAerogelRadiator : public DeRichSingleSolidRadiator
{

public:

  /// Standard constructor
  DeRichAerogelRadiator( const std::string &name = "" );

  virtual ~DeRichAerogelRadiator() = default; ///< Destructor

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  inline const CLID &clID() const override { return classID(); }

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
  StatusCode initialize() override;

public:

  /// Indicates if this represents a sub-tile(true) or full primary tile (false)
  inline bool subTile() const noexcept { return m_subTile; }

  /// Access the primary Aerogel tile ID number
  inline int primaryTileID() const noexcept { return m_tileNumber; }

  /** Returns the sub tile copy number (unique number for all subtiles)
   *
   *  If sub tiles are not active, returns -1
   */
  inline int subtileCopynumber() const noexcept { return m_subtilecopynumber; }

  /** Returns the sub-tile number within a primary tile
   *
   *  If sub tiles are not active, returns -1
   */
  inline int subtileIDInTile() const noexcept { return m_subtileNumInTile; }

  /// Returns a unique tile ID number, when sub-tiles are both active and inactive
  inline int tileID() const
  {
    return ( !subTile() ? primaryTileID() : 1000 * primaryTileID() + subtileIDInTile() );
  }

private:

  /// method to update the refractive index of the radiator
  StatusCode updateProperties();

  /// method for the calculation of the refractive index from the Sellmeir
  /// coeficients and update of the Tabulated Property
  StatusCode calcSellmeirRefIndex( const std::vector< double > &momVect,
                                   TabulatedProperty *          tabProp );

  /// method for the calculation of Rayleigh scattering from
  /// clarity and update of the Tabulated Property
  StatusCode calcRayleigh( const std::vector< double > &momVect, TabulatedProperty *tabProp );

  /// method for the calculation of Absorption from const A
  /// and update of the Tabulated Property
  StatusCode calcAbsorption( const std::vector< double > &momVect, TabulatedProperty *tabProp );

  /// Access on demand the DeRich1 detector element
  inline DetectorElement *deRich1() const noexcept { return m_deRich1; }

private:

  /// access to DeRich1 for user parameters
  DetectorElement *m_deRich1 = nullptr;

  /// Condition holding the aerogel parameters
  SmartRef< Condition > m_AerogelCond;

  /// conertion from nm to eV
  double m_photMomWaveConv { 0 };

  /// Aerogel tile number
  int m_tileNumber { -1 };

  /// Aerogel sub tile copy number
  int m_subtilecopynumber { -1 };

  /// Aerogel subtile number in a tile
  int m_subtileNumInTile { -1 };

  /// Flag to say if this is a sub tile or not
  bool m_subTile { true };
};
