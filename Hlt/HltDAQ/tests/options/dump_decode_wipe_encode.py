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

import os
if os.path.exists("rewritten.mdf"):
    print "EXISTS"
    os.system("rm rewritten.mdf")
else:
    print "NOTHING!"

LHCbApp().EvtMax = 10
dump = RawEventDump(RawBanks=[], DumpData=True)
decoders = GaudiSequencer("Decode", Members=[])
killer = bankKiller(BankTypes=[])
killer2 = bankKiller("Killer2", BankTypes=[])
encoders = GaudiSequencer("Encode", Members=[])

from Configurables import IODataManager
IODataManager().DisablePFNWarning = True

for atype in ["Dec", "Sel"
              ]:  #can't do track just yet, can be added easily, though later
    bank = "Hlt" + atype + "Reports"
    dump.RawBanks.append(bank)
    #exec("from Configurables import "+bank+"Decoder")
    decoders.Members.append(bank + "Decoder")
    killer.BankTypes.append(bank)
    encoders.Members.append(bank + "Writer")

from Configurables import HltANNSvc
HltANNSvc().Hlt1SelectionID = {'PV3D': 10103, 'ProtoPV3D': 10117}

killer2.BankTypes = killer.BankTypes + ["ODIN"]
killer2.DefaultIsKill = True

ApplicationMgr().TopAlg += [dump, decoders, killer, encoders, killer2]

#writer
from GaudiConf import IOHelper
from Configurables import LHCb__MDFWriter
IOHelper('MDF', 'MDF').outStream(
    "rewritten.mdf", LHCb__MDFWriter("writer", Compress=0), writeFSR=False)

#inputfile
from PRConfig import TestFileDB
TestFileDB.test_file_db["2012_raw_default"].run()
from GaudiConf import IOExtension
IOExtension().inputFiles(["../options/OnlyHltBanks.mdf"], clear=True)
