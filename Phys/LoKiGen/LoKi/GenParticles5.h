// ============================================================================
#ifndef LOKI_GENPARTICLES5_H
#define LOKI_GENPARTICLES5_H 1
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Kinematics.h"
#include "LoKi/GenTypes.h"
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
namespace LoKi
{
  // ==========================================================================
  namespace GenParticles
  {
    // ========================================================================
    /** @class FlightDistance
     *  evaluator of flight  distance of particle
     *  @see LoKi::Cuts::GPVFLIGHT
     *  @see LoKi::Cuts::GFLIGHTPV
     *  @see LoKi::Cuts::GFLIGHT
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2013-04-14
     */
    class GAUDI_API Flight : public LoKi::GenTypes::GFunc
    {
    public:
      // ======================================================================
      /** constructor from the flag
       *  @param primary use primary vertex, otherwise use own vertex
       */
      Flight ( const bool primary = false ) ;
      /// MANDATORY: clone method ("virtual destructor")
      Flight* clone() const  override;
      /// MANDATORY: the only one essential method
      double operator() ( const HepMC::GenParticle* p ) const  override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
      /// use primary vertex ?
      bool primary() const { return m_primary ; } // use primary vertex ?
      // ======================================================================
    private:
      // ======================================================================
      /// use primary vertex
      bool m_primary ; // use primary vertex
      // ======================================================================
    } ;
    // ========================================================================
  } //                                      end of namespace LoKi::GenParticles
  // ==========================================================================
  namespace Cuts
  {
    // ========================================================================
    /** @var GFLIGHTPV
     *  evaluator of flight distance from PV
     *  @see LoKi::GenParticles::Flight
     *  @see LoKi::Cuts::GPVFLIGHT
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2013-04-14
     */
    const auto GFLIGHTPV = LoKi::GenParticles::Flight{ true  } ;
    // ========================================================================
    /** @var GPVFLIGHT
     *  evaluator of flight distance from PV
     *  @code
     *  @endcode
     *  @see LoKi::GenParticles::Flight
     *  @see LoKi::Cuts::GFLIGHTPV
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2013-04-14
     */
    const auto GPVFLIGHT = LoKi::GenParticles::Flight{ true } ;
    // ========================================================================
    /** @var GFLIGHT
     *  evaluator of flight distance
     *  @code
     *  @endcode
     *  @see LoKi::GenParticles::Flight
     *  @see LoKi::Cuts::GFLIGHTPV
     *  @see LoKi::Cuts::GPVFLIGHT
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2013-04-14
     */
    const auto GFLIGHT  = LoKi::GenParticles::Flight{ false } ;
    // ========================================================================
  } //                                              end of namespace LoKi::Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENPARTICLES5_H
// ============================================================================
