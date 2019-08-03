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
/** @file DeRichSingleSolidRadiator.h
 *
 *  Header file for detector description class : DeRichSingleSolidRadiator
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */
//----------------------------------------------------------------------------

#pragma once

// Include files
#include "RichDet/DeRichRadiator.h"

// External declarations
extern const CLID CLID_DeRichSingleSolidRadiator;

//----------------------------------------------------------------------------
/** @class DeRichSingleSolidRadiator RichDet/DeRichSingleSolidRadiator.h
 *
 * Implementation of DeRichRadiator for single solids (can be boolean solids)
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */
//----------------------------------------------------------------------------

class DeRichSingleSolidRadiator : public DeRichRadiator {

public:
  /**
   * Constructor for this class
   */
  DeRichSingleSolidRadiator( const std::string& name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRichSingleSolidRadiator() = default;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  inline const CLID& clID() const override { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID();

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  virtual StatusCode initialize() override;

  // Finds the next intersection point with radiator.
  StatusCode nextIntersectionPoint( const Gaudi::XYZPoint&  pGlobal,    //
                                    const Gaudi::XYZVector& vGlobal,    //
                                    Gaudi::XYZPoint&        returnPoint //
                                    ) const override;

  // Finds the entry and exit points of the radiator. For boolean solids
  // this is the first and last intersection point.
  virtual StatusCode intersectionPoints( const Gaudi::XYZPoint&  position,   //
                                         const Gaudi::XYZVector& direction,  //
                                         Gaudi::XYZPoint&        entryPoint, //
                                         Gaudi::XYZPoint&        exitPoint   //
                                         ) const override;

  // Finds the intersection points with radiator. For boolean solids there
  // can be more than two intersection points
  virtual unsigned int intersectionPoints( const Gaudi::XYZPoint&        pGlobal, //
                                           const Gaudi::XYZVector&       vGlobal, //
                                           std::vector<Gaudi::XYZPoint>& points   //
                                           ) const override;

  // Finds the intersections (entry/exit) with radiator. For boolean solids there
  //  can be more than one intersections
  virtual unsigned int intersections( const Gaudi::XYZPoint&         pGlobal,      //
                                      const Gaudi::XYZVector&        vGlobal,      //
                                      Rich::RadIntersection::Vector& intersections //
                                      ) const override;

  // Returns the refractive index at the given photon energy for this radiator
  virtual double refractiveIndex( const double energy, const bool hlt = true ) const override;

  /**
   * Returns a pointer to the material of this radiator
   *
   * @return Pointer to the material
   */
  inline const Material* material() const noexcept { return m_material; }

protected:
  /// prepare the momentum vector for the calculation of the
  /// refractive index
  StatusCode prepareMomentumVector( std::vector<double>& photonMomentumVect, //
                                    const double         min,                //
                                    const double         max,                //
                                    const unsigned int   nbins               //
                                    ) const;

private:
  const ISolid*   m_solid    = nullptr; ///< top most solid of the radiator
  const Material* m_material = nullptr; ///< pointer to the radiator material
};
