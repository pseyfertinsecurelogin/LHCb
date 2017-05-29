// $Id: GeometryInfoException.cpp,v 1.7 2009-04-17 08:54:24 cattanem Exp $
/// GaudiKernel
#include <iomanip>
#include <ostream>
#include <string>

#include "DetDesc/GeometryInfoException.h"
#include "DetDesc/IGeometryInfo.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/StatusCode.h"
///


/////////////////////////////////////////////////////////////////////////////////////////////////
GeometryInfoException::GeometryInfoException( const std::string  & name ,
                                              const IGeometryInfo * gi   ,
                                              const StatusCode   & sc   )
  : GaudiException( name , "*GeometryInfoException*" , sc )
  , m_gie_geometryInfo  ( gi   )
{}
///////////////////////////////////////////////////////////////////////////////////////////////////
GeometryInfoException::GeometryInfoException( const std::string    & name ,
                                              const GaudiException & ge   ,
                                              const IGeometryInfo   * gi   ,
                                              const StatusCode     & sc   )
  : GaudiException( name , "*GeometryInfoException*" , sc , ge )
  , m_gie_geometryInfo  ( gi   )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
GeometryInfoException::~GeometryInfoException() throw() {}
////////////////////////////////////////////////////////////////////////////////////////////////////
GaudiException* GeometryInfoException::clone() const  { return  new GeometryInfoException(*this); }
////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& GeometryInfoException::printOut( std::ostream& os ) const
{
  ///
  os << "\t" << tag() << " \t" << message() ;
  ///
  switch( code() )
    {
    case StatusCode::SUCCESS : os << "\t StatusCode=SUCCESS"                    ;  break ;
    case StatusCode::FAILURE : os << "\t StatusCode=FAILURE"                    ;  break ;
    default                  : os << "\t StatusCode=" << std::setw(7) << code() ;  break ;
    }
  ///
  os << m_gie_geometryInfo << std::endl ;
  ///
  return previous() ? ( previous()->printOut( os ) ) :  os  ;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
MsgStream&    GeometryInfoException::printOut( MsgStream&  os     ) const
{
  ///
  os << "\t" << tag() << " \t" << message() ;
  ///
  switch( code() )
    {
    case StatusCode::SUCCESS : os << "\t StatusCode=SUCCESS"                    ;  break ;
    case StatusCode::FAILURE : os << "\t StatusCode=FAILURE"                    ;  break ;
    default                  : os << "\t StatusCode=" << std::setw(7) << code() ;  break ;
    }
  ///
  os << m_gie_geometryInfo << endmsg ;
  ///
  return previous() ? ( previous()->printOut( os ) ) :  os  ;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
