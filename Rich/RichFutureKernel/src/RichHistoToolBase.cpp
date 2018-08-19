
//-----------------------------------------------------------------------------
/** @file RichHistoToolBase.cpp
 *
 *  Implementation file for class : RichHistoToolBase
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-04-05
 */
//-----------------------------------------------------------------------------

// local
#include "RichFutureKernel/RichHistoToolBase.h"

// ============================================================================
// Force creation of templated class
#include "RichHistoBase.icpp"
template class Rich::Future::CommonBase< GaudiHistoTool >;
template class Rich::Future::HistoBase< GaudiHistoTool >;
// ============================================================================
