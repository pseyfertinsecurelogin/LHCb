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
// $Id: selection.h,v 1.4 2006-06-11 15:23:47 ibelyaev Exp $ 
// ====================================================================
// CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.4 $
// ====================================================================
// $Log: not supported by cvs2svn $
// ====================================================================
// Incldue file 
// ====================================================================
// STD & STL 
// ====================================================================
#include <string>
#include <vector>
// ====================================================================
// Relations 
// ====================================================================
#include "Relations/RelationsDict.h"
// ====================================================================
// Other incldue files: put your own includes here:
// ====================================================================
//#include ...
//#include ...
//#include ...
// ====================================================================

/** @file
 *
 *  Helper file for building the simplest Reflex dictionaries
 *  
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-05
 */

namespace RelationsDict
{
  struct __Instantiations
  {
    GaudiDict::Relation1DDict<int,int>           _1 ;
    GaudiDict::Relation2DDict<std::string,int>   _2 ;
  };
}

// ====================================================================
// The END 
// ====================================================================
