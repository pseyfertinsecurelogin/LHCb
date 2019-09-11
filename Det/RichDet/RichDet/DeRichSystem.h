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

/** @file DeRichSystem.h
 *
 *  Header file for detector description class : DeRichSystem
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2006-01-26
 */

#pragma once

// STL
#include <algorithm>
#include <map>
#include <unordered_map>

// base class
#include "RichDet/DeRichBase.h"

// LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"
#include "Kernel/RichSmartID.h"

// RichUtils
#include "RichUtils/RichDAQDefinitions.h"
#include "RichUtils/RichMap.h"

// local
#include "RichDet/DeRich.h"
#include "RichDet/DeRichPDPanel.h"
#include "RichDet/RichDetConfigType.h"
class DeRichPD;

// External declarations
extern const CLID CLID_DERichSystem;

/** @class DeRichSystem RichDet/DeRichSystem.h
 *
 * Class for generic info about the Rich system. In particular
 * to provide conversions and mappings between the various
 * RICH numbering schemes and to provide data on which PDs, Level0 and Level1
 * boards are currently active.
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date   27/01/2006
 */
class DeRichSystem : public DeRichBase {

public:
  /// Constructor for this class
  DeRichSystem( const std::string& name = "" ) : DeRichBase( name ) {}

  /// Default destructor
  virtual ~DeRichSystem() = default;

  /** Retrieves reference to class identifier
   *  @return the class identifier for this class
   */
  const CLID& clID() const override { return classID(); }

  /** Retrieves reference to class identifier
   *  @return the class identifier for this class
   */
  static const CLID& classID();

  /** This is where most of the geometry is read and variables initialised
   *  @return Status of initialisation
   *  @retval StatusCode::FAILURE Initialisation failed, program should
   *  terminate
   */
  StatusCode initialize() override;

public:
  /** Convert an PD RichSmartID into the corresponding PD hardware number
   *  @param smartID The RichSmartID for the PD
   *  @return The corresponding PD hardware ID
   */
  const Rich::DAQ::PDHardwareID hardwareID( const LHCb::RichSmartID& smartID ) const;

  /** Convert a RICH PD hardware number into the corresponding PD RichSmartID
   *  @param hID The hardware ID for the PD
   *  @return The corresponding PD RichSmartID
   */
  const LHCb::RichSmartID richSmartID( const Rich::DAQ::PDHardwareID& hID ) const;

  /** Convert a RICH Level0 hardware number into the corresponding PD RichSmartID
   *  @param l0ID The PD level0 for the PD
   *  @return The corresponding PD RichSmartID
   */
  const LHCb::RichSmartID richSmartID( const Rich::DAQ::Level0ID& l0ID ) const;

  /** Convert a RICH Copy Number into the corresponding PD RichSmartID
   *  @param copyNumber The PD Copy Number
   *  @return The corresponding PD RichSmartID
   */
  const LHCb::RichSmartID richSmartID( const Rich::DAQ::PDCopyNumber& copyNumber ) const;

  /** Convert a RICH smartID to the corresponding PD Copy Number
   *  @param smartID The PD RichSmartID
   *  @return The corresponding PD Copy Number
   */
  const Rich::DAQ::PDCopyNumber copyNumber( const LHCb::RichSmartID& smartID ) const;

  /// Returns a list of all active PDs identified by their RichSmartID
  const LHCb::RichSmartID::Vector& activePDRichSmartIDs() const noexcept;

  /// Returns a list of all inactive PDs identified by their RichSmartID
  const LHCb::RichSmartID::Vector& inactivePDRichSmartIDs() const noexcept;

  /// Returns a list of all (active and inactive) PDs identified by their RichSmartID
  const LHCb::RichSmartID::Vector& allPDRichSmartIDs() const noexcept;

  /// Returns a list of all active PDs identified by their hardware IDs
  const Rich::DAQ::PDHardwareIDs& activePDHardwareIDs() const noexcept;

  /// Returns a list of all inactive PDs identified by their hardware IDs
  const Rich::DAQ::PDHardwareIDs& inactivePDHardwareIDs() const noexcept;

  /// Returns a list of all (active and inactive) PDs identified by their hardware IDs
  const Rich::DAQ::PDHardwareIDs& allPDHardwareIDs() const noexcept;

  /** Ask whether a given PD is currently active or dead
   *  @param id The RichSmartID for the PD
   *  @return boolean indicating if the given PD is active or not
   *  @attention For speed, this method does NOT check if the given PD id is a valid one
   *             or not. Invalid PD ids will return true
   */
  bool pdIsActive( const LHCb::RichSmartID& id ) const;

