// $Id: HybridEngine.h,v 1.2 2007-08-14 18:18:43 ibelyaev Exp $
// ===========================================================================
#ifndef LOKI_HYBRID_ENGINE_H 
#define LOKI_HYBRID_ENGINE_H 1
// ===========================================================================
// Include files
// ===========================================================================
#include <string>
// ===========================================================================
// LoKi
// ===========================================================================
#include "LoKi/IHybridTool.h"
// ===========================================================================
namespace LoKi 
{  
  namespace Hybrid 
  {
    /** @class Engine HybridEngine.h LoKi/HybridEngine.h
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
    class Engine
    {
    public:
      /// constructor
      Engine () ;
      /// destructor 
      virtual ~Engine( );
      /// add the cut 
      StatusCode process
      ( const std::string&          name , 
        const LoKi::Types::Cuts&    cut  ) const ;
      /// add the cut 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::VCuts&   cut  ) const ;
      /// add the cut 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::ACuts&   cut  ) const ;
      /// add the function 
      StatusCode process
      ( const std::string&          name , 
        const LoKi::Types::Func&    cut  ) const ;
      /// add the function 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::VFunc&   cut  ) const ;
      /// add the function 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::AFunc&   cut  ) const ;
    } ;  
  } // end of namespace LoKi::Hybrid
} // end of namespace LoKi
// ===========================================================================
// The END 
// ===========================================================================
#endif // LOKI_CUTSHOLDERHELPER_H
// ===========================================================================
