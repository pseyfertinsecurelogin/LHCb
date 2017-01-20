// GaudiKernel
#include "GaudiKernel/System.h"
// DetDecs 
#include "DetDesc/Surface.h"
#include "DetDesc/TabulatedProperty.h"

////////////////////////////////////////////////////////////////////////////////
std::ostream&     Surface::fillStream ( std::ostream& s ) const 
{
  s << " Surface " 
    << "  model="      << std::setw(1)   << model  () 
    << "  finish="     << std::setw(1)   << finish () 
    << "  type="       << std::setw(1)   << type   () 
    << "  value="      << std::setw(10)  << value  ()
    << "\tfirstVol='"  << firstVol ()    << "'"
    << "\tsecondVol='" << secondVol()    << "'" 
    << "\t#props="     << std::setw(2)   << m_props.size() 
    << '\n';
  for (const auto& i : m_props) s << '\t' << i ;
  return s;
}
////////////////////////////////////////////////////////////////////////////////
MsgStream&        Surface::fillStream ( MsgStream&    s ) const 
{
  s << " Surface " 
    << "  model="      << std::setw(1)  << model  () 
    << "  finish="     << std::setw(1)  << finish () 
    << "  type="       << std::setw(1)  << type   () 
    << "  value="      << std::setw(10) << value  ()
    << "\tfirstVol='"  << firstVol ()   <<"'"
    << "\tsecondVol='" << secondVol()   <<"'" 
    << "\t#props="     << std::setw(2)  << m_props.size() 
    << endmsg ;
  for (const auto& i : m_props) s << "\t" << i ; 
  return s;
}
////////////////////////////////////////////////////////////////////////////////

