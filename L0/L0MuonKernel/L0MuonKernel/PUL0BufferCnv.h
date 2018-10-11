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
#ifndef L0MUONKERNEL_PUL0BUFFERCNV_H
#define L0MUONKERNEL_PUL0BUFFERCNV_H


#include "L0MuonKernel/L0BufferCnv.h"
#include <string>

namespace L0Muon {

  /** @class PUL0BufferCnv PUL0BufferCnv.h  L0MuonKernel/PUL0BufferCnv.h

  Class for writing out the l0Buffer of the L0Muon PU FPGA.

  It inherits from L0BufferCnv. There is one such converter per PU.

  As input, it uses :
  - TileRegister with optical link data named : FORMATTED_OL_Q<q>R<r><xy>
  - TileRegister with neighbour data named : FORMATTED_NEIGH_Q<q>R<r><xy>
  - Register with PU candidates named : CAND_PUQ<q>R<r><xy>
  .
  , where q(=1,...,4), r=(1,...,4), x,y refers to the PU attached to this converter.

  @author Julien Cogan
  @date 2008-08-22

  */
  class PUL0BufferCnv : public L0BufferCnv {

  public:

    /** Constructor with MuonTileID

    @param id : MuonTileID of the PU attached to the converter

    */
    PUL0BufferCnv(LHCb::MuonTileID id);

    /// Destructor
    ~PUL0BufferCnv();

    /// Give a static type name to the unit
    std::string type() override {
      return "L0BufferProcUnit";
    }

    /** Write the event in the output file

        @param ievt : current event number
    */
    void write(int ievt) override;

  private:

    CandRegisterHandler m_candRegHandler; ///< Handle the register containing the PU candidates
    TileRegister* m_ol;     ///< Pointer to the TileRegister with the optical links at the PU input
                            ///< (formatted as in the L0Buffer)
    TileRegister* m_neigh;  ///< Pointer to the TileRegister with the neighbour data at the PU input
                            ///< (formatted as in the L0Buffer)

  };
} // namespace L0Muon

#endif    // L0MUONKERNEL_PUL0BUFFERCNV_H
