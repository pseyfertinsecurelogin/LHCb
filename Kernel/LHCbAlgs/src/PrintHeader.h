// $Id: PrintHeader.h,v 1.1 2007-03-02 08:49:27 cattanem Exp $
#ifndef PRINTHEADER_H
#define PRINTHEADER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class PrintHeader PrintHeader.h cmt/PrintHeader.h
 *
 *  Print event and run number in debug mode
 *
 *  @author Gloria Corti
 *  @date   2003-03-16
 */

class PrintHeader final : public GaudiAlgorithm
{

public:

  /// Standard constructor
  PrintHeader( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~PrintHeader( ) = default; ///< Destructor

  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  bool        m_printFile;           ///< Flag to control printing of file name
  unsigned long long  m_nEvents{0};  ///< Counter of events processed

};
#endif // CMT_PRINTHEADER_H
