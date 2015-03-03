
/** @file DeRichSystem.h
 *
 *  Header file for detector description class : DeRichSystem
 *
 *  CVS Log :-
 *  $Id: DeRichSystem.h,v 1.3 2006-11-23 17:31:26 jonrob Exp $
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2006-01-26
 */

#ifndef RICHDET_DERICHSYSTEM_H
#define RICHDET_DERICHSYSTEM_H 1

// DetDesc
#include "DetDesc/DetectorElement.h"

// LHCbKernel
#include "Kernel/RichSide.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSmartID.h"

// boost
#include "boost/array.hpp"

//local
#include "RichDet/RichDAQDefinitions.h"

// External declarations
extern const CLID& CLID_DERichSystem;

/** @namespace DeRichLocation
 *
 *  Namespace for the xml location of the detector elements RichSystem
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */
namespace DeRichLocation {
                                                                                
  /// RichSystem location in transient detector store
  static const std::string& RichSystem = "/dd/Structure/LHCb/AfterMagnetRegion/Rich2/RichSystem";
}


/** @class DeRichSystem DeRichSystem.h
 *
 * Class for generic info about the Rich system. In particular
 * to provide conversions and mappings between the various
 * RICH numbering schemes and to provide data on which HPDs, Level0 and Level1
 * boards are currently active.
 *
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date   27/01/2006
 */
class DeRichSystem: public DetectorElement {

public:
  /**
   * Constructor for this class
   */
  DeRichSystem();

  /**
   * Default destructor
   */
  virtual ~DeRichSystem();

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const {
    return classID();
  }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID();

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  virtual StatusCode initialize();


  // Convert a RichSmartID into the corresponding hardware number
  const RichDAQ::HPDHardwareID hardwareID( const LHCb::RichSmartID smartID ) const;

  // Convert a RICH HPD hardware number into the corresponding RichSmartID
  const LHCb::RichSmartID richSmartID( const RichDAQ::HPDHardwareID hID ) const;

  // Convert a RICH Level0 hardware number into the corresponding RichSmartID
  const LHCb::RichSmartID richSmartID( const RichDAQ::Level0ID hID ) const;

  // Access a list of all active HPDs identified by their RichSmartID
  const LHCb::RichSmartID::Vector & activeHPDRichSmartIDs() const;

  // Access a list of all active HPDs identified by their hardware IDs
  const RichDAQ::HPDHardwareIDs & activeHPDHardwareIDs() const;

  // Ask whether a given HPD is currently active or dead
  bool hpdIsActive( const LHCb::RichSmartID id ) const;

  // Ask whether a given HPD is currently active or dead
  bool hpdIsActive( const RichDAQ::HPDHardwareID id ) const;

  // Obtain the Level0 ID number for a given RichSmartID
  const RichDAQ::Level0ID level0ID( const LHCb::RichSmartID smartID ) const;

  // Obtain the Level0 ID number for a given HPD hardware ID
  const RichDAQ::Level0ID level0ID( const RichDAQ::HPDHardwareID hardID ) const;

  // Obtain the Level1 ID number for a given RichSmartID
  const RichDAQ::Level1ID level1ID( const LHCb::RichSmartID smartID ) const;

  // Obtain the Level1 ID number for a given HPD hardware ID
  const RichDAQ::Level1ID level1ID( const RichDAQ::HPDHardwareID hardID ) const;

  // Obtain the Level1 input number for a given RichSmartID
  const RichDAQ::Level1Input level1InputNum( const LHCb::RichSmartID smartID ) const;

  /// Obtain the Level1 input number for a given HPD hardware ID
  const RichDAQ::Level1Input level1InputNum( const RichDAQ::HPDHardwareID hardID ) const;

  // Obtain a list of RichSmartID HPD identifiers for a given level 1 ID
  const LHCb::RichSmartID::Vector & l1HPDSmartIDs( const RichDAQ::Level1ID l1ID ) const;

  // Obtain a list of HPD hardware identifiers for a given level 1 ID
  const RichDAQ::HPDHardwareIDs & l1HPDHardIDs( const RichDAQ::Level1ID l1ID ) const;

  // Access mapping between Level 1 IDs and HPD RichSmartIDs
  const RichDAQ::L1ToSmartIDs & l1HPDSmartIDs() const;

  // Access mapping between Level 1 IDs and HPD RichSmartIDs
  const RichDAQ::L1ToHardIDs & l1HPDHardIDs() const;

