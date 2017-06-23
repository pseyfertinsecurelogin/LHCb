#ifndef _ITReadoutTool_H
#define _ITReadoutTool_H

#include <string>

// STDAQ
#include "STReadoutTool.h"
#include "Kernel/STChannelID.h"

/** @class ITReadoutTool ITReadoutTool.h
 *
 *  Concrete class for things related to the Readout - Tell1 Board
 *
 *  @author M.Needham
 *  @date   13/3/2002
*/

class ITReadoutTool: public STReadoutTool{

public:

  /// Constructor
  ITReadoutTool(const std::string& type,
                const std::string& name,
                const IInterface* parent);


  /// init
  StatusCode initialize() override;

  /// get region
  unsigned int region(const LHCb::STChannelID aChan) const override;

  /** Add the mapping of source ID to board number for IT and TT */
  const  std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const override;

  /** Add the mapping of board number to source ID for IT and TT */
  const  std::map<unsigned int, unsigned int>& TELLNumberToSourceIDMap() const override;

private:

  StatusCode createBoards();

  unsigned int m_firstStation = 512;


};






#endif // _ITReadoutTool_H
