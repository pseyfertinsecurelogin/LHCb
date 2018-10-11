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
// STD & STL
// ============================================================================
#include <sstream>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
// ============================================================================
/** @file
 *  Implementation file for class Decays::iTree
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2009-05-06
 */
// ============================================================================
// conversion to the string
// ============================================================================
std::string Decays::iTree::toString() const
{
  std::ostringstream s ;
  fillStream ( s ) ;
  return s.str () ;
}
// ============================================================================
/*  The standard output stream operator for the sub-tree
 *  @param s the reference to the outptu stream
 *  @param o the decay tree objects
 *  @return the reference to the outptu stream
 */
// ============================================================================
std::ostream& Decays::operator<< ( std::ostream& s , const iTree& o )
{ return o.fillStream ( s ) ; }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
