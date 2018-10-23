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
#ifndef     __DETDESC_TRANSPORTSVC_TRANSPORTSVCEXCEPTION_H__
#define     __DETDESC_TRANSPORTSVC_TRANSPORTSVCEXCEPTION_H__ 1

#include "GaudiKernel/GaudiException.h"

///
///  class TransportSvcException: an exception class for Transport Service
///

struct TransportSvcException: GaudiException
{
  /// constructor
  TransportSvcException( const std::string& Message                           ,
                         const StatusCode&  statusCode = StatusCode::FAILURE  )
  : GaudiException( Message , "*TransportSvcException*" , statusCode )
  {};

  ///
  TransportSvcException( const std::string&    Message                             ,
                         const GaudiException& Exception                           ,
                         const StatusCode&     statusCode = StatusCode::FAILURE    )
  : GaudiException( Message , "*TransportSvcException*" , statusCode , Exception )
  {};

};

#endif  //  __DETDESC_TRANSPORTSVC_TRANSPORTSVCEXCEPTION_H__
