
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
#include <vector>
#include <list>
#include <ostream>
#include <memory>

// Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SerializeSTL.h"

// Kernel
#include "Kernel/RichSmartID.h"
#include "Kernel/FastAllocList.h"
#include "Kernel/FastAllocVector.h"

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
    using Vector = LHCb::STL::Vector<PDPixelCluster>;

  public:

    /// Default Constructor
    PDPixelCluster() = default;

  public:

    /// Constructor with reserved size
    explicit PDPixelCluster( const SmartIDVector::size_type resSize )
    { m_ids.reserve(resSize); }

    /// Constructor from a single channel (one pixel cluster) and optional DePD pointer
    explicit PDPixelCluster( const LHCb::RichSmartID & id,
                             const DeRichPD * dePD = nullptr )
      : m_side(id.panel()), m_rich(id.rich()), m_ids(1,id), m_dePD{dePD} 
    { }

    /// Copy Constructor from a vector of RichSmartIDs
    explicit PDPixelCluster( const SmartIDVector & ids,
                             const DeRichPD * dePD = nullptr ) 
      : m_ids(ids), m_dePD{dePD}
    {
      updateCachedEnums();
    }

    /// Move Constructor from a vector of RichSmartIDs
    explicit PDPixelCluster( SmartIDVector && ids,
                             const DeRichPD * dePD = nullptr ) 
      : m_ids(std::move(ids)), m_dePD{dePD}
    {
      updateCachedEnums();
    }

  public:

    /// const access to the list of PD pixels
    inline const SmartIDVector & smartIDs() const  & noexcept { return m_ids; }

    /// move access to the list of PD pixels
    inline       SmartIDVector&& smartIDs()       && noexcept { return std::move(m_ids); }

    /// The primary (seed) channel ID
    inline LHCb::RichSmartID primaryID() const noexcept
    {
      return ( !smartIDs().empty() ? smartIDs().front() : LHCb::RichSmartID() );
    }

    /// The RICH detector for this cluster
    inline Rich::DetectorType   rich() const noexcept { return m_rich; }

    /// The RICH panel for this cluster
    inline Rich::Side          panel() const noexcept { return m_side; }

    /// The RICH panel SmartID for this cluster
    inline LHCb::RichSmartID panelID() const noexcept { return primaryID().panelID(); }

    /// The RICH PD SmartID for this cluster
    inline LHCb::RichSmartID    pdID() const noexcept { return primaryID().pdID(); }

  public:

    /// Set the DeRichPD pointer
    inline void setDePD( const DeRichPD * dePD ) noexcept { m_dePD = dePD; }

    /// Access the DeRichPD object
    inline const DeRichPD * dePD() const noexcept { return m_dePD; }

  public:

    /// Number of channels in this cluster
    inline SmartIDVector::size_type size() const noexcept { return smartIDs().size(); }

    /// Is the cluster empty ?
    inline bool empty() const noexcept { return smartIDs().empty(); }

  public:

    /// Add a channel to this cluster
    inline void addChannel( const LHCb::RichSmartID& id ) noexcept
    {
      m_ids.emplace_back(id);
      updateCachedEnums();
    }

  public:

    /// overload printout to ostream operator <<
    friend inline std::ostream& operator << ( std::ostream& s,
                                              const PDPixelCluster & cluster )
    {
      return s << "[ " 
               << cluster.rich() << " "
               << Rich::text(cluster.rich(),cluster.panel()) << " "
               << (void*)cluster.dePD() << " "
               << cluster.smartIDs()
               << " ]";
    }

  private:

    /// Update the cached RICH and panel enums
    inline void updateCachedEnums() noexcept
    {
      m_side = primaryID().panel();
      m_rich = primaryID().rich();
    }

  private:

    /// Cache the RICH Side enum
    Rich::Side m_side = Rich::InvalidSide;

    /// Cache the detector enum
    Rich::DetectorType m_rich = Rich::InvalidDetector;

    /// The vector of RichSmartIDs for this cluster
    SmartIDVector m_ids;

    /// Pointer to associated DeRichPD object
    const DeRichPD * m_dePD = nullptr;

  };

  /// Pixel Cluster data locations
  namespace PDPixelClusterLocation
  {
    /// Default Location in TES for the decoded data map
    static const std::string Default = "Rec/Rich/PixelClusters/Default";
  }

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

      /// Collection of cluster pointers
      using PtnVector      = LHCb::STL::List< Cluster* >;
      /// Collection of cluster smart pointers (i.e. for ownership with memory management)
      using SmartPtnVector = LHCb::STL::List< std::unique_ptr<Cluster> >;

    public: // Constructors etc.

      /// Default Constructor
      Cluster() = default;

      /// Default destructor
      ~Cluster() = default;

      /// Default Copy Constructor
      Cluster( const Cluster& ) = default;

      /// Default Copy operator
      Cluster& operator=( const Cluster& ) = default;

      /// Default Move Constructor
      Cluster( Cluster&& ) = default;

      /// Default Move operator
      Cluster& operator=( Cluster&& ) = default;

    public: // methods

      /// Constructor from ID
      explicit Cluster( const int id ) : m_clusterID(id) { }

      /// Get cluster ID
      inline int id() const noexcept
      {
        return m_clusterID;
      }

      /// Add a pixel to this cluster
      inline void addPixel( const LHCb::RichSmartID& id )
      {
        m_cluster.addChannel( id );
      }

      /// Get read access to cluster data
      inline const Rich::PDPixelCluster & pixels() const noexcept
      {
        return m_cluster;
      }

      /// Shortcut to the cluster size
      inline LHCb::RichSmartID::Vector::size_type size() const noexcept
      {
        return pixels().smartIDs().size();
      }

    private:

      /// Cluster ID
      int m_clusterID{-1};

      /// list of pixels in this cluster. Initialize with reserved size 3 (rough guess).
      PDPixelCluster m_cluster{3};

    };

  public: // methods

    /// Constructor
    PDPixelClusters() = default;

    /// Destructor
    ~PDPixelClusters() = default;

  public:

    /// Read access to the vector of clusters
    inline const Cluster::SmartPtnVector & clusters() const noexcept { return m_allclus; }

    /// Write access to the vector of clusters
    inline       Cluster::SmartPtnVector & clusters()       noexcept { return m_allclus; }

    /// Create a new vector of suppressed RichSmartIDs
    void suppressIDs( PDPixelCluster::SmartIDVector & smartIDs,
                      const unsigned int maxSize ) const;

  public:

    /// Get the cluster for a given PD ID
    const Cluster * getCluster( const LHCb::RichSmartID & id ) const;

    /// Create and return a new cluster with the given ID
    inline Cluster * createNewCluster( const int id )
    {
      auto * clus = new PDPixelClusters::Cluster(id);
      m_allclus.emplace_back( clus );
      return clus;
    }

  public:

    /// Print in a human readable way
    MsgStream& fillStream( MsgStream& os ) const;

    /// Overload output to MsgStream
    friend inline MsgStream& operator << ( MsgStream& os,
                                           const PDPixelClusters & clus )
    { return clus.fillStream(os); }

  private: // data

    /// Vector of all created clusters
    Cluster::SmartPtnVector m_allclus;

  };

  //-----------------------------------------------------------------------------
  /** @class PDPixelClustersBuilder RichPixelCluster.h
   *
   *  Utility class used to build a set of clusters
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   21/03/2006
   */
  //-----------------------------------------------------------------------------
  class PDPixelClustersBuilder final
  {

  public:

    /// Default Constructor
    PDPixelClustersBuilder( )
    {
      memset ( m_data,     0, sizeof(m_data)     );
      memset ( m_clusters, 0, sizeof(m_clusters) );
    }

  public:

    /// Initialise for a new PD
    void initialise( PDPixelClusters * clus,
                     const PDPixelCluster::SmartIDVector & smartIDs );

  public:

    /// Maximum number of pixel columns
    inline int nPixelCols() const noexcept { return Rich::DAQ::NumPixelColumns; }

    /// Number of pixel rows (either 32 for LHCbmode or 256 for ALICE mode)
    inline int nPixelRows() const noexcept
    {
      return ( aliceMode() ? Rich::DAQ::MaxDataSizeALICE : Rich::DAQ::MaxDataSize );
    }

    /// Returns the 'correct' row number for the given RichSmartID (either LHCb or ALICE mode)
    inline int rowNumber( const LHCb::RichSmartID& id ) const noexcept
    {
      return ( !aliceMode() ? id.pixelRow() :
               ((Rich::DAQ::NumAlicePixelsPerLHCbPixel*id.pixelRow())+id.pixelSubRow()) );
    }

    /// Returns the 'correct' column number for the given RichSmartID (either LHCb or ALICE mode)
    inline int colNumber( const LHCb::RichSmartID& id ) const noexcept
    {
      return id.pixelCol();
    }

  public:

    /// Split the given clusters up into single channel clusters
    void splitClusters( const PDPixelClusters::Cluster::PtnVector & clusters );

    /// Get cluster for given pixel
    inline PDPixelClusters::Cluster * getCluster( const int row,
                                                  const int col ) const noexcept
    {
      return ( isOn(row,col) ? (m_clusters[row])[col] : nullptr );
    }

    /// Set cluster for given pixel
    inline void setCluster( const LHCb::RichSmartID & id,
                            const int row, const int col,
                            PDPixelClusters::Cluster * clus ) noexcept
    {
      // Set the pointer to the cluster for this (row,col)
      (m_clusters[row])[col] = clus;
      // save this hit to the list of pixels for this cluster
      clus->addPixel(id);
    }

    /// Create a new cluster with given ID
    inline PDPixelClusters::Cluster * createNewCluster()
    {
      return m_hpdClus->createNewCluster( ++m_lastID );
    }

    /** Create a new cluster with given ID
     *  @param clus1 First cluster to merge
     *  @param clus2 Second cluster to merge
     *  @attention clus1 and clus2 should both be considered invalid after the merger.
     *  @return A pointer to the merged cluster object.
     */
    PDPixelClusters::Cluster * mergeClusters( PDPixelClusters::Cluster *& clus1,
                                              PDPixelClusters::Cluster *& clus2 );

    /** Are we in LHCb or ALICE mode ?
     *  @return Boolean indicating data mode
     *  @retval TRUE  ALICE mode
     *  @retval FALSE LHCb mode
     */
    inline bool aliceMode() const noexcept { return m_aliceMode; }

    /** Set the mode (LHCb or ALICE)
     *  @param mode Boolean indicating if we are in ALICE(true) or LHCb(false) mode
     */
    inline void setAliceMode( const bool mode ) noexcept { m_aliceMode = mode; }

  public:

    /// Print in a human readable way
    MsgStream& fillStream( MsgStream& os ) const;

    /// Overload output to MsgStream
    friend inline MsgStream& operator << ( MsgStream& os,
                                           const PDPixelClustersBuilder & b )
    { return b.fillStream(os); }

  private: // methods

    /// Set given col and row on
    inline void setOn( const int row, const int col ) noexcept
    {
      (m_data[row])[col] = true;
    }

    /// Check if given row and col is on
    inline bool isOn( const int row, const int col ) const noexcept

    {
      return ( row>=0 && row<nPixelRows() &&
               col>=0 && col<nPixelCols() &&
               (m_data[row])[col] );
    }

    /// Remove a cluster
    void removeCluster( PDPixelClusters::Cluster * clus );

  private:

    /// The list of clusters to fill
    PDPixelClusters * m_hpdClus{nullptr};

    /// working variable, storing the last used cluster ID
    int m_lastID{0};

    /// Are we in ALICE mode ?
    bool m_aliceMode{false};

    /** Raw input data (row,col) (false means no hit, true means hit)
     *  @attention Hardcoding number of rows here to ALICE mode.
     *             In LHCb mode only the first Rich::DAQ::MaxDataSize are then used.
     */
    bool m_data[Rich::DAQ::MaxDataSizeALICE][Rich::DAQ::NumPixelColumns];

    /** Assigned cluster for each pixel (row,col)
     *  @attention Hardcoding number of rows here to ALICE mode.
     *             In LHCb mode only the first Rich::DAQ::MaxDataSize are then used.
     */
    PDPixelClusters::Cluster * m_clusters[Rich::DAQ::MaxDataSizeALICE][Rich::DAQ::NumPixelColumns];

  };

}
