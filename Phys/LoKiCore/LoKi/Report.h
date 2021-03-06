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
#ifndef LOKI_REPORT_H
#define LOKI_REPORT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
class GaudiException;
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
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class Report
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-01-17
   */
  class Report {
  public:
    // ========================================================================
    /** Print the error  message
     *  @param msg    error message
     *  @param st     status code
     *  @param mx     maximal number of printouts
     */
    static StatusCode Error( const std::string& msg, const StatusCode& st = StatusCode( StatusCode::FAILURE, true ),
                             const size_t mx = 10 );
    // ========================================================================
    /** Print the warning  message
     *  @param msg    warning message
     *  @param st     status code
     *  @param mx     maximal number of printouts
     */
    static StatusCode Warning( const std::string& msg, const StatusCode& st = StatusCode( StatusCode::FAILURE, true ),
                               const size_t mx = 10 );
    // ========================================================================
    /** Print the message
     *  @param msg    warning message
     *  @param st     status code
     *  @param lev    print level
     */
    static void Print( const std::string& msg, const StatusCode& st = StatusCode( StatusCode::SUCCESS, true ),
                       const MSG::Level lev = MSG::INFO );
    // ========================================================================
    /** Assertion - throw exception, if condition is not fulfilled
     *  @see GaudiException
     *  @see LoKi::Exception
     *  @exception LoKi::Exception for invalid condifition
     *  @exception GaudiException for invalid condifition
     *  @param ok           condition which should be "true"
     *  @param message      message to be associated with the exception
     *  @param sc           status code to be returned (artificial)
     */
    inline static void Assert( const bool ok, const std::string& message = "",
                               const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ) ) {
      sc.ignore();
      if ( !ok ) { Exception( message, sc ); }
    }
    // ========================================================================
    /** Create and (re)-throw the exception
     *  @see LoKi::Exception
     *  @exception LoKi::Exception always!
     *  @param msg    exception message
     *  @param exc    (previous) exception of type GaudiException
     *  @param sc     status code
     */
    static void Exception( const std::string& msg, const GaudiException& exc,
                           const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ) );
    // ========================================================================
    /** Create and (re)-throw the exception
     *  @see GaudiException
     *  @exception GaudiException always!
     *  @param msg    exception message
     *  @param exc    (previous) exception of type std::exception
     *  @param sc     status code
     *  @return       status code (fictive)
     */
    static void Exception( const std::string& msg, const std::exception& exc,
                           const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ) );
    // ========================================================================
    /** Create and throw the exception
     *  @see GaudiException
     *  @exception GaudiException always!
     *  @param msg    exception message
     *  @param sc     status code
     *  @return       status code (fictive)
     */
    static void Exception( const std::string& msg = "no message",
                           const StatusCode&  sc  = StatusCode( StatusCode::FAILURE, true ) );
    // ========================================================================
    /// make a report
    static void report();
    // ========================================================================
  };
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_REPORT_H
