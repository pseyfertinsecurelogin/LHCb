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
// ============================================================================
// Include files
// ============================================================================
// local
// ============================================================================
#include "LoKi/Context.h"
// ============================================================================
/** @file 
 *  Implementation file for class LoKi::Context
 *  @date 2018-08-13 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
// ============================================================================
/*  constructor from algorithm pointers
 *  @param algo   own GaudiAlgorithm
 *  @param dvalgo own IDVAlgorithm
 */
// ============================================================================
LoKi::Context::Context
( const GaudiAlgorithm* algo   ,
  const IDVAlgorithm*   dvalgo ) 
  : m_algo   ( algo   ) 
  , m_dvalgo ( dvalgo )
{}
// ============================================================================
//  destructor 
// ============================================================================
LoKi::Context::~Context(){}
// ============================================================================
//                                                                      The END 
// ============================================================================
