/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//-----------------------------------------------------------------------------
/** @file RichAlgBase.cpp
 *
 *  Implementation file for class : Rich::AlgBase
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-04-05
 */
//-----------------------------------------------------------------------------

// local
#include "RichFutureKernel/RichAlgBase.h"

// ============================================================================
// Force creation of templated classes
#include "RichCommonBase.icpp"
template class Rich::Future::CommonBase< GaudiAlgorithm >;
// ============================================================================
