#ifndef _UTREADOUTTOOL_H
#define _UTREADOUTTOOL_H

#include <map>
#include <string>

#include "GaudiKernel/StatusCode.h"
#include "Kernel/STChannelID.h"
// STDAQ
#include "STReadoutTool.h"

class IInterface;
namespace LHCb {
class STChannelID;
}  // namespace LHCb

/** @class UTReadoutTool UTReadoutTool.h
 *
 *
 *  @author Jianchun Wang
 *  @date   17/7/2012
*/

class UTReadoutTool: public STReadoutTool{

public:

  /// Constructer
  UTReadoutTool(const std::string& type,
                const std::string& name,
                const IInterface* parent);


  /// init
  StatusCode initialize() override;

  /// get region
  unsigned int region(const LHCb::STChannelID aChan) const override;

  /** Add the mapping of source ID to board number for IT / TT / UT */
  const  std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const override;

  /** Add the mapping of board number to source ID for IT / TT / UT */
  const  std::map<unsigned int, unsigned int>& TELLNumberToSourceIDMap() const override;

private:

  StatusCode createBoards();
  StatusCode createTell1Map();


  unsigned int m_nRegionA;
  unsigned int m_firstStation;

};






#endif // _UTREADOUTTOOL_H
