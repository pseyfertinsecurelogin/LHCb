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
#ifndef L0MUONKERNEL_PROCUNIT_H
#define L0MUONKERNEL_PROCUNIT_H     1


#include "L0MuonKernel/L0MUnit.h"

namespace L0Muon {

  /** @class ProcUnit ProcUnit.h L0MuonKernel/ProcUnit.h

  Class representing a processing unit L0Muon processor.

  It inherits from L0MUnit.

  There is one such unit per PU (i.e. 4 by processing board). Its parent is a BoardUnit.

  It contains :
  - 1 FormattingUnit (performs data exchange between Processing Units)
  - 1 CoreUnit (candidate search)
  - 1 FormattingOutUnit (formats the data for output)

  This unit is essentially a container. It does not have any input nor output registers.
  It triggers the initialize, execute, ... methods of its daughters.

  @author  Luisanna Tocco, Julien Cogan
  @date  24 September 2003

  */
  class ProcUnit : public L0MUnit {

  public:

    /// Default constructor
    ProcUnit();

    /// Constructor
    ProcUnit(DOMNode* pNode);

    /// Destructor
    ~ProcUnit();

    /// Return the MuonTileID of the PU
    LHCb::MuonTileID puId(){return m_mid;}

    /// Give a static type name to the unit
    std::string type() override {
      return "ProcUnit";
    }

  private:
  };

}  // namespace L0Muon

#endif      // L0MUONKERNEL_PROCUNIT_H
