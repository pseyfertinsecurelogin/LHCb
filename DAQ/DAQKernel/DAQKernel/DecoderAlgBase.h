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
/** @file DecoderAlgBase.h
 *
 *  Header file for algorithm base class : Decoder::AlgBase
 *
 *  @author Rob Lambert   Rob.Lambert@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERALGBASE_H
#define DAQKERNEL_DECODERALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiAlgorithm.h"

// local
#include "DAQKernel/DecoderCommonBase.h"

namespace Decoder
{
  //-----------------------------------------------------------------------------
  /** @class AlgBase DAQAlgBase.h DAQKernel/DecoderAlgBase.h
   *
   *  Abstract base class for DAQ algorithms providing
   *  some basic functionality.
   *
   *  @author Rob Lambert   Rob.Lambert@cern.ch
   *  @date   02/10/2014
   */
  //-----------------------------------------------------------------------------

  using AlgBase = Decoder::CommonBase<GaudiAlgorithm>;

}

#endif // DAQKERNEL_DECODERALGBASE_H
