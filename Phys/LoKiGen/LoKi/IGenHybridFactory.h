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
#ifndef LOKI_IGENHYBRIDFACTORY_H
#define LOKI_IGENHYBRIDFACTORY_H 1
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
#include "LoKi/GenTypes.h"
// ============================================================================
/** @file LoKi/IGenHybridFactory.h
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
  /** @class IGenHybridFactory IMCHybridFactory.h LoKi/IMCHybridFactory.h
   *  The abstract interface to "hybrid factory"
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-06-10
   */
  struct GAUDI_API IGenHybridFactory : extend_interfaces<IAlgTool>
  {
    // ========================================================================
    DeclareInterfaceID( IGenHybridFactory , 5 , 0 ) ;
    // ========================================================================
    // predicates
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param cuts the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string& pycode       ,
      LoKi::Types::GCut& cuts         ,
      const std::string& context = "" )  = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param cuts the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&  pycode       ,
      LoKi::Types::GVCut& cuts         ,
      const std::string&  context = "" )  = 0 ;
    // ========================================================================
    // functions
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string& pycode       ,
      LoKi::Types::GFun& func         ,
      const std::string& context = "" )  = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&  pycode       ,
      LoKi::Types::GVFun& func         ,
      const std::string&  context = "" )  = 0 ;
    // ========================================================================
    // maps
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string& pycode       ,
      LoKi::Types::GMap& func         ,
      const std::string& context = "" )  = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&  pycode       ,
      LoKi::Types::GVMap& func         ,
      const std::string&  context = "" )  = 0 ;
    // ========================================================================
    // pipes
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&  pycode       ,
      LoKi::Types::GPipe& func         ,
      const std::string&  context = "" )  = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&   pycode       ,
      LoKi::Types::GVPipe& func         ,
      const std::string&   context = "" )  = 0 ;
    // ========================================================================
    // fun-vals
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&    pycode       ,
      LoKi::Types::GFunVal& func         ,
      const std::string&    context = "" )  = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&     pycode       ,
      LoKi::Types::GVFunVal& func         ,
      const std::string&     context = "" )  = 0 ;
    // ========================================================================
    // cut-vals
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&    pycode       ,
      LoKi::Types::GCutVal& func         ,
      const std::string&    context = "" )  = 0 ;
    // ========================================================================
    // sources
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&     pycode       ,
      LoKi::Types::GSource&  func         ,
      const std::string&     context = "" )  = 0 ;
    // ========================================================================
    /** "Factory": get the the object form python code
     *  @param pycode the python pseudo-code of the function
     *  @param func the placeholder for the result
     *  @param context context code fragment to be executed
     *  @return StatusCode
     */
    virtual StatusCode get
    ( const std::string&      pycode       ,
      LoKi::Types::GVSource&  func         ,
      const std::string&      context = "" )  = 0 ;
    // ========================================================================
  } ;
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_IGENHYBRIDFACTORY_H
// ============================================================================
