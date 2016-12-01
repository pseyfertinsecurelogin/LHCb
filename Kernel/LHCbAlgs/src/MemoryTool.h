// $Id: MemoryTool.h,v 1.5 2009-10-08 13:34:27 ibelyaev Exp $
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
class MemoryTool final : public GaudiHistoTool, virtual public IGenericTool {

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

  ///< Counter of calls to the tool
  unsigned int m_counter{0};
  /// the previous measurement of virtual memory
  double m_prev{-1.e+6};  
  /// the counter for total memory
  StatEntity &m_totMem;
  /// the counter for delta memory
  StatEntity &m_delMem;
  /// the histogram of total memory
  AIDA::IHistogram1D* m_plot1 = nullptr;
  /// the histogram of delta memory
  AIDA::IHistogram1D* m_plot2 = nullptr;
  /// Mutex to secure non thread safe code, that is on demand initializations
  std::mutex m_mutex;
};
#endif // MEMORYTOOL_H
