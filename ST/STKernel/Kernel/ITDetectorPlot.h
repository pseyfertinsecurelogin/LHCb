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
#ifndef ITDETECTORPLOT_H
#define ITDETECTORPLOT_H 1

/** @class ITDetectorPlot ITDetectorPlot.h Kernel/ITDetectorPlot.h
 *
 *  class to plot detector view (ie displays the detector with
 *  one bin per sector) for the IT.
 *
 *  @author Johan Luisier
 *  @author M Needham
 *  @date   2009-07-22
 *  Modified by Mark Tobin so that the number of bins per sector can be
 *  given as an input where 3 (12) bins displays per Beetle (Port)
 */

#include "Kernel/STChannelID.h"
#include "Kernel/STHisto2DProperties.h"
#include <string>

namespace ST {
  class ITDetectorPlot : public ST::Histo2DProperties {

  public:
    struct Bins {
      double xBin;
      int    yBin;
    };

    ITDetectorPlot( const std::string& name, const std::string& title, const unsigned int xBinsPerSector = 1u );

    ~ITDetectorPlot();
    typedef Bins Bins;
    Bins         toBins( const LHCb::STChannelID& chan ) const;

  private:
    bool m_plotByPort;
    bool m_plotByBeetle;
    int  m_xBinsPerSector;
  };
} // namespace ST

#endif // STDETECTORPLOT_H
