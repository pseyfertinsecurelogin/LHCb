
//--------------------------------------------------------------------------------
/** @file RichPixelCluster.h
 *
 *  Header file for pixel clustering class Rich::DAQ::PixelCluster
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   02/02/2007
 */
//--------------------------------------------------------------------------------

#pragma once

// STL
#include <algorithm>
#include <cstdint>
#include <list>
#include <memory>
#include <ostream>
#include <vector>

// Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SerializeSTL.h"

// Kernel
#include "Kernel/FastAllocList.h"
#include "Kernel/FastAllocVector.h"
#include "Kernel/RichSmartID.h"

// RichUtils
#include "RichUtils/RichDAQDefinitions.h"

// RichDet
class DeRichPD;

namespace Rich
{
  // Needed to get vector printing to work
  using GaudiUtils::operator<<;

  //-----------------------------------------------------------------------------------------------
  /** @class PDPixelCluster RichPixelCluster.h RichUtils/RichPixelCluster.h
   *
   *  Utility class used to represent a single cluster of PD pixels.
   *
   *  @author Chris Jones
   *  @date   07/02/2007
   */
  //-----------------------------------------------------------------------------------------------
  class PDPixelCluster final
  {

  public:

    /// RichSmartID vector type
    using SmartIDVector = LHCb::RichSmartID::Vector;

    /// Vector of clusters
    using Vector = LHCb::STL::Vector< PDPixelCluster >;

  public:

    /// Default Constructor
    PDPixelCluster() = default;

  public:

    /// Constructor from a single channel (one pixel cluster) and optional DePD pointer
    explicit PDPixelCluster( const LHCb::RichSmartID id, const DeRichPD *dePD = nullptr )
      : m_rich( id.rich() )
      , m_side( id.panel() )
      , m_primaryID( id )
      , m_empty( false )
      , m_dePD { dePD }
    {}

    /// Constructor from a single channel (one pixel cluster) and optional DePD pointer
    /// In addition pass in the rich and side enum, to avoid re-computing them from the ID
    explicit PDPixelCluster( const Rich::DetectorType rich,
                             const Rich::Side         side,
                             const LHCb::RichSmartID  id,
                             const DeRichPD *         dePD = nullptr )
      : m_rich( rich ), m_side( side ), m_primaryID( id ), m_empty( false ), m_dePD { dePD }
    {}

    /// Constructor from a single channel (one pixel cluster), and vector of secondary IDs
    /// and optional DePD pointer.
    /// In addition pass in the rich and side enum, to avoid re-computing them from the ID
    explicit PDPixelCluster( const Rich::DetectorType rich,
                             const Rich::Side         side,
                             const LHCb::RichSmartID  id,
                             SmartIDVector &&         secIDs,
                             const DeRichPD *         dePD = nullptr )
      : m_rich( rich )
      , m_side( side )
      , m_primaryID( id )
      , m_empty( false )
      , m_dePD { dePD }
      , m_secondaryIDs( std::forward< SmartIDVector >( secIDs ) )
    {}

    /// Constructor from a single channel (one pixel cluster), and vector of secondary IDs
    /// and optional DePD pointer.
    /// In addition pass in the rich and side enum, to avoid re-computing them from the ID
    explicit PDPixelCluster( const LHCb::RichSmartID id,
                             SmartIDVector &&        secIDs,
                             const DeRichPD *        dePD = nullptr )
      : m_rich( id.rich() )
      , m_side( id.panel() )
      , m_primaryID( id )
      , m_empty( false )
      , m_dePD { dePD }
      , m_secondaryIDs( std::forward< SmartIDVector >( secIDs ) )
    {}

  public:

    /// const access to the list of secondary PD pixels
    inline const SmartIDVector &secondaryIDs() const &noexcept { return m_secondaryIDs; }

    /// move access to the list of secondary PD pixels
    inline SmartIDVector &&secondaryIDs() && noexcept { return std::move( m_secondaryIDs ); }

    /// The primary (seed) channel ID
    inline LHCb::RichSmartID primaryID() const noexcept { return m_primaryID; }

    /// The RICH detector for this cluster
    inline Rich::DetectorType rich() const noexcept { return m_rich; }

    /// The RICH panel for this cluster
    inline Rich::Side panel() const noexcept { return m_side; }

    /// The RICH panel SmartID for this cluster
    inline LHCb::RichSmartID panelID() const noexcept { return primaryID().panelID(); }

    /// The RICH PD SmartID for this cluster
    inline LHCb::RichSmartID pdID() const noexcept { return primaryID().pdID(); }

