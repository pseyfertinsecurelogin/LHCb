
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
#include "RichFutureKernel/RichTupleAlgBase.h"

// ============================================================================
// Force creation of templated class
#include "RichHistoBase.icpp"
template class Rich::Future::CommonBase< GaudiTupleAlg >;
template class Rich::Future::HistoBase< GaudiTupleAlg >;
// ============================================================================