  /** Ask whether a given PD is currently active or dead
   *  @param id The hardware id for the PD
   *  @return boolean indicating if the given PD is active or not
   *  @attention For speed, this method does NOT check if the given PD id is a valid one
   *             or not. Invalid PD ids will return true
   */
  bool pdIsActive( const Rich::DAQ::PDHardwareID& id ) const;

  /** Obtain the Level0 ID number for a given PD RichSmartID
   *  @param smartID The RichSmartID for the PD
   *  @return The corresponding PD Level0 ID
   */
  const Rich::DAQ::Level0ID level0ID( const LHCb::RichSmartID& smartID ) const;

  /** Obtain the Level0 ID number for a given PD hardware ID
   *  @param hardID The hardware ID for the PD
   *  @return The corresponding PD Level0 ID
   */
  const Rich::DAQ::Level0ID level0ID( const Rich::DAQ::PDHardwareID& hardID ) const;

  /** Obtain the Level1 hardware ID number for a given PD Level0 ID
   *  @param l0ID The Level0 ID
   *  @return The corresponding Level1 hardware ID
   */
  const Rich::DAQ::Level1HardwareID level1HardwareID( const Rich::DAQ::Level0ID& l0ID ) const;

  /** Obtain the Level1 hardware ID number for a given PD RichSmartID
   *  @param smartID The RichSmartID for the PD
   *  @return The corresponding Level1 hardware ID
   */
  const Rich::DAQ::Level1HardwareID level1HardwareID( const LHCb::RichSmartID& smartID ) const;

  /** Obtain the Level1 hardware ID number for a given PD hardware ID
   *  @param hardID The hardware ID for the PD
   *  @return The corresponding Level1 hardware ID
   */
  const Rich::DAQ::Level1HardwareID level1HardwareID( const Rich::DAQ::PDHardwareID& hardID ) const;

  /** Obtain the Level1 hardware ID number for a Level1 logical ID
   *  @param rich The RICH detector
   *  @param logID The logical ID for the PD
   *  @return The corresponding Level1 hardware ID
   */
  const Rich::DAQ::Level1HardwareID level1HardwareID( const Rich::DetectorType          rich,
                                                      const Rich::DAQ::Level1LogicalID& logID ) const;

  /** Obtain the Level1 logical ID number for a given PD RichSmartID
   *  @param smartID The RichSmartID for the PD
   *  @return The corresponding Level1 logical ID
   */
  const Rich::DAQ::Level1LogicalID level1LogicalID( const LHCb::RichSmartID& smartID ) const;

  /** Obtain the Level1 logical ID number for a Level1 hardware ID
   *  @param hardID The hardware ID for the PD
   *  @return The corresponding PD Level1 logical ID
   */
  const Rich::DAQ::Level1LogicalID level1LogicalID( const Rich::DAQ::Level1HardwareID& hardID ) const;

  /** Obtain the Level1 logical ID number for a Level1 hardware ID
   *  @param hardID The hardware ID for the PD
   *  @return The corresponding PD Level1 logical ID
   */
  const Rich::DAQ::Level1CopyNumber copyNumber( const Rich::DAQ::Level1HardwareID& hardID ) const;

  /** Obtain the Level1 input number for a given PD RichSmartID
   *  @param smartID The RichSmartID for the PD
   *  @return The corrresponding Level1 input number
   */
  const Rich::DAQ::HPD::Level1Input level1InputNum( const LHCb::RichSmartID& smartID ) const;

  /** Obtain the Level1 input number for a given PD hardware ID
   *  @param hardID The hardware ID for the PD
   *  @return The corrresponding Level1 input number
   */
  const Rich::DAQ::HPD::Level1Input level1InputNum( const Rich::DAQ::PDHardwareID& hardID ) const;

  /** Access the PD hardware ID for the given L1 hardwareID and input number
   *  @param L1HardID L1 board hardware ID
   *  @param L1Input  L1 input number
   *  @return PD hardware ID
   */
  const Rich::DAQ::PDHardwareID pdHardwareID( const Rich::DAQ::Level1HardwareID& L1HardID,
                                              const Rich::DAQ::HPD::Level1Input& L1Input ) const;

