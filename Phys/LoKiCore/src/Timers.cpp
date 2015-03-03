// $Id: Timers.cpp 121023 2011-03-29 18:09:53Z ibelyaev $
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <string>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Timers.h"
// ============================================================================
/** @file 
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  By usage of this code one clearly states the disagreement 
 *  with the campain of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2011-02-02
 *  
 *                    $Revision: 121023 $
 *  Last modification $Date: 2011-03-29 20:09:53 +0200 (Tue, 29 Mar 2011) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
// constructor from the timer name 
// ============================================================================
LoKi::Timer:: Timer ( const std::string& name ) : m_name ( name ) {}
// ============================================================================
// destructor
// ============================================================================
LoKi::Timer::~Timer () {}
// ============================================================================
// The END 
// ============================================================================
