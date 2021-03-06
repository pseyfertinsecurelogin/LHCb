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

//------------------------------------------------------------------------------------
/** @file IRichMCTruthTool.h
 *
 *  Header file for tool interface : Rich::IMCTruthTool
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-07-31
 */
//------------------------------------------------------------------------------------

#ifndef RICHKERNEL_IRICHMCTRUTHTOOL_H
#define RICHKERNEL_IRICHMCTRUTHTOOL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Kernel
#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadiatorType.h"

// Event Model
#include "Event/MCRichHit.h"

#include "Event/Track.h"

namespace LHCb {
  class MCRichOpticalPhoton;
  class MCRichSegment;
  class MCRichTrack;
  class MCRichDigit;
  class MCParticle;
  class MCRichDigitSummary;
} // namespace LHCb
namespace Rich {
  class PDPixelCluster;
}

namespace Rich {
  namespace MC {

    //------------------------------------------------------------------------------------
    /** @class IMCTruthTool IRichMCTruthTool.h RichKernel/IRichMCTruthTool.h
     *
     *  Interface for tool performing MC truth associations between Rich event objects
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   15/03/2002
     */
    //------------------------------------------------------------------------------------

    struct IMCTruthTool : extend_interfaces<IAlgTool> {

      /** static interface identification
       *  @return unique interface identifier
       */
      DeclareInterfaceID( IMCTruthTool, 1, 0 );

      /** Find best MCParticle association for a given reconstructed Track
       *
       *  @param track Pointer to a Track
       *  @param minWeight The minimum association weight to accept for a MCParticle
       *
       *  @return Pointer to MCParticle
       *  @retval NULL  No Monte Carlo association was possible
       *  @retval !NULL Association was successful
       */
      virtual const LHCb::MCParticle* mcParticle( const LHCb::Track* track, const double minWeight = 0.5 ) const = 0;

      /** get the MCRichHits associated to a given MCParticle
       *
       *  @param mcp Pointer to an MCParticle
       *
       *  @return Vector of associated MCRichHits
       */
      virtual const SmartRefVector<LHCb::MCRichHit>& mcRichHits( const LHCb::MCParticle* mcp ) const = 0;

      /** Get the MCRichHits associated to a given RichSmartID
       *
       *  @param smartID RichSmartID
       *
       *  @return Vector of associated MCRichHits
       */
      virtual const SmartRefVector<LHCb::MCRichHit>& mcRichHits( const LHCb::RichSmartID smartID ) const = 0;

      /** Get the MCRichHits associated to a cluster of RichSmartIDs
       *
       *  @param cluster cluster of RichSmartIDs
       *  @param hits The associated hits
       *
       *  @return Vector of associated MCRichHits
       */
      virtual void mcRichHits( const Rich::PDPixelCluster& cluster, SmartRefVector<LHCb::MCRichHit>& hits ) const = 0;

      /** Get a vector of MCParticles associated to given RichSmartID
       *
       *  @param id      RichSmartID identifying the RICH readout channel
       *  @param mcParts Vector of associated MCParticles
       *
       *  @return boolean indicating if any associated MCParticles where found
       */
      virtual bool mcParticles( const LHCb::RichSmartID id, std::vector<const LHCb::MCParticle*>& mcParts ) const = 0;

      /** Get a vector of MCParticles associated to given RichSmartID cluster
       *
       *  @param cluster RichSmartID cluster
       *  @param mcParts Vector of associated MCParticles
       *
       *  @return boolean indicating if any associated MCParticles where found
       */
      virtual bool mcParticles( const Rich::PDPixelCluster&           cluster,
                                std::vector<const LHCb::MCParticle*>& mcParts ) const = 0;

      /** Determines the particle mass hypothesis for a given MCParticle
       *
       *  @param mcPart Pointer to an MCParticle
       *
       *  @return The true particle type of the MCParticle
       */
      virtual Rich::ParticleIDType mcParticleType( const LHCb::MCParticle* mcPart ) const = 0;

