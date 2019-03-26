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
#ifndef L0MUONKERNEL_MUONTRIGGERUNIT_H
#define L0MUONKERNEL_MUONTRIGGERUNIT_H 1

#include "L0MuonKernel/L0MUnit.h"
#include <vector>

namespace L0Muon {

  /** @class MuonTriggerUnit L0MuonKernel/MuonTriggerUnit.h

  Class representing the L0Muon Trigger.

  It inherits from L0MUnit.

  It is the 'Top Unit' containing the 4 CrateUnit of the L0muon processor (if fully configured).

  Contains also methods to get the trigger parameters. They are propagated to the Tower via the CoreUnit.

  Otherwise, this unit is essentialy a container. It does not have any input nor output registers.
  It triggers the initialize, execute, ... methods of its daughters.

  @author  Luisanna Tocco, Julien Cogan
  @date  24 September 2003
  */
  class MuonTriggerUnit : public L0MUnit {

  public:
    /// Constructor
    MuonTriggerUnit();

    /// Constructor
    MuonTriggerUnit( DOMNode* pNode );

    /// Destructor
    ~MuonTriggerUnit();

    /// Give a static type name to the unit
    std::string type() override { return "MuonTriggerUnit"; }
  };
} // namespace L0Muon

#endif // L0MUONKERNEL_MUONTRIGGERPROCESSORUNIT_H
