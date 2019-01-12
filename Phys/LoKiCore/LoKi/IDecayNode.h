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
#ifndef LOKI_IDECAYNODEFINDER_H
#define LOKI_IDECAYNODEFINDER_H 1
// ============================================================================
// Include files
// ============================================================================
// GauidKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/iNode.h"
// ============================================================================
/** @file LoKi/IDecayNode.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
namespace Decays
{
  // ==========================================================================
  /** @class IDecayNode LoKi/IDecayNode.h
   *  Simple factory for creation of decay nodes
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date   2009-05-22
   */
  struct IDecayNode : extend_interfaces<IAlgTool>
  {
    DeclareInterfaceID( IDecayNode, 2, 0 );
    // ========================================================================
    /** build the node form the node descriptor
     *
     *  @code
     *
     *  const Decays::IDecayNode* finder = ... ;
     *
     *  Decays::Node node = finder -> node ( " Beauty & Charm & Baryon " ) ;
     *
     *  if ( !node.valid() ){  .. error here ... }
     *
     *  @endcode
     *
     *  For the empty string the "default" node is returned
     *
     *  @param descriptor (INPUT)  node descriptor
     *  @param constructed node
     */
    virtual Node node ( const std::string& descriptor = "" ) const = 0 ;
    // ========================================================================
  }; //                                             end class IDecayNodeFactory
  // ==========================================================================
} //                                                    end of namespace Decays
// ============================================================================
// The  END
// ============================================================================
#endif // LOKI_IDECAYNODEFINDER_H
