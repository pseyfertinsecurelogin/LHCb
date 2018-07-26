/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "GaudiAlg/Consumer.h"
#include "GaudiAlg/IGenericTool.h"
#include "GaudiKernel/ToolHandle.h"

/** @class ODINEncoder ODINEncoder.cpp
 *
 * \brief Trivial algorithm that calls the ODINEncodeTool.
 *
 * @code
 * from Configurables import ODINEncoder, ODINEncodeTool
 * ODINEncoder().addTool(ODINEncodeTool)
 * ODINEncoder().ODINEncodeTool.ODINLocation = "ODIN/Emulated"
 * @endcode
 *
 */
class ODINEncoder final : public Gaudi::Functional::Consumer<void()> {
public:
  ODINEncoder( const std::string& name, ISvcLocator* pSvcLocator ) : Consumer( name, pSvcLocator ) {}

  StatusCode initialize() override {
    const StatusCode sc = Consumer::initialize();
    if ( !sc ) return sc;
    if ( !m_encodeOdin.retrieve().isSuccess() ) return StatusCode::FAILURE;
    return sc;
  }

  void operator()() const override { m_encodeOdin->execute(); }

private:
  mutable ToolHandle<IGenericTool> m_encodeOdin{"ODINEncodeTool", this};
};

DECLARE_COMPONENT( ODINEncoder )
