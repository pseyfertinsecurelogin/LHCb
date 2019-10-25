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
// ============================================================================
#ifndef DICT_CALODETDICT_H
#define DICT_CALODETDICT_H
// ============================================================================
// Include files
// ============================================================================
// CaloDet
// ============================================================================
#include "CaloDet/DeCalorimeter.h"
#include "CaloDet/DeSubCalorimeter.h"
#include "CaloDet/DeSubSubCalorimeter.h"
// ============================================================================

namespace {
  // ==========================================================================
  struct _Instantiations {
    // ========================================================================
    /// fictive constructor
    _Instantiations(); // fictive constructor
    // ========================================================================
    /// instantiations
    CaloVector<CellParam> m_v1;
    CaloVector<CaloPin>   m_v2;
    // ========================================================================
  };
  // ==========================================================================
} // namespace

// ============================================================================
#endif // DICT_CALODETDICT_H
