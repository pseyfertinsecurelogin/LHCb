
//-----------------------------------------------------------------------------
/** @file RichTupleAlgBase.cpp
 *
 *  Implementation file for class : RichTupleAlgBase
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-04-05
 */
//-----------------------------------------------------------------------------

// local
#include "RichKernel/RichTupleAlgBase.h"

#include "RichKernel/RichCommonBase.h"

// ============================================================================
// Force creation of templated class
#include "RichHistoBase.icpp" // IWYU pragma: keep
template class Rich::CommonBase < GaudiTupleAlg > ;
template class Rich::HistoBase  < GaudiTupleAlg > ;
// ============================================================================
