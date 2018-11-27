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
// local
// ============================================================================
#include "LoKi/CoreEngine.h"
#include "LoKi/CoreEngineActor.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Hybrid::CoreEngine
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-09-18
 */
// ============================================================================
namespace
{
  template <class TYPE>
  inline StatusCode _process
  ( const std::string& name , const TYPE& cut )
  {
    const LoKi::Hybrid::CoreEngineActor& actor =
      LoKi::Hybrid::CoreEngineActor::instance() ;
    return actor.process ( name , cut ) ;
  }
}
namespace LoKi { namespace Hybrid {
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode CoreEngine::process ( const std::string&        name ,
                                 const LoKi::Types::FCuts& cut  ) const
{ return _process ( name , cut ) ; }
// ============================================================================
//  propagate the cut to the tool
// ============================================================================
StatusCode CoreEngine::process( const std::string&        name ,
                                const LoKi::Types::FFunc& cut  ) const
{ return _process ( name , cut ) ; }
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode CoreEngine::process ( const std::string&        name ,
                                 const LoKi::Types::XCuts& cut  ) const
{ return _process ( name , cut ) ; }
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode CoreEngine::process ( const std::string&        name ,
                                 const LoKi::Types::XFunc& cut  ) const
{ return _process ( name , cut ) ; }
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode CoreEngine::process ( const std::string&           name ,
                                 const LoKi::Types::XMaps&    cut  ) const
{ return _process ( name , cut ) ; }
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode CoreEngine::process ( const std::string&           name ,
                                 const LoKi::Types::XFunVals& cut  ) const
{ return _process ( name , cut ) ; }
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode CoreEngine::process ( const std::string&           name ,
                                 const LoKi::Types::XSources& cut  ) const
{ return _process ( name , cut ) ; }

} }
// ============================================================================
/// get the current context 
// ============================================================================
const LoKi::Context* LoKi::Hybrid::CoreEngine::context () const 
{
  const LoKi::Hybrid::CoreEngineActor& actor =
    LoKi::Hybrid::CoreEngineActor::instance() ;
  return actor.context () ;
}
// ============================================================================
// The END
// ============================================================================
