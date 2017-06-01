
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

#include "RichFutureKernel/RichCommonBase.h"

// ============================================================================
// Force creation of templated class
#include "RichHistoBase.icpp" // IWYU pragma: keep
template class Rich::Future::CommonBase < GaudiTupleTool > ;
template class Rich::Future::HistoBase  < GaudiTupleTool > ;
// ============================================================================
