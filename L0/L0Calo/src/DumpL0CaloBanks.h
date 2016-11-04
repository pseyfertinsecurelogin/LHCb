// $Id: DumpL0CaloBanks.h,v 1.2 2008-07-16 20:20:17 robbep Exp $
#ifndef DUMPL0CALOBANKS_H
#define DUMPL0CALOBANKS_H 1
// Include files

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

/** @class DumpL0CaloBanks DumpL0CaloBanks.h
 *  Algorithm to dump the content of the L0Calo banks
 *  to a file for debuggin
 *
 *  @author Patrick Robbe
 *  @date   27/05/2008
 */
class DumpL0CaloBanks: public GaudiAlgorithm {
public:
  /// Standard constructor
  DumpL0CaloBanks(const std::string& name, ISvcLocator* pSvcLocator );

  /// Standard destructor
  virtual ~DumpL0CaloBanks( );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:
  int m_event ;
  std::string m_fileName ; ///< File name
  std::ofstream * m_file ; ///< the file object
  bool m_nzsBanks ; ///< Dump Non zero-suppressed banks
  bool m_errorBanks ; ///< Dump error banks
};
#endif // DUMPL0CALOBANKS_H
