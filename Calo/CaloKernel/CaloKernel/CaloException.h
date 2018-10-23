/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ===========================================================================
#ifndef      CALOKERNEL_CALOEXCEPTION_H
#define      CALOKERNEL_CALOEXCEPTION_H 1
/// ===========================================================================
// GaudiKernel
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/GaudiException.h"

/** @class CaloException CaloException.h CaloKernel/CaloException.h
 *
 *  It is just a base class for all exceptions in the Calorimeter system
 *
 *  @author  Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date    26/11/1999
 */

struct CaloException : GaudiException
{
  /** constructor
   *  @param ex  exception message
   *  @param sc  status code assiciated with exception
   */
  CaloException( const std::string& ex   = "unspecified exception",
                 const StatusCode&  sc   =  StatusCode::FAILURE )
    : GaudiException( ex , "*CaloException*" , sc )  {}

  /** constructor
   *  @param ex          exception message
   *  @param Exception   previous exception
   *  @param sc          status code associated with the exception
   */
  CaloException( const std::string   & ex                        ,
                 const GaudiException& Ex                        ,
                 const StatusCode&     sc =  StatusCode::FAILURE )
    : GaudiException( ex, "*CaloException*" , sc , Ex ) {}

  /** clone (virtual constructor)
   */
  GaudiException* clone() const override { return new CaloException(*this); }

};

// ===========================================================================
// The End
// ===========================================================================
#endif  ///<   CALOKERNEL_CALOEXCEPTION_H
// ===========================================================================
