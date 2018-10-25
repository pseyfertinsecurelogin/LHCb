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
// Include files
#include "AIDA/IHistogram1D.h"
#include "GaudiKernel/Memory.h"
#include "MemoryTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MemoryTool
//
// 2005-12-14 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( MemoryTool )

// ============================================================================
// Standard constructor, initializes variables
// ============================================================================
MemoryTool::MemoryTool(const std::string& type,
                        const std::string& name,
                       const IInterface* parent) :
  base_class(type, name, parent),
  m_totMem(counter("Total Memory/MB")),
  m_delMem(counter("Delta Memory/MB")) {
  setProperty ("HistoPrint", false);
  declareInterface<IGenericTool>(this);
}
// ============================================================================
// initialize the tool
// ============================================================================
StatusCode MemoryTool::initialize() {
  auto sc = base_class::initialize ();
  if (produceHistos()) {
     m_plot1  = book( m_histo1 ) ;
     m_plot2  = book( m_histo2 ) ;
  }
  return sc;
}

// ============================================================================
// finalize the tool
// ============================================================================
StatusCode MemoryTool::finalize () {
  if (1 < m_delMem.nEntries()    &&
      0 < m_delMem.flagMean()    &&
      0 < m_delMem.flagMeanErr() &&
      m_delMem.flagMean() > 3 * m_delMem.flagMeanErr()) {
    Warning("Mean 'delta-memory' exceeds 3*sigma", StatusCode::SUCCESS).ignore();
  }
  m_plot1 = nullptr;
  m_plot2 = nullptr;
  return base_class::finalize ();
}
//=============================================================================
// Plot the memory usage
//=============================================================================
void MemoryTool::execute()
{
  const auto lmem = System::virtualMemory();
  const double mem = lmem;

  // get/set "previous" measurement
  auto prev = double(m_prev.exchange(lmem))/1000.;  // memory in MB

  const auto memMB = mem / 1000. ;

  /// grab current value, and (post!)increment event counter
  auto counter = m_counter++;

  // Fill the counter for "valid" previous measurements
  m_totMem += memMB;
  // Fill the plot
  fill (m_plot1, memMB, 1, m_histo1.value().title());

  // Fill the counter for "valid" previous measurements
  const auto deltaMem = memMB - prev;
  if (0 <= m_prev && (m_skip.value() == 0 || m_counter >= m_skip.value())) {
    // fill the counter
    m_delMem += deltaMem;
    // fill the counter
    fill(m_plot2, deltaMem, 1, m_histo2.value().title());
  }


  if (m_bins.value() > 0) {
    if (m_bins.value() > counter) {
      plot(counter+1, "Virtual mem, all entries", "Virtual memory (kB), first 'HistoSize' entries",
           0.5, m_bins.value()+0.5, m_bins.value(), mem);
    }
    if (0 == counter % m_bins.value()) {
      const unsigned int bin = 1 + (counter/m_bins.value());
      plot(bin, "Virtual mem, downscaled", "Virtual memory (kB), downscaled entries",
           0.5, m_bins.value()+0.5, m_bins.value(), mem);
    }
  }


  // check Total Memory for the particular event
  if ( 16 <  m_totMem.nEntries() &&
       0  <  m_totMem.flagMean() &&
       0  <  m_totMem.flagRMS()  &&
       0  <  memMB               &&
      memMB > m_totMem.flagMean() + 3 * m_totMem.flagRMS ()) {
    Warning ("Total Memory for the event exceeds 3*sigma" ,
             StatusCode::SUCCESS, m_maxPrint.value()).ignore() ;
    if (UNLIKELY(msgLevel(MSG::DEBUG)))
      debug () << " Total Memory : " << memMB
               << " Mean : (" << m_totMem.flagMean ()
               << "+-" << m_totMem.flagRMS() << ")" << endmsg ;
  }
  // check the particular event
  if ( 0 <= prev                &&
      16 <  m_delMem.nEntries() &&
       0 <  m_delMem.flagRMS()  &&
       0 <  deltaMem            &&
      deltaMem > m_delMem.flagMean() + 3 * m_delMem.flagRMS ()) {
    Warning ("Delta Memory for the event exceeds 3*sigma" ,
             StatusCode::SUCCESS, m_maxPrint.value()).ignore() ;
    if (UNLIKELY(msgLevel(MSG::DEBUG)))
      debug () << " Delta Memory : " << deltaMem
               << " Mean : (" << m_delMem.flagMean ()
               << "+-" << m_delMem.flagRMS() << ")" << endmsg ;
  }
  /// check the tendency:
  if (((0 < m_check.value() && 0 == counter % m_check.value()) || 1 == m_check.value()) &&
      16 < m_delMem.nEntries()    &&
      0  < m_delMem.flagMean()    &&
      0  < m_delMem.flagMeanErr() &&
      m_delMem.flagMean() > 3 * m_delMem.flagMeanErr()) {
    Warning ("Mean 'Delta-Memory' exceeds 3*sigma" ,
             StatusCode::SUCCESS, m_maxPrint.value()).ignore() ;
    if (UNLIKELY(msgLevel(MSG::DEBUG)))
      debug () << " Memory Leak? "
               << "("  << m_delMem.flagMean()
               << "+-" << m_delMem.flagMeanErr() << ")" << endmsg ;
  }

}
