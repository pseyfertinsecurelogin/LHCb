/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef KERNEL_IDVALGORITHM_H
#define KERNEL_IDVALGORITHM_H 1
// ============================================================================
// Include files
// ============================================================================
// from STL
// ============================================================================
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/INamedInterface.h"
// ============================================================================
// LHCb
// ============================================================================
#include "Event/Particle.h"
#include "Event/RecVertex.h"
// ============================================================================
// Forward declarations
// ============================================================================
struct IDistanceCalculator ;
struct ILifetimeFitter     ;
struct IVertexFit          ;
struct IMassFit            ;
struct IDirectionFit       ;
struct IDecayTreeFit       ;
struct IParticleFilter     ;
struct IParticleReFitter   ;
struct IParticleCombiner   ;
struct IPVReFitter         ;
class GaudiAlgorithm      ;
struct IBTaggingTool       ;
// ============================================================================
/** @class IDVAlgorithm Kernel/IDVAlgorithm.h
 *
 *  Abstract interface containing some of the more useful methods currently
 *  in DVAlgorithm.
 *
 *  @author Juan PALACIOS
 *  @date   2010-01-12
 */
struct GAUDI_API IDVAlgorithm : extend_interfaces<INamedInterface>
{
  // ==========================================================================
  DeclareInterfaceID(IDVAlgorithm, 4, 0);
  // ==========================================================================
  /// get the distance calculator tool
  virtual const IDistanceCalculator*
  distanceCalculator    ( const std::string& nickname = "" ) const = 0 ;
  /// get lifetime fitter tool
  virtual const ILifetimeFitter*
  lifetimeFitter        ( const std::string& nickname = "" ) const = 0 ;
  /// get the vertex fitter tool
  virtual const IVertexFit*
  vertexFitter          ( const std::string& nickname = "" ) const = 0 ;
  /// get particle re-fitter tool
  virtual const IParticleReFitter*
  particleReFitter      ( const std::string& nickname = "" ) const = 0 ;
  /// get particle filter tool
  virtual const IParticleFilter*
  particleFilter        ( const std::string& nickname = "" ) const = 0 ;
  /// Accessor for decay-tree fitter
  virtual       IDecayTreeFit*
  decayTreeFitter       ( const std::string& nickname = "" ) const = 0 ;
  /// Accessor for ParticleCombiner tool
  virtual const IParticleCombiner*
  particleCombiner      ( const std::string& nickname = "" ) const = 0 ;
  /// Accessor for mass-fitter tool
  virtual const IMassFit*
  massFitter            ( const std::string& nickname = "" ) const = 0 ;
  /// Accessor for direction-fitter tool
  virtual const IDirectionFit*
  directionFitter       ( const std::string& nickname = "" ) const = 0 ;
  /// Accessor for primary vertex re-fitter tool
  virtual const IPVReFitter*
  primaryVertexReFitter ( const std::string& nickname = "" ) const = 0 ;
  /// Access the Flavour tagging tool
  virtual IBTaggingTool* flavourTagging() const = 0;
  // ==========================================================================
  // implementation
  // ==========================================================================
  /** Handle to the concrete implementation, or to the parent if
   * implementation does not derive from GaudiAlgorithm
   */
  virtual const GaudiAlgorithm* gaudiAlg() const = 0;
  // ==========================================================================
  // data
  // ==========================================================================
  /// Return a container of local LHCb::Particle*
  virtual const LHCb::Particle::Range
  particles       () const = 0 ;
  /// Return a container of LHCb::RecVertex*, containing primary vertices.
  virtual const LHCb::RecVertex::Range
  primaryVertices () const = 0 ;
  /// Return the best primary vertex for a given LHCb::Particle.
  virtual const LHCb::VertexBase*
  bestVertex      ( const LHCb::Particle* ) const = 0 ;
  /// unrelate related PV
  virtual void  unRelatePV ( const LHCb::Particle* ) const = 0 ;
  // ==========================================================================
};
// ============================================================================
// The END
// ============================================================================
#endif // KERNEL_IDVALGORITHM_H
// ============================================================================
