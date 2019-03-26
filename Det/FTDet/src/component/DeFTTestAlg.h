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
#ifndef DEFTTESTALG_H
#define DEFTTESTALG_H 1

// Gaudi
#include "GaudiAlg/GaudiTupleAlg.h"

// LHCb Event
#include "Event/MCHit.h"

// FTDet
#include "FTDet/DeFTDetector.h"

/** @class DeFTTestAlg DeFTTestAlg.h "FTDet/DeFTTestAlg.h"
 *
 *  A GaudiAlgorithm with solely test purposes, which calls (tests)
 *  the methods defined in the FT det. element classes.
 *
 *  @author Plamen Hopchev
 *  @date   2012-04-25
 */

class DeFTTestAlg : public GaudiTupleAlg {

public:
  /// Standard constructor
  using GaudiTupleAlg::GaudiTupleAlg;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  /// A pointer to the top-level FT det. element
  DeFTDetector* m_deFT = nullptr;

  Gaudi::Property<std::string> m_mcHitsLocation{this, "MCHitsLocation", LHCb::MCHitLocation::FT,
                                                "TES location of the FT MCHits"};
};
#endif // DEFTTESTALG_H
