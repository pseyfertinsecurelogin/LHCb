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

//-----------------------------------------------------------------------------
/** @file RichHistoBase.h
 *
 *  Header file for RICH base class : Rich::HistoBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2009-07-27
 */
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>

// Local
#include "RichFutureKernel/RichCommonBase.h"
#include "RichFutureKernel/RichHistoUtils.h"

// RichUtils
#include "RichUtils/RichHashMap.h"
#include "RichUtils/RichHistoID.h"
#include "RichUtils/RichMap.h"

#include "GaudiKernel/Property.h"

// boost
#include "boost/array.hpp"

// AIDA
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
//#include "AIDA/IHistogram3D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IProfile2D.h"

namespace Rich::Future {

  // import histogram utils
  using namespace Rich::Future::HistoUtils;

  //-----------------------------------------------------------------------------
  /** @class HistoBase RichFutureKernel/RichHistoBase.h
   *
   *  Common base class for all RICH histogramming tools and algorithms.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2009-07-27
   */
  //-----------------------------------------------------------------------------

  template <class PBASE>
  class HistoBase : public CommonBase<PBASE> {

  public:
    // inherit constructors
    using CommonBase<PBASE>::CommonBase;

  protected:
    /// Histogram Constructor initisalisations
    inline void initRichHistoConstructor() {
      // Place all histograms under RICH/ sub-dir
      this->setProperty( "HistoTopDir", "RICH/" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    }

    /// Tuple Constructor initisalisations
    inline void initRichTupleConstructor() {
      // Place all tuples under RICH/ sub-dir
      this->setProperty( "NTupleTopDir", "RICH/" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      this->setProperty( "NTupleLUN", "RICHTUPLE1" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    }

  public:
    /// System initialize
    virtual StatusCode sysInitialize() override;

    // Initialization of the algorithm after creation
    virtual StatusCode initialize() override;

    // Finalization of the algorithm before deletion
    virtual StatusCode finalize() override;

  protected:
    /// Number of bins for 1D histograms
    inline unsigned int nBins1D() const noexcept { return m_nBins1D; }

    /// Number of bins for 2D histograms
    inline unsigned int nBins2D() const noexcept { return m_nBins2D; }

  private:
    /// Number of bins for 1D histograms
    Gaudi::Property<unsigned int> m_nBins1D{this, "NBins1DHistos", 100};

    /// Number of bins for 2D histograms
    Gaudi::Property<unsigned int> m_nBins2D{this, "NBins2DHistos", 50};

  protected:
    /** @brief Place to book all histograms which must be present after initialisation
     *
     *  Useful for online monitoring where booking on-demand does not play well with
     *  the online histogram systems
     *
     *  This method is automatically called during initialisation. It is not neccessary
     *  to call it by hand. Simply implement this method in any monitoring code which
     *  needs to pre-book histograms.
     *
     *  @return StatusCode indicating if booking was successful
     */
    virtual StatusCode prebookHistograms();

  protected:
    // definitions

    /// short name for bin labels
    using BinLabels = std::vector<std::string>;
    /// short name for axis label
    using AxisLabel = std::string;

  protected:
    /** Book a 1D histogram
     *
     * @param id         Histogram identifier
     * @param title      Histogram title
     * @param low        Lower histogram edge
     * @param high       Upper histogram edge
     * @param bins       Number of bins
     * @param xAxisLabel Label for the X Axis
     * @param yAxisLabel Label for the Y Axis
     * @param binLabels  Labels for the X axis bins
     *
     * @return Pointer to booked histogram
     */
    AIDA::IHistogram1D* richHisto1D( const Rich::HistogramID& id,              //
                                     const std::string&       title,           //
                                     const double             low,             //
                                     const double             high,            //
                                     const unsigned int       bins,            //
                                     const AxisLabel&         xAxisLabel = "", //
                                     const AxisLabel&         yAxisLabel = "", //
                                     const BinLabels&         binLabels  = BinLabels() );

    /** Book a 2D histogram
     *
     * @param id         Histogram identifier
     * @param title      Histogram title
     * @param lowX       Lower histogram edge in X
     * @param highX      Upper histogram edge in X
     * @param binsX      Number of bins in X
     * @param lowY       Lower histogram edge in Y
     * @param highY      Upper histogram edge in Y
     * @param binsY      Number of bins in Y
     * @param xAxisLabel Label for the X Axis
     * @param yAxisLabel Label for the Y Axis
     * @param zAxisLabel Label for the Z Axis
     * @param xBinLabels Labels for the X axis bins
     * @param yBinLabels Labels for the Y axis bins
     *
     * @return Pointer to booked histogram
     */
    AIDA::IHistogram2D* richHisto2D( const Rich::HistogramID& id,                       //
                                     const std::string&       title,                    //
                                     const double             lowX,                     //
                                     const double             highX,                    //
                                     const unsigned int       binsX,                    //
                                     const double             lowY,                     //
                                     const double             highY,                    //
                                     const unsigned int       binsY,                    //
                                     const AxisLabel&         xAxisLabel = "",          //
                                     const AxisLabel&         yAxisLabel = "",          //
                                     const AxisLabel&         zAxisLabel = "",          //
                                     const BinLabels&         xBinLabels = BinLabels(), //
                                     const BinLabels&         yBinLabels = BinLabels() );

    /** Book a 1D profile histogram
     *
     * @param id         Histogram identifier
     * @param title      Histogram title
     * @param low        Lower hisstogram edge
     * @param high       Upper histogram edge
     * @param bins       Number of bins
     * @param xAxisLabel Label for the X Axis
     * @param yAxisLabel Label for the Y Axis
     * €param binLabels  Labels for the X axis bins
     *
     * @return Pointer to booked histogram
     */
    AIDA::IProfile1D* richProfile1D( const Rich::HistogramID& id,              //
                                     const std::string&       title,           //
                                     const double             low,             //
                                     const double             high,            //
                                     const unsigned int       bins,            //
                                     const AxisLabel&         xAxisLabel = "", //
                                     const AxisLabel&         yAxisLabel = "", //
                                     const BinLabels&         binLabels  = BinLabels() );

    /** Book a 2D profile histogram
     *
     * @param id         Histogram identifier
     * @param title      Histogram title
     * @param lowX       Lower histogram edge in X
     * @param highX      Upper histogram edge in X
     * @param binsX      Number of bins in X
     * @param lowY       Lower histogram edge in Y
     * @param highY      Upper histogram edge in Y
     * @param binsY      Number of bins in Y
     * @param xAxisLabel Label for the X Axis
     * @param yAxisLabel Label for the Y Axis
     * @param zAxisLabel Label for the Z Axis
     * @param xBinLabels Labels for the X axis bins
     * @param yBinLabels Labels for the Y axis bins
     *
     * @return Pointer to booked histogram
     */
    AIDA::IProfile2D* richProfile2D( const Rich::HistogramID& id,                       //
                                     const std::string&       title,                    //
                                     const double             lowX,                     //
                                     const double             highX,                    //
                                     const unsigned int       binsX,                    //
                                     const double             lowY,                     //
                                     const double             highY,                    //
                                     const unsigned int       binsY,                    //
                                     const AxisLabel&         xAxisLabel = "",          //
                                     const AxisLabel&         yAxisLabel = "",          //
                                     const AxisLabel&         zAxisLabel = "",          //
                                     const BinLabels&         xBinLabels = BinLabels(), //
                                     const BinLabels&         yBinLabels = BinLabels() );

    //-----------------------------------------------------------------------------------------

  private:
    // Types for histogram lookup

    /** @class HistoMap RichFutureKernel/RichHistoBase.h
     *
     *  Private class to implement mapping between RICH classes and histograms
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   2009-07-27
     */
    template <class HTYPE>
    class HistoMap {
    public:
      using StringToHist = Rich::HashMap<std::string, HTYPE*>;

    private:
      using Map = Rich::Map<Rich::PackedPIDInfo::Pack32_t, StringToHist>;

    private:
      Map m_map;

    public:
      inline StringToHist&       getmap( const Rich::PackedPIDInfo& info ) { return m_map[info.raw()]; }
      inline const StringToHist& getmap( const Rich::PackedPIDInfo& info ) const {
        static const StringToHist defaultMap;
        const auto                i = m_map.find( info.raw() );
        return ( i != m_map.end() ? i->second : defaultMap );
      }
    };

    /// Mapping between IDs and 1D histogram pointers
    using Map1DH = HistoMap<AIDA::IHistogram1D>;

    /// Mapping between IDs and 2D histogram pointers
    using Map2DH = HistoMap<AIDA::IHistogram2D>;

    /// Mapping between IDs and 1D Profile histogram pointers
    using Map1DP = HistoMap<AIDA::IProfile1D>;

    /// Mapping between IDs and 2D Profile histogram pointers
    using Map2DP = HistoMap<AIDA::IProfile2D>;

  private:
    /// Flag to indicate if histograms have been booked or not
    bool m_histosAreBooked{false};

    /// 1D histo map
    Map1DH m_1dhmap;

    /// 2D histo map
    Map2DH m_2dhmap;

    /// 1D Profile map
    Map1DP m_1dpmap;

    /// 2D Profile map
    Map2DP m_2dpmap;

  protected:
    //-----------------------------------------------------------------------------------------

    /** Access 1D histogram by id
     *
     * @param id      Histogram identifier
     *
     * @return Pointer to booked histogram
     */
    AIDA::IHistogram1D* richHisto1D( const Rich::HistogramID& id ) const;

    /** Access 2D histogram by id
     *
     * @param id      Histogram identifier
     *
     * @return Pointer to booked histogram
     */
    AIDA::IHistogram2D* richHisto2D( const Rich::HistogramID& id ) const;

    /** Access 1D profile histogram by id
     *
     * @param id      Histogram identifier
     *
     * @return Pointer to booked histogram
     */
    AIDA::IProfile1D* richProfile1D( const Rich::HistogramID& id ) const;

    /** Access 2D profile histogram by id
     *
     * @param id      Histogram identifier
     *
     * @return Pointer to booked histogram
     */
    AIDA::IProfile2D* richProfile2D( const Rich::HistogramID& id ) const;

    //-----------------------------------------------------------------------------------------
  };

} // namespace Rich::Future
