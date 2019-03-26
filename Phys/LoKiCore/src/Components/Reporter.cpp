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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgorithm.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ErrorReport.h"
#include "LoKi/IReporter.h"
#include "LoKi/Welcome.h"
// ============================================================================
/** @file
 *
 *  The implementation file for the class LoKi::Reporter
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
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class Reporter Reporter.h LoKi/Reporter.h
   *
   *  Simple class (tool)  for error/warning/exception
   *  reports from different LoKi components
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2003-01-16
   */
  class Reporter : public extends<GaudiTool, LoKi::IReporter> {
  public:
    // ========================================================================
    StatusCode Error( const std::string& msg, const StatusCode st, const size_t mx ) const override {
      const std::size_t mx_ = std::min( mx, LoKi::IReporter::maxErrorPrint() );
      return GaudiTool::Error( m_printMyAlg ? msg + getMyAlg() : msg, st, mx_ );
    }
    //
    StatusCode Warning( const std::string& msg, const StatusCode st, const size_t mx ) const override {
      const std::size_t mx_ = std::min( mx, LoKi::IReporter::maxWarningPrint() );
      return GaudiTool::Warning( m_printMyAlg ? msg + getMyAlg() : msg, st, mx_ );
    }
    //
    StatusCode Print( const std::string& msg, const StatusCode st, const MSG::Level lev ) const override {
      return GaudiTool::Print( m_printMyAlg ? msg + getMyAlg() : msg, st, lev );
    }
    //
    void Assert( const bool ok, const std::string& message, const StatusCode sc ) const override {
      GaudiTool::Assert( ok, m_printMyAlg ? message + getMyAlg() : message, sc );
    }
    //
    void Exception( const std::string& msg, const GaudiException& exc, const StatusCode sc ) const override {
      GaudiTool::Exception( m_printMyAlg ? msg + getMyAlg() : msg, exc, sc );
    }
    //
    void Exception( const std::string& msg, const std::exception& exc, const StatusCode sc ) const override {
      GaudiTool::Exception( m_printMyAlg ? msg + getMyAlg() : msg, exc, sc );
    }
    //
    void Exception( const std::string& msg, const StatusCode sc ) const override {
      GaudiTool::Exception( m_printMyAlg ? msg + getMyAlg() : msg, sc );
    }
    //
    // ========================================================================
  public:
    // ========================================================================
    /** standard initialization
     *  @see AlgTool
     *  @return status code
     */
    StatusCode initialize() override {
      StatusCode sc = base_class::initialize();
      if ( sc.isFailure() ) { return sc; }
      //
      LoKi::ErrorReport& rep = LoKi::ErrorReport::instance();
      if ( 0 == rep.reporter() ) { rep.setReporter( this ).ignore(); }
      // locate LoKi service
      return StatusCode::SUCCESS;
    }
    /** standard finalization
     *  @see AlgTool
     *  @return status code
     */
    StatusCode finalize() override {
      // check for global reporter
      LoKi::ErrorReport& rep = LoKi::ErrorReport::instance();
      // owner for reporter?
      if ( rep.reporter() == this ) {
        rep.report().ignore();
        rep.setReporter( 0 ).ignore();
      }
      // finalize the base class
      return base_class::finalize();
    }
    /** Standard constructor for AlgTools
     *  @see AlgTool
     *  @param type   tool type
     *  @param name   tool name
     *  @param parent tool parent
     */
    Reporter( const std::string& type, const std::string& name, const IInterface* parent )
        : base_class( type, name, parent ) {
      //
      declareProperty( "PrintMyAlg", m_printMyAlg = true );
      //
      StatusCode sc = setProperty( "TypePrint", false );
      Assert( sc.isSuccess(), "Unable to set Property 'TypePrint'", sc );
      sc = setProperty( "PropertiesPrint", false );
      Assert( sc.isSuccess(), "Unable to set Property 'PropertiesPrint'", sc );
      //
    }

  private:
    // ========================================================================
    /// get the correct algorithm context
    std::string getMyAlg() const {
      const IAlgContextSvc* asvc    = contextSvc();
      const IAlgorithm*     current = ( asvc ? asvc->currentAlg() : NULL );
      return ( current ? " [" + current->name() + "]" : "" );
    }
    // ========================================================================
  private:
    // ========================================================================
    bool m_printMyAlg; ///< Print Algorithm Context
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
/// the factory for instantiation
DECLARE_COMPONENT( LoKi::Reporter )
// ============================================================================
// The END
// ============================================================================
