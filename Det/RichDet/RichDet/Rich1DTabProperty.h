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

/** @file Rich1DTabProperty.h
 *
 *  Header file for utility class : Rich::TabulatedProperty1D
 *
 *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
 *  @date   2003-08-13
 */

#pragma once

// Base class
#include "RichDet/Rich1DTabFunc.h"

// Gaudi
#include "DetDesc/TabulatedProperty.h"

class ISvcLocator;
class IUpdateManagerSvc;

namespace Rich
{

  /** @class TabulatedProperty1D RichDet/Rich1DTabProperty.h
   *
   *  A derived class from RichTabulatedFunction1D for tabulated properties
   *
   *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
   *  @date   2003-08-13
   *
   *  @todo Figure out why releasing the services during destruction causes a crash
   */

  class TabulatedProperty1D final : public TabulatedFunction1D
  {

  public:

    /// Default Constructor
    TabulatedProperty1D() = default;

    /** Constructor from tabulated property and gsl interpolator type
     *
     *  @param tab         Pointer to a tabulated property
     *  @param registerUMS Flag to indicate if this interpolator should register
     *                     itself to the UMS, so that it is automatically updated
     *                     when the underlying TabulatedProperty is updated
     */
    explicit TabulatedProperty1D( const TabulatedProperty *tab,
                                  const bool               registerUMS = false,
                                  const gsl_interp_type *  interType   = gsl_interp_linear );

    /// Destructor
    virtual ~TabulatedProperty1D();

    /** The underlying tabulated property used to initialise the interpolator
     *
     *  @return Pointer to the tabulated property
     */
    inline const TabulatedProperty *tabProperty() const noexcept { return m_tabProp; }

    /** @brief The UMS update method
     *
     *  Running this triggers a re-initialisation of the interpolator from
     *  the underlying tabulated property.
     *
     *  Can either be called automatically, if configured to do so at
     *  construction, or can be called "by hand" by the user
     *
     *  @return StatusCode indicating if the update was successfully or not
     */
    StatusCode updateTabProp();

    /** Initialisation from Tabulated Property
     *
     *  @param tab         Pointer to a tabulated property
     *  @param registerUMS Flag to indicate if this interpolator should register
     *                     itself to the UMS, so that it is automatically updated
     *                     when the underlying TabulatedProperty is updated
     *  @param interType GSL Interpolator type (If not given, currently configured type is used)
     */
    bool initInterpolator( const TabulatedProperty *tab,
                           const bool               registerUMS = false,
                           const gsl_interp_type *  interType   = gsl_interp_linear );

  private: // methods

    /// Service locator
    ISvcLocator *svcLocator();

    /// Access the UpdateManagerSvc
    IUpdateManagerSvc *updMgrSvc();

    /// Access the message service
    IMessageSvc *msgSvc();

    /// Set up the UMS updates for the TabulatedProperty
    bool configureUMS( const TabulatedProperty *tab );

    /** Issue an out of range warning
     *  @param x    The requested x value
     *  @param retx The x value to use (corrected to be in range)
     *  @return x value to use
     */
    virtual double rangeWarning( const double x, const double retx ) const override;

  private: // data

    /// Pointer to the underlying TabulatedProperty
    const TabulatedProperty *m_tabProp = nullptr;

    /// Flag to say if we have registered a dependency with the UMS
    bool m_registedUMS = false;
  };

} // namespace Rich
