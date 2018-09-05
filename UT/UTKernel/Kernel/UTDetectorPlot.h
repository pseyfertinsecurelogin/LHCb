/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef UTDETECTORPLOT_H 
#define UTDETECTORPLOT_H 1

/** @class UTDetectorPlot UTDetectorPlot.h Kernel/UTDetectorPlot.h
 *
 *  class to plot detector view (ie displays the detector with
 *  one bin per sector) for the IT.
 *
 *  @author Andy Beiter based on code by:
 *  @author Johan Luisier
 *  @author M Needham
 *  @date   2018-09-04
 */

#include "Kernel/UTHisto2DProperties.h"
#include <string>

class DeUTSector;

namespace UT
{
  class UTDetectorPlot: public UT::Histo2DProperties {

public:

    struct Bins{
      int  xBin;
      int beginBinY;
      int endBinY;
    };

    UTDetectorPlot(const std::string& name, const std::string& title,
                   const unsigned int xBinsPerSector = 1u);

    ~UTDetectorPlot();
    typedef Bins Bins;
    Bins toBins(const DeUTSector* sector) const;
         
  };
}

#endif // UTDETECTORPLOT_H 