    /// Access the DeRichPD object
    inline const DeRichPD *dePD() const noexcept { return m_dePD; }

  public:

    /// Set the DeRichPD pointer
    inline void setDePD( const DeRichPD *dePD ) noexcept { m_dePD = dePD; }

    /// Clear this cluster
    inline void clear() noexcept
    {
      m_empty     = true;
      m_primaryID = LHCb::RichSmartID();
      m_rich      = Rich::InvalidDetector;
      m_side      = Rich::InvalidSide;
      m_dePD      = nullptr;
      m_secondaryIDs.clear();
    }

  public:

    /// Number of channels in this cluster
    inline decltype( auto ) size() const noexcept
    {
      return ( !empty() ? 1u + secondaryIDs().size() : 0u );
    }

    /// Is the cluster empty ?
    inline bool empty() const noexcept { return m_empty; }

  public:

    /// Add a channel to this cluster
    void addChannel( const LHCb::RichSmartID id ) noexcept;

  public:

    /// overload printout to ostream operator <<
    friend inline std::ostream &operator<<( std::ostream &s, const PDPixelCluster &cluster )
    {
      s << "[ " << cluster.rich() << " " << Rich::text( cluster.rich(), cluster.panel() ) << " "
        << (void *)cluster.dePD() << " " << cluster.primaryID();
      if ( !cluster.secondaryIDs().empty() ) { s << " " << cluster.secondaryIDs(); }
      return s << " ]";
    }

  private:

    /// Update the cached RICH and panel enums
    inline void updateCachedEnums() noexcept
    {
      m_side = primaryID().panel();
      m_rich = primaryID().rich();
    }

  private:

    /// Cache the detector enum
    Rich::DetectorType m_rich = Rich::InvalidDetector;

    /// Cache the RICH Side enum
    Rich::Side m_side = Rich::InvalidSide;

    /// Primary ID for this cluster
    LHCb::RichSmartID m_primaryID;

    /// Flag to indicate if this cluster is empty or not
    bool m_empty { true };

    /// Pointer to associated DeRichPD object
    const DeRichPD *m_dePD = nullptr;

    /// The vector of 'secondary' RichSmartIDs for this cluster
    SmartIDVector m_secondaryIDs;
  };

  /// Pixel Cluster data locations
  namespace PDPixelClusterLocation
  {
    /// Default Location in TES for the decoded data map
    static const std::string Default = "Rec/Rich/PixelClusters/Default";
  } // namespace PDPixelClusterLocation

  //-----------------------------------------------------------------------------
  /** @class PDPixelClusters RichPixelCluster.h
   *
   *  Utility class representing a set of clusters for a single PD
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   21/03/2006
   */
  //-----------------------------------------------------------------------------
  class PDPixelClusters final
  {

  public:

    //-----------------------------------------------------------------------------
    /** @class Cluster RichPixelCluster.h
     *
     *  Utility class representing a cluster of PD pixels together with a unique
     *  (within an PD) cluster ID
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   21/03/2006
     */
    //-----------------------------------------------------------------------------
    class Cluster final
    {

    public: // definitions

      /// Collection of clusters
      using Collection = LHCb::STL::Vector< Cluster >;
      /// Collection of cluster pointers
      using PtnVector = LHCb::STL::Vector< Cluster * >;
      /// Collection of cluster smart pointers (i.e. for ownership with memory management)
      using SmartPtnVector = LHCb::STL::Vector< std::unique_ptr< Cluster > >;

    public: // methods

      /// Constructor from ID
      explicit Cluster( const std::int16_t id ) : m_clusterID( id ) {}

      /// Get cluster ID
      inline std::int16_t id() const noexcept { return m_clusterID; }

      /// Add a pixel to this cluster
      inline void addPixel( const LHCb::RichSmartID id ) noexcept { m_cluster.addChannel( id ); }

      /// Get read access to cluster data
      inline const Rich::PDPixelCluster &pixels() const &noexcept { return m_cluster; }

      /// Get read/write access to cluster data
      inline Rich::PDPixelCluster &pixels() & noexcept { return m_cluster; }

      /// Get move access to cluster data
      inline Rich::PDPixelCluster &&pixels() && noexcept { return std::move( m_cluster ); }

      /// Shortcut to the cluster size
      inline decltype( auto ) size() const noexcept { return pixels().size(); }

      /// Clear this cluster
      inline void clear() noexcept
      {
        m_clusterID = -1;
        m_cluster.clear();
      }

    private:

      /// Cluster ID
      std::int16_t m_clusterID { -1 };

      /// list of pixels in this cluster.
      PDPixelCluster m_cluster;
    };

