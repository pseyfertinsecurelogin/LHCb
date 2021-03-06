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
// Include files
// ============================================================================
// STD &STL
// ============================================================================
#include <string>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Assert.h"
#include "LoKi/Exception.h"
// ============================================================================
/** @file
 *  Implementation file for function LoKi::throwException
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-08-11
 */
// ============================================================================
void LoKi::throwException( const std::string& message ) { throw LoKi::Exception( message ); }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
