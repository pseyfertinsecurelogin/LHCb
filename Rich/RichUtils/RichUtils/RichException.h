
//-----------------------------------------------------------------------------
/** @file RichException.h
 *
 *  Header file for utility class : Rich::Exception
 *
 *
 *  @author  Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date    09/05/2004
 */
//-----------------------------------------------------------------------------

#pragma once

// GaudiKernel
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatusCode.h"

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class Exception RichException.h RichUtils/RichException.h
   *
   *  Base class for all exceptions in the RICH system
   *
   *  @author  Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date    09/05/2004
   */
  //-----------------------------------------------------------------------------

  class Exception final : public GaudiException
  {

  public:
    /** Default Constructor
     *
     *  @param ex  exception message
     *  @param sc  status code associated with the exception
     */
    Exception( const std::string &ex = "unspecified exception",
               const StatusCode & sc = StatusCode::FAILURE )
      : GaudiException( ex, "*RichException*", sc )
    {}

    /** Constructor from another GaudiException
     *
     *  @param ex          exception message
     *  @param Ex          previous exception
     *  @param sc          status code associated with the exception
     */
    Exception( const std::string &   ex,
               const GaudiException &Ex,
               const StatusCode &    sc = StatusCode::FAILURE )
      : GaudiException( ex, "*RichException*", sc, Ex )
    {}

    /** destructor
     */
    virtual ~Exception() throw() {}

    /** clone (virtual constructor)
     */
    GaudiException *clone() const override { return new Exception( *this ); }
  };

} // namespace Rich
