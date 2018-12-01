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
// ===========================================================================
#ifndef LOKI_GENCUTSHOLDER_H
#define LOKI_GENCUTSHOLDER_H 1
// ===========================================================================
// Include files
// ===========================================================================
// STD&STL
// ===========================================================================
#include <stack>
// ===========================================================================
// LoKi
// ===========================================================================
#include "LoKi/Interface.h"
#include "LoKi/IGenHybridTool.h"
#include "LoKi/Context.h"
// ===========================================================================
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
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class GenEngineActor LoKi/GenHybridEngineActor.h
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
     *  @date   2004-06-29
     */
    class GenEngineActor
    {
    public:
      // ======================================================================
      // get the static instance
      static GenEngineActor& instance() ;
      /// connect the hybrid tool for code translation
      StatusCode connect    ( const LoKi::IGenHybridTool* factory , 
                              const LoKi::Context&        context ) ;
      /// disconnect the tool
      StatusCode disconnect ( const LoKi::IGenHybridTool* factory ) ;
      // ======================================================================
      /** get the current context
       *  contex is valid only in between <code>connect/disconnect</code>
       *  @return the current active context 
       */
      const LoKi::Context* context () const ;
      // ======================================================================
    public:
      // ======================================================================
      // predicates
      // ======================================================================
      /// propagate the cut to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GCuts&   cut  ) const ;
      /// propagate the cut to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GVCuts&  cut  ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // functions
      // ======================================================================
      /// propagate the function to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GFunc&   func ) const ;
      /// propagate the function to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GVFunc&  func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // maps
      // ======================================================================
      /// propagate the map to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GMaps&   func ) const ;
      /// propagate the map to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GVMaps&  func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // pipes
      // ======================================================================
      /// propagate the pipe to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GPipes&  func ) const ;
      /// propagate the pipe to the tool
      StatusCode process
      ( const std::string&          name ,
        const LoKi::Types::GVPipes& func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals
      // ======================================================================
      /// propagate the fun-val to the tool
      StatusCode process
      ( const std::string&            name ,
        const LoKi::Types::GFunVals&  func ) const ;
      /// propagate the fun-val to the tool
      StatusCode process
      ( const std::string&            name ,
        const LoKi::Types::GVFunVals& func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals
      // ======================================================================
      /// propagate the fun-val to the tool
      StatusCode process
      ( const std::string&            name ,
        const LoKi::Types::GCutVals&  func ) const ;
      // ======================================================================
    public:
      // ======================================================================
      // sources
      // ======================================================================
      /// propagate the source to the tool
      StatusCode process
      ( const std::string&            name ,
        const LoKi::Types::GSources&  func ) const ;
      /// propagate the source to the tool
      StatusCode process
      ( const std::string&            name ,
        const LoKi::Types::GVSources& func ) const ;
      // ======================================================================
    protected:
      // ======================================================================
      /// Standard constructor
      GenEngineActor() = default;
      // ======================================================================
    private:
      // ======================================================================
      /// just to save some lines
      template <class TYPE>
      inline StatusCode _add
      ( const std::string& name , const TYPE& cut ) const ;
      // ======================================================================
      // the copy contructor is disabled
      GenEngineActor           ( const GenEngineActor& ) = delete;
      // the assignement operator is disabled
      GenEngineActor& operator=( const GenEngineActor& ) = delete;
      // ======================================================================
    private:
      // ======================================================================
      typedef LoKi::Interface<LoKi::IGenHybridTool>   Tool  ;
      typedef std::pair<Tool,LoKi::Context>           Entry ;
      typedef std::stack<Entry>                       Stack ;
      ///  the stack of active factories 
      Stack m_stack {} ; // the stack of active factories 
      // ======================================================================
    };
    // ========================================================================
  } //                                             end of namespace LoKi:Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCCUTSHOLDER_H
// ============================================================================
