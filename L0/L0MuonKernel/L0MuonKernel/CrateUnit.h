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
#ifndef L0MUONKERNEL_CRATEUNIT_H
#define L0MUONKERNEL_CRATEUNIT_H     1

#include "L0MuonKernel/L0MUnit.h"

namespace L0Muon {

  /** @class CrateUnit CrateUnit.h L0MuonKernel/CrateUnit.h

  Class representing a quarter of the L0Muon processor.

  It inherits from L0MUnit.

  If fully configured, it contains:
  - 12 BoardUnit,
  - 1 CtrlUnit.

  This unit is essentialy a container. It does not have any input nor output registers.
  It triggers the initialize, execute, ... methods of its daughters.

  @author  Luisanna Tocco, Julien Cogan
  @date  24 September 2003
  */
  class CrateUnit : public L0MUnit {

  public:

    /// Default Constructor
    CrateUnit();

    /// Constructor from xml
    CrateUnit(DOMNode* pNode);

    /// Destructor
    ~CrateUnit();

    /// Give a static type name to the unit
    std::string type() override {
      return "CrateUnit";
    }

  private:

  };

}  // namespace L0Muon

#endif      // L0MUONKERNEL_CRATEUNIT_H
