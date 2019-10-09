/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Vector4DTypes.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/ParticleID.h"
#include <algorithm>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class Vertex;
  class ProtoParticle;

  // Class ID definition
  static const CLID CLID_Particle = 801;

  // Namespace for locations in TDS
  namespace ParticleLocation {
    inline const std::string User       = "Phys/User/Particles";
    inline const std::string Production = "Phys/Prod/Particles";
  } // namespace ParticleLocation

  /** @class Particle Particle.h
   *
   * Physics Analysis Particle. A chosen hypotesis for a possible candidate
   * particle.
   *
   * @author Patrick Koppenburg
   *
   */

  class Particle : public KeyedObject<int> {
  public:
    /// typedef for std::vector of Particle
    typedef std::vector<Particle*>       Vector;
    typedef std::vector<const Particle*> ConstVector;

    /// typedef for KeyedContainer of Particle
    typedef KeyedContainer<Particle, Containers::HashMap> Container;

    /// Vector of information for combined PIDs
    typedef GaudiUtils::VectorMap<int, double> ExtraInfo;
    /// The container type for shared particles (without ownership)
    typedef SharedObjectsContainer<LHCb::Particle> Selection;
    /// For uniform access to containers in TES (KeyedContainer,SharedContainer)
    typedef Gaudi::NamedRange_<ConstVector> Range;

    /// Additional information
    enum additionalInfo {
      Unknown   = -1,                     // Unknown/illegal value of the index for 'additional info'
      ConfLevel = 0,                      // Confidence Level for the particle
      Weight,                             // 'Weight' for the particle
      Chi2OfMassConstrainedFit      = 10, // Chi2 of Mass Constrained Fit
      Chi2OfDirectionConstrainedFit = 11, // Chi2 of Direction Constrained Fit
      Chi2OfVertexConstrainedFit    = 12, // Chi2 of Vertex  Constrained Fit
      Chi2OfParticleReFitter        = 15, // Chi2 From Particle ReFitter
      HasBremAdded                  = 16, // particle momentum is brem-corrected (0/1)
      NumVtxWithinChi2WindowOneTrack =
          7000, // Number of compatible vertices found within a given chi2 window when adding one track
      SmallestDeltaChi2OneTrack =
          NumVtxWithinChi2WindowOneTrack + 1, // Value of the smallest vertex DeltaChi2 with an extra track
      SmallestDeltaChi2MassOneTrack = NumVtxWithinChi2WindowOneTrack + 2, // Mass of the vertex giving the smallest
                                                                          // vertex DeltaChi2 with an extra track
      SmallestDeltaChi2TwoTracks =
          NumVtxWithinChi2WindowOneTrack + 3, // Value of the smallest vertex DeltaChi2 with two extra tracks
      SmallestDeltaChi2MassTwoTracks = NumVtxWithinChi2WindowOneTrack + 4, // Mass of the vertex giving the smallest
                                                                           // vertex DeltaChi2 with two extra tracks
      FlavourTaggingIPPUs    = 8000,              // IPs wrt PU vertices (for FlavourTagging internal use)
      FlavourTaggingTaggerID = 8001,              // ID of tagger if !=0 (for FlavourTagging internal use)
      Cone1Angle             = 9000,              // Cone angle, cone size 1
      Cone1Mult              = Cone1Angle + 1,    // Cone multiplicity, cone size 1
      Cone1PX                = Cone1Angle + 2,    // Cone Px, cone size 1
      Cone1PY                = Cone1Angle + 3,    // Cone Py, cone size 1
      Cone1PZ                = Cone1Angle + 4,    // Cone Pz, cone size 1
      Cone1P                 = Cone1Angle + 5,    // Cone P, cone size 1
      Cone1PT                = Cone1Angle + 6,    // Cone Pt, cone size 1
      Cone1PXAsym            = Cone1Angle + 7,    // Cone Px asymmetry, cone size 1
      Cone1PYAsym            = Cone1Angle + 8,    // Cone Py asymmetry, cone size 1
      Cone1PZAsym            = Cone1Angle + 9,    // Cone Pz asymmetry, cone size 1
      Cone1PAsym             = Cone1Angle + 10,   // Cone P asymmetry, cone size 1
      Cone1PTAsym            = Cone1Angle + 11,   // Cone Pt asymmetry, cone size 1
      Cone1DeltaEta          = Cone1Angle + 12,   // Cone DeltaEta, cone size 1
      Cone1DeltaPhi          = Cone1Angle + 13,   // Cone DeltaPhi, cone size 1
      Cone2Angle             = Cone1Angle + 14,   // Cone angle, cone size 2
      Cone2Mult              = Cone2Angle + 1,    // Cone multiplicity, cone size 2
      Cone2PX                = Cone2Angle + 2,    // Cone Px, cone size 2
      Cone2PY                = Cone2Angle + 3,    // Cone Py, cone size 2
      Cone2PZ                = Cone2Angle + 4,    // Cone Pz, cone size 2
      Cone2P                 = Cone2Angle + 5,    // Cone P, cone size 2
      Cone2PT                = Cone2Angle + 6,    // Cone Pt, cone size 2
      Cone2PXAsym            = Cone2Angle + 7,    // Cone Px asymmetry, cone size 2
      Cone2PYAsym            = Cone2Angle + 8,    // Cone Py asymmetry, cone size 2
      Cone2PZAsym            = Cone2Angle + 9,    // Cone Pz asymmetry, cone size 2
      Cone2PAsym             = Cone2Angle + 10,   // Cone P asymmetry, cone size 2
      Cone2PTAsym            = Cone2Angle + 11,   // Cone Pt asymmetry, cone size 2
      Cone2DeltaEta          = Cone2Angle + 12,   // Cone DeltaEta, cone size 2
      Cone2DeltaPhi          = Cone2Angle + 13,   // Cone DeltaPhi, cone size 2
      Cone3Angle             = Cone2Angle + 14,   // Cone angle, cone size 3
      Cone3Mult              = Cone3Angle + 1,    // Cone multiplicity, cone size 3
      Cone3PX                = Cone3Angle + 2,    // Cone Px, cone size 3
      Cone3PY                = Cone3Angle + 3,    // Cone Py, cone size 3
      Cone3PZ                = Cone3Angle + 4,    // Cone Pz, cone size 3
      Cone3P                 = Cone3Angle + 5,    // Cone P, cone size 3
      Cone3PT                = Cone3Angle + 6,    // Cone Pt, cone size 3
      Cone3PXAsym            = Cone3Angle + 7,    // Cone Px asymmetry, cone size 3
      Cone3PYAsym            = Cone3Angle + 8,    // Cone Py asymmetry, cone size 3
      Cone3PZAsym            = Cone3Angle + 9,    // Cone Pz asymmetry, cone size 3
      Cone3PAsym             = Cone3Angle + 10,   // Cone P asymmetry, cone size 3
      Cone3PTAsym            = Cone3Angle + 11,   // Cone Pt asymmetry, cone size 3
      Cone3DeltaEta          = Cone3Angle + 12,   // Cone DeltaEta, cone size 3
      Cone3DeltaPhi          = Cone3Angle + 13,   // Cone DeltaPhi, cone size 3
      Cone4Angle             = Cone3Angle + 14,   // Cone angle, cone size 4
      Cone4Mult              = Cone4Angle + 1,    // Cone multiplicity, cone size 4
      Cone4PX                = Cone4Angle + 2,    // Cone Px, cone size 4
      Cone4PY                = Cone4Angle + 3,    // Cone Py, cone size 4
      Cone4PZ                = Cone4Angle + 4,    // Cone Pz, cone size 4
      Cone4P                 = Cone4Angle + 5,    // Cone P, cone size 4
      Cone4PT                = Cone4Angle + 6,    // Cone Pt, cone size 4
      Cone4PXAsym            = Cone4Angle + 7,    // Cone Px asymmetry, cone size 4
      Cone4PYAsym            = Cone4Angle + 8,    // Cone Py asymmetry, cone size 4
      Cone4PZAsym            = Cone4Angle + 9,    // Cone Pz asymmetry, cone size 4
      Cone4PAsym             = Cone4Angle + 10,   // Cone P asymmetry, cone size 4
      Cone4PTAsym            = Cone4Angle + 11,   // Cone Pt asymmetry, cone size 4
      Cone4DeltaEta          = Cone4Angle + 12,   // Cone DeltaEta, cone size 4
      Cone4DeltaPhi          = Cone4Angle + 13,   // Cone DeltaPhi, cone size 4
      EWCone1Index           = Cone4Angle + 14,   // First index for EW cone variables, cone size 1
      EWCone2Index           = EWCone1Index + 23, // First index for EW cone variables, cone size 2
      EWCone3Index           = EWCone2Index + 23, // First index for EW cone variables, cone size 3
      EWCone4Index           = EWCone3Index + 23, // First index for EW cone variables, cone size 4
      FirstJetIndex          = 9500,              // The first index allocated for jet-related studies
      JetActiveArea,                              // Jet Active Area estimation
      JetActiveAreaError,                         // Uncertanty in Jet Active Area evaluation
      JetActiveAreaPx,                            // Px-component of Jet active area four-momentum
      JetActiveAreaPy,                            // Py-component of Jet active area four-momentum
      JetActiveAreaPz,                            // Pz-component of Jet active area four-momentum
      JetActiveAreaE,                             // E-component  of Jet active area four-momentum
      JetPtPerUnitArea,                           // Underlying event activity per unit of Active Area
      LastJetIndex = FirstJetIndex + 199,         // The last  index allocated for jet-related studies
      LastGlobal   = 10000 // The last 'global' index, other values are specific for user analysis
    };

    /// Copy constructor
    Particle( const Particle& part )
        : KeyedObject<int>()
        , m_particleID( part.m_particleID )
        , m_measuredMass( part.m_measuredMass )
        , m_measuredMassErr( part.m_measuredMassErr )
        , m_momentum( part.m_momentum )
        , m_referencePoint( part.m_referencePoint )
        , m_momCovMatrix( part.m_momCovMatrix )
        , m_posCovMatrix( part.m_posCovMatrix )
        , m_posMomCovMatrix( part.m_posMomCovMatrix )
        , m_extraInfo( part.m_extraInfo )
        , m_endVertex( part.m_endVertex )
        , m_proto( part.m_proto )
        , m_daughters( part.m_daughters ) {}

    /// Constructor from ParticleID
    explicit Particle( const LHCb::ParticleID& pid )
        : KeyedObject<int>()
        , m_particleID( pid )
        , m_measuredMass( -1.0 )
        , m_measuredMassErr( 0.0 )
        , m_momentum( 0.0, 0.0, 0.0, -1 * Gaudi::Units::GeV )
        , m_referencePoint( 0.0, 0.0, -100 * Gaudi::Units::m )
        , m_momCovMatrix()
        , m_posCovMatrix()
        , m_posMomCovMatrix()
        , m_extraInfo() {}

    /// Constructor from ParticleID and key
    Particle( const LHCb::ParticleID& pid, const int key )
        : KeyedObject<int>( key )
        , m_particleID( pid )
        , m_measuredMass( -1.0 )
        , m_measuredMassErr( 0.0 )
        , m_momentum( 0.0, 0.0, 0.0, -1 * Gaudi::Units::GeV )
        , m_referencePoint( 0.0, 0.0, -100 * Gaudi::Units::m )
        , m_momCovMatrix()
        , m_posCovMatrix()
        , m_posMomCovMatrix()
        , m_extraInfo() {}

    /// Default Constructor
    Particle()
        : m_particleID()
        , m_measuredMass( -1.0 )
        , m_measuredMassErr( 0.0 )
        , m_momentum( 0.0, 0.0, 0.0, -1 * Gaudi::Units::GeV )
        , m_referencePoint( 0.0, 0.0, -100 * Gaudi::Units::m )
        , m_momCovMatrix()
        , m_posCovMatrix()
        , m_posMomCovMatrix()
        , m_extraInfo() {}

    /// Default Destructor
    virtual ~Particle() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type additionalInfo
    static LHCb::Particle::additionalInfo additionalInfoToType( const std::string& aName );

    /// conversion to string for enum type additionalInfo
    static const std::string& additionalInfoToString( int aEnum );

    /// Clone particle
    Particle* clone() const;

    /// Assignment operator, note that the original vertex and protoParticle are kept
    Particle& operator=( const Particle& rhs );

    /// Confidence Level of the particleID. If not set the default is -1.
    double confLevel() const;

    /// set confidence Level of the particleID.
    void setConfLevel( double cl );

    /// Weight of the particle, whatever that means. If not set the default is 1.
    double weight() const;

    /// set Weight of the particle, whatever that means.
    void setWeight( double w );

    /// Charge in units of +e (i.e. electron charge = -1)
    int charge() const;

    /// short cut for transerve momentum
    double pt() const;

    /// short cut for |P|
    double p() const;

    /// full error matrix on position, 4-momentum. @todo Check the maths of this
    Gaudi::SymMatrix7x7 covMatrix() const;

    /// a basic particle has no daughters
    bool isBasicParticle() const;

    /// Get daughters as a ConstVector
    Particle::ConstVector daughtersVector() const;

    /// has information for specified key
    bool hasInfo( int key ) const;

    ///  add new information, associated with the key
    bool addInfo( int key, double info );

    /// extract the information associated with the given key. If there is no such infomration the default value will be
    /// returned.
    double info( int key, double def ) const;

    /// erase the information associated with the given key
    unsigned long eraseInfo( int key );

    /// Returns slopes ( Px/Pz, Py/Pz, 1 ) at reference point
    Gaudi::XYZVector slopes() const;

    /// Print this Particle in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  PDG code
    const LHCb::ParticleID& particleID() const;

    /// Update  PDG code
    void setParticleID( const LHCb::ParticleID& value );

    /// Retrieve const  Measured Mass
    double measuredMass() const;

    /// Update  Measured Mass
    void setMeasuredMass( double value );

    /// Retrieve const  Error on measured mass
    double measuredMassErr() const;

    /// Update  Error on measured mass
    void setMeasuredMassErr( double value );

    /// Retrieve const  Momentum four vector
    const Gaudi::LorentzVector& momentum() const;

    /// Update  Momentum four vector
    void setMomentum( const Gaudi::LorentzVector& value );

    /// Retrieve const  Point at which the momentum is given in LHCb reference frame - convention: take the one that
    /// minimizes the extrapolations needs
    const Gaudi::XYZPoint& referencePoint() const;

    /// Update  Point at which the momentum is given in LHCb reference frame - convention: take the one that minimizes
    /// the extrapolations needs
    void setReferencePoint( const Gaudi::XYZPoint& value );

    /// Retrieve const  Covariance matrix relative to momentum (4x4)
    const Gaudi::SymMatrix4x4& momCovMatrix() const;

    /// Update  Covariance matrix relative to momentum (4x4)
    void setMomCovMatrix( const Gaudi::SymMatrix4x4& value );

    /// Retrieve const  Covariance matrix relative to point at which the momentum is given (3x3)
    const Gaudi::SymMatrix3x3& posCovMatrix() const;

    /// Update  Covariance matrix relative to point at which the momentum is given (3x3)
    void setPosCovMatrix( const Gaudi::SymMatrix3x3& value );

    /// Retrieve const  Matrix with correlation errors between momemtum and pointOnTrack (momentum x point)
    const Gaudi::Matrix4x3& posMomCovMatrix() const;

    /// Update  Matrix with correlation errors between momemtum and pointOnTrack (momentum x point)
    void setPosMomCovMatrix( const Gaudi::Matrix4x3& value );

    /// Retrieve const  Some addtional user information. Don't use directly. Use *Info() methods.
    const ExtraInfo& extraInfo() const;

    /// Retrieve (const)  Reference to end vertex (ex. decay)
    const LHCb::Vertex* endVertex() const;

    /// Retrieve  Reference to end vertex (ex. decay)
    LHCb::Vertex* endVertex();

    /// Update  Reference to end vertex (ex. decay)
    void setEndVertex( const SmartRef<LHCb::Vertex>& value );

    /// Update (pointer)  Reference to end vertex (ex. decay)
    void setEndVertex( const LHCb::Vertex* value );

    /// Retrieve (const)  Reference to original ProtoParticle
    const LHCb::ProtoParticle* proto() const;

    /// Update  Reference to original ProtoParticle
    void setProto( const SmartRef<LHCb::ProtoParticle>& value );

    /// Update (pointer)  Reference to original ProtoParticle
    void setProto( const LHCb::ProtoParticle* value );

    /// Retrieve (const)  Reference to daughter particles. Users are strongly discouraged to use setDaughters method.
    const SmartRefVector<LHCb::Particle>& daughters() const;

    /// Update  Reference to daughter particles. Users are strongly discouraged to use setDaughters method.
    void setDaughters( const SmartRefVector<LHCb::Particle>& value );

    /// Add to  Reference to daughter particles. Users are strongly discouraged to use setDaughters method.
    void addToDaughters( const SmartRef<LHCb::Particle>& value );

    /// Att to (pointer)  Reference to daughter particles. Users are strongly discouraged to use setDaughters method.
    void addToDaughters( const LHCb::Particle* value );

    /// Remove from  Reference to daughter particles. Users are strongly discouraged to use setDaughters method.
    void removeFromDaughters( const SmartRef<LHCb::Particle>& value );

    /// Clear  Reference to daughter particles. Users are strongly discouraged to use setDaughters method.
    void clearDaughters();

    friend std::ostream& operator<<( std::ostream& str, const Particle& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::ParticleID     m_particleID;      ///< PDG code
    double               m_measuredMass;    ///< Measured Mass
    double               m_measuredMassErr; ///< Error on measured mass
    Gaudi::LorentzVector m_momentum;        ///< Momentum four vector
    Gaudi::XYZPoint m_referencePoint;   ///< Point at which the momentum is given in LHCb reference frame - convention:
                                        ///< take the one that minimizes the extrapolations needs
    Gaudi::SymMatrix4x4 m_momCovMatrix; ///< Covariance matrix relative to momentum (4x4)
    Gaudi::SymMatrix3x3 m_posCovMatrix; ///< Covariance matrix relative to point at which the momentum is given (3x3)
    Gaudi::Matrix4x3 m_posMomCovMatrix; ///< Matrix with correlation errors between momemtum and pointOnTrack (momentum
                                        ///< x point)
    ExtraInfo              m_extraInfo; ///< Some addtional user information. Don't use directly. Use *Info() methods.
    SmartRef<LHCb::Vertex> m_endVertex; ///< Reference to end vertex (ex. decay)
    SmartRef<LHCb::ProtoParticle>  m_proto;     ///< Reference to original ProtoParticle
    SmartRefVector<LHCb::Particle> m_daughters; ///< Reference to daughter particles. Users are strongly discouraged to
                                                ///< use setDaughters method.

    static const GaudiUtils::VectorMap<std::string, additionalInfo>& s_additionalInfoTypMap();

  }; // class Particle

  /// Definition of Keyed Container for Particle
  typedef KeyedContainer<Particle, Containers::HashMap> Particles;

  inline std::ostream& operator<<( std::ostream& s, LHCb::Particle::additionalInfo e ) {
    switch ( e ) {
    case LHCb::Particle::Unknown:
      return s << "Unknown";
    case LHCb::Particle::ConfLevel:
      return s << "ConfLevel";
    case LHCb::Particle::Weight:
      return s << "Weight";
    case LHCb::Particle::Chi2OfMassConstrainedFit:
      return s << "Chi2OfMassConstrainedFit";
    case LHCb::Particle::Chi2OfDirectionConstrainedFit:
      return s << "Chi2OfDirectionConstrainedFit";
    case LHCb::Particle::Chi2OfVertexConstrainedFit:
      return s << "Chi2OfVertexConstrainedFit";
    case LHCb::Particle::Chi2OfParticleReFitter:
      return s << "Chi2OfParticleReFitter";
    case LHCb::Particle::HasBremAdded:
      return s << "HasBremAdded";
    case LHCb::Particle::NumVtxWithinChi2WindowOneTrack:
      return s << "NumVtxWithinChi2WindowOneTrack";
    case LHCb::Particle::SmallestDeltaChi2OneTrack:
      return s << "SmallestDeltaChi2OneTrack";
    case LHCb::Particle::SmallestDeltaChi2MassOneTrack:
      return s << "SmallestDeltaChi2MassOneTrack";
    case LHCb::Particle::SmallestDeltaChi2TwoTracks:
      return s << "SmallestDeltaChi2TwoTracks";
    case LHCb::Particle::SmallestDeltaChi2MassTwoTracks:
      return s << "SmallestDeltaChi2MassTwoTracks";
    case LHCb::Particle::FlavourTaggingIPPUs:
      return s << "FlavourTaggingIPPUs";
    case LHCb::Particle::FlavourTaggingTaggerID:
      return s << "FlavourTaggingTaggerID";
    case LHCb::Particle::Cone1Angle:
      return s << "Cone1Angle";
    case LHCb::Particle::Cone1Mult:
      return s << "Cone1Mult";
    case LHCb::Particle::Cone1PX:
      return s << "Cone1PX";
    case LHCb::Particle::Cone1PY:
      return s << "Cone1PY";
    case LHCb::Particle::Cone1PZ:
      return s << "Cone1PZ";
    case LHCb::Particle::Cone1P:
      return s << "Cone1P";
    case LHCb::Particle::Cone1PT:
      return s << "Cone1PT";
    case LHCb::Particle::Cone1PXAsym:
      return s << "Cone1PXAsym";
    case LHCb::Particle::Cone1PYAsym:
      return s << "Cone1PYAsym";
    case LHCb::Particle::Cone1PZAsym:
      return s << "Cone1PZAsym";
    case LHCb::Particle::Cone1PAsym:
      return s << "Cone1PAsym";
    case LHCb::Particle::Cone1PTAsym:
      return s << "Cone1PTAsym";
    case LHCb::Particle::Cone1DeltaEta:
      return s << "Cone1DeltaEta";
    case LHCb::Particle::Cone1DeltaPhi:
      return s << "Cone1DeltaPhi";
    case LHCb::Particle::Cone2Angle:
      return s << "Cone2Angle";
    case LHCb::Particle::Cone2Mult:
      return s << "Cone2Mult";
    case LHCb::Particle::Cone2PX:
      return s << "Cone2PX";
    case LHCb::Particle::Cone2PY:
      return s << "Cone2PY";
    case LHCb::Particle::Cone2PZ:
      return s << "Cone2PZ";
    case LHCb::Particle::Cone2P:
      return s << "Cone2P";
    case LHCb::Particle::Cone2PT:
      return s << "Cone2PT";
    case LHCb::Particle::Cone2PXAsym:
      return s << "Cone2PXAsym";
    case LHCb::Particle::Cone2PYAsym:
      return s << "Cone2PYAsym";
    case LHCb::Particle::Cone2PZAsym:
      return s << "Cone2PZAsym";
    case LHCb::Particle::Cone2PAsym:
      return s << "Cone2PAsym";
    case LHCb::Particle::Cone2PTAsym:
      return s << "Cone2PTAsym";
    case LHCb::Particle::Cone2DeltaEta:
      return s << "Cone2DeltaEta";
    case LHCb::Particle::Cone2DeltaPhi:
      return s << "Cone2DeltaPhi";
    case LHCb::Particle::Cone3Angle:
      return s << "Cone3Angle";
    case LHCb::Particle::Cone3Mult:
      return s << "Cone3Mult";
    case LHCb::Particle::Cone3PX:
      return s << "Cone3PX";
    case LHCb::Particle::Cone3PY:
      return s << "Cone3PY";
    case LHCb::Particle::Cone3PZ:
      return s << "Cone3PZ";
    case LHCb::Particle::Cone3P:
      return s << "Cone3P";
    case LHCb::Particle::Cone3PT:
      return s << "Cone3PT";
    case LHCb::Particle::Cone3PXAsym:
      return s << "Cone3PXAsym";
    case LHCb::Particle::Cone3PYAsym:
      return s << "Cone3PYAsym";
    case LHCb::Particle::Cone3PZAsym:
      return s << "Cone3PZAsym";
    case LHCb::Particle::Cone3PAsym:
      return s << "Cone3PAsym";
    case LHCb::Particle::Cone3PTAsym:
      return s << "Cone3PTAsym";
    case LHCb::Particle::Cone3DeltaEta:
      return s << "Cone3DeltaEta";
    case LHCb::Particle::Cone3DeltaPhi:
      return s << "Cone3DeltaPhi";
    case LHCb::Particle::Cone4Angle:
      return s << "Cone4Angle";
    case LHCb::Particle::Cone4Mult:
      return s << "Cone4Mult";
    case LHCb::Particle::Cone4PX:
      return s << "Cone4PX";
    case LHCb::Particle::Cone4PY:
      return s << "Cone4PY";
    case LHCb::Particle::Cone4PZ:
      return s << "Cone4PZ";
    case LHCb::Particle::Cone4P:
      return s << "Cone4P";
    case LHCb::Particle::Cone4PT:
      return s << "Cone4PT";
    case LHCb::Particle::Cone4PXAsym:
      return s << "Cone4PXAsym";
    case LHCb::Particle::Cone4PYAsym:
      return s << "Cone4PYAsym";
    case LHCb::Particle::Cone4PZAsym:
      return s << "Cone4PZAsym";
    case LHCb::Particle::Cone4PAsym:
      return s << "Cone4PAsym";
    case LHCb::Particle::Cone4PTAsym:
      return s << "Cone4PTAsym";
    case LHCb::Particle::Cone4DeltaEta:
      return s << "Cone4DeltaEta";
    case LHCb::Particle::Cone4DeltaPhi:
      return s << "Cone4DeltaPhi";
    case LHCb::Particle::EWCone1Index:
      return s << "EWCone1Index";
    case LHCb::Particle::EWCone2Index:
      return s << "EWCone2Index";
    case LHCb::Particle::EWCone3Index:
      return s << "EWCone3Index";
    case LHCb::Particle::EWCone4Index:
      return s << "EWCone4Index";
    case LHCb::Particle::FirstJetIndex:
      return s << "FirstJetIndex";
    case LHCb::Particle::JetActiveArea:
      return s << "JetActiveArea";
    case LHCb::Particle::JetActiveAreaError:
      return s << "JetActiveAreaError";
    case LHCb::Particle::JetActiveAreaPx:
      return s << "JetActiveAreaPx";
    case LHCb::Particle::JetActiveAreaPy:
      return s << "JetActiveAreaPy";
    case LHCb::Particle::JetActiveAreaPz:
      return s << "JetActiveAreaPz";
    case LHCb::Particle::JetActiveAreaE:
      return s << "JetActiveAreaE";
    case LHCb::Particle::JetPtPerUnitArea:
      return s << "JetPtPerUnitArea";
    case LHCb::Particle::LastJetIndex:
      return s << "LastJetIndex";
    case LHCb::Particle::LastGlobal:
      return s << "LastGlobal";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Particle::additionalInfo";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "Event/ProtoParticle.h"
#include "Event/Vertex.h"

inline const CLID& LHCb::Particle::clID() const { return LHCb::Particle::classID(); }

inline const CLID& LHCb::Particle::classID() { return CLID_Particle; }

inline const GaudiUtils::VectorMap<std::string, LHCb::Particle::additionalInfo>&
LHCb::Particle::s_additionalInfoTypMap() {
  static const GaudiUtils::VectorMap<std::string, additionalInfo> m = {
      {"Unknown", Unknown},
      {"ConfLevel", ConfLevel},
      {"Weight", Weight},
      {"Chi2OfMassConstrainedFit", Chi2OfMassConstrainedFit},
      {"Chi2OfDirectionConstrainedFit", Chi2OfDirectionConstrainedFit},
      {"Chi2OfVertexConstrainedFit", Chi2OfVertexConstrainedFit},
      {"Chi2OfParticleReFitter", Chi2OfParticleReFitter},
      {"HasBremAdded", HasBremAdded},
      {"NumVtxWithinChi2WindowOneTrack", NumVtxWithinChi2WindowOneTrack},
      {"SmallestDeltaChi2OneTrack", SmallestDeltaChi2OneTrack},
      {"SmallestDeltaChi2MassOneTrack", SmallestDeltaChi2MassOneTrack},
      {"SmallestDeltaChi2TwoTracks", SmallestDeltaChi2TwoTracks},
      {"SmallestDeltaChi2MassTwoTracks", SmallestDeltaChi2MassTwoTracks},
      {"FlavourTaggingIPPUs", FlavourTaggingIPPUs},
      {"FlavourTaggingTaggerID", FlavourTaggingTaggerID},
      {"Cone1Angle", Cone1Angle},
      {"Cone1Mult", Cone1Mult},
      {"Cone1PX", Cone1PX},
      {"Cone1PY", Cone1PY},
      {"Cone1PZ", Cone1PZ},
      {"Cone1P", Cone1P},
      {"Cone1PT", Cone1PT},
      {"Cone1PXAsym", Cone1PXAsym},
      {"Cone1PYAsym", Cone1PYAsym},
      {"Cone1PZAsym", Cone1PZAsym},
      {"Cone1PAsym", Cone1PAsym},
      {"Cone1PTAsym", Cone1PTAsym},
      {"Cone1DeltaEta", Cone1DeltaEta},
      {"Cone1DeltaPhi", Cone1DeltaPhi},
      {"Cone2Angle", Cone2Angle},
      {"Cone2Mult", Cone2Mult},
      {"Cone2PX", Cone2PX},
      {"Cone2PY", Cone2PY},
      {"Cone2PZ", Cone2PZ},
      {"Cone2P", Cone2P},
      {"Cone2PT", Cone2PT},
      {"Cone2PXAsym", Cone2PXAsym},
      {"Cone2PYAsym", Cone2PYAsym},
      {"Cone2PZAsym", Cone2PZAsym},
      {"Cone2PAsym", Cone2PAsym},
      {"Cone2PTAsym", Cone2PTAsym},
      {"Cone2DeltaEta", Cone2DeltaEta},
      {"Cone2DeltaPhi", Cone2DeltaPhi},
      {"Cone3Angle", Cone3Angle},
      {"Cone3Mult", Cone3Mult},
      {"Cone3PX", Cone3PX},
      {"Cone3PY", Cone3PY},
      {"Cone3PZ", Cone3PZ},
      {"Cone3P", Cone3P},
      {"Cone3PT", Cone3PT},
      {"Cone3PXAsym", Cone3PXAsym},
      {"Cone3PYAsym", Cone3PYAsym},
      {"Cone3PZAsym", Cone3PZAsym},
      {"Cone3PAsym", Cone3PAsym},
      {"Cone3PTAsym", Cone3PTAsym},
      {"Cone3DeltaEta", Cone3DeltaEta},
      {"Cone3DeltaPhi", Cone3DeltaPhi},
      {"Cone4Angle", Cone4Angle},
      {"Cone4Mult", Cone4Mult},
      {"Cone4PX", Cone4PX},
      {"Cone4PY", Cone4PY},
      {"Cone4PZ", Cone4PZ},
      {"Cone4P", Cone4P},
      {"Cone4PT", Cone4PT},
      {"Cone4PXAsym", Cone4PXAsym},
      {"Cone4PYAsym", Cone4PYAsym},
      {"Cone4PZAsym", Cone4PZAsym},
      {"Cone4PAsym", Cone4PAsym},
      {"Cone4PTAsym", Cone4PTAsym},
      {"Cone4DeltaEta", Cone4DeltaEta},
      {"Cone4DeltaPhi", Cone4DeltaPhi},
      {"EWCone1Index", EWCone1Index},
      {"EWCone2Index", EWCone2Index},
      {"EWCone3Index", EWCone3Index},
      {"EWCone4Index", EWCone4Index},
      {"FirstJetIndex", FirstJetIndex},
      {"JetActiveArea", JetActiveArea},
      {"JetActiveAreaError", JetActiveAreaError},
      {"JetActiveAreaPx", JetActiveAreaPx},
      {"JetActiveAreaPy", JetActiveAreaPy},
      {"JetActiveAreaPz", JetActiveAreaPz},
      {"JetActiveAreaE", JetActiveAreaE},
      {"JetPtPerUnitArea", JetPtPerUnitArea},
      {"LastJetIndex", LastJetIndex},
      {"LastGlobal", LastGlobal}};
  return m;
}

inline LHCb::Particle::additionalInfo LHCb::Particle::additionalInfoToType( const std::string& aName ) {
  auto iter = s_additionalInfoTypMap().find( aName );
  return iter != s_additionalInfoTypMap().end() ? iter->second : Unknown;
}

inline const std::string& LHCb::Particle::additionalInfoToString( int aEnum ) {
  static const std::string s_Unknown = "Unknown";
  auto                     iter =
      std::find_if( s_additionalInfoTypMap().begin(), s_additionalInfoTypMap().end(),
                    [&]( const std::pair<const std::string, additionalInfo>& i ) { return i.second == aEnum; } );
  return iter != s_additionalInfoTypMap().end() ? iter->first : s_Unknown;
}

inline const LHCb::ParticleID& LHCb::Particle::particleID() const { return m_particleID; }

inline void LHCb::Particle::setParticleID( const LHCb::ParticleID& value ) { m_particleID = value; }

inline double LHCb::Particle::measuredMass() const { return m_measuredMass; }

inline void LHCb::Particle::setMeasuredMass( double value ) { m_measuredMass = value; }

inline double LHCb::Particle::measuredMassErr() const { return m_measuredMassErr; }

inline void LHCb::Particle::setMeasuredMassErr( double value ) { m_measuredMassErr = value; }

inline const Gaudi::LorentzVector& LHCb::Particle::momentum() const { return m_momentum; }

inline void LHCb::Particle::setMomentum( const Gaudi::LorentzVector& value ) { m_momentum = value; }

inline const Gaudi::XYZPoint& LHCb::Particle::referencePoint() const { return m_referencePoint; }

inline void LHCb::Particle::setReferencePoint( const Gaudi::XYZPoint& value ) { m_referencePoint = value; }

inline const Gaudi::SymMatrix4x4& LHCb::Particle::momCovMatrix() const { return m_momCovMatrix; }

inline void LHCb::Particle::setMomCovMatrix( const Gaudi::SymMatrix4x4& value ) { m_momCovMatrix = value; }

inline const Gaudi::SymMatrix3x3& LHCb::Particle::posCovMatrix() const { return m_posCovMatrix; }

inline void LHCb::Particle::setPosCovMatrix( const Gaudi::SymMatrix3x3& value ) { m_posCovMatrix = value; }

inline const Gaudi::Matrix4x3& LHCb::Particle::posMomCovMatrix() const { return m_posMomCovMatrix; }

inline void LHCb::Particle::setPosMomCovMatrix( const Gaudi::Matrix4x3& value ) { m_posMomCovMatrix = value; }

inline const LHCb::Particle::ExtraInfo& LHCb::Particle::extraInfo() const { return m_extraInfo; }

inline const LHCb::Vertex* LHCb::Particle::endVertex() const { return m_endVertex; }

inline LHCb::Vertex* LHCb::Particle::endVertex() { return m_endVertex; }

inline void LHCb::Particle::setEndVertex( const SmartRef<LHCb::Vertex>& value ) { m_endVertex = value; }

inline void LHCb::Particle::setEndVertex( const LHCb::Vertex* value ) { m_endVertex = value; }

inline const LHCb::ProtoParticle* LHCb::Particle::proto() const { return m_proto; }

inline void LHCb::Particle::setProto( const SmartRef<LHCb::ProtoParticle>& value ) { m_proto = value; }

inline void LHCb::Particle::setProto( const LHCb::ProtoParticle* value ) { m_proto = value; }

inline const SmartRefVector<LHCb::Particle>& LHCb::Particle::daughters() const { return m_daughters; }

inline void LHCb::Particle::setDaughters( const SmartRefVector<LHCb::Particle>& value ) { m_daughters = value; }

inline void LHCb::Particle::addToDaughters( const SmartRef<LHCb::Particle>& value ) { m_daughters.push_back( value ); }

inline void LHCb::Particle::addToDaughters( const LHCb::Particle* value ) { m_daughters.push_back( value ); }

inline void LHCb::Particle::removeFromDaughters( const SmartRef<LHCb::Particle>& value ) {
  auto i = std::remove( m_daughters.begin(), m_daughters.end(), value );
  m_daughters.erase( i, m_daughters.end() );
}

inline void LHCb::Particle::clearDaughters() { m_daughters.clear(); }

inline LHCb::Particle* LHCb::Particle::clone() const { return new Particle( *this ); }

inline LHCb::Particle& LHCb::Particle::operator=( const Particle& rhs ) {

  if ( this != &rhs ) {
    m_particleID      = rhs.m_particleID;
    m_momentum        = rhs.m_momentum;
    m_referencePoint  = rhs.m_referencePoint;
    m_measuredMass    = rhs.m_measuredMass;
    m_measuredMassErr = rhs.m_measuredMassErr;
    m_posCovMatrix    = rhs.m_posCovMatrix;
    m_momCovMatrix    = rhs.m_momCovMatrix;
    m_posMomCovMatrix = rhs.m_posMomCovMatrix;
    m_extraInfo       = rhs.m_extraInfo;
    m_endVertex       = rhs.m_endVertex;
    m_proto           = rhs.m_proto;
    m_daughters       = rhs.m_daughters;
  }
  return *this;
}

inline double LHCb::Particle::confLevel() const { return info( LHCb::Particle::additionalInfo::ConfLevel, -1. ); }

inline void LHCb::Particle::setConfLevel( double cl ) { addInfo( LHCb::Particle::additionalInfo::ConfLevel, cl ); }

inline double LHCb::Particle::weight() const { return info( LHCb::Particle::additionalInfo::Weight, 1. ); }

inline void LHCb::Particle::setWeight( double w ) { addInfo( LHCb::Particle::additionalInfo::Weight, w ); }

inline int LHCb::Particle::charge() const { return particleID().threeCharge() / 3; }

inline double LHCb::Particle::pt() const { return m_momentum.Pt(); }

inline double LHCb::Particle::p() const { return m_momentum.R(); }

inline bool LHCb::Particle::isBasicParticle() const { return m_daughters.empty(); }

inline LHCb::Particle::ConstVector LHCb::Particle::daughtersVector() const {

  return LHCb::Particle::ConstVector( m_daughters.begin(), m_daughters.end() );
}

inline bool LHCb::Particle::hasInfo( int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

inline bool LHCb::Particle::addInfo( int key, double info ) { return m_extraInfo.insert( key, info ).second; }

inline double LHCb::Particle::info( int key, double def ) const {

  ExtraInfo::const_iterator i = m_extraInfo.find( key );
  return m_extraInfo.end() == i ? def : i->second;
}

inline unsigned long LHCb::Particle::eraseInfo( int key ) { return m_extraInfo.erase( key ); }

inline Gaudi::XYZVector LHCb::Particle::slopes() const {

  return Gaudi::XYZVector( fabs( m_momentum.Z() ) > 0 ? m_momentum.X() / m_momentum.Z() : 0,
                           fabs( m_momentum.Z() ) > 0 ? m_momentum.Y() / m_momentum.Z() : 0, 1.0 );
}
