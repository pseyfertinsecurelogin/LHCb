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
#ifndef LOKIHYBRID_HYBRIDBASE_H
#define LOKIHYBRID_HYBRIDBASE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD &SLT
// ============================================================================
#include <vector>
#include <map>
#include <mutex>
// ============================================================================
// GaudiAlg/GaudiTools
// ============================================================================
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/FunctorCache.h"
#include "LoKi/CacheFactory.h"
#include "LoKi/Context.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // =========================================================================
    /** @class Base LoKi/HybridBase.h
     *
     *  The base class for "hybrid" (C++/Python) tools
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
    class Base : public                 GaudiTool
    {
      // ======================================================================
    public:
      // ======================================================================
      typedef std::vector<std::string> Strings ;
      // ======================================================================
    public:
      // ======================================================================
      /// initialization of the tool
      StatusCode initialize ()  override;
      /// finalization   of the tool
      StatusCode finalize   ()  override;
      // ======================================================================
    protected:
      // ======================================================================
      /** execute the python code
       *  @attention the method must be invoked *ONLY* with the appropriate scope!!!
       *  @param code python code
       *  @return status code
       */
      StatusCode executeCode ( const std::string& code ) const ;
      // ======================================================================
    protected:
      // ======================================================================
      /// Standard constructor
      Base
      ( const std::string& type   ,
        const std::string& name   ,
        const IInterface*  parent ) ;
      // ======================================================================
    protected:
      // ======================================================================
      /// build the universal context 
      LoKi::Context make_context () const ;      
      // ======================================================================
    protected:
      // ======================================================================
      /// helper method to decrease amount of the typing
      template <class TYPE>
      void       _set
      ( std::unique_ptr<TYPE>& local , const TYPE& right ) ;
      // ======================================================================
      template <class TYPE1,class TYPE2>
      StatusCode _get_ ( const std::string&                              code  ,
                         std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&    local ,
                         LoKi::Assignable_t<LoKi::Functor<TYPE1,TYPE2>>& output) ;
      // ======================================================================
    public:
      // ======================================================================
      // flag to show the prepared code
      bool showCode () const { return m_showCode ; }
      // ======================================================================
    protected:
      // ======================================================================
      /** prepare the actual python code from the functor:
       *  @param modules list of modules
       *  @param actor the actual actor
       *  @param code the code for the functor
       *  @param lines more python lines to be used
       *  @param context  even more python code to be used
       *  @return the valid python code
       */
      std::string makeCode
      ( const Strings&     modules             ,
        const std::string& actor               ,
        const std::string& code                ,
        const Strings&     lines   = Strings() ,
        const std::string& context = ""        ) const ;
      // ======================================================================
    protected:
      // ======================================================================
      /// write C++ code
      void writeCpp () const ;
      // ======================================================================
    private:
      // ======================================================================
      // copy constructor is disabled
      Base           ( const Base& ) = delete ; ///< copy constructor is disabled
      // assignement operator is disabled
      Base& operator=( const Base& ) = delete ; ///< assignement operator is disabled
      // ======================================================================
    protected:
      // ======================================================================
      /// execute the code
      StatusCode executeCode () ;
      // ======================================================================
    private:
      // ======================================================================
      mutable bool             m_pyInit   = false;
      // flag to display the prepared code
      bool                     m_showCode ; ///< flag to display the prepared code
      // ======================================================================
    protected : // use python as factroy for LOK-functors ?
      // ======================================================================
      /// use python as factory for LoKi-functors ?
      bool m_use_python ;           // use python as factory for LoKi-functors ?
      /// use LoKi functor cache
      bool m_use_cache ;            // use LoKi functor cache ?
      // ======================================================================
    protected : // some stuff to deal with generation of C++ code
      // ======================================================================
      // make c++ code ?
      bool                           m_makeCpp    ; ///< make C++ code ?
      // file name for C++ code
      std::string                    m_cppname    ; ///< file name for C++ code
      // header lines for C++ code
      std::vector<std::string>       m_cpplines   ; ///< header lines for C++ code
      ///
      // information about the created functors
      typedef std::map<std::string,std::pair<std::string,std::string>> FUNCTIONS  ;
      std::map<std::string,FUNCTIONS>             m_allfuncs   ;

      mutable Gaudi::Accumulators::Counter<> m_pyInitCnt{ this, "Python is initialized!" };
      // ======================================================================
    protected:
      // ======================================================================
      // the mutex 
      mutable std::recursive_mutex m_mutex ;
      // ======================================================================
    } ;
    // ========================================================================
  } //                                            end of namespace LoKi::Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// helper method to decrease the typing
// ============================================================================
template <class TYPE>
inline void LoKi::Hybrid::Base::_set ( std::unique_ptr<TYPE>& local , const TYPE& right )
{
  // ==========================================================================
  if ( local && msgLevel ( MSG::DEBUG ) )
  { Warning ( "setCut/Fun(): Existing 'Function/Predicate' is substituted !" ).ignore() ; }
  // clone it!
  local.reset( right.clone() );
  // debug printput:
  if ( msgLevel ( MSG::DEBUG ) ) 
  { debug() << "The 'cut' is set to be '" << (*local) << "' = '"
            << System::typeinfoName ( typeid ( *local.get() ) ) << endmsg ; } 
  // ==========================================================================
}
// ============================================================================
template <class TYPE1,class TYPE2>
inline
StatusCode LoKi::Hybrid::Base::_get_
( const std::string&                                            code    ,
  std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&                  local   ,
  LoKi::Assignable_t<LoKi::Functor<TYPE1,TYPE2>>&               output  )
{
  // ==========================================================================
  //
  // 1) clear the placeholder, if needed
  // 2') look for cached functors:
  typedef LoKi::CacheFactory< LoKi::Functor<TYPE1,TYPE2> > cache_t;
  if ( !this->m_use_cache ) { local.reset ( nullptr  ) ; }
  {
    const LoKi::Context cntx = this->make_context() ;
    const auto          hash = LoKi::Cache::makeHash ( code ) ;
    local.reset ( cache_t::Factory::create ( cache_t::id ( hash ) , cntx ) ) ;
  }
  //
  if ( local ) 
  {
    output= *local ;
    this->counter("# loaded from CACHE" ) += 1 ;
    local.reset();
    return StatusCode::SUCCESS ;    // RETURN
  }
  //
  if ( !this->m_use_python ) { return StatusCode::FAILURE ; }
  //
  // 2") execute the code
  //
  const StatusCode sc = this->executeCode ( code ) ;
  if ( sc.isFailure() )
  { return Error ( "Error from LoKi::Hybrid::Base::executeCode", sc  ) ; } // RETURN
  if ( !local  )
  { return Error ( "Invalid object for the code"                     ) ; } // RETURN
  //
  // assign the result
  //
  output = *local ;                                         // ASSIGN
  //
  this->counter("# loaded from PYTHON") += 1 ;
  //
  local.reset();
  //
  if ( this->m_makeCpp )
  {
    const std::string funtype = System::typeinfoName ( typeid ( LoKi::Functor<TYPE1,TYPE2> ) ) ;
    const std::string cppcode = Gaudi::Utils::toCpp           ( output )   ;
    const std::string pytype  = Gaudi::Utils::toString        ( output )   ;
    m_allfuncs[ funtype ][ code ] = { cppcode , pytype } ;
    //
  }
  //
  return sc ;
  // ==========================================================================
}
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKIHYBRID_HYBRIDTOOL_H
// ============================================================================
