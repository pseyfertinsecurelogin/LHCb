// $Id: MemoryTool.h,v 1.5 2009-10-08 13:34:27 ibelyaev Exp $
// ============================================================================
#ifndef MEMORYTOOL_H
#define MEMORYTOOL_H 1
#include <string>

// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/IGenericTool.h"            // Interface
// ============================================================================
// Include files
// ============================================================================
//  GaudiKernel
// ============================================================================
#include "GaudiKernel/HistoDef.h"
#include "GaudiKernel/StatusCode.h"

class IInterface;
class StatEntity;
namespace AIDA {
class IHistogram1D;
}  // namespace AIDA

// ============================================================================
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
class MemoryTool final : public GaudiHistoTool, virtual public IGenericTool
{

 public:
  /// Standard constructor
  MemoryTool( const std::string& type,
              const std::string& name,
              const IInterface* parent);

  virtual ~MemoryTool( ) = default; ///< Destructor

  void execute() override; ///< Plot the current memory usage

  StatusCode finalize ()  override;

private:
  // ==========================================================================
  unsigned long long m_counter{0}; ///< Counter of calls to the tool
  unsigned int  m_bins{0};  ///< Number of bins of histogram (Property HistoSize)
  // ==========================================================================
  /// flag to skip/reset events for memory measurements
  unsigned int  m_skip   ;   // flag to skip/reset events for memory measurements
  /// the previous measurement of virtual memory
  double        m_prev{-1.e+6};
  // ==========================================================================
  /// the histogram definition (as property)
  Gaudi::Histo1DDef m_histo1 ;         // the histogram definition (as property)
  /// the histogram definition (as property)
  Gaudi::Histo1DDef m_histo2 ;         // the histogram definition (as property)
  // ==========================================================================
  /// how often check for the memory leaks ?
  unsigned int m_check    ; // how often check for the memory leaks ?
  /// maximalnumber of printouts
  unsigned int m_maxPrint ; // maximal number of printouts
  // ==========================================================================
private:
  // ==========================================================================
  /// the counter for total memory
  StatEntity* m_totMem = nullptr;
  /// the counter for delta memory
  StatEntity* m_delMem = nullptr;
  /// the histogram of total memory
  AIDA::IHistogram1D* m_plot1 = nullptr;
  /// the histogram of delta memory
  AIDA::IHistogram1D* m_plot2 = nullptr;
  // ==========================================================================
};
// ============================================================================
// The END
// ============================================================================
#endif // MEMORYTOOL_H
// ============================================================================
