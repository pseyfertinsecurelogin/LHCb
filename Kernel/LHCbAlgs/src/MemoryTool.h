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
// ============================================================================
#ifndef MEMORYTOOL_H
#define MEMORYTOOL_H 1

// Include files
#include "GaudiKernel/HistoDef.h"
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/IGenericTool.h"            // Interface

/** @class MemoryTool MemoryTool.h
 *  Tool to plot memory usage of the application at each call
 *
 *  New Memory measurements:
 *   - counter & plot of virtual memory
 *   - counter & plot of virtual memory increment
 *   - warnings: seek for suspicion events: total memory & delta memory
 *   - regular check for the tendency
 *
 *  @author Marco Cattaneo
 *  @date   2005-12-14
 */
class MemoryTool final : public extends<GaudiHistoTool, IGenericTool> {

 public:
  /// Standard constructor
  MemoryTool( const std::string& type,
              const std::string& name,
              const IInterface* parent);

  void execute() override;
  StatusCode initialize() override;
  StatusCode finalize () override;

private:

  Gaudi::Property<unsigned int> m_bins{this, "HistoSize", 500, "Number of bins of histogram"};
  Gaudi::Property<unsigned int> m_skip{this, "SkipEvents", 10, "Skip the first N events from delta memory counter"};
  Gaudi::Property<Gaudi::Histo1DDef> m_histo1{this, "TotalMemoryHisto", {"Total Memory [MB]", 0, 2000}, "The parameters of 'total memory' histogram"};
  Gaudi::Property<Gaudi::Histo1DDef> m_histo2{this, "DeltaMemoryHisto", {"Delta Memory [MB]", -25, 25}, "The parameters of 'delta memory' histogram"};
  Gaudi::Property<unsigned int> m_check{this, "Check", 20, "Frequency for checks for suspision memory leak"};
  Gaudi::Property<unsigned int> m_maxPrint{this, "MaxPrints", 0, "Maximal number of print-out"};

  /// the previous measurement of virtual memory
  mutable std::atomic<long> m_prev = {-1000000000};
  ///< Counter of calls to the tool
  mutable std::atomic<long long> m_counter = {0}; ///< Counter of calls to the tool
  /// the counter for total memory
  StatEntity &m_totMem;
  /// the counter for delta memory
  StatEntity &m_delMem;
  /// the histogram of total memory
  AIDA::IHistogram1D* m_plot1 = nullptr;
  /// the histogram of delta memory
  AIDA::IHistogram1D* m_plot2 = nullptr;
};
#endif // MEMORYTOOL_H
