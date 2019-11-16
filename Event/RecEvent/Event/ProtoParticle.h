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
#include "Event/CaloHypo.h"
#include "Event/Track.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class ParticleID;
  class RichPID;
  class MuonPID;

  // Class ID definition
  static const CLID CLID_ProtoParticle = 803;

  // Namespace for locations in TDS
  namespace ProtoParticleLocation {
    inline const std::string Charged     = "Rec/ProtoP/Charged";
    inline const std::string Upstream    = "Rec/ProtoP/Upstream";
    inline const std::string Neutrals    = "Rec/ProtoP/Neutrals";
    inline const std::string HltCharged  = "Hlt/ProtoP/Charged";
    inline const std::string HltNeutrals = "Hlt/ProtoP/Neutrals";
  } // namespace ProtoParticleLocation

  /** @class ProtoParticle ProtoParticle.h
   *
   * ProtoParticle, precursor of Particles. It holds information relative to
   * final/global reconstruction, mostly pointers to all elements used to make it
   *
   * @author Patrick Koppenburg, Chris Jones
   *
   */

  class ProtoParticle final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of ProtoParticle
    using Vector      = std::vector<ProtoParticle*>;
    using ConstVector = std::vector<const ProtoParticle*>;

    /// typedef for KeyedContainer of ProtoParticle
    typedef KeyedContainer<ProtoParticle, Containers::HashMap> Container;

    /// Vector of additional information
    typedef GaudiUtils::VectorMap<int, double> ExtraInfo;

    /// Additional information assigned to this ProtoParticle. A combination of the PID information from the RICH, CALO,
    /// MUON and VELO systems together with any additional user information
    enum additionalInfo {
      NoPID                = 0,   // Undefined PID Variable
      RichDLLe             = 100, // The RICH delta log likelihood value for the electron hypothesis
      RichDLLmu            = 101, // The RICH delta log likelihood value for the muon hypothesis
      RichDLLpi            = 102, // The RICH delta log likelihood value for the pion hypothesis
      RichDLLk             = 103, // The RICH delta log likelihood value for the kaon hypothesis
      RichDLLp             = 104, // The RICH delta log likelihood value for the proton hypothesis
      RichPIDStatus        = 105, // The RichPID bit packed PID status word
      RichDLLbt            = 106, // The RICH delta log likelihood value for the 'Below Threshold' hypothesis
      RichDLLd             = 107, // The RICH delta log likelihood value for the deuteron hypothesis
      MuonMuLL             = 200, // The log likelihood from the MUON system for the ProtoParticle to be a muon
      MuonBkgLL            = 201, // The log likelihood from the MUON system for the ProtoParticle to be background
      MuonNShared          = 202, // The number of MUON hits that are shared with other ProtoParticles
      MuonPIDStatus        = 203, // The MuonPID bit packed PID status word
      InAccMuon            = 204, // Acceptance flag for MUON system
      MuonChi2Corr         = 205, // The chi2 using the correlation of the hits
      MuonMVA1             = 206, // MVA for the muonID with tune 1
      MuonMVA2             = 207, // MVA for the muonID with tune 2
      MuonMVA3             = 208, // MVA for the muonID with tune 3
      MuonMVA4             = 209, // MVA for the muonID with tune 4
      InAccSpd             = 300, // Acceptance flag for Spd-based  CaloID (charged)
      InAccPrs             = 301, // Acceptance flag for Prs-based  CaloID (charged)
      InAccEcal            = 302, // Acceptance flag for Ecal-based CaloID (charged)
      InAccHcal            = 303, // Acceptance flag for Hcal-based CaloID (charged)
      InAccBrem            = 304, // Acceptance flag for Brem-based CaloID (charged)
      CaloTrMatch          = 310, // CaloID estimator : 2D chi2 for Track/CaloCluster matching (neutral + charged)
      CaloElectronMatch    = 311, // CaloID estimator : 3D chi2 for Track/CaloHypo(e) matching (charged)
      CaloBremMatch        = 312, // CaloID estimator : 3D chi2 for Track/CaloHypo(Brem) (charged)
      CaloChargedSpd       = 313, // Spd Digits (0,1) associated to CaloHypo (charged)
      CaloChargedPrs       = 314, // Prs Digits (0,sum) associated to CaloHypo (charged)
      CaloChargedEcal      = 315, // Cluster  energy associated to CaloHypo (charged)
      CaloChargedID        = 316, // CellID.all() associated to CaloHypo seed (neutral)
      CaloDepositID        = 320, // CaloID estimator : Spd/Prs deposit (neutral)
      ShowerShape          = 321, // CaloID estimator : 2nd order moment of the cluster (neutral)
      ClusterMass          = 322, // CaloID estimator : MergedPi0Alg cluster Mass (neutral)
      CaloNeutralSpd       = 323, // Spd Digits (0,N) associated to CaloHypo (neutral)
      CaloNeutralPrs       = 324, // Prs Digits (0,sum) associated to CaloHypo (neutral)
      CaloNeutralEcal      = 325, // Cluster energy associated to CaloHypo (neutral)
      CaloNeutralHcal2Ecal = 326, // Hcal/Ecal energy ratio associated to CaloHypo (neutral)
      CaloNeutralE49       = 327, // 2x2/3x3 energy ratio associated to CaloHypo (neutral)
      CaloNeutralID        = 328, // CellID.all() associated to CaloHypo seed (neutral)
      CaloSpdE             = 330, // CaloID estimator : Spd  hit (charged)
      CaloPrsE             = 331, // CaloID estimator : Prs  energy deposit (charged)
      CaloEcalE            = 332, // CaloID estimator : Ecal energy deposit (charged)
      CaloHcalE            = 333, // CaloID estimator : Hcal energy deposit (charged)
      CaloEcalChi2         = 334, // CaloID estimator : currently equivalent to CaloElectronMatch
      CaloBremChi2         = 335, // CaloID estimator : currently equivalent to CaloBremMatch
      CaloClusChi2         = 336, // CaloID estimator : currently equivalent to CaloTrMatch
      CaloTrajectoryL      = 337, // CaloID estimator : Longitudinal parameter of Ecal Shower
      CaloEoverP           = 338, // Electron e/p
      CaloNeutralE19       = 339, // seed/3x3 ecal energy ratio associated to CaloHypo - input to neutralID (neutral)
      CaloPrsNeutralE19    = 340, // Seed/3x3 prs energy ratio associated to CaloHypo - input to neutralID (neutral)
      CaloPrsNeutralE49    = 341, // 2x2/3x3 prs energy ratio associated to CaloHypo - input to neutralID  (neutral)
      CaloPrsNeutralE4max  = 342, // Max 2x2 prs energy  associated to CaloHypo - input to neutralID (neutral)
      CaloNeutralPrsM      = 343, // Prs multiplicity (0,N) associated to CaloHypo - input to neutralID (neutral)
      CaloShapeFr2r4       = 344, // Cluster Fr2r4 shape variable - input to gamma/pi0 separation (neutral)
      CaloShapeKappa       = 345, // Cluster Fkappa shape variable - input to gamma/pi0 separation (neutral)
      CaloShapeAsym        = 346, // Cluster Fasym shape variable - input to gamma/pi0 separation (neutral)
      CaloShapeE1          = 347, // Cluster E1/Ecl variable - input to gamma/pi0 separation (neutral)
      CaloShapeE2          = 348, // Cluster E2/Ecl variable - input to gamma/pi0 separation (neutral)
      CaloPrsShapeE2       = 349, // Cluster EPrs2/E variable - input to gamma/pi0 separation (neutral)
      CaloPrsShapeEmax     = 350, // Cluster max(EPrs)/E variable - input to gamma/pi0 separation (neutral)
      CaloPrsShapeFr2      = 351, // Cluster Fr2 shape variable - input to gamma/pi0 separation (neutral)
      CaloPrsShapeAsym     = 352, // Cluster Fasym shape variable - input to gamma/pi0 separation (neutral)
      CaloPrsM             = 353, // Prs mult (>0) shape variable - input to gamma/pi0 separation (neutral)
      CaloPrsM15           = 354, // Prs mult (>15) shape variable - input to gamma/pi0 separation (neutral)
      CaloPrsM30           = 355, // Prs mult (>30) shape variable - input to gamma/pi0 separation (neutral)
      CaloPrsM45           = 356, // Prs mult (>45) shape variable - input to gamma/pi0 separation (neutral)
      CaloClusterCode      = 357, // Cluster configuration code
      CaloClusterFrac      = 358, // Cluster energy fraction
      Saturation           = 359, // Number of saturated cells (neutral)
      EcalPIDe             = 360, // Ecal-based DLL for electron-ID (charged)
      PrsPIDe              = 361, // Prs-based  DLL for electron-ID (charged)
      BremPIDe             = 362, // Brem-based DLL for electron-ID (charged)
      HcalPIDe             = 363, // Hcal-based DLL for electron-ID (charged)
      HcalPIDmu            = 364, // Hcal-based DLL for muon-ID DLL (charged)
      EcalPIDmu            = 365, // Ecal-based DLL for muon-ID DLL (charged)
      PhotonID             = 380, // Combined PDF for photonID - OBSOLETE DLL-based neutralID (neutral)
      IsPhoton             = 381, // Gamma/Pi0 separation variable (neutral)
      IsNotE               = 382, // MLP-based neutralID - anti-electron ID (neutral)
      IsNotH               = 383, // MLP-based neutralID - anti-hadron ID   (neutral)
      IsPhotonXGB          = 384, // XGBoost-based Gamma/Pi0 separation (neutral)
      ClusterAsX           = 390, // Cluster Barycenter X
      ClusterAsY           = 391, // Cluster Barycenter Y
      VeloCharge           = 400, // The dE/dx charge from the VELO system
      TrackChi2PerDof      = 500, // The track fit Chi^2 per degree of freedom
      TrackNumDof          = 501, // Number of degrees of freedom in the track fit
      TrackType            = 502, // The track type flag
      TrackHistory         = 503, // The track history code
      TrackP               = 504, // The track momentum
      TrackPt              = 505, // The track transverse momentum
      CombDLLe             = 600, // The combined delta log likelihood value for the electron hypothesis
      CombDLLmu            = 601, // The combined delta log likelihood value for the muon hypothesis
      CombDLLpi            = 602, // The combined delta log likelihood value for the pion hypothesis
      CombDLLk             = 603, // The combined delta log likelihood value for the kaon hypothesis
      CombDLLp             = 604, // The combined delta log likelihood value for the proton hypothesis
      CombDLLd             = 605, // The combined delta log likelihood value for the deuteron hypothesis
      ProbNNe              = 700, // The ANN probability for the electron hypothesis
      ProbNNmu             = 701, // The ANN probability for the muon hypothesis
      ProbNNpi             = 702, // The ANN probability for the pion hypothesis
      ProbNNk              = 703, // The ANN probability for the kaon hypothesis
      ProbNNp              = 704, // The ANN probability for the proton hypothesis
      ProbNNghost          = 705, // The ANN probability for the ghost hypothesis
      ProbNNd              = 706, // The ANN probability for the deuteron hypothesis
      LastGlobal =
          10000 // The last official flag value. Private user variables should use flags greater than this value
    };

    /// Copy constructor. Creates a new ProtoParticle with the same physics information
    ProtoParticle( const LHCb::ProtoParticle& proto )
        : KeyedObject<int>()
        , m_extraInfo( proto.extraInfo() )
        , m_calo( proto.calo() )
        , m_track( proto.track() )
        , m_richPID( proto.richPID() )
        , m_muonPID( proto.muonPID() ) {}

    /// Default Constructor
    ProtoParticle() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type additionalInfo
    static LHCb::ProtoParticle::additionalInfo additionalInfoToType( const std::string& aName );

    /// conversion to string for enum type additionalInfo
    static const std::string& additionalInfoToString( int aEnum );

    /// Clone this ProtoParticle. Returns a pointer to a new ProtoParticle (user must take ownership)
    ProtoParticle* clone() const;

    /// Does this protoparticle have information for the specified key
    bool hasInfo( int key ) const;

    /// Add new information, associated with the specified key
    bool addInfo( int key, double info );

    /// Extract the information associated with the specified key. If there is no such information the default value
    /// will be returned.
    double info( int key, double def ) const;

    /// Erase the information associated with the specified key
    ProtoParticle::ExtraInfo::size_type eraseInfo( int key );

    /// Returns the measured particle charge
    int charge() const;

    /// Print this ProtoParticle in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Remove all Calo Hypo information of the given type
    ProtoParticle::ExtraInfo::size_type clearCalo( const LHCb::CaloHypo::Hypothesis& hypo );

    /// Remove all Combined DLL PID information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeCombinedInfo();

    /// Remove all RICH information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeRichInfo();

    /// Remove all MUON information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeMuonInfo();

    /// Remove all CALO-ECAL information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeCaloEcalInfo();

    /// Remove all CALO-BREM information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeCaloBremInfo();

    /// Remove all CALO-SPD information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeCaloSpdInfo();

    /// Remove all CALO-PRS information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeCaloPrsInfo();

    /// Remove all CALO-HCAL information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeCaloHcalInfo();

    /// Remove all VELO (dE/dx) information stored in this ProtoParticle
    ProtoParticle::ExtraInfo::size_type removeVeloInfo();

    /// Convert a string to an extra info enum
    static ProtoParticle::additionalInfo convertExtraInfo( const std::string& name );

    /// Retrieve const  Additional particle ID and user information. Do not access directly, use *Info() methods
    /// instead.
    const ExtraInfo& extraInfo() const;

    /// Retrieve (const)  References to possible calorimeter results
    const SmartRefVector<LHCb::CaloHypo>& calo() const;

    /// Update  References to possible calorimeter results
    void setCalo( const SmartRefVector<LHCb::CaloHypo>& value );

    /// Add to  References to possible calorimeter results
    void addToCalo( const SmartRef<LHCb::CaloHypo>& value );

    /// Att to (pointer)  References to possible calorimeter results
    void addToCalo( const LHCb::CaloHypo* value );

    /// Remove from  References to possible calorimeter results
    void removeFromCalo( const SmartRef<LHCb::CaloHypo>& value );

    /// Clear  References to possible calorimeter results
    void clearCalo();

    /// Retrieve (const)  Reference to possible track used
    const LHCb::Track* track() const;

    /// Update  Reference to possible track used
    void setTrack( const SmartRef<LHCb::Track>& value );

    /// Update (pointer)  Reference to possible track used
    void setTrack( const LHCb::Track* value );

    /// Retrieve (const)  Reference to possible RichPID result
    const LHCb::RichPID* richPID() const;

    /// Update  Reference to possible RichPID result
    void setRichPID( const SmartRef<LHCb::RichPID>& value );

    /// Update (pointer)  Reference to possible RichPID result
    void setRichPID( const LHCb::RichPID* value );

    /// Retrieve (const)  Reference to possible MuonPID result
    const LHCb::MuonPID* muonPID() const;

    /// Update  Reference to possible MuonPID result
    void setMuonPID( const SmartRef<LHCb::MuonPID>& value );

    /// Update (pointer)  Reference to possible MuonPID result
    void setMuonPID( const LHCb::MuonPID* value );

    friend std::ostream& operator<<( std::ostream& str, const ProtoParticle& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    ExtraInfo m_extraInfo; ///< Additional particle ID and user information. Do not access directly, use *Info() methods
                           ///< instead.
    SmartRefVector<LHCb::CaloHypo> m_calo;    ///< References to possible calorimeter results
    SmartRef<LHCb::Track>          m_track;   ///< Reference to possible track used
    SmartRef<LHCb::RichPID>        m_richPID; ///< Reference to possible RichPID result
    SmartRef<LHCb::MuonPID>        m_muonPID; ///< Reference to possible MuonPID result

    static const GaudiUtils::VectorMap<std::string, additionalInfo>& s_additionalInfoTypMap();

  }; // class ProtoParticle

  /// Definition of Keyed Container for ProtoParticle
  typedef KeyedContainer<ProtoParticle, Containers::HashMap> ProtoParticles;

  inline std::ostream& operator<<( std::ostream& s, LHCb::ProtoParticle::additionalInfo e ) {
    switch ( e ) {
    case LHCb::ProtoParticle::NoPID:
      return s << "NoPID";
    case LHCb::ProtoParticle::RichDLLe:
      return s << "RichDLLe";
    case LHCb::ProtoParticle::RichDLLmu:
      return s << "RichDLLmu";
    case LHCb::ProtoParticle::RichDLLpi:
      return s << "RichDLLpi";
    case LHCb::ProtoParticle::RichDLLk:
      return s << "RichDLLk";
    case LHCb::ProtoParticle::RichDLLp:
      return s << "RichDLLp";
    case LHCb::ProtoParticle::RichPIDStatus:
      return s << "RichPIDStatus";
    case LHCb::ProtoParticle::RichDLLbt:
      return s << "RichDLLbt";
    case LHCb::ProtoParticle::RichDLLd:
      return s << "RichDLLd";
    case LHCb::ProtoParticle::MuonMuLL:
      return s << "MuonMuLL";
    case LHCb::ProtoParticle::MuonBkgLL:
      return s << "MuonBkgLL";
    case LHCb::ProtoParticle::MuonNShared:
      return s << "MuonNShared";
    case LHCb::ProtoParticle::MuonPIDStatus:
      return s << "MuonPIDStatus";
    case LHCb::ProtoParticle::InAccMuon:
      return s << "InAccMuon";
    case LHCb::ProtoParticle::MuonChi2Corr:
      return s << "MuonChi2Corr";
    case LHCb::ProtoParticle::MuonMVA1:
      return s << "MuonMVA1";
    case LHCb::ProtoParticle::MuonMVA2:
      return s << "MuonMVA2";
    case LHCb::ProtoParticle::MuonMVA3:
      return s << "MuonMVA3";
    case LHCb::ProtoParticle::MuonMVA4:
      return s << "MuonMVA4";
    case LHCb::ProtoParticle::InAccSpd:
      return s << "InAccSpd";
    case LHCb::ProtoParticle::InAccPrs:
      return s << "InAccPrs";
    case LHCb::ProtoParticle::InAccEcal:
      return s << "InAccEcal";
    case LHCb::ProtoParticle::InAccHcal:
      return s << "InAccHcal";
    case LHCb::ProtoParticle::InAccBrem:
      return s << "InAccBrem";
    case LHCb::ProtoParticle::CaloTrMatch:
      return s << "CaloTrMatch";
    case LHCb::ProtoParticle::CaloElectronMatch:
      return s << "CaloElectronMatch";
    case LHCb::ProtoParticle::CaloBremMatch:
      return s << "CaloBremMatch";
    case LHCb::ProtoParticle::CaloChargedSpd:
      return s << "CaloChargedSpd";
    case LHCb::ProtoParticle::CaloChargedPrs:
      return s << "CaloChargedPrs";
    case LHCb::ProtoParticle::CaloChargedEcal:
      return s << "CaloChargedEcal";
    case LHCb::ProtoParticle::CaloChargedID:
      return s << "CaloChargedID";
    case LHCb::ProtoParticle::CaloDepositID:
      return s << "CaloDepositID";
    case LHCb::ProtoParticle::ShowerShape:
      return s << "ShowerShape";
    case LHCb::ProtoParticle::ClusterMass:
      return s << "ClusterMass";
    case LHCb::ProtoParticle::CaloNeutralSpd:
      return s << "CaloNeutralSpd";
    case LHCb::ProtoParticle::CaloNeutralPrs:
      return s << "CaloNeutralPrs";
    case LHCb::ProtoParticle::CaloNeutralEcal:
      return s << "CaloNeutralEcal";
    case LHCb::ProtoParticle::CaloNeutralHcal2Ecal:
      return s << "CaloNeutralHcal2Ecal";
    case LHCb::ProtoParticle::CaloNeutralE49:
      return s << "CaloNeutralE49";
    case LHCb::ProtoParticle::CaloNeutralID:
      return s << "CaloNeutralID";
    case LHCb::ProtoParticle::CaloSpdE:
      return s << "CaloSpdE";
    case LHCb::ProtoParticle::CaloPrsE:
      return s << "CaloPrsE";
    case LHCb::ProtoParticle::CaloEcalE:
      return s << "CaloEcalE";
    case LHCb::ProtoParticle::CaloHcalE:
      return s << "CaloHcalE";
    case LHCb::ProtoParticle::CaloEcalChi2:
      return s << "CaloEcalChi2";
    case LHCb::ProtoParticle::CaloBremChi2:
      return s << "CaloBremChi2";
    case LHCb::ProtoParticle::CaloClusChi2:
      return s << "CaloClusChi2";
    case LHCb::ProtoParticle::CaloTrajectoryL:
      return s << "CaloTrajectoryL";
    case LHCb::ProtoParticle::CaloEoverP:
      return s << "CaloEoverP";
    case LHCb::ProtoParticle::CaloNeutralE19:
      return s << "CaloNeutralE19";
    case LHCb::ProtoParticle::CaloPrsNeutralE19:
      return s << "CaloPrsNeutralE19";
    case LHCb::ProtoParticle::CaloPrsNeutralE49:
      return s << "CaloPrsNeutralE49";
    case LHCb::ProtoParticle::CaloPrsNeutralE4max:
      return s << "CaloPrsNeutralE4max";
    case LHCb::ProtoParticle::CaloNeutralPrsM:
      return s << "CaloNeutralPrsM";
    case LHCb::ProtoParticle::CaloShapeFr2r4:
      return s << "CaloShapeFr2r4";
    case LHCb::ProtoParticle::CaloShapeKappa:
      return s << "CaloShapeKappa";
    case LHCb::ProtoParticle::CaloShapeAsym:
      return s << "CaloShapeAsym";
    case LHCb::ProtoParticle::CaloShapeE1:
      return s << "CaloShapeE1";
    case LHCb::ProtoParticle::CaloShapeE2:
      return s << "CaloShapeE2";
    case LHCb::ProtoParticle::CaloPrsShapeE2:
      return s << "CaloPrsShapeE2";
    case LHCb::ProtoParticle::CaloPrsShapeEmax:
      return s << "CaloPrsShapeEmax";
    case LHCb::ProtoParticle::CaloPrsShapeFr2:
      return s << "CaloPrsShapeFr2";
    case LHCb::ProtoParticle::CaloPrsShapeAsym:
      return s << "CaloPrsShapeAsym";
    case LHCb::ProtoParticle::CaloPrsM:
      return s << "CaloPrsM";
    case LHCb::ProtoParticle::CaloPrsM15:
      return s << "CaloPrsM15";
    case LHCb::ProtoParticle::CaloPrsM30:
      return s << "CaloPrsM30";
    case LHCb::ProtoParticle::CaloPrsM45:
      return s << "CaloPrsM45";
    case LHCb::ProtoParticle::CaloClusterCode:
      return s << "CaloClusterCode";
    case LHCb::ProtoParticle::CaloClusterFrac:
      return s << "CaloClusterFrac";
    case LHCb::ProtoParticle::Saturation:
      return s << "Saturation";
    case LHCb::ProtoParticle::EcalPIDe:
      return s << "EcalPIDe";
    case LHCb::ProtoParticle::PrsPIDe:
      return s << "PrsPIDe";
    case LHCb::ProtoParticle::BremPIDe:
      return s << "BremPIDe";
    case LHCb::ProtoParticle::HcalPIDe:
      return s << "HcalPIDe";
    case LHCb::ProtoParticle::HcalPIDmu:
      return s << "HcalPIDmu";
    case LHCb::ProtoParticle::EcalPIDmu:
      return s << "EcalPIDmu";
    case LHCb::ProtoParticle::PhotonID:
      return s << "PhotonID";
    case LHCb::ProtoParticle::IsPhoton:
      return s << "IsPhoton";
    case LHCb::ProtoParticle::IsNotE:
      return s << "IsNotE";
    case LHCb::ProtoParticle::IsNotH:
      return s << "IsNotH";
    case LHCb::ProtoParticle::IsPhotonXGB:
      return s << "IsPhotonXGB";
    case LHCb::ProtoParticle::ClusterAsX:
      return s << "ClusterAsX";
    case LHCb::ProtoParticle::ClusterAsY:
      return s << "ClusterAsY";
    case LHCb::ProtoParticle::VeloCharge:
      return s << "VeloCharge";
    case LHCb::ProtoParticle::TrackChi2PerDof:
      return s << "TrackChi2PerDof";
    case LHCb::ProtoParticle::TrackNumDof:
      return s << "TrackNumDof";
    case LHCb::ProtoParticle::TrackType:
      return s << "TrackType";
    case LHCb::ProtoParticle::TrackHistory:
      return s << "TrackHistory";
    case LHCb::ProtoParticle::TrackP:
      return s << "TrackP";
    case LHCb::ProtoParticle::TrackPt:
      return s << "TrackPt";
    case LHCb::ProtoParticle::CombDLLe:
      return s << "CombDLLe";
    case LHCb::ProtoParticle::CombDLLmu:
      return s << "CombDLLmu";
    case LHCb::ProtoParticle::CombDLLpi:
      return s << "CombDLLpi";
    case LHCb::ProtoParticle::CombDLLk:
      return s << "CombDLLk";
    case LHCb::ProtoParticle::CombDLLp:
      return s << "CombDLLp";
    case LHCb::ProtoParticle::CombDLLd:
      return s << "CombDLLd";
    case LHCb::ProtoParticle::ProbNNe:
      return s << "ProbNNe";
    case LHCb::ProtoParticle::ProbNNmu:
      return s << "ProbNNmu";
    case LHCb::ProtoParticle::ProbNNpi:
      return s << "ProbNNpi";
    case LHCb::ProtoParticle::ProbNNk:
      return s << "ProbNNk";
    case LHCb::ProtoParticle::ProbNNp:
      return s << "ProbNNp";
    case LHCb::ProtoParticle::ProbNNghost:
      return s << "ProbNNghost";
    case LHCb::ProtoParticle::ProbNNd:
      return s << "ProbNNd";
    case LHCb::ProtoParticle::LastGlobal:
      return s << "LastGlobal";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ProtoParticle::additionalInfo";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "Event/MuonPID.h"
#include "Event/RichPID.h"
#include "Kernel/ParticleID.h"

inline const CLID& LHCb::ProtoParticle::clID() const { return LHCb::ProtoParticle::classID(); }

inline const CLID& LHCb::ProtoParticle::classID() { return CLID_ProtoParticle; }

inline const GaudiUtils::VectorMap<std::string, LHCb::ProtoParticle::additionalInfo>&
LHCb::ProtoParticle::s_additionalInfoTypMap() {
  static const GaudiUtils::VectorMap<std::string, additionalInfo> m = {{"NoPID", NoPID},
                                                                       {"RichDLLe", RichDLLe},
                                                                       {"RichDLLmu", RichDLLmu},
                                                                       {"RichDLLpi", RichDLLpi},
                                                                       {"RichDLLk", RichDLLk},
                                                                       {"RichDLLp", RichDLLp},
                                                                       {"RichPIDStatus", RichPIDStatus},
                                                                       {"RichDLLbt", RichDLLbt},
                                                                       {"RichDLLd", RichDLLd},
                                                                       {"MuonMuLL", MuonMuLL},
                                                                       {"MuonBkgLL", MuonBkgLL},
                                                                       {"MuonNShared", MuonNShared},
                                                                       {"MuonPIDStatus", MuonPIDStatus},
                                                                       {"InAccMuon", InAccMuon},
                                                                       {"MuonChi2Corr", MuonChi2Corr},
                                                                       {"MuonMVA1", MuonMVA1},
                                                                       {"MuonMVA2", MuonMVA2},
                                                                       {"MuonMVA3", MuonMVA3},
                                                                       {"MuonMVA4", MuonMVA4},
                                                                       {"InAccSpd", InAccSpd},
                                                                       {"InAccPrs", InAccPrs},
                                                                       {"InAccEcal", InAccEcal},
                                                                       {"InAccHcal", InAccHcal},
                                                                       {"InAccBrem", InAccBrem},
                                                                       {"CaloTrMatch", CaloTrMatch},
                                                                       {"CaloElectronMatch", CaloElectronMatch},
                                                                       {"CaloBremMatch", CaloBremMatch},
                                                                       {"CaloChargedSpd", CaloChargedSpd},
                                                                       {"CaloChargedPrs", CaloChargedPrs},
                                                                       {"CaloChargedEcal", CaloChargedEcal},
                                                                       {"CaloChargedID", CaloChargedID},
                                                                       {"CaloDepositID", CaloDepositID},
                                                                       {"ShowerShape", ShowerShape},
                                                                       {"ClusterMass", ClusterMass},
                                                                       {"CaloNeutralSpd", CaloNeutralSpd},
                                                                       {"CaloNeutralPrs", CaloNeutralPrs},
                                                                       {"CaloNeutralEcal", CaloNeutralEcal},
                                                                       {"CaloNeutralHcal2Ecal", CaloNeutralHcal2Ecal},
                                                                       {"CaloNeutralE49", CaloNeutralE49},
                                                                       {"CaloNeutralID", CaloNeutralID},
                                                                       {"CaloSpdE", CaloSpdE},
                                                                       {"CaloPrsE", CaloPrsE},
                                                                       {"CaloEcalE", CaloEcalE},
                                                                       {"CaloHcalE", CaloHcalE},
                                                                       {"CaloEcalChi2", CaloEcalChi2},
                                                                       {"CaloBremChi2", CaloBremChi2},
                                                                       {"CaloClusChi2", CaloClusChi2},
                                                                       {"CaloTrajectoryL", CaloTrajectoryL},
                                                                       {"CaloEoverP", CaloEoverP},
                                                                       {"CaloNeutralE19", CaloNeutralE19},
                                                                       {"CaloPrsNeutralE19", CaloPrsNeutralE19},
                                                                       {"CaloPrsNeutralE49", CaloPrsNeutralE49},
                                                                       {"CaloPrsNeutralE4max", CaloPrsNeutralE4max},
                                                                       {"CaloNeutralPrsM", CaloNeutralPrsM},
                                                                       {"CaloShapeFr2r4", CaloShapeFr2r4},
                                                                       {"CaloShapeKappa", CaloShapeKappa},
                                                                       {"CaloShapeAsym", CaloShapeAsym},
                                                                       {"CaloShapeE1", CaloShapeE1},
                                                                       {"CaloShapeE2", CaloShapeE2},
                                                                       {"CaloPrsShapeE2", CaloPrsShapeE2},
                                                                       {"CaloPrsShapeEmax", CaloPrsShapeEmax},
                                                                       {"CaloPrsShapeFr2", CaloPrsShapeFr2},
                                                                       {"CaloPrsShapeAsym", CaloPrsShapeAsym},
                                                                       {"CaloPrsM", CaloPrsM},
                                                                       {"CaloPrsM15", CaloPrsM15},
                                                                       {"CaloPrsM30", CaloPrsM30},
                                                                       {"CaloPrsM45", CaloPrsM45},
                                                                       {"CaloClusterCode", CaloClusterCode},
                                                                       {"CaloClusterFrac", CaloClusterFrac},
                                                                       {"Saturation", Saturation},
                                                                       {"EcalPIDe", EcalPIDe},
                                                                       {"PrsPIDe", PrsPIDe},
                                                                       {"BremPIDe", BremPIDe},
                                                                       {"HcalPIDe", HcalPIDe},
                                                                       {"HcalPIDmu", HcalPIDmu},
                                                                       {"EcalPIDmu", EcalPIDmu},
                                                                       {"PhotonID", PhotonID},
                                                                       {"IsPhoton", IsPhoton},
                                                                       {"IsNotE", IsNotE},
                                                                       {"IsNotH", IsNotH},
                                                                       {"IsPhotonXGB", IsPhotonXGB},
                                                                       {"ClusterAsX", ClusterAsX},
                                                                       {"ClusterAsY", ClusterAsY},
                                                                       {"VeloCharge", VeloCharge},
                                                                       {"TrackChi2PerDof", TrackChi2PerDof},
                                                                       {"TrackNumDof", TrackNumDof},
                                                                       {"TrackType", TrackType},
                                                                       {"TrackHistory", TrackHistory},
                                                                       {"TrackP", TrackP},
                                                                       {"TrackPt", TrackPt},
                                                                       {"CombDLLe", CombDLLe},
                                                                       {"CombDLLmu", CombDLLmu},
                                                                       {"CombDLLpi", CombDLLpi},
                                                                       {"CombDLLk", CombDLLk},
                                                                       {"CombDLLp", CombDLLp},
                                                                       {"CombDLLd", CombDLLd},
                                                                       {"ProbNNe", ProbNNe},
                                                                       {"ProbNNmu", ProbNNmu},
                                                                       {"ProbNNpi", ProbNNpi},
                                                                       {"ProbNNk", ProbNNk},
                                                                       {"ProbNNp", ProbNNp},
                                                                       {"ProbNNghost", ProbNNghost},
                                                                       {"ProbNNd", ProbNNd},
                                                                       {"LastGlobal", LastGlobal}};
  return m;
}

inline LHCb::ProtoParticle::additionalInfo LHCb::ProtoParticle::additionalInfoToType( const std::string& aName ) {
  auto iter = s_additionalInfoTypMap().find( aName );
  return iter != s_additionalInfoTypMap().end() ? iter->second : LastGlobal;
}

inline const std::string& LHCb::ProtoParticle::additionalInfoToString( int aEnum ) {
  static const std::string s_LastGlobal = "LastGlobal";
  auto                     iter =
      std::find_if( s_additionalInfoTypMap().begin(), s_additionalInfoTypMap().end(),
                    [&]( const std::pair<const std::string, additionalInfo>& i ) { return i.second == aEnum; } );
  return iter != s_additionalInfoTypMap().end() ? iter->first : s_LastGlobal;
}

inline const LHCb::ProtoParticle::ExtraInfo& LHCb::ProtoParticle::extraInfo() const { return m_extraInfo; }

inline const SmartRefVector<LHCb::CaloHypo>& LHCb::ProtoParticle::calo() const { return m_calo; }

inline void LHCb::ProtoParticle::setCalo( const SmartRefVector<LHCb::CaloHypo>& value ) { m_calo = value; }

inline void LHCb::ProtoParticle::addToCalo( const SmartRef<LHCb::CaloHypo>& value ) { m_calo.push_back( value ); }

inline void LHCb::ProtoParticle::addToCalo( const LHCb::CaloHypo* value ) { m_calo.push_back( value ); }

inline void LHCb::ProtoParticle::removeFromCalo( const SmartRef<LHCb::CaloHypo>& value ) {
  auto i = std::remove( m_calo.begin(), m_calo.end(), value );
  m_calo.erase( i, m_calo.end() );
}

inline void LHCb::ProtoParticle::clearCalo() { m_calo.clear(); }

inline const LHCb::Track* LHCb::ProtoParticle::track() const { return m_track; }

inline void LHCb::ProtoParticle::setTrack( const SmartRef<LHCb::Track>& value ) { m_track = value; }

inline void LHCb::ProtoParticle::setTrack( const LHCb::Track* value ) { m_track = value; }

inline const LHCb::RichPID* LHCb::ProtoParticle::richPID() const { return m_richPID; }

inline void LHCb::ProtoParticle::setRichPID( const SmartRef<LHCb::RichPID>& value ) { m_richPID = value; }

inline void LHCb::ProtoParticle::setRichPID( const LHCb::RichPID* value ) { m_richPID = value; }

inline const LHCb::MuonPID* LHCb::ProtoParticle::muonPID() const { return m_muonPID; }

inline void LHCb::ProtoParticle::setMuonPID( const SmartRef<LHCb::MuonPID>& value ) { m_muonPID = value; }

inline void LHCb::ProtoParticle::setMuonPID( const LHCb::MuonPID* value ) { m_muonPID = value; }

inline LHCb::ProtoParticle* LHCb::ProtoParticle::clone() const { return new LHCb::ProtoParticle( *this ); }

inline bool LHCb::ProtoParticle::hasInfo( const int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

inline bool LHCb::ProtoParticle::addInfo( const int key, const double info ) {

  return m_extraInfo.insert( key, info ).second;
}

inline double LHCb::ProtoParticle::info( const int key, const double def ) const {

  auto i = m_extraInfo.find( key );
  return m_extraInfo.end() == i ? def : i->second;
}

inline LHCb::ProtoParticle::ExtraInfo::size_type LHCb::ProtoParticle::eraseInfo( const int key ) {

  return m_extraInfo.erase( key );
}

inline int LHCb::ProtoParticle::charge() const { return ( track() != nullptr ? track()->charge() : 0 ); }
