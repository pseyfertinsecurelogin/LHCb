#ifndef WRITEPACKEDDST_H
#define WRITEPACKEDDST_H 1

#include <string>
#include <vector>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"
#include "PackedBank.h"

class ISvcLocator;
class PackedBank;
namespace LHCb {
class RawEvent;
}  // namespace LHCb

/** @class WritePackedDst WritePackedDst.h
 *  Write the packed DST as a MDF file
 *
 *  @author Olivier Callot
 *  @date   2008-12-01
 */
class WritePackedDst : public GaudiAlgorithm
{

public:

  /// Standard constructor
  WritePackedDst( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

protected:

  void storeInBlob( PackedBank& pBnk,
                    const void* data,
                    unsigned int nb,
                    unsigned int bSize );

private:

  std::vector<std::string> m_containers;
  LHCb::RawEvent* m_dst = nullptr;
  unsigned int m_blobNumber{0};
  unsigned int m_bankNb{0};

};

#endif // WRITEPACKEDDST_H
