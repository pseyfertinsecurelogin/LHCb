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
