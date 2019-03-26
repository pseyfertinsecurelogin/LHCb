###############################################################################
# (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
from Configurables import LHCbApp
from Gaudi.Configuration import *

LHCbApp().EvtMax = 100

from PRConfig import TestFileDB
TestFileDB.test_file_db["2012_raw_default"].run()

from Configurables import EventCountAlg, PostScaler
ApplicationMgr().TopAlg += [
    EventCountAlg(), PostScaler(),
    GaudiSequencer("WAAAATUP")
]

from Configurables import LHCbTimingAuditor, LHCbSequencerTimerTool, TimingAuditor, AuditorSvc

LHCbTimingAuditor().addTool(LHCbSequencerTimerTool, name="TIMER")
LHCbTimingAuditor().TIMER.SummaryFile = "timing.dat"
LHCbTimingAuditor().TIMER.OutputLevel = DEBUG
LHCbTimingAuditor().TIMER.NameSize = 2
ApplicationMgr().ExtSvc.append('AuditorSvc')
AuditorSvc().Auditors.append(LHCbTimingAuditor())
LHCbTimingAuditor().Enable = True
LHCbTimingAuditor().OutputLevel = VERBOSE
AuditorSvc().Auditors.append(TimingAuditor())

TimingAuditor().Enable = True

from Configurables import IODataManager
IODataManager().DisablePFNWarning = True
