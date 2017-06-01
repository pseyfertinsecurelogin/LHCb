
//-----------------------------------------------------------------------------
/** @file RichHistoAlgBase.cpp
 *
 *  Implementation file for class : RichHistoAlgBase
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-04-05
 */
//-----------------------------------------------------------------------------

// local
#include "RichKernel/RichHistoAlgBase.h"

#include "RichKernel/RichCommonBase.h"

// ============================================================================
// Force creation of templated class
#include "RichHistoBase.icpp" // IWYU pragma: keep
template class Rich::CommonBase < GaudiHistoAlg > ;
template class Rich::HistoBase  < GaudiHistoAlg > ;
// ============================================================================
