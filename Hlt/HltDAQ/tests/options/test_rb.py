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
import re
import sys
import os
import argparse
import inspect
import shelve
import Configurables

from Configurables import HltRoutingBitsWriter
from Configurables import bankKiller
from Configurables import LHCb__RawEventCopy as RawEventCopy
from GaudiConf import IOHelper
from Configurables import LHCbApp, EventSelector
from Configurables import ApplicationMgr, GaudiSequencer
from Configurables import IODataManager
from Configurables import HistogramPersistencySvc
from Configurables import HltRoutingBitsFilter
from DAQSys.Decoders import DecoderDB
from Configurables import LoKiSvc
from Configurables import ToolSvc
from PRConfig.TestFileDB import test_file_db

parser = argparse.ArgumentParser(
    usage='usage: %(prog)s test_file_db_entry stream tck')

parser.add_argument("db_entry", nargs=1)
parser.add_argument("stream", type=str, nargs=1,
                    help="Which stream")
parser.add_argument("tck", type=str, nargs=1,
                    help="What TCK")
parser.add_argument("-n", "--nevents", type=int, dest="nevt", default=-1,
                    help="Maximum number of events")

args = parser.parse_args()

file = test_file_db[args.db_entry[0]].filenames[0]

hlt2_tck = int(args.tck[0], 16)
if (hlt2_tck & (2 << 28)) != (2 << 28):
    print parser.usage
    print 'FAILED'
    sys.exit(-1)

stream = args.stream[0].capitalize()
if stream not in ('Turbo', 'Full', 'Turcal', 'Beamgas', 'Nobias',
                  'Turboraw'):
    print 'Stream must be set'
    print 'FAILED'
    sys.exit(-1)

script_dir = os.path.dirname(inspect.stack()[0][1])
db_file = os.path.join(script_dir, '..', 'data',
                       '0x%08x.props' % hlt2_tck)
if not os.path.exists(db_file):
    print 'Properties db for TCK 0x%08x is missing.' % hlt2_tck
    print 'FAILED'
    sys.exit(-1)

db = shelve.open(db_file, 'r')
writer_props = db['writers']
factory_props = db['factories']

# General configuration
app = LHCbApp()
app.EvtMax = -1
app.DataType = '2016'
app.CondDBtag = 'cond-20160517'
app.DDDBtag = 'dddb-20150724'

EventSelector().PrintFreq = 1000

IODataManager().DisablePFNWarning = True

HistogramPersistencySvc().OutputLevel = 5

topSeq = GaudiSequencer("TopSequence")

# Filter nanofied events if the file is HLT2 accepted
nano_filter = HltRoutingBitsFilter(
    'NonNanoFilter', RequireMask=[0x0, 0x0, 0x80000000])
topSeq.Members += [nano_filter]

# Decode Hlt DecReports
for dec in ("HltDecReportsDecoder/Hlt1DecReportsDecoder",
            "HltDecReportsDecoder/Hlt2DecReportsDecoder"):
    topSeq.Members.append(DecoderDB[dec].setup())
    # tell HltConfigSvc to leave the just explicitly configured decoders
    # alone..

copier = RawEventCopy(
    "RawEventCopy", Destination='/Event/DAQ/CopyRawEvent', DeepCopy=True)
killer = bankKiller('KillRoutingBits', BankTypes=['HltRoutingBits'],
                    DefaultIsKill=False,
                    RawEventLocations=['DAQ/CopyRawEvent'])

# Configure the routing bits writer
rb_name = '{stream}{stage}RoutingBitsWriter'
rb_name = rb_name.format(stage='Hlt1' if not hlt2_tck else 'Hlt2',
                         stream=stream)
props = writer_props['HltRoutingBitsWriter/' + rb_name]
writer = HltRoutingBitsWriter(rb_name,
                              Hlt1DecReportsLocation='Hlt1/DecReports',
                              Hlt2DecReportsLocation='Hlt2/DecReports',
                              RoutingBits=eval(props['RoutingBits']),
                              RawEventLocation="DAQ/CopyRawEvent",
                              UpdateExistingRawBank=False)


# Configure the LoKi factories
# Get rid of a few things we don't need that live in dependent projects
subs = {"Modules": r",[ ]*'LoKiTrigger.[a-z]*'",
        "Lines": r",[ ]*'import HltTracking.Hlt1StreamerConf'"}

for factory, props in factory_props.iteritems():
    factory_type, factory_name = factory.split('/')
    factory_type = factory_type.replace(':', '_')
    try:
        ft = getattr(Configurables, factory_type)
    except AttributeError:
        continue
    ToolSvc().addTool(ft, factory_name)
    fact = getattr(ToolSvc(), factory_name)
    for prop, val in props.iteritems():
        if prop in subs:
            val = re.sub(subs[prop], '', val)
        setattr(fact, prop, eval(val))


topSeq.Members += [copier, killer, writer]

ApplicationMgr().TopAlg = [topSeq]

LoKiSvc().Welcome = False

IOHelper("MDF").inputFiles([file])


from GaudiPython.Bindings import AppMgr, gbl
gaudi = AppMgr()

gaudi.initialize()
TES = gaudi.evtSvc()

# Force loading of HltInterfaces dictionary to get firedRoutingBits function
getattr(gbl, "IANNSvc")
firedRoutingBits = gbl.Hlt.firedRoutingBits

error = False
nEvt = 0

while (args.nevt == -1) or (nEvt < args.nevt):
    gaudi.run(1)
    if not TES['/Event']:
        break

    nEvt += 1

    # Check for bit 95 (non-nanofied event) in case of Hlt2
    rb_orig = set([b for b in firedRoutingBits(TES['DAQ/RawEvent'])])
    if hlt2_tck and 95 not in rb_orig:
        continue

    rb_copy = set([b for b in firedRoutingBits(TES['DAQ/CopyRawEvent'])])
    diff = rb_orig.symmetric_difference(rb_copy)
    if diff:
        odin = TES['DAQ/ODIN']
        print 'Different routing bits!:'
        print '{0} {1}: {2}'.format(odin.runNumber(), odin.eventNumber(),
                                    list(diff))
        error = True
        break

gaudi.stop()
gaudi.finalize()
gaudi.exit()

if not error:
    print 'PASSED'
else:
    print 'FAILED'
