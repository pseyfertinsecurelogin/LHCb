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
// $Id: $
#ifndef GENEVENT_BEAMFORINITIALIZATION_H
#define GENEVENT_BEAMFORINITIALIZATION_H 1

namespace LHCb {
  class BeamParameters;
}

/** @class BeamForInitialization BeamForInitialization.h GenEvent/BeamForInitialization.h
 *  Simple class to hold beam parameters for initialization of generators
 *
 *  @author Patrick Robbe
 *  @data   2011-05-27
 */
class BeamForInitialization {
public:
  static LHCb::BeamParameters*& getInitialBeamParameters();

protected:
private:
  static LHCb::BeamParameters* s_initialBeamParameters;
};
#endif