      /** Finds the MCRichDigit association for a RichSmartID channel identifier
       *
       *  @param id RichSmartID
       *
       *  @return Pointer to an MCRichDigit
       *  @retval NULL  No Monte Carlo association was possible
       *  @retval !NULL Association was successful
       */
      virtual const LHCb::MCRichDigit* mcRichDigit( const LHCb::RichSmartID id ) const = 0;

      /** Finds the MCRichTrack associated to a given MCParticle
       *
       *  @param mcPart Pointer to an MCParticle
       *
       *  @return Pointer to an MCRichTrack
       *  @retval NULL  No Monte Carlo association was possible
       *  @retval !NULL Association was successful
       */
      virtual const LHCb::MCRichTrack* mcRichTrack( const LHCb::MCParticle* mcPart ) const = 0;

      /** Finds the MCRichOpticalPhoton associated to a given MCRichHit
       *
       *  @param mcHit Pointer to an MCRichHit
       *
       *  @return Pointer to an MCRichOpticalPhoton
       *  @retval NULL  No Monte Carlo association was possible
       *  @retval !NULL Association was successful
       */
      virtual const LHCb::MCRichOpticalPhoton* mcOpticalPhoton( const LHCb::MCRichHit* mcHit ) const = 0;

      /** Access the bit-pack history objects for the given RichSmartID
       *
       *  @param id        RichSmartID
       *  @param histories Vector of pointers to history objects
       *
       *  @return Boolean indicating if at least one history object was found
       *  @retval true  History objects were found
       *  @retval false No history objects were found
       */
      virtual bool getMcHistories( const LHCb::RichSmartID                       id,
                                   std::vector<const LHCb::MCRichDigitSummary*>& histories ) const = 0;

      /** Access the bit-pack history objects for the given cluster of RichSmartIDs
       *
       *  @param cluster   Cluster of RichSmartID
       *  @param histories Vector of pointers to history objects
       *
       *  @return Boolean indicating if at least one history object was found
       *  @retval true  History objects were found
       *  @retval false No history objects were found
       */
      virtual bool getMcHistories( const Rich::PDPixelCluster&                   cluster,
                                   std::vector<const LHCb::MCRichDigitSummary*>& histories ) const = 0;

      /** Checks if the given RichSmartID is the result of a background
       *  hit, i.e. not a true Cherenkov hit
       *
       *  @param id RichSmartID
       *
       *  @return Boolean indicating if the digit is background
       *  @retval true  RichSmartID originated from a background hit
       *  @retval false RichSmartID originated from Cherenkov Radiation
       */
      virtual bool isBackground( const LHCb::RichSmartID id ) const = 0;

      /** Checks if the given RichSmartID is the result of a photon which
       *  underwent reflections inside the PD
       *
       *  @param id RichSmartID
       *
       *  @return Boolean indicating if the digit is an PD reflection
       *  @retval true  RichSmartID originated from a reflection hit
       *  @retval false RichSmartID did not undergo reflection
       */
      virtual bool isHPDReflection( const LHCb::RichSmartID id ) const = 0;

      /** Checks if the given RichSmartID is the result of backscattering
       *   of the PD silicon sensor
       *
       *  @param id RichSmartID
       *
       *  @return Boolean indicating if the digit is from backscattering
       *  @retval true  RichSmartID originated from back-scattering
       *  @retval false RichSmartID did not originate from back-scattering
       */
      virtual bool isSiBackScatter( const LHCb::RichSmartID id ) const = 0;

      /** Checks if the given RichSmartID is the result of scintillation
       *   of the radiator
       *
       *  @param id RichSmartID
       *
       *  @return Boolean indicating if the digit is from scintillation
       *  @retval true  RichSmartID originated from scintillation
       *  @retval false RichSmartID did not originate from scintillation
       */
      virtual bool isRadScintillation( const LHCb::RichSmartID id ) const = 0;

