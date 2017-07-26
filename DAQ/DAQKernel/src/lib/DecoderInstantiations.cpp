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
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiAlg/GaudiTupleTool.h"

// ============================================================================
// Explicitly instantiate the relevant templates
#include "DecoderCommonBase.icpp"
template class Decoder::CommonBase < GaudiAlgorithm> ;
template class Decoder::CommonBase < GaudiTool> ;
template class Decoder::CommonBase < GaudiHistoAlg > ;
template class Decoder::CommonBase < GaudiHistoTool > ;
template class Decoder::CommonBase < GaudiTupleAlg > ;
template class Decoder::CommonBase < GaudiTupleTool > ;
// ============================================================================
