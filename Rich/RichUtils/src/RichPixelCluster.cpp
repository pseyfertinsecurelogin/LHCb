
//--------------------------------------------------------------------------------
/** @file RichPixelCluster.cpp
 *
 *  Implementation file for pixel clustering class Rich::DAQ::PixelCluster
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   02/02/2007
 */
//--------------------------------------------------------------------------------

// local
#include "RichUtils/RichPixelCluster.h"
#include "RichUtils/RichException.h"

// boost
#include "boost/format.hpp"

//#include <cstring> // for memset with gcc 4.3

using namespace Rich;

//=========================================================================================
// Methods for the builders
//=========================================================================================

void
HPDPixelClustersBuilder::
initialise( const PDPixelCluster::SmartIDVector & smartIDs )
{
  // reset the first cluster ID to 0
  m_lastID  = 0;
  // clear the pixel data
  m_pdClus.clear();
  // reserve size in data container for max possible clusters.
  // Neccessary to avoid reallocation later on that would
  // invalidate iterators etc.
  m_pdClus.reserve( smartIDs.size() );

  // use the smartIDs to set the active pixels
  if ( !smartIDs.empty() )
  {

    // What mode are we in ?
    // note, assuming here that there is NOT a mixture of LHCb and ALICE mode hits
    // as this does not make sense (and likely not even technically possible from L1)
    setAliceMode( smartIDs.front().pixelSubRowDataIsValid() );

    // Initialise the 'is set' array as required.
    memset ( m_data.get(), 0, nPixelRows() * nPixelCols() );

    // set the hit pixels as "on"
    for ( const auto S : smartIDs )
    {
      // double check all pixels are in same mode
      if ( UNLIKELY( aliceMode() != S.pixelSubRowDataIsValid() ) )
      {
        throw Rich::Exception( "Invalid mix of LHCb and ALICE mode RichSmartIDs" );
      }
      setOn( rowNumber(S), colNumber(S) );
    }

  }

}

void
PMTPixelClustersBuilder::
initialise( const PDPixelCluster::SmartIDVector & smartIDs )
{
  // reset the first cluster ID to 0
  m_lastID  = 0;
  // clear the pixel data
  m_pdClus.clear();
  // reserve size in data container for max possible clusters.
  // Neccessary to avoid reallocation later on that would
  // invalidate iterators etc.
  m_pdClus.reserve( smartIDs.size() );
  // use the smartIDs to set the active pixels
  if ( !smartIDs.empty() )
  {
    // Initialise the 'is set' array as required.
    memset ( m_data.get(), 0, nPixelRows() * nPixelCols() );
    // set the hit pixels as "on"
    for ( const auto S : smartIDs )
    { setOn( rowNumber(S), colNumber(S) ); }
  }
}

PDPixelClusters::Cluster *
PDPixelClustersBuilder::mergeClusters( PDPixelClusters::Cluster *& clus1,
                                       PDPixelClusters::Cluster *& clus2 )
{
  // add pixels in clus2 to clus1
  // primary ID
  const auto p2 = clus2->pixels().primaryID();
  setCluster( p2, rowNumber(p2), colNumber(p2), clus1 );
  // secondary pixels
  for ( const auto id : clus2->pixels().secondaryIDs() )
  {
    setCluster( id, rowNumber(id), colNumber(id), clus1 );
  }
  // delete clus2
  removeCluster( clus2 );
  // invalidate the now deleted cluster
  clus2 = nullptr;
  // return clus1 as merged cluster
  return clus1;
}

MsgStream& PDPixelClustersBuilder::fillStream ( MsgStream & os ) const
{

  // column numbers
  os << " c     |";
  for ( std::int16_t col = 0; col < nPixelCols(); ++col )
  {
    os << boost::format("%3i") % col ;
  }
  os << endmsg;

  // print out each row
  for ( std::int16_t row = 0; row < nPixelRows(); ++row )
  {
    os << boost::format( " r %3i | " ) % row ;
    for ( std::int16_t col = 0; col < nPixelCols(); ++col )
    {
      const auto clus = getCluster(row,col);
      if ( clus ) { os << boost::format("%2i ") % clus->id(); }
      else        { os << " . "; }
    }
    os << endmsg;
  }

  return os;
}

void
PDPixelClustersBuilder::
splitClusters( const PDPixelClusters::Cluster::PtnVector & clusters )
{
  // loop over the clusters to break up
  for ( auto C : clusters )
  {
    // primary ID
    const auto p2 = C->pixels().primaryID();
    setCluster( p2, rowNumber(p2), colNumber(p2), createNewCluster() );
    // secondary IDs
    for ( const auto & S : C->pixels().secondaryIDs() )
    {
      // for each ID, make a single channel new cluster
      setCluster( S, rowNumber(S), colNumber(S), createNewCluster() );
    }
    // remove and delete original cluster
    removeCluster( C );
  }
}

void
PDPixelClustersBuilder::removeCluster( PDPixelClusters::Cluster * clus )
{
  const auto iF = std::find_if( m_pdClus.clusters().begin(),
                                m_pdClus.clusters().end(),
                                [clus]( const auto & c )
                                { return &c == clus; } );
  if ( iF != m_pdClus.clusters().end() )
  {
    // for list, remove
    //m_pdClus.clusters().erase( iF );
    // for vector, just make empty
    (*iF).clear();
  }
}

//=========================================================================================
// Methods for the cluster classs
//=========================================================================================

void PDPixelCluster::addChannel( const LHCb::RichSmartID id ) noexcept
{
  if ( empty() ) 
  {
    // primary ID is not set, so use this one
    m_primaryID = id;
    // Update the cached RICH and panel emuns
    updateCachedEnums();
    // set as no longer empty
    m_empty = false;
  }
  else                
  {
    // add to secondary IDs 
    // If first time reserve rough guess at size.
    if ( m_secondaryIDs.empty() ) { m_secondaryIDs.reserve(8); }
    // add to the secondary ID list
    m_secondaryIDs.emplace_back(id);
  }
}

void
PDPixelClusters::suppressIDs( PDPixelCluster::SmartIDVector & smartIDs,
                              const unsigned int maxSize ) const
{
  // make a local copy of the orginal IDs
  const auto cache_ids(smartIDs);
  // clear the list
  smartIDs.clear();
  // refill the list with those not suppressed
  for ( const auto& S : cache_ids )
  {
    const auto * c = getCluster(S);
    if ( c && c->size() <= maxSize ) { smartIDs.emplace_back(S); }
  }
}

const PDPixelClusters::Cluster *
PDPixelClusters::getCluster( const LHCb::RichSmartID id ) const
{
  // should look to implement this better sometime...

  // cluster pointer to return
  const Cluster * clus = nullptr;

  // loop over clusters
  for ( const auto& c : clusters() )
  {
    // primary ID
    if ( c.pixels().primaryID() == id )
    {
      // found the cluster...
      clus = &c;
      break;
    }
    // now try the secondary IDs
    for ( const auto& i : c.pixels().secondaryIDs() )
    {
      if ( i == id )
      {
        // found the cluster...
        clus = &c;
        break;
      }
    }
    if ( clus ) { break; }
  }

  // return the found cluster
  return clus;
}

MsgStream& PDPixelClusters::fillStream( MsgStream& os ) const
{
  for ( const auto& clus : clusters() )
  {
    os << "ID=" << clus.id() << " " << clus.pixels();
  }
  return os;
}
