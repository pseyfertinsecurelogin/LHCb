
#pragma once

// STL
#include <vector>

// Kernel
#include "Kernel/RichSmartID.h"

// Event model
#include "Event/MCRichDigitSummary.h"
#include "Event/MCRichDigitHistoryCode.h"

// Kernel
#include "Kernel/RichRadiatorType.h"

// Rich Utils
#include "RichUtils/RichMap.h"
#include "RichUtils/RichPixelCluster.h"

namespace Rich::Future::MC::Relations
{

  /// Helper class for RichSmartID MC Histories
  class SmartIDUtils
  {

  public:

    /// Constructor from a summaries object
    SmartIDUtils( const LHCb::MCRichDigitSummarys & histories );

  public:

    /// Returns a vector of the MCParticles associated to a given pixel cluster
    LHCb::MCParticle::ConstVector
    mcParticles( const Rich::PDPixelCluster& cluster ) const;

    /// Returns a vector of the MCParticles associated to a given RichSmartID
    LHCb::MCParticle::ConstVector
    mcParticles( const LHCb::RichSmartID id ) const;

    /// Returns a vector of MCRichDigitHistoryCodes associated to a given cluster
    std::vector<LHCb::MCRichDigitHistoryCode>
    mcDigitHistoryCodes( const Rich::PDPixelCluster& cluster ) const;

    /// Returns a vector of MCRichDigitHistoryCodes associated to a given RichSmartID
    std::vector<LHCb::MCRichDigitHistoryCode>
    mcDigitHistoryCodes( const LHCb::RichSmartID id ) const;

    /// Is the RichSmartID due to true MC Cherenkov radiation from given radiator medium ?
    const LHCb::MCParticle *
    trueCherenkovRadiation( const LHCb::RichSmartID id,
                            const Rich::RadiatorType rad ) const;

    /** Is any of the RichSmartIDs in nthe cluster due to true MC Cherenkov radiation
     *  from given radiator medium */
    const LHCb::MCParticle *
    trueCherenkovRadiation( const Rich::PDPixelCluster& cluster,
                            const Rich::RadiatorType rad ) const;

    /// Checks if the given RichSmartID is the result of a background hit
    bool isBackground ( const LHCb::RichSmartID id ) const;

    /// Checks if the given RichSmartID is the result of true Cherenkov
    bool isCherenkovRadiation( const LHCb::RichSmartID id,
                               const Rich::RadiatorType rad ) const;

    /// Checks if the given cluster is the result of true Cherenkov
    bool isCherenkovRadiation( const Rich::PDPixelCluster& cluster,
                               const Rich::RadiatorType rad ) const;

  private:

    /// Type for vector of pointers to MCRichDigitSummaries
    using DigitSummaries = std::vector<const LHCb::MCRichDigitSummary*>;

    /// Type for map between RichSmartIDs and MCRichDigitSummary objects
    using SummaryMap = Rich::Map<const LHCb::RichSmartID,DigitSummaries>;

  private:

    /// map relating a smartID to its list of history objects
    SummaryMap m_sumMap;

  };

}
