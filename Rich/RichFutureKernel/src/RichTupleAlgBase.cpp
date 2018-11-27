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
