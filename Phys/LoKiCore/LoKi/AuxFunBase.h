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
#ifndef LOKI_AUXFUNBASE_H
#define LOKI_AUXFUNBASE_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <ostream>
#include <string>
#include <tuple>
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/Interface.h"
#include "LoKi/ToCpp.h"
// ============================================================================
// forward declaration
// ============================================================================
class MsgStream;
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
  /** @class AuxFunBase
   *  Helpful function to serve as common pseudo-base for all LoKi functions
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-15
   */
  class GAUDI_API AuxFunBase {
  protected:
    // ========================================================================
    /// default constructor
    AuxFunBase();
    /// constructor with arguments :
    template <typename... ARGS>
    AuxFunBase( const std::tuple<ARGS...>& tup ) : m_cargs{Gaudi::Utils::toCpp_lst( tup )} {}
    // ========================================================================
    /// copy constructor
    AuxFunBase( const AuxFunBase& right ); // copy constructor
    // assignment
    AuxFunBase& operator=( const AuxFunBase& right ) = default; // copy constructor
    // do NOT implement move, as this class is intended to be used as a virtual baseclass,
    // and in case of multiple inheritance this may cause move to be called more than
    // once on the same (virtual!) base instance -- leading to a `move` from an already
    // `moved` instance, overwriting the result of previous move and leaving the
    // 'moved-to' instance in a 'moved-from' state...
    // move constructor
    AuxFunBase( AuxFunBase&& rhs ) = delete;
    // move assignment
    AuxFunBase& operator=( AuxFunBase&& rhs ) = delete;
    /// destructor
    virtual ~AuxFunBase(); // destructor
  public:
    // ========================================================================
    // ========================================================================
    /** print error message
     *  @param msg  error message
     *  @param sc   status code
     */
    StatusCode Error( const std::string& msg, const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ),
                      int mxprint = 10 ) const;
    /** print warning message
     *  @param msg  warning message
     *  @param sc   status code
     */
    StatusCode Warning( const std::string& msg, const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ),
                        int mxprint = 10 ) const;
    /** thrown an exception
     *  @param msg  warning message
     *  @param sc   status code
     */
    void Exception( const std::string& msg, const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ) ) const;
    /** assert the condition
     *  @param condition the condition to be asserted
     *  @param message the message to be associated with the exception
     *  @param sc status code
     */
    inline void Assert( const bool condition, const std::string& message,
                        const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ) ) const {
      if ( !condition ) { Exception( message, sc ); };
      sc.ignore();
    }
    /** assert the condition
     *  @param condition the condition to be asserted
     *  @param message the message to be associated with the exception
     *  @param sc status code
     */
    inline void Assert( const bool condition, const char* message,
                        const StatusCode& sc = StatusCode( StatusCode::FAILURE, true ) ) const {
      if ( !condition ) { Exception( message, sc ); };
      sc.ignore();
    }
    // ========================================================================
  public:
    // ========================================================================
    /** (virtual) printout to std::ostream
     *  @param s output stream
     *  @return reference to the stream
     */
    virtual std::ostream& fillStream( std::ostream& s ) const;
    /** (virtual) printout in form of std::string
     *  @return string representation (based on fillStream)
     */
    virtual std::string printOut() const;
    /// the actual object type
    virtual std::string objType() const;
    /// unique function ID (hash); see LoKi::genericID
    virtual std::size_t id() const;
    /** (virtual) printout in form of std::string
     *  @return string representation (must be valid C++)
     */
    virtual std::string toCpp() const;
    // ========================================================================
  public:
    // ========================================================================
    /// get the event-ID
    unsigned long long event() const { return m_event; }
    /// set the event-ID
    void setEvent( const unsigned long long evt ) const { m_event = evt; }
    /// set the event-ID from LoKi service
    void setEvent() const;
    // ========================================================================
  public:
    // ========================================================================
    /// check the data for the same event
    bool sameEvent() const;
    /// get constructor arguments
    const std::string& cargs() const { return m_cargs; }
    // ========================================================================
  public:
    // ========================================================================
    /// get LoKi service
    static const LoKi::Interface<LoKi::ILoKiSvc>& lokiSvc();
    /// set LoKi service
    static bool setLoKiSvc( const LoKi::ILoKiSvc* svc );
    /// set LoKi service
    static bool setLoKiSvc( const LoKi::Interface<LoKi::ILoKiSvc>& svc );
    // ========================================================================
  public:
    // ========================================================================
    // does Gaudi/LoKi run?
    bool gaudi() const { return !( !lokiSvc() ); }
    // ========================================================================
  public:
    // ========================================================================
    friend void swap( AuxFunBase& lhs, AuxFunBase& rhs ) {
      using std::swap;
      swap( lhs.m_event, rhs.m_event );
      swap( lhs.m_cargs, rhs.m_cargs );
    }
    // ========================================================================
  private:
    // ========================================================================
    /// the event ID
    mutable unsigned long long m_event = 0; // the event ID
    /// constructor arguments
    std::string m_cargs = ":::UNKNOWN+PLEASE+FIX+CODE::CALL+AUXFUNBASE+CONSTRUCTOR:::";
    /// =======================================================================
  };
  // ==========================================================================
  /** simple function to generate the default generic
   *  (hopefully unique?) ID for the functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   */
  GAUDI_API
  std::size_t genericID( const AuxFunBase& );
  // ==========================================================================
  /** output operator of function objects to std::ostream
   *  @param stream reference to the stream
   *  @param obj object to be printed
   *  @return reference to the stream
   */
  GAUDI_API
  std::ostream& operator<<( std::ostream& stream, const AuxFunBase& obj );
  // ==========================================================================
  /** output operator of function objects to std::ostream
   *  @param stream reference to the stream
   *  @param obj object to be printed
   *  @return reference to the stream
   */
  GAUDI_API
  MsgStream& operator<<( MsgStream& stream, const AuxFunBase& obj );
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  namespace Utils {
    // ========================================================================
    /** string representation of the object
     */
    GAUDI_API
    std::string toString( const LoKi::AuxFunBase& o );
    // ========================================================================
    /** dump the object to the stream
     */
    GAUDI_API
    std::ostream& toStream( const LoKi::AuxFunBase& o, std::ostream& s );
    // ========================================================================
    /** string representation of the object (valid C++ code)
     */
    GAUDI_API
    std::string toCpp( const LoKi::AuxFunBase& o );
    // ========================================================================
  } // namespace Utils
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_AUXFUNBASE_H
