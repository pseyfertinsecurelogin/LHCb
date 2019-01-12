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
#ifndef LOKI_IFUNCTORFACTORY_H
#define LOKI_IFUNCTORFACTORY_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKerbel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/CoreTypes.h"
// ============================================================================
/** @file LoKi/ICoreFactory.h
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
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class ICoreFactory LoKi/IFunctorFactory.h
     *
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date   2008-10-19
     */
    struct ICoreFactory : extend_interfaces<IAlgTool>
    {
      DeclareInterfaceID( ICoreFactory , 2 , 0 ) ;
      // ======================================================================
      // (void->...)
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::FCut&   cuts         ,
        const std::string&   context = "" )  = 0 ;
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::FFun&   cuts         ,
        const std::string&   context = "" )  = 0 ;
      // ======================================================================
      // (double->...)
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::XCut&   cuts         ,
        const std::string&   context = "" )  = 0 ;
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::XFun&   cuts         ,
        const std::string&   context = "" )  = 0 ;
      // ======================================================================

      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&  pycode ,
        LoKi::Types::XMap&  cuts  , // vector<double> -> vector<double>
        const std::string&   context = "" )  = 0 ;
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&    pycode       ,
        LoKi::Types::XFunVal& cuts         , // vetcor<double> -> double
        const std::string&    context = "" )  = 0 ;
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&    pycode       ,
        LoKi::Types::XSource& cuts         ,  // void -> vector<double>
        const std::string&    context = "" )  = 0 ;
      // ======================================================================
    } ;
    // ========================================================================
  } // end of namespace LoKi::Hybrid
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_IFUNCTORFACTORY_H