  /** Obtain a list of RichSmartID PD identifiers for a given level1 hardwareID
   *  @param l1ID The level1 ID number
   *  @return Vector of all PD RichSmartIDs for that Level1 board
   */
  const LHCb::RichSmartID::Vector& l1PDSmartIDs( const Rich::DAQ::Level1HardwareID& l1ID ) const;

  /** Obtain a list of PD hardware identifiers for a given level1 hardware ID
   *  @param l1ID The level1 ID number
   *  @return Vector of all PD hardware IDs for that Level1 board
   */
  const Rich::DAQ::PDHardwareIDs& l1PDHardIDs( const Rich::DAQ::Level1HardwareID& l1ID ) const;

  /** Reconst Rich::DAQ::PDHardwareIDturn which RICH detector a given Level1 hardware ID is for
   *  @param l1ID The Level 1 hardware ID
   *  @return The RICH detector
   */
  Rich::DetectorType richDetector( const Rich::DAQ::Level1HardwareID& l1ID ) const;

  /// Direct access to the mapping between Level1 IDs and PD RichSmartIDs
  const Rich::DAQ::L1ToSmartIDs& l1PDSmartIDs() const noexcept;

  /// Direct access to the mapping between Level1 IDs and PD RichSmartIDs
  const Rich::DAQ::L1ToHardIDs& l1PDHardIDs() const noexcept;

  /// Returns a list of all valid Level1 board hardware IDs
  const Rich::DAQ::Level1HardwareIDs& level1HardwareIDs() const noexcept;

  /// Returns the number of PDs in the given RICH detector
  unsigned int nPDs( const Rich::DetectorType rich ) const;

  /// Returns the number of PDs in the given RICH detector and side
  unsigned int nPDs( const Rich::DetectorType rich, const Rich::Side side ) const;

  /// Returns the total number of PDs
  unsigned int nPDs() const noexcept;

  /// Access the DeRichPanel for the given RICH and Panel
  inline const DeRichPDPanel* dePDPanel( const Rich::DetectorType rich, const Rich::Side side ) const {
    return m_deRich[rich]->pdPanel( side );
  }

  /// Access the DeRichPanel for the given PD ID
  inline const DeRichPDPanel* dePDPanel( const LHCb::RichSmartID pdID ) const {
    return dePDPanel( pdID.rich(), pdID.panel() );
  }

  /** Get the correct DeRichPD object for the given RichSmartID
   *  @param[in] hpdID The RichSmartID for the PD
   *  @return Pointer to the associated DeRichPD object
   */
  inline const DeRichPD* dePD( const LHCb::RichSmartID pdID ) const { return dePDPanel( pdID )->dePD( pdID ); }

  /// Is a 'large' PD
  inline bool isLargePD( const LHCb::RichSmartID pdID ) const { return dePDPanel( pdID )->isLargePD( pdID ); }

  /**
   * Retrieves the location of the PD/PMT in the detector store, so it can be
   * loaded using the getDet<DeRichPD> method.
   * @return The location of the PD in the detector store
   */
  std::string getDePDLocation( const LHCb::RichSmartID& smartID ) const;

  /// The photon detector type
  inline Rich::RichPhDetConfigType RichPhotoDetConfig() const noexcept { return m_photDetConf; }

private:
  // definitions

  /// Map type to use.
  template <typename TO, typename FROM>
  using MyMap = std::unordered_map<TO, FROM>;

private:
  // methods

  /// Update methods for PD mappings
  StatusCode buildPDMappings();

  /// Fill the maps for the given RICH detector
  StatusCode fillMaps( const Rich::DetectorType rich );

  /// Access on demand the Detector Elements for Rich1 and Rich2
  DeRich* deRich( const Rich::DetectorType rich ) const noexcept;

  /// Save information to a map, checking first it is not already set
  template <class SOURCE, class TARGET, class MAP>
  inline bool safeMapFill( const SOURCE& source, const TARGET& target, MAP& map ) {
    auto p = map.insert( typename MAP::value_type( source, target ) );
    if ( UNLIKELY( !p.second ) ) {
      error() << "Error filling map '" << System::typeinfoName( typeid( map ) ) << "' source "
              << System::typeinfoName( typeid( source ) ) << "=" << source << " already has an entry for target "
              << System::typeinfoName( typeid( target ) ) << " OLD=" << map[source] << " NEW=" << target << endmsg;
    }
    return p.second;
  }

  /// setup photon detector configuration
  void setupPhotDetConf() noexcept;

  /// The version of RichSystem
  inline int systemVersion() const noexcept {
    return ( exists( "systemVersion" ) ? param<int>( "systemVersion" ) : 0 );
  }

private:
  // data

