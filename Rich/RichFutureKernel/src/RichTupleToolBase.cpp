
//-----------------------------------------------------------------------------
/** @file RichTupleToolBase.cpp
 *
 *  Implementation file for class : RichTupleToolBase
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-04-05
 */
//-----------------------------------------------------------------------------

// local
#include "RichFutureKernel/RichTupleToolBase.h"

// ============================================================================
// Force creation of templated class
#include "RichHistoBase.icpp"
template class Rich::Future::CommonBase< GaudiTupleTool >;
template class Rich::Future::HistoBase< GaudiTupleTool >;
// ============================================================================
