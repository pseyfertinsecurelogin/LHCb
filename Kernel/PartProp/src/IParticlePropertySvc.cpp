/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleID.h"
#include "Kernel/ParticleProperty.h"
// ============================================================================
/** @file
 *  Implementation file for class LHCb::IParticlePropertySvc
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-08-03
 */
// ============================================================================
/* helper utility for mapping of LHCb::ParticleProperty object into
 *  non-negative integral sequential identifier
 *
 *  This appears to be useful operation, but since it is
 *  "pure technical" it does not appear as interface method.
 *
 *  For invalid/missing property and/or  service
 *  <c>0</c> is returned. The valid result is always
 *  satisfy the condition: <c> index <= service->size() </c>
 *
 *  @param property the property to be mapped
 *  @param service the service
 *  @return the sequential non-negative index
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-08-03
 */
// ============================================================================
size_t LHCb::ParticleProperties::index( const LHCb::ParticleProperty*     property,
                                        const LHCb::IParticlePropertySvc* service ) {
  if ( !property || !service ) { return 0; } // RETURN
  // ==========================================================================
  auto first = service->begin();
  auto last  = service->end();
  // start the binary_search
  static constexpr auto cmp   = LHCb::ParticleProperty::Compare();
  auto                  ifind = std::lower_bound( first, last, property, cmp );
  return last != ifind && !cmp( *ifind, property ) ? ( ifind - first + 1 ) : 0;
}
// ============================================================================
/* helper utility for mapping of LHCb::ParticleID object into
 *  non-negative integral sequential identifier
 *
 *  This appears to be useful operation, but since it is
 *  "pure technical" it does not appear as interface method.
 *
 *  For invalid/missing PID and/or  service
 *  <c>0</c> is returned. The valid result is always
 *  satisfy the condition: <c> index <= service->size() </c>
 *
 *  @param pid the object to be mapped
 *  @param service the service
 *  @return the sequential non-negative index
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-08-03
 */
// ============================================================================
size_t LHCb::ParticleProperties::index( const LHCb::ParticleID& pid, const LHCb::IParticlePropertySvc* service ) {
  if ( !service ) { return 0; } // RETURN
  // ==========================================================================
  const LHCb::ParticleProperty* pp = service->find( pid );
  return pp ? LHCb::ParticleProperties::index( pp, service ) : 0;
}
// ============================================================================
/* the inverse mapping of the integer sequential number onto
 *  LHCb::ParticleID object
 *
 *  This appears to be useful operation, but since it is
 *  "pure technical" it does not appear as interface method.
 *
 *  For invalid/missing PID and/or  service
 *  <c>NULL</c> is returned.
 *
 *  @param pid the object to be mapped
 *  @param service the service
 *  @return the sequential non-negative index
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-08-03
 */
// ============================================================================
const LHCb::ParticleProperty* LHCb::ParticleProperties::particle( const size_t                      index,
                                                                  const LHCb::IParticlePropertySvc* service ) {
  if ( 0 == index || !service ) { return nullptr; } // RETURN
  // get the iterators from the service
  auto first = service->begin();
  auto last  = service->end();
  if ( index > (size_t)std::distance( first, last ) ) { return 0; } // RETURN
  std::advance( first, index - 1 );
  return *first; // RETURN
}
// ============================================================================
/*  the inverse mapping of the integer sequential number onto
 *  LHCb::ParticleID object
 *
 *  This appears to be useful operation, but since it is
 *  "pure technical" it does not appear as interface method.
 *
 *  For invalid/missing index and/or  service
 *  <c>LHCb::ParticleID()</c> is returned.
 *
 *  @param pid the object to be mapped
 *  @param service the service
 *  @return the sequential non-negative index
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-08-03
 */
// ============================================================================
const LHCb::ParticleID LHCb::ParticleProperties::particleID( const size_t                      index,
                                                             const LHCb::IParticlePropertySvc* service ) {
  if ( 0 == index || !service ) { return LHCb::ParticleID(); } // RETURN
  const LHCb::ParticleProperty* pp = LHCb::ParticleProperties::particle( index, service );
  return pp ? pp->particleID() : LHCb::ParticleID();
}
// ============================================================================
/* mapping by pythiaID
 *
 *  @code
 *
 *   const int pythiaID = ... ;
 *
 *   const LHCb::IParticlePropertySvc* svc = ... ;
 *
 *   const LHCb::ParticleProeprty* pp = byPythiaID( pythiaID , svc ) ;
 *
 *  @endcode
 *
 *  @attention the method is not very efficient and should not be abused
 *  @see LHCb::ParticleProperties::particle
 *  @param pythia pythia identifier
 *  @param svc    pointer to particle property service
 *  @return the particle property for the given pythia ID
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-08-03
 */
// ============================================================================
const LHCb::ParticleProperty* LHCb::ParticleProperties::byPythiaID( const int                         pythia,
                                                                    const LHCb::IParticlePropertySvc* svc ) {
  if ( !svc ) { return nullptr; }
  // to be efficient
  // 1) try to use PDG-ID (fast, logarithmic search)
  const LHCb::ParticleProperty* pp = svc->find( LHCb::ParticleID( pythia ) );
  // 2) check the proper pythia ID
  if ( pp && pythia == pp->pythiaID() ) { return pp; } // RETURN
  // 3) use the resular (linear search)
  auto begin = svc->begin();
  auto end   = svc->end();
  auto found = std::find_if( begin, end, [&]( const LHCb::ParticleProperty* pp ) { return pp->pythiaID() == pythia; } );
  //
  return found != end ? *found : nullptr;
}
// ============================================================================
/*  mapping by EvtGen-name
 *
 *  @code
 *
 *   const std::string& evtGen = ...
 *
 *   const LHCb::IParticlePropertySvc* svc = ... ;
 *
 *   const LHCb::ParticleProperty* pp = byEvtGenName ( evtGen , svc ) ;
 *
 *  @endcode
 *
 *  @attention the method is not very efficient and should not be abused
 *  @see LHCb::ParticleProperties::particle
 *  @param evtGen the particle naem in EvtGen-generator
 *  @param svc    pointer to particle property service
 *  @return the particle property for the given EvtGen-name
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-08-03
 */
// ============================================================================
const LHCb::ParticleProperty* LHCb::ParticleProperties::byEvtGenName( const std::string&                evtGen,
                                                                      const LHCb::IParticlePropertySvc* svc ) {
  if ( !svc ) { return nullptr; }
  // to be more efficient:
  // 1) try to use the regualr name (fast, logarithmic search)
  const LHCb::ParticleProperty* pp = svc->find( evtGen );
  // 2) check the proper evtgen name
  if ( pp && evtGen == pp->evtGen() ) { return pp; } // RETURN
  // 3) use the resular (linear search)
  auto begin = svc->begin();
  auto end   = svc->end();
  auto found = std::find_if( begin, end, [&]( const LHCb::ParticleProperty* pp ) { return pp->evtGen() == evtGen; } );
  //
  return found != end ? *found : nullptr;
}
// ============================================================================
/*  get all the properties at once
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2008-08-03
 */
// ============================================================================
LHCb::IParticlePropertySvc::ParticleProperties
LHCb::ParticleProperties::allProperties( const LHCb::IParticlePropertySvc* service ) {
  return service ? LHCb::IParticlePropertySvc::ParticleProperties( service->begin(), service->end() )
                 : LHCb::IParticlePropertySvc::ParticleProperties();
}
// ============================================================================
// The END
// ============================================================================
