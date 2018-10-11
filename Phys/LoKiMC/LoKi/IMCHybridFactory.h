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
#ifndef LOKI_IMCHYBRIDFACTORY_H
#define LOKI_IMCHYBRIDFACTORY_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCTypes.h"
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
 */
namespace LoKi
{
  // ==========================================================================
  /** @class IMCHybridFactory IMCHybridFactory.h LoKi/IMCHybridFactory.h
   *  The abstract interface to "hybrid factory"
   *  @author Vanya BELYAEV ibelayev@physics.syr.edu
   *  @date   2007-06-10
   */
  struct GAUDI_API IMCHybridFactory : extend_interfaces<IAlgTool>
  {
    // ========================================================================
    DeclareInterfaceID( IMCHybridFactory , 6 , 0 ) ;
    // ========================================================================
    // predicates:
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param cuts the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::MCCut&  cuts         ,
      const std::string&   context = "" ) = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param cuts the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::MCVCut& cuts         ,
      const std::string&   context = "" ) = 0 ;
    // ========================================================================
    // functions:
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::MCFun&  func         ,
      const std::string&   context = "" ) = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::MCVFun& func         ,
      const std::string&   context = "" ) = 0 ;
    // ========================================================================
    // maps:
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::MCMap&  func         ,
      const std::string&   context = "" ) = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::MCVMap& func         ,
      const std::string&   context = "" ) = 0 ;
    // ========================================================================
    // pipes:
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::MCPipe& func         ,
      const std::string&   context = "" ) = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&    pycode       ,
      LoKi::Types::MCVPipe& func         ,
      const std::string&    context = "" ) = 0 ;
    // ========================================================================
    // fun-vals:
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&     pycode       ,
      LoKi::Types::MCFunVal& func         ,
      const std::string&     context = "" ) = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&      pycode       ,
      LoKi::Types::MCVFunVal& func         ,
      const std::string&      context = "" ) = 0 ;
    // ========================================================================
    // cut-vals:
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&     pycode       ,
      LoKi::Types::MCCutVal& func         ,
      const std::string&     context = "" ) = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&      pycode       ,
      LoKi::Types::MCVCutVal& func         ,
      const std::string&      context = "" ) = 0 ;
    // ========================================================================
    // sources:
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&      pycode       ,
      LoKi::Types::MCSource&  func         ,
      const std::string&      context = "" ) = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context the code fragment to be pre-executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&      pycode       ,
      LoKi::Types::MCVSource& func         ,
      const std::string&      context = "" ) = 0 ;
    // ========================================================================
  } ;
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_IHYBRIDFACTORY_H
// ============================================================================
