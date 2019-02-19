/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef _IUTReadoutTool_H
#define _IUTReadoutTool_H

#include "GaudiKernel/IAlgTool.h"

#include "Event/UTCluster.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTTell1ID.h"
#include <string>
#include <vector>

class UTTell1Board;
class DeUTSector;

namespace LHCb {
  class UTChannelID;
}

/** @class IUTReadoutTool IUTReadoutTool.h  UTDAQ/IUTReadoutTool
 *
 *  Interface Class for things related to the Readout - Tell1 Board
 *
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

// Declaration of the interface ID ( interface id, major version, minor version)

struct IUTReadoutTool : extend_interfaces<IAlgTool> {

  /// Static access to interface id
  DeclareInterfaceID( IUTReadoutTool, 1, 0 );

  /// number of boards
  virtual unsigned int nBoard() const = 0;

  /// return vector of Tell1IDs
  virtual std::vector<UTTell1ID> boardIDs() const = 0;

  /// convert UTChannelID to DAQ ChannelID
  virtual UTDAQ::chanPair offlineChanToDAQ( const LHCb::UTChannelID aOfflineChan, double isf ) const = 0;

  /// convert offline interStripFraction to DAQ interStripFraction
  virtual double interStripToDAQ( const LHCb::UTChannelID aOfflineChan, const UTTell1ID aBoardID,
                                  const double isf ) const = 0;

  virtual bool ADCOfflineToDAQ( const LHCb::UTChannelID aOfflineChan, const UTTell1ID aBoardID,
                                LHCb::UTCluster::ADCVector& adcs ) const = 0;

  /// find the Tell1 board given a board ID
  virtual UTTell1Board* findByBoardID( const UTTell1ID aBoardID ) const = 0;

  /// find a Tell board by storage order
  virtual UTTell1Board* findByOrder( const unsigned int aValue ) const = 0;

  /// get region
  virtual unsigned int region( const LHCb::UTChannelID aChan ) const = 0;

  /// service box
  virtual unsigned int nServiceBox() const = 0;

  /// service box number
  virtual std::string serviceBox( const LHCb::UTChannelID& aChan ) const = 0;

  /// printout
  virtual void printMapping() const = 0;

  /// write an xml file
  virtual StatusCode writeMappingToXML() const = 0;

  /// list of the readout sector ids on the board in a board
  virtual std::vector<LHCb::UTChannelID> sectorIDs( const UTTell1ID board ) const = 0;

  /// list of the readout sectors in a board
  virtual std::vector<DeUTSector*> sectors( const UTTell1ID board ) const = 0;

  /// list of the readout sectors ids in a service box
  virtual std::vector<LHCb::UTChannelID> sectorIDsOnServiceBox( const std::string& serviceBox ) const = 0;

  /// list of the readout sectors in a service box
  virtual std::vector<DeUTSector*> sectorsOnServiceBox( const std::string& serviceBox ) const = 0;

  /// list of service boxes
  virtual const std::vector<std::string>& serviceBoxes() const = 0;

  /// Add the mapping of source ID to TELL1 board number
  virtual const std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const = 0;

  /// Add the mapping of source ID to TELL1 board number
  virtual unsigned int SourceIDToTELLNumber( unsigned int sourceID ) const = 0;

  /// Add the mapping of TELL1 board number to source ID
  virtual const std::map<unsigned int, unsigned int>& TELLNumberToSourceIDMap() const = 0;

  /// Add the mapping of TELL1 board number to source ID
  virtual unsigned int TELLNumberToSourceID( unsigned int sourceID ) const = 0;
};

#endif // _IUTReadoutTool_H
