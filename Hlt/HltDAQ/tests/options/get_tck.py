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
import os, re
import argparse

parser = argparse.ArgumentParser(usage='usage: %(prog)s file')

parser.add_argument(
    "--odin",
    action='store_true',
    dest="odin",
    default=False,
    help="Use ODIN to get the TCK")
parser.add_argument("file", nargs=1)

args = parser.parse_args()

# General configuration
from GaudiConf import IOHelper
from Gaudi.Configuration import *
from Configurables import LHCbApp
app = LHCbApp()
app.EvtMax = -1
app.DataType = '2015'
app.CondDBtag = 'cond-20150828'
app.DDDBtag = 'dddb-20150724'
# app.Simulation = True

IOHelper("MDF").inputFiles(args.file)

# Top level sequence
topSeq = GaudiSequencer("TopSequence")

from DAQSys.Decoders import DecoderDB
for dec in ("HltDecReportsDecoder/Hlt1DecReportsDecoder",
            "HltDecReportsDecoder/Hlt2DecReportsDecoder"):
    decoder = DecoderDB[dec].setup()
    decoder.OutputLevel = 5
    topSeq.Members.append(decoder)

ApplicationMgr().TopAlg = [topSeq]

from GaudiPython.Bindings import AppMgr, gbl
gaudi = AppMgr()
TES = gaudi.evtSvc()

hlt1_tck, hlt2_tck = 0, 0

while True:
    gaudi.run(1)
    if not TES['/Event']:
        break
    raw = TES['DAQ/RawEvent']
    if args.odin:
        odin = TES['DAQ/ODIN']
        hlt1_tck = odin.triggerConfigurationKey()
        break
    elif not raw.banks(gbl.LHCb.RawBank.HltDecReports).size():
        # Nanofied event
        continue
    else:
        hlt1_tck = TES['Hlt1/DecReports'].configuredTCK()
        hlt2_tck = TES['Hlt2/DecReports'].configuredTCK()
        break

gaudi.stop()
gaudi.finalize()
gaudi.exit()

print '0x{0:08x} 0x{1:08x}'.format(hlt1_tck, hlt2_tck)
