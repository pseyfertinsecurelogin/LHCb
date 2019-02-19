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
// $Id: HistoCondition.h,v 1.1 2009-10-23 09:43:13 ibelyaev Exp $
// ============================================================================
#ifndef DETDESC_HISTOCONDITION_H
#define DETDESC_HISTOCONDITION_H 1
// ============================================================================
// Include files
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/Condition.h"
#include "DetDesc/HistoParam.h"
// ============================================================================
namespace DetDesc {
  // ===========================================================================
  /** @class Histo1DCond DetDesc/HistoCondition.h
   *  Simple condition which "keeps" 1D histogram
   *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
   *  @date   2009-10-22
   */
  class Histo1DCond : public Condition {
  public:
    // ========================================================================
    /// the constructor
    Histo1DCond(); //    the constructor
    // ========================================================================
  public:
    // ========================================================================
    /// perform the specific initialization (==creation of the histogram)
    StatusCode initialize() override;
    /// update from other data object:
    using Condition::update;
    void update( ValidDataObject& right ) override;
    /// update from other data object:
    virtual void update( Histo1DCond& right );
    // ========================================================================
  public:
    // ========================================================================
    /// The unique Class ID
    const CLID& clID() const override { return classID(); }
    /// The unique Class ID
    static const CLID& classID(); // The unique Class ID
    // ========================================================================
  public:
    // ========================================================================
    /// acces to the histogram ( only const version)
    inline const TH1D& histo() const { return m_histo; }
    // ========================================================================
  private:
    // ========================================================================
    /// the histogram itself
    TH1D m_histo; // the histogram itself
    // ========================================================================
  };
  // ===========================================================================
  /** @class Histo2DCond DetDesc/HistoCondition.h
   *  Simple condition which "keeps" 2D histogram
   *  @author Vanya BELYAEV Ivan.Bwlyaev@nikhef.nl
   *  @date   2009-10-22
   */
  class Histo2DCond : public Condition {
  public:
    // ========================================================================
    /// the constructor
    Histo2DCond(); //    the constructor
    // ========================================================================
  public:
    // ========================================================================
    /// perform the specific initialization (==creation of the histogram)
    StatusCode initialize() override;
    /// update from other data object:
    using Condition::update;
    void update( ValidDataObject& right ) override;
    /// update from other data object:
    virtual void update( Histo2DCond& right );
    // ========================================================================
  public:
    // ========================================================================
    /// The unique Class ID
    const CLID& clID() const override { return classID(); }
    /// The unique Class ID
    static const CLID& classID(); // The unique Class ID
    // ========================================================================
  public:
    // ========================================================================
    /// acces to the histogram ( only const version)
    inline const TH2D& histo() const { return m_histo; }
    // ========================================================================
  private:
    // ========================================================================
    /// the histogram itself
    TH2D m_histo; // the histogram itself
    // ========================================================================
  };
  // ==========================================================================
} //                                                   end of namespace DetDesc
// ============================================================================
// The END
// ============================================================================
#endif // DETDESC_HISTOCONDITION_H
