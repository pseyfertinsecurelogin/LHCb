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
#ifndef LOKI_FILTERTOOL_H
#define LOKI_FILTERTOOL_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
/** @file LoKi/FilterTool.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-05
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class FilterTool LoKi/FilterTool.h
   *  The base class for implementation of various
   *  "hybrid" filter tools
   *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
   *  @date   2011-06-05
   */
  class FilterTool : public GaudiTool {
  public:
    // ========================================================================
    /// the initialization of the tool
    StatusCode initialize() override;
    /// the finalization of the tool
    StatusCode finalize() override;
    // ========================================================================
  protected:
    // ========================================================================
    /// standard constructor
    FilterTool( const std::string& type,    // tool type (?)
                const std::string& name,    // toolinstance name
                const IInterface*  parent ); // tool's parent
    // ========================================================================
    /// the copy constructor is disabled
    FilterTool( const FilterTool& ) = delete; // the copy constructor is disabled
    /// the assignement operator is disabled
    FilterTool& operator=( const FilterTool& ) = delete; // the assignement is disabled
    // ========================================================================
  public: // property update handlers
    // ========================================================================
    /// update the factory
    void updateFactory( Property& /* p */ ); // update the factory
    /// update the code
    void updateCode( Property& /* p */ ); // update the code
    /// update the preambulo
    void updatePreambulo( Property& /* p */ ); // update the preambulo
    // ========================================================================
  protected:
    // ========================================================================
    /// get the type/name of the factory
    const std::string& factory() const { return m_factory; }
    /// get the code itself
    const std::string& code() const { return m_code; }
    /// get the preambulo
    const std::vector<std::string>& preambulo_() const { return m_preambulo_; }
    /// get the preambulo
    const std::string& preambulo() const { return m_preambulo; }
    // check the nesessity of updated
    inline bool updateRequired() const { return m_factory_updated || m_code_updated || m_preambulo_updated; }
    // add to preambulo
    void addToPreambulo( const std::string& item );
    // set the preambulo
    void setPreambulo( const std::vector<std::string>& items );
    // ========================================================================
  public:
    /// =======================================================================
    /** decode the functor
     *  it is assumed that this method is implemented as
     *
     *  StatusCode MyTool::decode()
     *  {
     *     typedef XXXXXX FACTORY ;
     *     tydedef LoKi::BasicFunctor<XXXX>::PredicateFromPredicate  FUNCTOR ;
     *     // or...
     *     // tydedef LoKi::BasicFunctor<XXXX>::FunctionFromFunction FUNCTOR ;
     *
     *     FUNCTOR& functor = ... ;
     *
     *     return i_decode<FACTORY>( functor ) ;
     *  }
     */
    virtual StatusCode decode() = 0;
    /// =======================================================================
  protected:
    // ========================================================================
    /** perform the actual decoding
     *  @param functor placeholder where to decode the fucntor
     */
    template <class FACTORY, class FUNCTOR>
    StatusCode i_decode( FUNCTOR& functor ) {
      // get the factory
      FACTORY* _factory = tool<FACTORY>( factory(), this );
      // use the factory
      StatusCode sc = _factory->get( code(), functor, preambulo() );
      // release the factory (not needed anymore)
      this->releaseTool( _factory ).ignore();
      if ( sc.isFailure() ) { return Error( "Unable to decode functor '" + code() + "'", sc ); }
      //
      debug() << "Requested code: \n'" << code() << "'\n -> Decoded functor :\n'" << functor << "'" << endmsg;
      //
      m_factory_updated   = false;
      m_code_updated      = false;
      m_preambulo_updated = false;
      //
      return sc;
    }
    /// =======================================================================
  protected:
    // ========================================================================
    void set_code_updated( const bool value ) { m_code_updated = value; }
    void set_factory_updated( const bool value ) { m_factory_updated = value; }
    void set_preambulo_updated( const bool value ) { m_preambulo_updated = value; }
    /// =======================================================================
  private:
    // ========================================================================
    /// the type/name for LoKi/Bender "hybrid" factory
    std::string m_factory; // the type/name for LoKi/Bender "hybrid" factory
    /// the filter/code criteria itself
    std::string m_code; // the filter/code criteria itself
    /// the preambulo itself
    std::vector<std::string> m_preambulo_; // the preambulo itself
    /// the preambulo itself
    std::string m_preambulo; // the preambulo itself
    // ========================================================================
  private:
    // ========================================================================
    /// flag which indicated that factory has been updated
    bool m_factory_updated = false; // the factory has been updated
    /// flag which indicated that code has been updated
    bool m_code_updated = false; // the code has been updated
    /// flag which indicated that preambulo has been updated
    bool m_preambulo_updated = false; // the preambulo has been updated
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_FILTERTOOL_H