  // Return which RICH detector a given Level 1 is used with
  const Rich::DetectorType richDetector( const RichDAQ::Level1ID l1ID ) const;

  // Returns a list of all valid Level1 ids
  const RichDAQ::Level1IDs & level1IDs() const;

private: // methods

  /// Update methods for HPD mappings
  StatusCode buildHPDMappings();

  /// Fill the maps for the given RICH detector
  StatusCode fillMaps( const Rich::DetectorType rich );

private: // data

  /// Type for mapping from RichSmartID to RichDAQ::HPDHardwareID
  typedef GaudiUtils::HashMap< const LHCb::RichSmartID, RichDAQ::HPDHardwareID > SoftToHard;
  SoftToHard m_soft2hard; ///< Software ID to hardware ID map

  /// Type for mapping from RichDAQ::HPDHardwareID to RichSmartID
  typedef GaudiUtils::HashMap< const RichDAQ::HPDHardwareID, LHCb::RichSmartID > HardToSoft;
  HardToSoft m_hard2soft; ///< HPD Hardware ID to software ID map

  /// Type for mapping from RichDAQ::HPDHardwareID to RichSmartID
  typedef GaudiUtils::HashMap< const RichDAQ::Level0ID, LHCb::RichSmartID > L0HardToSoft;
  L0HardToSoft m_l0hard2soft; ///< Level0 Hardware ID to software ID map

  /// List of all active HPD RichSmartIDs
  LHCb::RichSmartID::Vector m_smartIDs;

  /// List of all inactive HPD RichSmartIDs
  LHCb::RichSmartID::Vector m_inactiveSmartIDs;

  /// List of all active HPD hardware IDs
  RichDAQ::HPDHardwareIDs m_hardIDs;

  /// List of all inactive HPD hardware IDs
  RichDAQ::HPDHardwareIDs m_inactiveHardIDs;

  /// Typedef for mapping from RichSmartID to Level0 ID
  typedef GaudiUtils::HashMap< const LHCb::RichSmartID, RichDAQ::Level0ID > SmartIDToL0;
  SmartIDToL0 m_smartid2L0; ///< HPD RichSmartID to L0 ID map

  /// Typedef for mapping from HPD Hardware ID to Level0 ID
  typedef GaudiUtils::HashMap< const RichDAQ::HPDHardwareID, RichDAQ::Level0ID > HardIDToL0;
  HardIDToL0 m_hardid2L0; ///< HPD Hardware ID to L0 ID map

  /// Typedef for mapping from RichSmartID to Level1 ID
  typedef GaudiUtils::HashMap< const LHCb::RichSmartID, RichDAQ::Level1ID > SmartIDToL1;
  SmartIDToL1 m_smartid2L1; ///< HPD RichSmartID to L1 ID map

  /// Typedef for mapping from HPD Hardware ID to Level1 ID
  typedef GaudiUtils::HashMap< const RichDAQ::HPDHardwareID, RichDAQ::Level1ID > HardIDToL1;
  HardIDToL1 m_hardid2L1; ///< HPD Hardware ID to L1 ID map

  /// Typedef for mapping from RichSmartID to Level1 input number
  typedef GaudiUtils::HashMap< const LHCb::RichSmartID, RichDAQ::Level1Input > SmartIDToL1In;
  SmartIDToL1In m_smartid2L1In; ///< HPD RichSmartID to L1 input number map

  /// Typedef for mapping from HPD Hardware ID to Level1 input number
  typedef GaudiUtils::HashMap< const RichDAQ::HPDHardwareID, RichDAQ::Level1Input > HardIDToL1In;
  HardIDToL1In m_hardid2L1In; ///< HPD Hardware ID to L1 input number map

  /// Typedef for mapping between L1 boards and RICH detector
  typedef GaudiUtils::HashMap< const RichDAQ::Level1ID, Rich::DetectorType > L1ToRICH;
  L1ToRICH m_l1ToRich; ///< L1 to RICH map

  RichDAQ::L1ToSmartIDs m_l12smartids; ///< L1 ID to RichSmartIDs map
  RichDAQ::L1ToHardIDs  m_l12hardids;  ///< L1 ID to HPD hardware IDs map

  /// List of all valid Level1 IDs
  RichDAQ::Level1IDs m_l1IDs;

  /// Location of RICH Numbering schemes in Conditions DB
  boost::array<std::string, Rich::NRiches> m_condBDLocs;
};

#endif    // RICHDET_DERICHSYSTEM_H