  public: // methods

    /// Default Constructor
    PDPixelClusters() = default;

    /// Reset for a new PD
    inline void clear() noexcept { m_allclus.clear(); }

    /// Reserve size
    inline void reserve( const std::size_t n ) noexcept { m_allclus.reserve( n ); }

  public:

    /// Read access to the vector of clusters
    inline const Cluster::Collection &clusters() const &noexcept { return m_allclus; }

    /// Write access to the vector of clusters
    inline Cluster::Collection &clusters() & noexcept { return m_allclus; }

    /// Move access to the vector of clusters
    inline Cluster::Collection &&clusters() && noexcept { return std::move( m_allclus ); }

    /// Create a new vector of suppressed RichSmartIDs
    void suppressIDs( PDPixelCluster::SmartIDVector &smartIDs, const unsigned int maxSize ) const;

  public:

    /// Get the cluster for a given PD ID
    const Cluster *getCluster( const LHCb::RichSmartID id ) const;

    /// Create and return a new cluster with the given ID
    inline Cluster *createNewCluster( const std::int16_t id )
    {
      m_allclus.emplace_back( id );
      return &m_allclus.back();
    }

  public:

    /// Print in a human readable way
    MsgStream &fillStream( MsgStream &os ) const;

    /// Overload output to MsgStream
    friend inline MsgStream &operator<<( MsgStream &os, const PDPixelClusters &clus )
    {
      return clus.fillStream( os );
    }

  private: // data

    /// Vector of all created clusters
    Cluster::Collection m_allclus;
  };

  //-----------------------------------------------------------------------------
  /** @class PDPixelClustersBuilder RichPixelCluster.h
   *
   *  Utility base class used to build a set of clusters
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   21/03/2006
   */
  //-----------------------------------------------------------------------------
  class PDPixelClustersBuilder
  {

  public:

    // Note : Do not remove the destructor below !! It is required.

    /// virtual destructor
    virtual ~PDPixelClustersBuilder() = default;

  public:

    /// Initialise for a new PD
    virtual void initialise( const PDPixelCluster::SmartIDVector &smartIDs ) = 0;

  public:

    /// Const Access the data for this builder
    const PDPixelClusters &data() const noexcept { return m_pdClus; }
    /// Access the data for this builder
    PDPixelClusters &data() noexcept { return m_pdClus; }

  public:

    /// Maximum number of pixel columns
    inline decltype( auto ) nPixelCols() const noexcept { return m_nPixCols; }

    /// Number of pixel rows
    inline decltype( auto ) nPixelRows() const noexcept { return m_nPixRows; }

    /// Returns the 'correct' row number for the given RichSmartID
    inline decltype( auto ) rowNumber( const LHCb::RichSmartID id ) const noexcept
    {
      return (
        !id.pixelSubRowDataIsValid() ?
          id.pixelRow() :
          ( ( Rich::DAQ::HPD::NumAlicePixelsPerLHCbPixel * id.pixelRow() ) + id.pixelSubRow() ) );
    }

    /// Returns the 'correct' column number for the given RichSmartID
    inline decltype( auto ) colNumber( const LHCb::RichSmartID id ) const noexcept
    {
      return id.pixelCol();
    }

  public:

    /// Split the given clusters up into single channel clusters
    void splitClusters( const PDPixelClusters::Cluster::PtnVector &clusters );

    /// Get cluster for given pixel
    inline decltype( auto ) getCluster( const std::int16_t row, const std::int16_t col ) const
      noexcept
    {
      return ( isOn( row, col ) ? ( m_clusters )[index( row, col )] : nullptr );
    }

    /// Set cluster for given pixel
    inline void setCluster( const LHCb::RichSmartID   id,
                            const std::int16_t        row,
                            const std::int16_t        col,
                            PDPixelClusters::Cluster *clus ) noexcept
    {
      // Set the pointer to the cluster for this (row,col)
      ( m_clusters )[index( row, col )] = clus;
      // save this hit to the list of pixels for this cluster
      clus->addPixel( id );
    }

    /// Create a new cluster with unique ID
    inline decltype( auto ) createNewCluster() { return m_pdClus.createNewCluster( ++m_lastID ); }

    /** Create a new cluster with given ID
     *  @param clus1 First cluster to merge
     *  @param clus2 Second cluster to merge
     *  @attention clus1 and clus2 should both be considered invalid after the merger.
     *  @return A pointer to the merged cluster object.
     */
    PDPixelClusters::Cluster *mergeClusters( PDPixelClusters::Cluster *&clus1,
                                             PDPixelClusters::Cluster *&clus2 );

