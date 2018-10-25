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
// $Id: L0BufferCnvContainer.h,v 1.2 2008-11-07 16:23:38 jucogan Exp $
#ifndef L0MUONKERNEL_L0BUFFERCNVCONTAINER_H
#define L0MUONKERNEL_L0BUFFERCNVCONTAINER_H 1

// Include files


#include "L0MuonKernel/L0BufferCnv.h"
#include "Kernel/MuonTileID.h"

namespace L0Muon {

  /** @class L0BufferCnvContainer L0BufferCnvContainer.h L0MuonKernel/L0BufferCnvContainer.h

  Class to hold a set of pointers to L0BufferCnv.

  Used to binds the various L0Buffer converters in python (see L0MuonPyTools/L0MuonKernelWrapper).

  @author Julien Cogan
  @date   2007-08-22
  */
  class L0BufferCnvContainer {

  public:
    /// Standard constructor
    L0BufferCnvContainer( );
    L0BufferCnvContainer(std::vector<LHCb::MuonTileID> lpus );

    virtual ~L0BufferCnvContainer( ); ///< Destructor

    void open(std::string path);
    void close();
    void write(int ievt);

  protected:

  private:
    std::vector<L0Muon::L0BufferCnv *> m_cnvs;

  };

} // namespace L0Muon
#endif // L0MUONKERNEL_L0BUFFERCNVCONTAINER_H
