// Include files 
// ============================================================================
// STD &STL
// ============================================================================
#include <iomanip>
#include <iostream>
#include <string>

#include "GaudiKernel/StringKey.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/Time.icpp"
// ============================================================================
// LHCbKErnel
// ============================================================================
#include "Kernel/LHCbID.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/ParticleID.h"
#include "Kernel/iNode.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/KinTypes.h"
#include "LoKi/ToCpp.h"

// ============================================================================
LoKi::StrKeep::StrKeep( const std::string& data) 
  : m_data( data ) 
{}
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LoKi::StrKeep& s ) { return s.data() ; }
// ============================================================================
std::string Gaudi::Utils::toCpp
( const long double    o ,
  const unsigned short p ) 
{
  std::ostringstream ss ; 
  //
#if defined(__GNUC__)&&(__GNUC__<6)
  std::ios::fmtflags orig_flags = ss.flags();
  ss.unsetf ( std::ios::floatfield );     
  ss << std::setprecision ( p ) << o ;
  ss.flags(orig_flags);
#else 
  ss << std::defaultfloat 
     << std::setprecision ( p ) << o ;
#endif 
  //
  return ss.str() ;
}    
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LoKi::LorentzVector&v ) 
{
  return 
    "LoKi::LorentzVector(" 
    + toCpp ( v.Px() ) + "," 
    + toCpp ( v.Py() ) + "," 
    + toCpp ( v.Pz() ) + "," 
    + toCpp ( v.E () ) + ")" ;  
}
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LoKi::ThreeVector&v ) 
{
  return 
    "LoKi::ThreeVector(" 
    + toCpp ( v.X () ) + "," 
    + toCpp ( v.Y () ) + "," 
    + toCpp ( v.Z () ) + ")";
}
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LoKi::Point3D& v ) 
{
  return 
    "LoKi::Point3D(" 
    + toCpp ( v.X() ) + "," 
    + toCpp ( v.Y() ) + "," 
    + toCpp ( v.Z() ) + ")";
}
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LHCb::ParticleID& v ) 
{ return "LHCb::ParticleID(" + toCpp ( v.pid() ) + ")" ; }
// ============================================================================
std::string Gaudi::Utils::toCpp ( const Decays::iNode& v ) 
{ return "\"" + v.toString() + "\"" ; }
// ============================================================================
std::string Gaudi::Utils::toCpp ( const Gaudi::StringKey& v ) 
{ return "Gaudi::StringKey(" + toCpp ( v.str() )+ ")" ; }
// ============================================================================
std::string Gaudi::Utils::toCpp ( const Gaudi::Time& v ) 
{ return "Gaudi::Time("      + toCpp ( v.ns() )+ ")" ; }
// ============================================================================
std::string Gaudi::Utils::toCpp ( const Gaudi::TimeSpan& v ) 
{ return "Gaudi::TimeSpan("  + toCpp ( v.ns() )+ ")" ; }
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LHCb::LHCbID& v ) 
{ return "LHCb::LHCbID("  + toCpp ( v.lhcbID() )+ ")" ; }
// ============================================================================
// The END
// ============================================================================


