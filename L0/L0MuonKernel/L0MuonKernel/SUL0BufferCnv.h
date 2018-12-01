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
#ifndef L0MUONKERNEL_SUL0BUFFERCNV_H
#define L0MUONKERNEL_SUL0BUFFERCNV_H


#include "L0MuonKernel/L0BufferCnv.h"
#include <string>

namespace L0Muon {

  /** @class SUL0BufferCnv SUL0BufferCnv.h  L0MuonKernel/SUL0BufferCnv.h

  Class for writing out the l0Buffer of the L0Muon SU FPGA.

  It inherits from L0BufferCnv. There is one such converter per quarter.

  As input, it uses (with q(=1,...,4), the quarter attached to this converter) :
  - Register with BCSU candidates named : BOARDCAND_Q<q>_<iboard> (iboard is the board index in the quarter);
    note that this register is an alias to CAND_BCSUQ<q'>R<r'><x',y'>,
    where q',r,x',y' refers to the BCSU's MuonTileID coordinates
  - Register with the controller board candidates named : CAND_CTRLQ<q>

  @author Julien Cogan
  @date 2008-08-22

  */
  class SUL0BufferCnv : public L0BufferCnv {

  public:

    /** Constructor with MuonTileID

    @param id : MuonTileID specifying the quarter attached to the converter

    */
    SUL0BufferCnv(LHCb::MuonTileID id);

    /// Destructor
    ~SUL0BufferCnv();

    /// Give a static type name to the unit : L0BufferSlaveUnit
    std::string type() override {
      return "L0BufferSlaveUnit";
    }

    /** Write the event in the output file.

        @param ievt : current event number
    */
    void write(int ievt) override;

  private:

    CandRegisterHandler m_candRegHandlerBCSU[12]; ///< Handle the registers with the BCSU's candidates
    CandRegisterHandler m_candRegHandler; ///< Handle the register with the controller board candidates

  };
} // namespace L0Muon

#endif    // L0MUONKERNEL_SUL0BUFFERCNV_H
