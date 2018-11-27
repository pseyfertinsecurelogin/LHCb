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
#pragma once

/**
 * An abstract interface to any object exhibiting a cover in a shpae of a box
 */
class IBoxCover {

public:

  /// accessor to "minimal x" value
  inline double xMin() const { return m_xMin; }
  /// accessor to "maximal x" value
  inline double xMax() const { return m_xMax; }

  /// accessor to "minimal y" value
  inline double yMin() const { return m_yMin; }
  /// accessor to "maximal y" value
  inline double yMax() const { return m_yMax; }

  /// accessor to "minimal z" value
  inline double zMin() const { return m_zMin; }
  /// accessor to "maximal z" value
  inline double zMax() const { return m_zMax; }


  /// accessor to "minimal x" value
  inline void setXMin(double xMin) { m_xMin = xMin; }
  /// accessor to "maximal x" value
  inline void setXMax(double xMax) { m_xMax = xMax; }

  /// accessor to "minimal y" value
  inline void setYMin(double yMin) { m_yMin = yMin; }
  /// accessor to "maximal y" value
  inline void setYMax(double yMax) { m_yMax = yMax; }

  /// accessor to "minimal z" value
  inline void setZMin(double zMin) { m_zMin = zMin; }
  /// accessor to "maximal z" value
  inline void setZMax(double zMax) { m_zMax = zMax; }

private:

  double m_xMin = 1000000.;
  double m_xMax = -1000000.;
  double m_yMin = 1000000.;
  double m_yMax = -1000000.;
  double m_zMin = 1000000.;
  double m_zMax = -1000000.;

};
