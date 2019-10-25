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

//---------------------------------------------------------------------------------
/** @file RichDecodedData.h
 *
 *  Header file for Rich::DAQ::DecodedData
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-06
 */
//---------------------------------------------------------------------------------

#pragma once

// STL
#include <utility>
#include <vector>

// local
#include "RichUtils/RichDAQDefinitions.h"
#include "RichUtils/RichDAQHeaderPD_V4.h"
#include "RichUtils/RichDAQL1IngressHeader.h"
#include "RichUtils/RichDAQParityFooter.h"
#include "RichUtils/RichMap.h"

/// New DAQ namespace
namespace Rich::Future::DAQ {

  // import from old
  using namespace Rich::DAQ;

  /** @class PDInfo RichUtils/RichDecodedData.h
   *  Decoded PD information
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   20/04/2007
   */
  class PDInfo final {

  public:
    /// The PD Header type
    using Header = RichDAQHeaderV4::RichDAQHeaderPD;
    /// The PD Footer type
    using Footer = ParityFooter;

  public:
    /// Default Constructor
    PDInfo() = default;

    /// Constructor from PD data bank information
    PDInfo( const LHCb::RichSmartID& input, Header header, Footer footer )
        : m_pdID( input ), m_header( std::move( header ) ), m_footer( std::move( footer ) ) {}

    /// Constructor from PD data bank information
    PDInfo( const LHCb::RichSmartID& input, const Header::WordType headerPrimWord )
        : m_pdID( input ), m_header( headerPrimWord ) {}

    /// Constructor from PD data bank information
    PDInfo( const LHCb::RichSmartID& input, const Header::WordType headerPrimWord,
            const Header::ExtendedHeaderWords& headerExWords, const Footer::FooterWords& footerWords )
        : m_pdID( input ), m_header( headerPrimWord, headerExWords ), m_footer( footerWords ) {}

    /**  Access the PD ID (LHCb::RichSmartID) for this PD
     *   @attention It is possible this PDID is invalid (for instance
     *              if the L0ID->RichSmartID database lookup failed.
     *              Users should check for validity using the method
     *              RichSmartID::isvalid()
     */
    [[nodiscard]] const LHCb::RichSmartID& pdID() const& noexcept { return m_pdID; }
    /// Access the Header words
    [[nodiscard]] const Header& header() const& noexcept { return m_header; }
    /// Access the Footer words
    [[nodiscard]] const Footer& footer() const& noexcept { return m_footer; }

    /// set the Level1Input
    void setPdID( const LHCb::RichSmartID& input ) noexcept { m_pdID = input; }
    /// Set the Header
    void setHeader( const Header& header ) noexcept { m_header = header; }
    /// Set the footer
    void setFooter( const Footer& footer ) noexcept { m_footer = footer; }

    /// Read access to the RichSmartIDs for the hit pixels in this PD
    [[nodiscard]] const LHCb::RichSmartID::Vector& smartIDs() const& { return m_smartIds; }
    /// Write access to the RichSmartIDs for the hit pixels in this PD
    LHCb::RichSmartID::Vector& smartIDs() & { return m_smartIds; }

  public:
    /// Move the Header words
    Header&& header() && { return std::move( m_header ); }
    /// Move the Footer words
    Footer&& footer() && { return std::move( m_footer ); }
    /// Move the smart IDs
    LHCb::RichSmartID::Vector&& smartIDs() && { return std::move( m_smartIds ); }

  private:
    LHCb::RichSmartID         m_pdID;     ///< The RichSmartID for this PD
    Header                    m_header;   ///< The PDHeader word
    Footer                    m_footer;   ///< The PDFooter word
    LHCb::RichSmartID::Vector m_smartIds; ///< The decoded pixels in this PD
  };

  /** Map for RICH PD data, sorted by PD number
   *  @todo Convert this to a PMT version once available or make generic... */
  using L1InToPDMap = std::vector<std::pair<Rich::DAQ::HPD::Level1Input, PDInfo>>;

