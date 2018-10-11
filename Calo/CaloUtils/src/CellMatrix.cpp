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
// CaloKernel
#include "CaloKernel/CaloException.h"
// local
#include "CaloUtils/CellMatrix.h"

// ============================================================================
/** @file CellMatrix.cpp
 *
 *  Implementation file for class : CellMatrix
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 07/11/2001 
 */
// ============================================================================


// ============================================================================
/** throw the exception 
 *  @exception CaloException 
 *  @param message exception message 
 */
// ============================================================================
void CellMatrix::Exception
( const std::string& message ) const 
{ throw CaloException( " CellMatrix:: " + message ); }

// ============================================================================
// The End 
// ============================================================================
