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
#ifndef PHISTRIPCATEGORYMAP_H
#define PHISTRIPCATEGORYMAP_H 1

#include <vector>

namespace Velo {
  /** @class PhiStripCategoryMap PhiStripCategoryMap.h
   *
   *  This class maps each channel (i.e. not strip) number
   *  to one category of LHCb Velo phi sennsor strips.  The
   *  categories differ by strip lengths and routing lines.
   *  The distinction is necessary in the computation of the
   *  common mode because there common mode noise contributions
   *  different for each category.
   *
   *  Determination of the category is purely algorithmic, relying
   *  on hardwired numbers.
   *
   *  @author Kurt Rinnert
   *  @date   2006-06-20
   */
  class PhiStripCategoryMap {
  public:
    PhiStripCategoryMap() {
      m_map[0]  = 0;
      m_map[1]  = 1;
      m_map[2]  = 2;
      m_map[3]  = 2;
      m_map[4]  = 0;
      m_map[5]  = 0;
      m_map[6]  = 1;
      m_map[7]  = 2;
      m_map[8]  = 1;
      m_map[9]  = 0;
      m_map[10] = 2;
      m_map[11] = 1;
    }

    ~PhiStripCategoryMap() { ; }

    /// get the number of categories
    inline unsigned int nCategories() const;

    /// get the category for a given channel
    inline unsigned int categoryByChannel( unsigned int channel ) const;

  private:
    unsigned int m_map[12];
  };

  unsigned int PhiStripCategoryMap::nCategories() const { return 3; }

  unsigned int PhiStripCategoryMap::categoryByChannel( unsigned int channel ) const { return m_map[channel % 12]; }
} // namespace Velo
#endif // PHISTRIPCATEGORYMAP_H
