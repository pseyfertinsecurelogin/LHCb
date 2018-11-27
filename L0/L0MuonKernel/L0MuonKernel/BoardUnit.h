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
#ifndef L0MUONKERNEL_BOARDUNIT_H
#define L0MUONKERNEL_BOARDUNIT_H     1

#include <boost/dynamic_bitset.hpp>
#include <vector>
#include "L0MuonKernel/ProcUnit.h"
#include "L0MuonKernel/L0MUnit.h"


namespace L0Muon {

  /** @class BoardUnit L0MuonKernel/BoardUnit.h

  Class representing a processing board of the L0Muon processor.

  It inherits from L0MUnit.

  There is one such unit per processing board. Its parent is a CrateUnit.

  If fully configured, it contains:
  - 4 ProcUnit,
  - 1 BCSUnit.

  This unit is essentialy a container. It does not have any input nor output registers.
  It triggers the initialize, execute, ... methods of its daughters.

  @author  Luisanna Tocco, Julien Cogan
  @date  24 September 2003
  */
  class BoardUnit : public L0MUnit {

  public:

    /// Default Constructor
    BoardUnit();

    /// Constructor with a MuonTileID
    BoardUnit(LHCb::MuonTileID id);

    /// Constructor from a xml node
    BoardUnit(DOMNode* pNode);

    /// Destructor
    ~BoardUnit();

    /// Give a static type name to the unit
    std::string type() override {
      return "BoardUnit";
    }

  private:


  };


}  // namespace L0Muon

#endif      // L0MUONKERNEL_BOARDUNIT_H
