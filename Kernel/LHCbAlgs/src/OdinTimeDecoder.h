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
#ifndef ODINTIMEDECODER_H
#define ODINTIMEDECODER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/IGenericTool.h"
#include "GaudiKernel/IEventTimeDecoder.h" // Interface
#include "GaudiKernel/ToolHandle.h"

// forward declarations
class IDataProviderSvc;
class IIncidentSvc;
namespace LHCb {
  class ODIN;
}

/** @class OdinTimeDecoder OdinTimeDecoder.h
 *
 *
 *  @author Marco Clemencic
 *  @date   2006-09-21
 */
class OdinTimeDecoder final : public extends<GaudiTool, IEventTimeDecoder> {

public:
  // inherited constructor
  OdinTimeDecoder( const std::string& type, const std::string& name, const IInterface* parent )
      : base_class( type, name, parent ) {}

  virtual ~OdinTimeDecoder() = default; ///< Destructor

  /// Initialize the tool
  StatusCode initialize() override;

  // --- implementation of IEventTimeDecoder ---
  /// Get the time of the current event from the ODIN object.
  /// @return The time of current event.
  Gaudi::Time getTime() const override;

private:
  // --- local methods ---
  /// Get the ODIN object from the T.E.S. or create it.
  /// @return Pointer to the ODIN object or NULL if it cannot be found.
  LHCb::ODIN* getODIN() const;

private:
  /// Tool to decode the ODIN bank
  ToolHandle<IGenericTool> m_odinDecoder{"ODINDecodeTool", this};

  // --- local data ---
  /// Used to remember the run number and spot a change of run number.
  mutable unsigned int m_currentRun{0};

  /// Used to detect if we switch from flagging to filtering mode (which implies
  /// a "run change" without actually changing the run number).
  mutable bool m_flaggingMode{true};
};

#endif // ODINTIMEDECODER_H
