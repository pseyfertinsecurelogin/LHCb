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
#ifndef TTDETECTORPLOT_H 
#define TTDETECTORPLOT_H 1

/** @class TTDetectorPlot TTDetectorPlot.h Kernel/TTDetectorPlot.h
 *
 *  class to plot detector view (ie displays the detector with
 *  one bin per sector) for the IT.
 *
 *  @author Johan Luisier
 *  @author M Needham
 *  @date   2009-07-22
 */

#include "Kernel/STHisto2DProperties.h"
#include <string>

class DeTTSector;

namespace ST
{
  class TTDetectorPlot: public ST::Histo2DProperties {

public:

    struct Bins{
      int  xBin;
      int beginBinY;
      int endBinY;
    };

    TTDetectorPlot(const std::string& name, const std::string& title,
                   const unsigned int xBinsPerSector = 1u);

    ~TTDetectorPlot();
    typedef Bins Bins;
    Bins toBins(const DeTTSector* sector) const;
         
  };
}

#endif // STDETECTORPLOT_H 
