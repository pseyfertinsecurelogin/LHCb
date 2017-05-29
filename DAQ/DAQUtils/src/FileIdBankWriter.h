#ifndef FILEIDBANKWRITER_H
#define FILEIDBANKWRITER_H 1

#include <string>
#include <vector>

#include "Event/FileId.h"
#include "Event/RawEvent.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class FileIdBankWriter FileIdBankWriter.h
 *
 *
 *  @author Jaap Panman
 *  @date   2009-10-01
 */
class FileIdBankWriter : public GaudiAlgorithm {
public:
  /// Standard constructor
  FileIdBankWriter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~FileIdBankWriter( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:
  std::string m_rawEventLocation;     // Location where we get the RawEvent
  std::string m_current_fname;        // current file ID string
  int         m_count_files;          // number of files read

  std::vector<unsigned int> m_bank;

  LHCb::FileId m_fileId;

private:

};
#endif // FILEIDBANKWRITER_H
