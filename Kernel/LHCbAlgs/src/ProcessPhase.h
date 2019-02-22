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
// $Id: ProcessPhase.h,v 1.2 2005-01-28 13:18:04 cattanem Exp $
#ifndef PROCESSPHASE_H
#define PROCESSPHASE_H

// Include files
#include "GaudiAlg/GaudiSequencer.h"

// Typedefs
typedef std::vector<std::string> VectorName;

/** @class ProcessPhase ProcessPhase.h LHCbAlgs/ProcessPhase.h
 *
 * Processing phase of LHCb application
 *
 * Creates and invokes subdetector processing algorithms
 * Convention: algorithm name = \<phase name\> + \<detector name\>
 *
 * @author: Marco Cattaneo
 * @date:   17th December 1999
 */

class ProcessPhase final : public GaudiSequencer {

public:
  ProcessPhase( const std::string& name, ISvcLocator* svcloc ); ///> Constructor
  virtual ~ProcessPhase() = default;                            ///> Destructor

  StatusCode initialize() override; ///> Create and initialise sequences of this phase

private:
  VectorName m_detList; ///> List of subdetectors to be processed
};

#endif // PROCESSPHASE_H
