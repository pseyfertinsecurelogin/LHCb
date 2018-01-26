/// GaudiKernel
#include "GaudiKernel/MsgStream.h"
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/GeometryInfoException.h"


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
GaudiException* GeometryInfoException::clone() const  { return  new GeometryInfoException(*this); }
////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& GeometryInfoException::printOut( std::ostream& os ) const
{
  ///
  os << "\t" << tag() << " \t" << message() << "\t StatusCode=" << code()
     << m_gie_geometryInfo << std::endl ;
  ///
  return previous() ? ( previous()->printOut( os ) ) :  os  ;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
MsgStream&    GeometryInfoException::printOut( MsgStream&  os     ) const
{
  ///
  os << "\t" << tag() << " \t" << message() << "\t StatusCode=" << code()
     << m_gie_geometryInfo << endmsg ;
  ///
  return previous() ? ( previous()->printOut( os ) ) :  os  ;
  ///
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
