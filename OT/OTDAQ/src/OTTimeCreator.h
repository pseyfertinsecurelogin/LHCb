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
#ifndef OTDAQ_OTTIMECREATOR_H
#define OTDAQ_OTTIMECREATOR_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Kernel
#include "GaudiKernel/ToolHandle.h"
#include "OTDAQ/IOTRawBankDecoder.h"

// forward declarations
struct IOTRawBankDecoder;
class DeOTDetector;

/** @class OTTimeCreator OTTimeCreator.h
 *
 *
 *  @author Jacopo Nardulli
 *  @date   2004-06-06
 */

class OTTimeCreator : public GaudiAlgorithm {
public:
  /// Standard constructor
  OTTimeCreator( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode finalize() override;
  StatusCode execute() override; ///< Algorithm execution

private:
  // job options
  std::string                   m_timeLocation;
  const DeOTDetector*           m_tracker = nullptr; ///< Pointer to XML geometry
  ToolHandle<IOTRawBankDecoder> m_decoder = {"OTRawBankDecoder"};
};

#endif // OTDAQ_OTTIMECREATOR_H
