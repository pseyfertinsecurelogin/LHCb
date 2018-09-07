// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/RecSummary.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/RecStat.h"
// ============================================================================
/** @file
 *  Implementation file for
 *  class LoKi::TES::HasRecSummary and
 *  class LoKi::TES::RecSummary
 *  @see LoKi::TES::HasRecSummary and
 *  @see LoKi::TES::RecSummary
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2011-02-02
 */
// ============================================================================
/*  Standard constructor from the key, location and 'rootInTES'
 *  @param key          the key to use
 *  @param location     TES location of LHCb::RecSummary obejct
 *  @param useRootInTES flato to use/ignore root-in-tes
 */
// ============================================================================
LoKi::TES::HasRecSummary::HasRecSummary
( const GaudiAlgorithm* algorithm , 
  const int             key          ,
  const std::string&    location     ,
  const bool            useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( algorithm , key , location , useRootInTES ) )
  , LoKi::TES::Exists ( algorithm , location , useRootInTES )
  , m_key ( key )
{
  if  ( location.empty () )
  { setLocation( LHCb::RecSummaryLocation::Default ) ; }
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::TES::HasRecSummary*
LoKi::TES::HasRecSummary::clone () const
{ return new LoKi::TES::HasRecSummary ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::TES::HasRecSummary::operator() (  ) const
{
  //
  const LHCb::RecSummary* summary =
    LoKi::TES::get_<LHCb::RecSummary> ( *this );
  //
  if ( NULL == summary )
  {
    Error ( "No LHCb::RecSummary is available at '" + location () + "'") ;
    return false ;
  }
  //
  return summary -> hasInfo ( m_key ) ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::TES::HasRecSummary::fillStream ( std::ostream& s ) const
{
  //
  s << " HASRECSUMMARY( " << m_key ;
  if ( !useRootInTES () || LHCb::RecSummaryLocation::Default != location() )
  { s << ",'" << location() << "'" ; }
  if ( !useRootInTES () ) { s << ", False " ; }
  //
  return s << " ) " ;
}
// ============================================================================
/*  Standard constructor from the key, location and 'rootInTES'
 *  @param key          the key to use
 *  @param bad          the bad value
 *  @param location     TES location of LHCb::RecSummary obejct
 *  @param useRootInTES flato to use/ignore root-in-tes
 */
// ============================================================================
LoKi::TES::RecSummary::RecSummary
( const GaudiAlgorithm* algorithm , 
  const int          key          ,
  const int          bad          ,
  const std::string& location     ,
  const bool         useRootInTES )
  : LoKi::AuxFunBase ( std::tie ( algorithm , key , bad , location , useRootInTES ) )
  , LoKi::TES::Contains ( algorithm , location , useRootInTES )
  , m_key ( key )
  , m_bad ( bad )
{
  if  ( location.empty () )
  { setLocation( LHCb::RecSummaryLocation::Default ) ; }
}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::TES::RecSummary*
LoKi::TES::RecSummary::clone () const
{ return new LoKi::TES::RecSummary ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::TES::RecSummary::operator() (  ) const
{
  //
  const LHCb::RecSummary* summary = LoKi::TES::get_<LHCb::RecSummary> ( *this );
  //
  if ( NULL == summary )
  {
    Error ( "No LHCb::RecSummary is available at '" + location () + "'") ;
    return m_bad ;
  }
  //
  return summary -> info ( m_key , m_bad ) ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream&
LoKi::TES::RecSummary::fillStream ( std::ostream& s ) const
{
  s << " RECSUMMARY( " << m_key << "," << m_bad ;
  if ( !useRootInTES() || LHCb::RecSummaryLocation::Default != location() )
  { s << ",'" << location() << "'" ; }
  if ( !useRootInTES() ) { s << ", False " ; }
  //
  return s << " ) " ;
}
// ============================================================================



// ============================================================================
// The END
// ============================================================================
