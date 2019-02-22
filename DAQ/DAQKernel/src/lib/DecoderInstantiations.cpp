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
/** @file DAQAlgBase.cpp
 *
 *  Implementation file for class : DAQ::AlgBase
 *
 *  @author Rob Lambert    Rob.Lambert@cern.ch
 *  @date   2014-02-10
 */
//-----------------------------------------------------------------------------

// local
#include "DAQKernel/DecoderAlgBase.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiAlg/GaudiTupleTool.h"

// ============================================================================
// Explicitly instantiate the relevant templates
#include "DecoderCommonBase.icpp"
template class Decoder::CommonBase<GaudiAlgorithm>;
template class Decoder::CommonBase<GaudiTool>;
template class Decoder::CommonBase<GaudiHistoAlg>;
template class Decoder::CommonBase<GaudiHistoTool>;
template class Decoder::CommonBase<GaudiTupleAlg>;
template class Decoder::CommonBase<GaudiTupleTool>;
// ============================================================================
