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
// STD & STL
// ============================================================================
#include <cstdio>
#include <iostream>
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/IInterface.h"
// ============================================================================
// Relations
// ============================================================================
#include "Relations/RelationUtils.h"
// ============================================================================
/** @file
 *
 *  Implementation file for method from namespace Relation
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 28/01/2002
 */
// ============================================================================
/* Function to create the unique relation
 * interfaceID from interface name, CLIDs and types
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   28/01/2002
 *
 *  @param name       interface name
 */
// ============================================================================
InterfaceID Relations::interfaceID( const std::string& name ) { return InterfaceID( name.c_str(), 1, 0 ); }
// ============================================================================
/*  Function to create the uniqie relation
 *  clid from object name, CLIDs and types
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   28/01/2002
 *
 *  @param name       object name
 */
// ============================================================================
CLID Relations::clid( const std::string& name ) {
  CLID cl = Relations::interfaceID( name ).id();
  // set ObjectVector bit to NULL
  cl = ~CLID_ObjectVector & cl;
  // set ObjectList   bit to NULL
  cl = ~CLID_ObjectList & cl;
  return cl;
}
// ============================================================================
/*  Simple function to convert CLID (Gaudi unique class identifier)
 *  to GUID (POOL unique class identifier)
 *
 *  The actual code is stollen from Markus Frank's lines
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-30
 *
 *  @param clID Gaudi unique class identifier
 *  @return POOL unique class identifier
 */
// ============================================================================
std::string Relations::guid( const CLID& clID ) {
  char clid_txt[64];
  sprintf( clid_txt, "%08X-0000-0000-0000-000000000000", (unsigned int)clID );
  return std::string( clid_txt, 36 );
}
// ============================================================================
/** Simple function to convers CLID (Gaudi unique class identifier)
 *  to GUID (POOL unique class identifier)
 *
 *  The actual code is stolen from Markus Frank's lines
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-30
 *
 *  @param name class name
 *  @return POOL unique class identifier
 */
// ============================================================================
std::string Relations::guid( const std::string& name ) { return Relations::guid( Relations::clid( name ) ); }
// ============================================================================

// ============================================================================
// The End
// ============================================================================