  /** @class IngressInfo RichUtils/RichDecodedData.h
   *  Information for each L1 ingress
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   20/04/2007
   */
  class IngressInfo final {

  public:
    /// Default Constructor
    IngressInfo() = default;

    /// Constructor with ingress header
    explicit IngressInfo( const L1IngressHeader& head ) : m_ingressHeader( head ) {}

  public:
    /// Get the L1IngressHeader
    [[nodiscard]] const L1IngressHeader& ingressHeader() const& noexcept { return m_ingressHeader; }
    /// Read access to the PD data for this Ingress
    [[nodiscard]] const L1InToPDMap& pdData() const& noexcept { return m_pdData; }
    /// Write access to the PD data for this Ingress
    L1InToPDMap& pdData() & noexcept { return m_pdData; }
    /// Set the L1IngressHeader
    void setIngressHeader( const L1IngressHeader& head ) noexcept { m_ingressHeader = head; }

  public:
    /// Move the L1IngressHeader
    L1IngressHeader&& ingressHeader() && { return std::move( m_ingressHeader ); }
    /// Move the PD data for this Ingress
    L1InToPDMap&& pdData() && { return std::move( m_pdData ); }

  private:
    L1IngressHeader m_ingressHeader; ///< The ingress header word
    L1InToPDMap     m_pdData;        ///< The PD data blocks for this ingress
  };

  /// Map for PD data for each L1 ingress
  using IngressMap = std::vector<std::pair<L1IngressID, IngressInfo>>;

  /// Map for RICH PD data, sorted by Level1 board number
  class L1Map final : public std::vector<std::pair<Level1HardwareID, IngressMap>> {
  public:
    /// Returns the total number of RICH hits in the decoded data
    [[nodiscard]] unsigned int nTotalHits() const noexcept {
      return m_nTotalHits[Rich::Rich1] + m_nTotalHits[Rich::Rich2];
    }
    /// Returns the total number of hits in the decoded data for the given RICH detector
    [[nodiscard]] unsigned int nTotalHits( const Rich::DetectorType rich ) const noexcept { return m_nTotalHits[rich]; }
    /// Append to the number of hits for each RICH
    void addToTotalHits( const DetectorArray<unsigned int>& nHits ) {
      for ( const auto rich : Rich::detectors() ) { m_nTotalHits[rich] += nHits[rich]; }
    }
    /// Returns the total number of active PDs in the decoded data
    [[nodiscard]] unsigned int nActivePDs() const noexcept {
      return m_nActivePDs[Rich::Rich1] + m_nActivePDs[Rich::Rich2];
    }
    /// Returns the total number of active PDs in the decoded data for the given RICH
    [[nodiscard]] unsigned int nActivePDs( const Rich::DetectorType rich ) const noexcept { return m_nActivePDs[rich]; }
    /// Append to the number of active PDs for each RICH
    void addToActivePDs( const DetectorArray<unsigned int>& nPDs ) {
      for ( const auto rich : Rich::detectors() ) { m_nActivePDs[rich] += nPDs[rich]; }
    }
    /// Append to the number of active PDs for the given RICH detector
    void addToActivePDs( const Rich::DetectorType rich, const unsigned int nPDs = 1 ) { m_nActivePDs[rich] += nPDs; }

  private:
    /// The total hit count for each RICH detector
    DetectorArray<unsigned int> m_nTotalHits = {{0, 0}};
    /// The total active PD count for each RICH detector
    DetectorArray<unsigned int> m_nActivePDs = {{0, 0}};
  };

  /// L1Map data locations
  namespace L1MapLocation {
    /// Default Location in TES for the decoded data map
    inline const std::string Default = "Raw/Rich/L1Data/RICH1RICH2";
  } // namespace L1MapLocation

} // namespace Rich::Future::DAQ