      /** Checks if the given RichSmartID is the result of true Cherenkov
       *  radiation from the given radiator
       *
       *  @param id  RichSmartID identifying channel
       *  @param rad Radiator medium
       *
       *  @return Boolean indicating if the hit is Cherenkov radiation
       *          in given radiator
       *  @retval true  RichSmartID did originate from Cherenkov radiation
       *  @retval false RichSmartID did not originate from Cherenkov radiation
       */
      virtual bool isCherenkovRadiation( const LHCb::RichSmartID id, const Rich::RadiatorType rad ) const = 0;

      /** Checks if the given cluster of RichSmartIDs is the result of background,
       *  i.e. none of the channels came from a true Cherenkov hit
       *
       *  @param cluster Cluster of RichSmartIDs
       *
       *  @return Boolean indicating if the cluster is background
       *  @retval true  RichSmartIDs originated from a background hit
       *  @retval false RichSmartIDs originated from Cherenkov Radiation
       */
      virtual bool isBackground( const Rich::PDPixelCluster& cluster ) const = 0;

      /** Checks if the given cluster of RichSmartIDs is the result of a photon which
       *  underwent reflections inside the PD
       *
       *  @param cluster Cluster of RichSmartIDs
       *
       *  @return Boolean indicating if the cluster is an PD reflection
       *  @retval true  RichSmartIDs originated from a reflection hit
       *  @retval false RichSmartIDs did not undergo reflection
       */
      virtual bool isHPDReflection( const Rich::PDPixelCluster& cluster ) const = 0;

      /** Checks if the given cluster of RichSmartIDs is the result of backscattering
       *   of the PD silicon sensor
       *
       *  @param cluster Cluster of RichSmartIDs
       *
       *  @return Boolean indicating if the cluster is from backscattering
       *  @retval true  RichSmartIDs originated from back-scattering
       *  @retval false RichSmartIDs did not originate from back-scattering
       */
      virtual bool isSiBackScatter( const Rich::PDPixelCluster& cluster ) const = 0;

      /** Checks if the given cluster of RichSmartIDs is the result of scintillation
       *   of radiator
       *
       *  @param cluster Cluster of RichSmartIDs
       *
       *  @return Boolean indicating if the cluster is from scintillation
       *  @retval true  RichSmartIDs originated from scintillation
       *  @retval false RichSmartIDs did not originate from scintillation
       */
      virtual bool isRadScintillation( const Rich::PDPixelCluster& cluster ) const = 0;

      /** Checks if the given cluster of RichSmartIDs is the result of true Cherenkov
       *  radiation from the given radiator. I.e. At least of of the associated
       *  hits came from a true Cherenkov hit in the correct radiator.
       *
       *  @param cluster Cluster of RichSmartIDs
       *  @param rad     Radiator medium
       *
       *  @return Boolean indicating if the hit is Cherenkov radiation
       *          in given radiator
       *  @retval true  RichSmartID did originate from Cherenkov radiation
       *  @retval false RichSmartID did not originate from Cherenkov radiation
       */
      virtual bool isCherenkovRadiation( const Rich::PDPixelCluster& cluster, const Rich::RadiatorType rad ) const = 0;

      /** Checks if MC summary information for the RICH hits (RichSmartIDs) are available
       *
       *  @retval true  MC summary inforamtion is available
       *  @retval false MC summary inforamtion is NOT available
       */
      virtual bool richMCHistoryAvailable() const = 0;

      /** Checks if RICH extended MC information (MCRichOpticalPhoton, MCRichSegment etc.)
       *  is available.
       *
       *  @retval true  Extended MC inforamtion is available
       *  @retval false Extended MC inforamtion is NOT available
       */
      virtual bool extendedMCAvailable() const = 0;
    };

  } // namespace MC
} // namespace Rich

#endif // RICHKERNEL_IRICHMCTRUTHTOOL_H
