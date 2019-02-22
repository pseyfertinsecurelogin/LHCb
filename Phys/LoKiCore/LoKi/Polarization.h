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
#ifndef LOKI_POLARIZATION_H
#define LOKI_POLARIZATION_H 1
// ============================================================================
// Include files
// ============================================================================
// STD& STD:
// ============================================================================
#include <array>
// ============================================================================
//  GaudiKernel
// ============================================================================
// #include "GaudiKernel/TaggedBool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/KinTypes.h"
// ============================================================================
/** @file
 *  Collection of  functions to deal with polarization axes
 *  @see  M.Beneke, M.Kramer, M.Vanttiner, Phys.Rev. D57 (1998) 4258
 *  @see https://doi.org/10.1103/PhysRevD.57.4258
 */
// ============================================================================
namespace LoKi {
  // ========================================================================
  namespace Kinematics {
    // ========================================================================
    /** @class Polarization
     *  helper class/namespace to hold the function to deal with polarization
     *  axes and frames
     *  @see  M.Beneke, M.Kramer, M.Vanttiner, Phys.Rev. D57 (1998) 4258
     *  @see https://doi.org/10.1103/PhysRevD.57.4258
     */
    class Polarization {
      // =====================================================================
    public:
      // =====================================================================
      /** @enum Frames
       *  the list of polarization frames
       */
      // =====================================================================
      enum class Frames : unsigned short {
        Recoil = 0, //  <--- Helicity
        GottfriedJackson,
        Target,
        CollinsSoper
      };
      // =====================================================================
      /** @typedef Frame
       *  three axes for polarization frame
       */
      typedef std::array<LoKi::LorentzVector, 4> Frame;
      // =====================================================================
      /** @typedef PolVectors
       *  three polarization vectors (-1,0,1)
       */
      typedef std::array<LoKi::ComplexLorentzVector, 3> PolVectors;
      // =====================================================================
      /// helper structure to keep (cos_theta,phi) result
      struct Angles {
        double cos_theta;
        double phi;
      };
      // =====================================================================
      /** @typedef Cosines
       *  The structire to keep the direction cosines
       */
      typedef std::array<double, 3> Cosines;
      // =====================================================================
      /** use Madison convention?
       *  @see "Polarization phenomena in nuclear reactions:
       *       proceedings of 3rd international symposium on polarization
       *       phenomena in nuclear reactions",
       *       Eds. H.H. Barschall and W. Haeberli,
       *       University of Wisconsin Press, Madison WI U.S.A. 1971.
       */
      // using UseMadisonConvention = Gaudi::tagged_bool<struct UseMadisonConvention_tag>;
      typedef bool UseMadisonConvention;
      // =====================================================================
    public:
      // =====================================================================
      /** get the three axes for polarization frame
       *  @param f  the frame
       *  @param P     4-momenta of the particle
       *  @param beam1 4-momenta of the first colliding particle
       *  @param beam2 4-momenta of the second colliding particle
       *  @param madison use Madison convention?
       *  @return three polarization axes: x,y&z
       */
      static Frame frame( const Frames f, const LoKi::LorentzVector& P, const LoKi::LorentzVector& beam1,
                          const LoKi::LorentzVector& beam2,
                          const UseMadisonConvention madison = UseMadisonConvention{true} );
      // =====================================================================
    public:
      // =====================================================================
      /** get the polarization vectors from the frame
       *  @param f the frame
       *  @return polarization vectors (-1,0,+1)
       */
      static PolVectors vectors( const Frame& f );
      // =====================================================================
    public: // direction cosines
      // =====================================================================
      /** get the direction cosines of the particle direction
       *  in the specified reference frame
       *  @param p the particle
       *  @param f the frame
       *  @return  direction  cosines
       */
      static Cosines cosines( const LoKi::LorentzVector& p, const Frame& f );
      // ====================================================================
      /** get the direction cosines of the particle direction
       *  in the rest frame of particle m,  and the beam-momenta p1& p2
       *  @param p the particle
       *  @param f the frame
       *  @param m the particle that defined the frame
       *  @param p1 4-momenta of the first colliding particle
       *  @param p2 4-momenta of the second colliding particle
       *  @param madison use Madison convention?
       *  @return  array of direction cosines
       */
      static Cosines cosines( const LoKi::LorentzVector& p, const Frames f, const LoKi::LorentzVector& m,
                              const LoKi::LorentzVector& beam1, const LoKi::LorentzVector& beam2,
                              const UseMadisonConvention madison = UseMadisonConvention{true} );
      // =====================================================================
      /** get the angles \$ (\cos \theta,\phi)\$ for the particle
       *  in the defined frame
       *  @param p the particle
       *  @param f the frame
       *  @return  \$ (\cos \theta,\phi)\$ structure
       */
      static Angles angles( const LoKi::LorentzVector& p, const Frame& f );
      // ====================================================================
      /** get the angles \$ (\cos \theta,\phi)\$ for the particle
       *  in the rest frame of particle m,  and the beam-momenta p1& p2
       *  @param p the particle
       *  @param f the frame
       *  @param m the particle that defined the frame
       *  @param p1 4-momenta of the first colliding particle
       *  @param p2 4-momenta of the second colliding particle
       *  @param madison use Madison convention?
       *  @return  \$ (\cos \theta,\phi)\$ structure
       */
      static Angles angles( const LoKi::LorentzVector& p, const Frames f, const LoKi::LorentzVector& m,
                            const LoKi::LorentzVector& beam1, const LoKi::LorentzVector& beam2,
                            const UseMadisonConvention madison = UseMadisonConvention{true} );
      // =====================================================================
    }; //                             The end of LoKi::KinematicsPolarization
    // =======================================================================
  } // namespace Kinematics
  // =========================================================================
} // namespace LoKi
// ===========================================================================
//                                                                     The END
// ===========================================================================
#endif // LOKI_POLARIZATION_H
