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
#ifndef DICT_MCINTERFACESDICT_H 
#define DICT_MCINTERFACESDICT_H 1

// Previously in RichKernel
#include "MCInterfaces/IRichMCTrackInfoTool.h"
#include "MCInterfaces/IRichMCTruthTool.h"

// Previously in TrackMCInterfaces
#include "MCInterfaces/IIdealStateCreator.h"
#include "MCInterfaces/ITrackGhostClassification.h"
#include "MCInterfaces/ILHCbIDsToMCParticles.h"
#include "MCInterfaces/ILHCbIDsToMCHits.h"

// Previously in MCEvent
#include "MCInterfaces/IForcedBDecayTool.h"
#include "MCInterfaces/IMCDecayFinder.h"
#include "MCInterfaces/IMCReconstructible.h"
#include "MCInterfaces/IMuonPad2MCTool.h"
#include "MCInterfaces/ISiDepositedCharge.h"
#include "MCInterfaces/IVisPrimVertTool.h"

// Previously in LHCbKernel
#include "MCInterfaces/IMCParticleSelector.h"

// Previously in DaVinciMCTools
#include "MCInterfaces/IPrintMCDecayTreeTool.h"

// New since merge
#include "MCInterfaces/IMCEventTypeFinder.h"
#include "MCInterfaces/IMCReconstructed.h"

#include "MCInterfaces/IHepMCParticleSelector.h"
#include "MCInterfaces/IGenCutTool.h"
#include "MCInterfaces/IFullGenEventCutTool.h"

#endif // DICT_MCINTERFACESDICT_H

