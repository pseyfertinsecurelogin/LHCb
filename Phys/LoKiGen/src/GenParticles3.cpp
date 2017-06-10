// ============================================================================
// Include files
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Algs.h"
// ============================================================================
// LoKiGen
// ============================================================================
#include "LoKi/GenParticles3.h"
#include "LoKi/GenAlgs.h"
#include "LoKi/GenExtract.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2012-01-28
 *
 */
// ============================================================================
namespace
{
  // ==========================================================================
  inline std::size_t count_parts
  ( HepMC::GenVertex*           v ,
    HepMC::IteratorRange        r ,
    const LoKi::GenTypes::GCut& c )
  {
    return v ?  LoKi::Algs::count_if ( v -> particles_begin ( r ) ,
                                       v -> particles_end   ( r ) , c.func() )
             : 0;
  }
  // ==========================================================================
  inline bool has_parts
  ( HepMC::GenVertex*           v ,
    HepMC::IteratorRange        r ,
    const LoKi::GenTypes::GCut& c )
  {
    return v && LoKi::Algs::found ( v -> particles_begin ( r ) ,
                                    v -> particles_end   ( r ) , c.func() ) ;
  }
  // ==========================================================================
}

namespace LoKi { namespace GenParticles {
// ============================================================================
/*  constructor from the criteria and "range"
 *  @param cut the criteria
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
Count::Count ( const LoKi::Types::GCuts& cut   ,
               HepMC::IteratorRange      range )
  : m_cut   ( cut   )
  , m_range ( range )
{}
// ============================================================================
/*  constructor from the criteria and "range"
 *  @param cut the criteria
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
Count::Count ( HepMC::IteratorRange      range ,
               const LoKi::Types::GCuts& cut   )
  : m_cut   ( cut   )
  , m_range ( range )
{}
// ============================================================================
// MANDATORY: clone method ("virtual contructor")
// ============================================================================
Count* Count::clone() const { return new Count ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double Count::operator() ( Count::argument p ) const
{
  //
  if ( 0 == p )
  {
    Error ( "HepMC::Particle* points to NULL, return -1000 " ) ;
    return -1000 ;
  }
  //
  switch ( m_range )
  {
  case HepMC::parents     :
  case HepMC::ancestors   :
    return count_parts ( p -> production_vertex () , m_range , m_cut ) ; // RETURN
  case HepMC::children    :
  case HepMC::descendants :
    return count_parts ( p -> end_vertex        () , m_range , m_cut ) ; // RETURN
  case HepMC::family      :
  case HepMC::relatives   :
    return 0 != p->production_vertex() ?
      count_parts ( p -> production_vertex () , m_range , m_cut ) :
      count_parts ( p -> end_vertex        () , m_range , m_cut ) ;      // RETURN
  default:
    // =
    Error ( "Invalid HepMC::Ityerator range, return -900 ") ;
    return -900 ;
  }
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& Count::fillStream ( std::ostream& stream ) const
{
  stream << " GCOUNT(" << m_cut << ",";
  switch ( m_range )
  {
  case HepMC::parents       : stream << "HepMC.parents"     ; break ;
  case HepMC::children      : stream << "HepMC.children"    ; break ;
  case HepMC::family        : stream << "HepMC.family"      ; break ;
  case HepMC::ancestors     : stream << "HepMC.ancestors"   ; break ;
  case HepMC::descendants   : stream << "HepMC.descendants" ; break ;
  case HepMC::relatives     : stream << "HepMC.relatives"   ; break ;
  default: stream << m_range       ; break ;
  } ;
  return stream << ") " ;
}
// ============================================================================


// ============================================================================
/*  constructor from the criteria and "range"
 *  @param cut the criteria
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
Has::Has ( const LoKi::Types::GCuts& cut   ,
           HepMC::IteratorRange      range )
  : m_cut   ( cut   )
  , m_range ( range )
{}
// ============================================================================
/*  constructor from the criteria and "range"
 *  @param cut the criteria
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
Has::Has ( HepMC::IteratorRange      range ,
           const LoKi::Types::GCuts& cut   )
  : m_cut   ( cut   )
  , m_range ( range )
{}
// ============================================================================
// MANDATORY: clone method ("virtual contructor")
// ============================================================================
Has* Has::clone() const { return new Has ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool Has::operator() ( Has::argument p ) const
{
  //
  if ( 0 == p )
  {
    Error ( "HepMC::Particle* points to NULL, return false" ) ;
    return false ;
  }
  //
  switch ( m_range )
  {
  case HepMC::parents     :
  case HepMC::ancestors   :
    return has_parts ( p -> production_vertex () , m_range , m_cut ) ; // RETURN
  case HepMC::children    :
  case HepMC::descendants :
    return has_parts ( p -> end_vertex        () , m_range , m_cut ) ; // RETURN
  case HepMC::family      :
  case HepMC::relatives   :
    return 0 != p->production_vertex() ?
      has_parts ( p -> production_vertex () , m_range , m_cut ) :
      has_parts ( p -> end_vertex        () , m_range , m_cut ) ;      // RETURN
  default:
    // =
    Error ( "Invalid HepMC::Ityerator range, return false") ;
    return false ;
  }
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& Has::fillStream ( std::ostream& stream ) const
{
  stream << " GHAS(" << m_cut << ",";
  switch ( m_range )
  {
  case HepMC::parents       : stream << "HepMC.parents"     ; break ;
  case HepMC::children      : stream << "HepMC.children"    ; break ;
  case HepMC::family        : stream << "HepMC.family"      ; break ;
  case HepMC::ancestors     : stream << "HepMC.ancestors"   ; break ;
  case HepMC::descendants   : stream << "HepMC.descendants" ; break ;
  case HepMC::relatives     : stream << "HepMC.relatives"   ; break ;
  default: stream << m_range       ; break ;
  } ;
  return stream << ") " ;
}





} }

// ============================================================================
// The END
// ============================================================================
