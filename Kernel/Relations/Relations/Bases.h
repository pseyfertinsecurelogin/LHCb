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
// $Id: Bases.h,v 1.2 2008-02-19 15:26:09 ibelyaev Exp $
// ============================================================================
#ifndef RELATIONS_BASES_H 
#define RELATIONS_BASES_H 1
// ============================================================================
// Include files
// ============================================================================
namespace Relations 
{
  // ==========================================================================
  /** @struct BaseRange
   *  helper structure to make easy interactive manipulations
   *  with Reflex dictionaries 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-06
   */
  struct BaseRange {} ;
  // ==========================================================================  
  /** @struct BaseTable
   *  helper structure to make easy interacyive manipulations
   *  with Reflex dictionaries 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-06
   */
  struct BaseTable {} ;
  // ==========================================================================  
  /** @struct BaseWeightedTable
   *  helper structure to make easy interacyive manipulations
   *  with Reflex dictionaries 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-06
   */
  struct BaseWeightedTable  : public BaseTable {} ;
  // ==========================================================================  
} // end of namespace Relations 
// ============================================================================
// The END 
// ============================================================================
#endif // RELATIONS_BASES_H
// ============================================================================
