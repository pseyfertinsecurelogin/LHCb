###############################################################################
# (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
import cppyy

from Configurables import (
    ApplicationMgr,
    LHCbApp,
    ODINEmulator,
    createODIN,
)
import GaudiPython as GP
from PRConfig import TestFileDB

LUMI_BIT = cppyy.gbl.LHCb.ODIN.Lumi
LUMI_FRACTION = 0.25
NOBIAS_BIT = cppyy.gbl.LHCb.ODIN.NoBias
NOBIAS_FRACTION = 0.5

creator = createODIN()
emulator = ODINEmulator(
    # Set our accept fractions for p-p crossings
    AcceptFractions={
        "Lumi": [0, 0, 0, LUMI_FRACTION],
        "NoBias": [0, 0, 0, NOBIAS_FRACTION],
    },
    OutputODINLocation="/Event/DAQ/ODINEmulated")

# Define the sequence
ApplicationMgr().TopAlg = [
    creator,
    emulator,
]

app = LHCbApp()
app.EvtMax = 1000

f = TestFileDB.test_file_db[
    "2017HLTCommissioning_BnoC_MC2015_Bs2PhiPhi_L0Processed"]
f.setqualifiers(configurable=app)
f.run(configurable=app, withDB=True)

print(app)

appMgr = GP.AppMgr()
TES = appMgr.evtsvc()

event = 0
nlumi = nnobias = 0
while event < app.EvtMax:
    odin = TES[str(emulator.InputODINLocation)]
    if not odin:
        event += 1
        appMgr.run(1)
        continue

    # In MC, the Lumi and NoBias bits are never set, and all events are p-p
    # crossings
    assert odin.eventType() & LUMI_BIT == 0, "Unexpected Lumi bit"
    assert odin.eventType() & NOBIAS_BIT == 0, "Unexpected NoBias bit"
    assert odin.bunchCrossingType(
    ) == cppyy.gbl.LHCb.ODIN.BeamCrossing, "Unexpected non beam-beam crossing"

    emulated = TES[str(emulator.OutputODINLocation)]
    assert emulated, "Emulated ODIN not found"
    if emulated.eventType() & LUMI_BIT:
        nlumi += 1
    if emulated.eventType() & NOBIAS_BIT:
        nnobias += 1

    event += 1
    appMgr.run(1)

assert nlumi > 0, "No emulated Lumi events found"
assert nnobias > 0, "No emulated NoBias events found"
