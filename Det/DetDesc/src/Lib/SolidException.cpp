// GaudiKernel
#include "DetDesc/ISolid.h"
#include "GaudiKernel/MsgStream.h"
// DetDesc
#include "DetDesc/SolidException.h"

//////////////////////////////////////////////////////////////////////////////////////
SolidException::SolidException( const std::string& message  ,
                                const ISolid*      solid    )
  : GaudiException( message , "*SolidException*" , StatusCode::FAILURE )
  , m_se_solid    ( solid      )
{ }
///////////////////////////////////////////////////////////////////////////////////////
SolidException::SolidException( const std::string   &  message   ,
                                const GaudiException&  Exception ,
                                const ISolid*          solid     )
  : GaudiException( message , "*SolidException*" , StatusCode::FAILURE , Exception )
  , m_se_solid    ( solid      )
{ }
///////////////////////////////////////////////////////////////////////////////////////
std::ostream& SolidException::printOut( std::ostream& os             ) const
{
  ///
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code()
     << " \tSolid=" <<  m_se_solid << std::endl ;
  ///
  return ( 0 != previous() ) ? previous()->printOut( os ) : os ;
  ///
}
///////////////////////////////////////////////////////////////////////////////////////
MsgStream&    SolidException::printOut( MsgStream&    os             ) const
{
  ///
  os << " \t" << tag() << " \t " << message() << "\t StatusCode=" << code()
     << " \tSolid=" <<  m_se_solid << endmsg ;
  ///
  return ( 0 != previous() ) ? previous()->printOut( os ) :  os ;
  ///
}
///////////////////////////////////////////////////////////////////////////////////////
GaudiException* SolidException::clone () const { return new SolidException(*this); }
///////////////////////////////////////////////////////////////////////////////////////
