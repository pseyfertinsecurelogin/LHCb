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
from Gaudi.Configuration import *
from Configurables import bankKiller, RawEventDump, GaudiSequencer
from Configurables import LHCbApp

LHCbApp().EvtMax = 10

dump = RawEventDump(RawBanks=[], DumpData=True)

from Configurables import IODataManager
IODataManager().DisablePFNWarning = True

for atype in ["Dec", "Sel"
              ]:  #can't do track just yet, can be added easily, though later
    bank = "Hlt" + atype + "Reports"
    dump.RawBanks.append(bank)

ApplicationMgr().TopAlg += [dump]

#inputfile
from PRConfig import TestFileDB
TestFileDB.test_file_db["2012_raw_default"].run()
from GaudiConf import IOExtension
IOExtension().inputFiles(["rewritten.mdf"], clear=True)
