import re
import sys
import os
import subprocess
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
from DAQSys.Decoders import DecoderDB
from Configurables import LoKiSvc
from Configurables import ToolSvc
from PRConfig.TestFileDB import test_file_db
from Gaudi.Configuration import importOptions

parser = argparse.ArgumentParser(usage='usage: %(prog)s stream file')

parser.add_argument("-s", "--stream", type=str, dest="stream", default="",
                    help="Which stream")
parser.add_argument("--tck", type=str, dest="tck", default="",
                    help="What TCK")
parser.add_argument("-n", "--nevents", type=int, dest="nevt", default=-1,
                    help="Maximum number of events")
parser.add_argument("db_entry", nargs=1)

args = parser.parse_args()

file = test_file_db[args.db_entry[0]].filenames[0]

if not args.tck:
    # Use inspect's stack to get the file of the current frame: us
    script_dir = os.path.dirname(inspect.stack()[0][1])
    cmd = ['python', os.path.join(script_dir, 'get_tck.py'), file]
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    o, e = p.communicate()

    if p.returncode:
        print 'Failed to get TCK'
        print o
        print 'FAILED'
        sys.exit(-1)

    output = [l.strip() for l in o.split('\n') if l.strip()]
    hlt1_tck, hlt2_tck = output[-1].split()
    try:
        hlt1_tck, hlt2_tck = (int(hlt1_tck, 16), int(hlt2_tck, 16))
    except ValueError:
        print o
        print 'Could not determine TCK from {}'.format(file)
        print 'FAILED'
        sys.exit(-1)

    if not (hlt1_tck and hlt2_tck):
        print o
        print 'Could not determine TCK from {}, both are 0'.format(file)
        print 'FAILED'
        sys.exit(-1)
else:
    tck = int(args.tck, 16)
    hlt1_tck, hlt2_tck = (0, 0)
    if (tck & (1 << 28)) == (1 << 28):
        hlt1_tck = tck
    else:
        hlt2_tck = tck

if hlt2_tck and (args.stream.capitalize() not in ('Turbo', 'Full', 'Turcal',
                                                  'Beamgas', 'Nobias',
                                                  'Turboraw')):
    print 'Stream must be set when running on an HLT2 accepted file'
    print 'FAILED'
    sys.exit(-1)

if not os.path.exists('0x%08x.props'):
    script_dir = os.path.dirname(inspect.stack()[0][1])
    cmd = ['python', os.path.join(script_dir, 'get_properties.py'),
           '0x%08x' % hlt2_tck if hlt2_tck else hlt1_tck]
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    o, e = p.communicate()

    if p.returncode:
        print 'Failed to get properties'
        print o
        print 'FAILED'
        sys.exit(-1)

db = shelve.open('0x%08x.props' % hlt2_tck if hlt2_tck else hlt1_tck)
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
if hlt2_tck:
    from Configurables import HltRoutingBitsFilter
    nano_filter = HltRoutingBitsFilter(
        'NonNanoFilter', RequireMask=[0x0, 0x0, 0x80000000])
    topSeq.Members += [nano_filter]

importOptions('$L0TCK/L0DUConfig.opts')

# Decode Hlt DecReports
for dec in ("L0DUDecoder/L0DUFromRaw",
            "HltDecReportsDecoder/Hlt1DecReportsDecoder"):
    topSeq.Members.append(DecoderDB[dec].setup())
    # tell HltConfigSvc to leave the just explicitly configured decoders
    # alone..

if hlt2_tck:
    topSeq.Members.append(
        DecoderDB["HltDecReportsDecoder/Hlt2DecReportsDecoder"].setup())


copier = RawEventCopy(
    "RawEventCopy", Destination='/Event/DAQ/CopyRawEvent', DeepCopy=True)
killer = bankKiller('KillRoutingBits', BankTypes=['HltRoutingBits'],
                    DefaultIsKill=False,
                    RawEventLocations=['DAQ/CopyRawEvent'])

# Configure the routing bits writer
rb_name = '{stream}{stage}RoutingBitsWriter'
rb_name = rb_name.format(stage='Hlt1' if not hlt2_tck else 'Hlt2',
                         stream=args.stream.capitalize())
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
