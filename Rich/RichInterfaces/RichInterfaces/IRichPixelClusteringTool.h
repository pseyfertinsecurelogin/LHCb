
//-----------------------------------------------------------------------------
/** @file IRichPixelClusteringTool.h
 *
 *  Header file for tool interface : Rich::DAQ::IPixelClusteringTool
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-07-31
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <memory>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Kernel
#include "Kernel/RichSmartID.h"

// RICH Utils
#include "RichUtils/RichPixelCluster.h"

namespace Rich::DAQ
{

  //-----------------------------------------------------------------------------
  /** @class IPixelClusteringTool IRichPixelClusteringTool.h RichKernel/IRichPixelClusteringTool.h
   *
   *  Interface to tools that perform RICH PD pixel clustering
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   15/03/2002
   */
  //-----------------------------------------------------------------------------

  class IPixelClusteringTool : public virtual IAlgTool
  {

  public:
    /// Interface ID
    DeclareInterfaceID( IPixelClusteringTool, 1, 0 );

    /** Applies pixel clustering to the given PD RichSmartIDs
     *
     *  @param smartIDs Reference to vector of pixel RichSmartIDs to cluster
     *
     *  @return A smart unique pointer to the set of clusters for this PD
     *
     *  @attention The user takes ownership of the returned object.
     */
    virtual std::unique_ptr< const Rich::PDPixelClusters >
    findClusters( const LHCb::RichSmartID::Vector &smartIDs ) const = 0;
  };

} // namespace Rich::DAQ
