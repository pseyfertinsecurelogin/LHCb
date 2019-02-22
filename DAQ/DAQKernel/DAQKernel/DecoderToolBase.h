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
//-----------------------------------------------------------------------------
/** @file DecoderToolBase.h
 *
 *  Header file for tool base class : Decoder::ToolBase
 *
 *  @author Rob Lambert   Rob.Lambert@cern.ch
 *  @date   10/02/2014
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERTOOLBASE_H
#define DAQKERNEL_DECODERTOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTool.h"

// local
#include "DAQKernel/DecoderCommonBase.h"

namespace Decoder {
  //-----------------------------------------------------------------------------
  /** @class ToolBase DAQToolBase.h DAQKernel/DecoderToolBase.h
   *
   *  Abstract base class for DAQ tools providing
   *  some basic functionality.
   *
   *  @author Rob Lambert   Rob.Lambert@cern.ch
   *  @date   05/04/2002
   */
  //-----------------------------------------------------------------------------

  using ToolBase = Decoder::CommonBase<GaudiTool>;

} // namespace Decoder

#endif // DAQKERNEL_DECODERTOOLBASE_H
