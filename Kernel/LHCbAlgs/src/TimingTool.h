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
// $Id: TimingTool.h,v 1.3 2005-09-07 12:23:40 cattanem Exp $
#ifndef TIMINGTOOL_H
#define TIMINGTOOL_H 1

// Include files

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/Chrono.h"
#include "GaudiKernel/IRndmGenSvc.h"

// from LHCbKernel
#include "Kernel/INormalizeTool.h"


/** @class TimingTool TimingTool.h
 *  Tool to get a timing normalisation for the Chrono service
 *  Adapted from BrunelAlgs/TimingAlg by M.Ferro-Luzzi
 *
 *  @author Marco Cattaneo
 *  @date   2003-11-04
 */
class TimingTool final : public GaudiTool,
                         virtual public INormalizeTool
{

public:

  /// Standard constructor
  TimingTool( const std::string& type,
              const std::string& name,
              const IInterface* parent);

  virtual ~TimingTool( ) = default; ///< Destructor

  StatusCode finalize() override;

  double normalize() override;    ///< Return normalization (in ns)
  IChronoStatSvc* chronoSvc(); ///< Returns pointer to ChronSvc
  IRndmGenSvc*    randSvc();   ///< Returns pointer to random number service

private:

  unsigned int  m_shots; ///< Number of random number shots for normalisation
  IChronoStatSvc* m_CSS = nullptr; ///< Pointer to Chrono service
  IRndmGenSvc*    m_RGS = nullptr; ///< Pointer to Random numbers service

};

#endif // TIMINGTOOL_H
