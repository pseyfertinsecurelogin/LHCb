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
#ifndef _TTReadoutTool_H
#define _TTReadoutTool_H

#include <string>

// STDAQ
#include "STReadoutTool.h"
#include "Kernel/STChannelID.h"

/** @class TTReadoutTool TTReadoutTool.h
 *
 *  Concrete class for things related to the Readout - Tell1 Board
 *
 *  @author M.Needham
 *  @date   13/3/2002
*/

class TTReadoutTool: public STReadoutTool{

public:

  /// Constructer
  TTReadoutTool(const std::string& type,
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

  unsigned int m_nRegionA     = 512;
  unsigned int m_firstStation = 512;

};

#endif // _TTReadoutTool_H
