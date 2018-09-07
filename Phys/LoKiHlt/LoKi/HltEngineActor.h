// ============================================================================
#ifndef LOKI_HltENGINEACTOR_H 
#define LOKI_HltENGINEACTOR_H 1
// ============================================================================
// Include files
// ============================================================================
// STD&STL
// ============================================================================
#include <stack>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Interface.h"
#include "LoKi/IHltAntiFactory.h"
#include "LoKi/Context.h"
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  namespace Hybrid 
  {
    // ========================================================================
    /** @class HltEngineActor LoKi/HltEngine.h
     *  
     *  Helper class for implementation of C++/Python "Hybrid" solution
     *
     *  This file is a part of LoKi project - 
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
     *  contributions and advices from G.Raven, J.van Tilburg, 
     *  A.Golutvin, P.Koppenburg have been used in the 
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date   2008-09-18
     */
    class HltEngineActor 
    {
    public:
      // ======================================================================
      // get the static instance 
      static HltEngineActor& instance() ;
      /// connect the hybrid tool for code translation 
      StatusCode connect    ( const LoKi::Hybrid::IHltAntiFactory* factory , 
                              const LoKi::Context&                 context ) ;
      /// disconnect the tool 
      StatusCode disconnect ( const LoKi::Hybrid::IHltAntiFactory* fatcory ) ;
      /** get the current context
       *  contex is valid only in between <code>connect/disconnect</code>
       *  @return the current active context 
       */
      const LoKi::Context* context () const ;
      // ======================================================================
    public: // L0
      // ======================================================================
      /// propagate the cut to the tool 
      StatusCode process
      ( const std::string&          name , 
        const LoKi::Types::L0_Cuts& cut  ) const ;
      /// propagate the cut to the tool 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::L0_Func& cut  ) const ;
      // ======================================================================
    public: // O1 
      // ======================================================================
      /// propagate the cut to the tool 
      StatusCode process
      ( const std::string&          name , 
        const LoKi::Types::ODIN_Cuts& cut  ) const ;
      /// propagate the cut to the tool 
      StatusCode process 
      ( const std::string&          name , 
        const LoKi::Types::ODIN_Func& cut  ) const ;
      // ======================================================================
    public: // HLT
      // ======================================================================
      /// propagate the cut to the tool 
      StatusCode process
      ( const std::string&           name , 
        const LoKi::Types::HLT_Cuts& cut  ) const ;
      /// propagate the cut to the tool 
      StatusCode process 
      ( const std::string&           name , 
        const LoKi::Types::HLT_Func& cut  ) const ;
      // ======================================================================
     protected:
      // ======================================================================
      /// Standard constructor
      HltEngineActor() = default;
      // ======================================================================
    private:
      // ======================================================================
      // just to save some lines 
      template <class TYPE>
      inline StatusCode _add 
      ( const std::string& name , const TYPE& cut ) const ; 
      // ======================================================================
    private:
      // ======================================================================
      // the copy contructor is disabled 
      HltEngineActor           ( const HltEngineActor& ) ;
      // the assignement operator is disabled 
      HltEngineActor& operator=( const HltEngineActor& ) ;
      // ======================================================================
    private:
      // ======================================================================
      typedef LoKi::Interface<LoKi::Hybrid::IHltAntiFactory> Tool  ;
      typedef std::pair<Tool,LoKi::Context>                  Entry ;
      typedef std::stack<Entry>                              Stack ;
      ///  the stack of active factories 
      Stack m_stack {} ; // the stack of active factories 
      // ======================================================================
    } ;
    // ========================================================================
  } //                                        The end of namespace LoKi::Hybrid 
  // ==========================================================================
} //                                                  The end of namespace LoKi 
// ============================================================================
//                                                                      The END  
// ============================================================================
#endif // LOKI_HltENGINEACTOR_H
// ============================================================================