  /// RICH PhotoDetector Configuration (assume HPD by default)
  Rich::RichPhDetConfigType m_photDetConf = Rich::HPDConfig;

  /// mapping from RichSmartID to Rich::DAQ::PDHardwareID
  MyMap<const LHCb::RichSmartID, Rich::DAQ::PDHardwareID> m_soft2hard;

  /// mapping from Rich::DAQ::PDHardwareID to RichSmartID
  MyMap<const Rich::DAQ::PDHardwareID, LHCb::RichSmartID> m_hard2soft;

  /// mapping from Rich::DAQ::PDHardwareID to RichSmartID
  MyMap<const Rich::DAQ::Level0ID, LHCb::RichSmartID> m_l0hard2soft;

  /// List of all active PD RichSmartIDs
  LHCb::RichSmartID::Vector m_activePDSmartIDs;

  /// List of all inactive PD RichSmartIDs
  LHCb::RichSmartID::Vector m_inactivePDSmartIDs;

  /// List of all PD RichSmartIDs
  LHCb::RichSmartID::Vector m_allPDSmartIDs;

  /// List of all active PD hardware IDs
  Rich::DAQ::PDHardwareIDs m_activePDHardIDs;

  /// List of all inactive PD hardware IDs
  Rich::DAQ::PDHardwareIDs m_inactivePDHardIDs;

  /// List of all PD hardware IDs
  Rich::DAQ::PDHardwareIDs m_allPDHardIDs;

  /// mapping from RichSmartID to Level0 ID
  MyMap<const LHCb::RichSmartID, Rich::DAQ::Level0ID> m_smartid2L0;

  /// mapping from PD Hardware ID to Level0 ID
  MyMap<const Rich::DAQ::PDHardwareID, Rich::DAQ::Level0ID> m_hardid2L0;

  /// mapping from Level0 to Level1 ID
  MyMap<const Rich::DAQ::Level0ID, Rich::DAQ::Level1HardwareID> m_l0ToL1;

  /// mapping from RichSmartID to Level1 ID
  MyMap<const LHCb::RichSmartID, Rich::DAQ::Level1HardwareID> m_smartid2L1;

  /// mapping from PD Hardware ID to Level1 ID
  MyMap<const Rich::DAQ::PDHardwareID, Rich::DAQ::Level1HardwareID> m_hardid2L1;

  /// mapping from RichSmartID to Level1 input number
  MyMap<const LHCb::RichSmartID, Rich::DAQ::HPD::Level1Input> m_smartid2L1In;

  /// mapping from PD Hardware ID to Level1 input number
  MyMap<const Rich::DAQ::PDHardwareID, Rich::DAQ::HPD::Level1Input> m_hardid2L1In;

  /// mapping between L1 boards and RICH detector
  MyMap<const Rich::DAQ::Level1HardwareID, Rich::DetectorType> m_l1ToRich;

  /// L1 ID to RichSmartIDs map
  Rich::DAQ::L1ToSmartIDs m_l12smartids;

  /// L1 ID to PD hardware IDs map
  Rich::DAQ::L1ToHardIDs m_l12hardids;

  /// L1 hardware ID + Input number pair
  using L1HardIDAndInput = std::pair<const Rich::DAQ::Level1HardwareID, const Rich::DAQ::HPD::Level1Input>;
  /// mapping L1 HardwareID + L1 input number to PD Hardware ID
  Rich::Map<L1HardIDAndInput, Rich::DAQ::PDHardwareID> m_L1HardIDAndInputToPDHardID;

  /// List of all valid Level1 IDs
  Rich::DAQ::Level1HardwareIDs m_l1IDs;

  /// smartID to copy number map
  MyMap<const LHCb::RichSmartID, Rich::DAQ::PDCopyNumber> m_smartid2copyNumber;

  /// copy number to smartID map
  MyMap<const Rich::DAQ::PDCopyNumber, LHCb::RichSmartID> m_copyNumber2smartid;

  /// L1 Logical ID to L1 Copy Number
  MyMap<const Rich::DAQ::Level1HardwareID, Rich::DAQ::Level1CopyNumber> m_l1H2CopyN;

  /// Rich1 & Rich2 detector elements
  Rich::DetectorArray<DeRich*> m_deRich = {{}};

  /// Logical to hardware L1 ID map
  Rich::DetectorArray<MyMap<const Rich::DAQ::Level1LogicalID, Rich::DAQ::Level1HardwareID>> m_l1LogToHard;

