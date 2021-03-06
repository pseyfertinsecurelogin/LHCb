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
#ifndef L0MUONKERNEL_OLCNV_H
#define L0MUONKERNEL_OLCNV_H

#include "L0MuonKernel/L0BufferCnv.h"
#include <string>

namespace L0Muon {

  /** @class OLCnv OLCnv.h  L0MuonKernel/OLCnv.h

  Class to write out the optical link in input of the PU in the format required for the high stat test.

    It inherits from L0BufferCnv. There is one such converter per PU.

  As input , it uses :
  - TileRegister with optical link data named : OLDUMP_Q<q>R<r><xy>
  , where q(=1,...,4), r=(1,...,4), x,y refers to the PU attached to this converter.

  @author Julien Cogan
  @date 2008-08-22

  */
  class OLCnv : public L0BufferCnv {

  public:
    /** Constructor with MuonTileID

    @param id : MuonTileID of the PU attached to the converter

    */
    OLCnv( LHCb::MuonTileID id );

    /// Destructor
    ~OLCnv();

    /// Give a static type name to the unit : OLDumpUnit
    std::string type() override { return "OLDumpUnit"; }

    /** Write the event in the output file.

        @param ievt : current event number
    */
    void write( int ievt ) override;

  private:
    TileRegister* m_ol; ///< TileRegister with the optical links at the PU input (formatted as requested by the high
                        ///< stat test).
  };
} // namespace L0Muon

#endif // L0MUONKERNEL_OLCNV_H