  public:

    /// Print in a human readable way
    MsgStream &fillStream( MsgStream &os ) const;

  protected: // methods

    /// Allocate storage
    inline void allocate()
    {
      const auto S = nPixelRows() * nPixelCols();
      m_data.reset( new bool[S] );
      memset( m_data.get(), 0, S );
      m_clusters.reset( new PDPixelClusters::Cluster *[S] );
      memset( m_clusters.get(), 0, S );
    }

    /// get index from row and col numbers
    inline std::int16_t index( const std::int16_t row, const std::int16_t col ) const noexcept
    {
      return ( row * nPixelCols() ) + col;
    }

    /// Set given col and row on
    inline void setOn( const std::int16_t row, const std::int16_t col ) noexcept
    {
      ( m_data )[index( row, col )] = true;
    }

    /// Check if given row and col is on
    inline bool isOn( const std::int16_t row, const std::int16_t col ) const noexcept

    {
      return ( row >= 0 && row < nPixelRows() && col >= 0 && col < nPixelCols() &&
               ( m_data )[index( row, col )] );
    }

    /// Remove a cluster
    void removeCluster( PDPixelClusters::Cluster *clus );

  public:

    /// Overload output to MsgStream
    friend inline MsgStream &operator<<( MsgStream &os, const PDPixelClustersBuilder &b )
    {
      return b.fillStream( os );
    }

  protected:

    /// The list of clusters to fill
    PDPixelClusters m_pdClus;

    /// working variable, storing the last used cluster ID
    std::int16_t m_lastID { 0 };

    /// Number of rows
    std::int16_t m_nPixRows { 0 };

    /// Number of columns
    std::int16_t m_nPixCols { 0 };

    /// Raw input data (row,col) (false means no hit, true means hit)
    std::unique_ptr< bool[] > m_data;

    /// Assigned cluster for each pixel (row,col)
    std::unique_ptr< PDPixelClusters::Cluster *[] > m_clusters;
  };

  //-----------------------------------------------------------------------------
  /** @class HPDPixelClustersBuilder RichPixelCluster.h
   *
   *  Utility class used to build a set of clusters for HPDs
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/05/2018
   */
  //-----------------------------------------------------------------------------
  class HPDPixelClustersBuilder final : virtual public PDPixelClustersBuilder
  {

  public:

    /// Construct for HPDs
    HPDPixelClustersBuilder()
    {
      // Set sizes for LHCb mode HPDs
      m_nPixRows = Rich::DAQ::HPD::MaxDataSize;
      m_nPixCols = Rich::DAQ::HPD::NumPixelColumns;
      // init data arays
      allocate();
    }

  public:

    /// Initialise for a new PD
    void initialise( const PDPixelCluster::SmartIDVector &smartIDs ) override;

  private:

    /** Are we in LHCb or ALICE mode ?
     *  @return Boolean indicating data mode
     *  @retval TRUE  ALICE mode
     *  @retval FALSE LHCb mode
     */
    inline bool aliceMode() const noexcept { return m_aliceMode; }

    /** Set the mode (LHCb or ALICE)
     *  @param mode Boolean indicating if we are in ALICE(true) or LHCb(false) mode
     */
    inline void setAliceMode( const bool mode ) noexcept
    {
      // do we need to re-allocate ?
      const bool reallocate = m_aliceMode != mode;
      if ( reallocate )
      {
        // set flag
        m_aliceMode = mode;
        // set data size
        m_nPixRows =
          ( m_aliceMode ? Rich::DAQ::HPD::MaxDataSizeALICE : Rich::DAQ::HPD::MaxDataSize );
        // reallocate storage
        allocate();
      }
    }

  private:

    /// Are we in ALICE mode ?
    bool m_aliceMode { false };
  };

  //-----------------------------------------------------------------------------
  /** @class PMTPixelClustersBuilder RichPixelCluster.h
   *
   *  Utility class used to build a set of clusters for PMTs
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/05/2018
   */
  //-----------------------------------------------------------------------------
  class PMTPixelClustersBuilder final : virtual public PDPixelClustersBuilder
  {

  public:

    /// Construct for PMTs
    PMTPixelClustersBuilder()
    {
      // Set sizes for LHCb mode PMTs
      m_nPixRows = Rich::DAQ::PMT::NumPixelRows;
      m_nPixCols = Rich::DAQ::PMT::NumPixelColumns;
      // init data arays
      allocate();
    }

  public:

    /// Initialise for a new PD
    void initialise( const PDPixelCluster::SmartIDVector &smartIDs ) override;
  };

} // namespace Rich
