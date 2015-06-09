// $Id: GenHybridEngine.h 155778 2013-04-29 10:03:16Z cattanem $
// ============================================================================
#ifndef LOKI_GENHYBRIDENGINE_H 
#define LOKI_GENHYBRIDENGINE_H 1
// ============================================================================
// Include files
// ============================================================================
#include <string>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/IGenHybridTool.h"
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
 *                    $Revision: 155778 $
 *  Last modification $Date: 2013-04-29 12:03:16 +0200 (Mon, 29 Apr 2013) $
 *                 by $Author: cattanem $
 */
// ============================================================================
namespace LoKi 
{  
  // ==========================================================================
  namespace Hybrid 
  {
    // ========================================================================
    /** @class GenEngine GenHybridEngine.h LoKi/GenHybridEngine.h
     *  
     *  Helper class forn implementation of Hybrid Tools 
     *
     *  This file is a part of LoKi project - 
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
     *  contributions and advices from G.Raven, J.van Tilburg, 
     *  A.Golutvin, P.Koppenburg have been used in the design.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2004-06-30
     */
    class GenEngine
    {
    public:
      // ======================================================================
      /// constructor
      GenEngine () ;
      /// destructor 
      virtual ~GenEngine( );
      // ======================================================================
    public:
      // ======================================================================
      // predicates 
      // ======================================================================
      /// add the cut 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GCuts&   cut  ) const ;
      /// add the cut 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GVCuts&  cut  ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // functions 
      // ======================================================================
      /// add the function 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GFunc&   func ) const ;
      /// add the function 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GVFunc&  func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // maps 
      // ======================================================================
      /// add the map 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GMaps&   func ) const ;
      /// add the map
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GVMaps&  func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // pipes
      // ======================================================================
      /// add the pipe
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GPipes&  func ) const ;
      /// add the pipe 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::GVPipes& func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals
      // ======================================================================
      /// add the fun-val
      StatusCode process 
      ( const std::string&            name , 
        const LoKi::Types::GFunVals&  func ) const ;
      /// add the fun-val
      StatusCode process 
      ( const std::string&            name , 
        const LoKi::Types::GVFunVals& func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals
      // ======================================================================
      /// add the fun-val
      StatusCode process 
      ( const std::string&            name , 
        const LoKi::Types::GCutVals&  func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // sources
      // ======================================================================
      /// add the source  
      StatusCode process 
      ( const std::string&             name , 
        const LoKi::Types::GSources&   func ) const ;
      /// add the source 
      StatusCode process 
      ( const std::string&             name , 
        const LoKi::Types::GVSources&  func ) const ;
      // ======================================================================
    } ;  
    // ========================================================================
  } //                                            end of namespace LoKi::Hybrid 
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_CUTSHOLDERHELPER_H
// ============================================================================
