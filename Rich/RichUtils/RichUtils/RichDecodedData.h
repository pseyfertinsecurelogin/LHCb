
//---------------------------------------------------------------------------------
/** @file RichDecodedData.h
 *
 *  Header file for Rich::DAQ::DecodedData
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-06
 */
//---------------------------------------------------------------------------------

#ifndef RICHUTILS_RICHDECODEDDATA_H
#define RICHUTILS_RICHDECODEDDATA_H 1

// local
#include "RichUtils/RichDAQDefinitions.h"
#include "RichUtils/RichDAQHeaderPD_V4.h"
#include "RichUtils/RichDAQParityFooter.h"
#include "RichUtils/RichDAQL1IngressHeader.h"
#include "RichUtils/RichPoolMap.h"

namespace Rich
{
  namespace DAQ
  {

    /** @class PDInfo RichUtils/RichDecodedData.h
     *  Decoded PD information
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   20/04/2007
     */
    class PDInfo final // : public LHCb::MemPoolAlloc<PDInfo>
    {

    public:

      /// The PD Header type
      using Header = RichDAQHeaderV4::RichDAQHeaderPD;
      /// The PD Footer type
      using Footer = ParityFooter;
      
    public:

      /// Default Constructor
      PDInfo() = default;

      /// Constructor from PD data bank information
      PDInfo( const LHCb::RichSmartID& input,
               const Header&            header,
               const Footer&            footer )
        : m_pdID   ( input  ),
          m_header ( header ),
          m_footer ( footer )
      { }

      /// Constructor from PD data bank information
      PDInfo( const LHCb::RichSmartID&   input,
               const Header::HeaderWords& header,
               const Footer::FooterWords& footer )
        : m_pdID   ( input  ),
          m_header ( header ),
          m_footer ( footer )
      { }

      /**  Access the PD ID (LHCb::RichSmartID) for this PD
       *   @attention It is possible this PDID is invalid (for instance
       *              if the L0ID->RichSmartID database lookup failed.
       *              Users should check for validity using the method
       *              RichSmartID::isvalid()
       */
      inline const LHCb::RichSmartID& pdID()  const & noexcept { return m_pdID; }
      /// Access the Header words
      inline const Header&           header() const & noexcept { return m_header; }
      /// Access the Footer words
      inline const Footer&           footer() const & noexcept { return m_footer; }

      /// set the Level1Input
      inline void setPdID( const LHCb::RichSmartID& input ) noexcept { m_pdID  = input;  }
      /// Set the Header
      inline void setHeader( const Header & header )         noexcept { m_header = header; }
      /// Set the footer
      inline void setFooter( const Footer & footer )         noexcept { m_footer = footer; }

      /// Read access to the RichSmartIDs for the hit pixels in this PD
      inline const LHCb::RichSmartID::Vector& smartIDs() const & { return m_smartIds; }
      /// Write access to the RichSmartIDs for the hit pixels in this PD
      inline       LHCb::RichSmartID::Vector& smartIDs()       & { return m_smartIds; }

    public:

      /// Move the Header words
      inline Header&& header() && { return std::move(m_header); }
      /// Move the Footer words
      inline Footer&& footer() && { return std::move(m_footer); }
      /// Move the smart IDs
      inline LHCb::RichSmartID::Vector&& smartIDs() && { return std::move(m_smartIds); }

    private:

      LHCb::RichSmartID m_pdID;             ///< The RichSmartID for this PD
      Header m_header;                      ///< The PDHeader word
      Footer m_footer;                      ///< The PDFooter word
      LHCb::RichSmartID::Vector m_smartIds; ///< The decoded pixels in this PD

    };

    /// Map for RICH PD data, sorted by PD number
    using L1InToPDMap = Rich::PoolMap< Rich::DAQ::Level1Input, PDInfo >;

    /** @class IngressInfo RichUtils/RichDecodedData.h
     *  Information for each L1 ingress
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   20/04/2007
     */
    class IngressInfo final // : public LHCb::MemPoolAlloc<IngressInfo>
    {

    public:

      /// Default Constructor
      IngressInfo() = default;

      /// Constructor with ingress header
      explicit IngressInfo( const L1IngressHeader& head ) : m_ingressHeader(head) { }

    public:

      /// Get the L1IngressHeader
      inline const L1IngressHeader& ingressHeader() const & noexcept { return m_ingressHeader; }
      /// Read access to the PD data for this Ingress
      inline const L1InToPDMap &               pdData() const & noexcept { return m_pdData; }
      /// Write access to the PD data for this Ingress
      inline       L1InToPDMap &               pdData()       & noexcept { return m_pdData; }
      /// Set the L1IngressHeader
      inline void setIngressHeader( const L1IngressHeader& head ) { m_ingressHeader = head; }
  
    public:

      /// Move the L1IngressHeader
      inline L1IngressHeader&& ingressHeader() && { return std::move(m_ingressHeader); }
      /// Move the PD data for this Ingress
      inline L1InToPDMap &&          pdData()  && { return std::move(m_pdData); }

    private:

      L1IngressHeader m_ingressHeader; ///< The ingress header word
      L1InToPDMap      m_pdData;       ///< The PD data blocks for this ingress

    };

    /// Map for PD data for each L1 ingress
    using IngressMap = Rich::PoolMap< L1IngressID, IngressInfo >;
    
    /// Map for RICH PD data, sorted by Level1 board number
    class L1Map final : public Rich::PoolMap< Level1HardwareID, IngressMap >
    {
    public:
      /// Returns the total number of RICH hits in the decoded data
      unsigned int nTotalHits() const noexcept { return m_nTotalHits; }
      /// Sets the total number of RICH hits in the decoded data
      void setNTotalHits( const unsigned int nHits ) { m_nTotalHits = nHits; }
      /// Append to the number of hits
      void addToTotalHits( const unsigned int nHits ) { m_nTotalHits += nHits; }
    private:
      unsigned int m_nTotalHits{0};
    };
    
    /// L1Map data locations
    namespace L1MapLocation
    {
      /// Default Location in TES for the decoded data map
      static const std::string Default = "Raw/Rich/L1Data/Default";
    }

  }
}

#endif // RICHUTILS_RICHDECODEDDATA_H