  /// Logical to hardware L1 ID map
  MyMap<const Rich::DAQ::Level1HardwareID, Rich::DAQ::Level1LogicalID> m_l1HardToLog;
};

//=========================================================================
// Access the Detector Elements for Rich1 and Rich2
//=========================================================================
inline DeRich* DeRichSystem::deRich( const Rich::DetectorType rich ) const noexcept { return m_deRich[rich]; }

//=========================================================================
// activePDRichSmartIDs
//=========================================================================
inline const LHCb::RichSmartID::Vector& DeRichSystem::activePDRichSmartIDs() const noexcept {
  return m_activePDSmartIDs;
}

//=========================================================================
// inactivePDRichSmartIDs
//=========================================================================
inline const LHCb::RichSmartID::Vector& DeRichSystem::inactivePDRichSmartIDs() const noexcept {
  return m_inactivePDSmartIDs;
}

//=========================================================================
// allPDRichSmartIDs
//=========================================================================
inline const LHCb::RichSmartID::Vector& DeRichSystem::allPDRichSmartIDs() const noexcept { return m_allPDSmartIDs; }

//=========================================================================
// activePDHardwareIDs
//=========================================================================
inline const Rich::DAQ::PDHardwareIDs& DeRichSystem::activePDHardwareIDs() const noexcept { return m_activePDHardIDs; }

//=========================================================================
// activePDHardwareIDs
//=========================================================================
inline const Rich::DAQ::PDHardwareIDs& DeRichSystem::inactivePDHardwareIDs() const noexcept {
  return m_inactivePDHardIDs;
}

//=========================================================================
// allPDHardwareIDs
//=========================================================================
inline const Rich::DAQ::PDHardwareIDs& DeRichSystem::allPDHardwareIDs() const noexcept { return m_allPDHardIDs; }

//=========================================================================
// pdIsActive
//=========================================================================
inline bool DeRichSystem::pdIsActive( const LHCb::RichSmartID& id ) const {
  const auto& c = m_inactivePDSmartIDs;
  return ( c.empty() || std::find( c.begin(), c.end(), id.pdID() ) == c.end() );
}

//=========================================================================
// pdIsActive
//=========================================================================
inline bool DeRichSystem::pdIsActive( const Rich::DAQ::PDHardwareID& id ) const {
  const auto& c = m_inactivePDHardIDs;
  return ( c.empty() || std::find( c.begin(), c.end(), id ) == c.end() );
}

//=========================================================================
// l1PDSmartIDs
// Access mapping between Level 1 IDs and PD RichSmartIDs
//=========================================================================
inline const Rich::DAQ::L1ToSmartIDs& DeRichSystem::l1PDSmartIDs() const noexcept { return m_l12smartids; }

//=========================================================================
// l1PDHardIDs
// Access mapping between Level 1 IDs and PD RichSmartIDs
//=========================================================================
inline const Rich::DAQ::L1ToHardIDs& DeRichSystem::l1PDHardIDs() const noexcept { return m_l12hardids; }

//=========================================================================
// level1 hardware IDs
//=========================================================================
inline const Rich::DAQ::Level1HardwareIDs& DeRichSystem::level1HardwareIDs() const noexcept { return m_l1IDs; }

//=========================================================================
// Number HPDs
//=========================================================================
inline unsigned int DeRichSystem::nPDs() const noexcept { return allPDRichSmartIDs().size(); }

//=========================================================================
// L1 logical ID
//=========================================================================
inline const Rich::DAQ::Level1LogicalID DeRichSystem::level1LogicalID( const LHCb::RichSmartID& smartID ) const {
  return level1LogicalID( level1HardwareID( smartID ) );
}

//=========================================================================
// PD per RICH
//=========================================================================
inline unsigned int DeRichSystem::nPDs( const Rich::DetectorType rich ) const {
  return std::count_if( allPDRichSmartIDs().begin(), allPDRichSmartIDs().end(),
                        [&rich]( const auto& PD ) { return PD.rich() == rich; } );
}

//=========================================================================
// PD per RICH and side
//=========================================================================
inline unsigned int DeRichSystem::nPDs( const Rich::DetectorType rich, const Rich::Side side ) const {
  return std::count_if( allPDRichSmartIDs().begin(), allPDRichSmartIDs().end(),
                        [&rich, &side]( const auto& PD ) { return ( PD.rich() == rich && PD.panel() == side ); } );
}
