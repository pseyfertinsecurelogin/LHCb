import sys, os, re
import subprocess
import argparse
import inspect

parser = argparse.ArgumentParser(usage = 'usage: %(prog)s stream file')

parser.add_argument("-s", "--stream", type = str, dest = "stream", default = "",
                    help = "Which stream")
parser.add_argument("--tck", type = str, dest = "tck", default = "",
                    help = "What TCK")
parser.add_argument("-n", "--nevents", type = int, dest = "nevt", default = -1,
                    help = "Maximum number of events")
parser.add_argument("db_entry", nargs = 1)

args = parser.parse_args()

from PRConfig.TestFileDB import test_file_db
file = test_file_db[args.db_entry[0]].filenames[0]

if not args.tck:
    # Use inspect's stack to get the file of the current frame: us
    script_dir = os.path.dirname(inspect.stack()[0][1])
    cmd = ['python', os.path.join(script_dir, 'get_tck.py'), file]
    p = subprocess.Popen(cmd, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
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
                                                  'Beamgas', 'Nobias', 'Turboraw')):
    print 'Stream must be set when running on an HLT2 accepted file'
    print 'FAILED'
    sys.exit(-1)

# General configuration
from GaudiConf import IOHelper
from Gaudi.Configuration import *
from Configurables import LHCbApp
app = LHCbApp()
app.EvtMax = -1
app.DataType = '2016'
app.CondDBtag = 'cond-20160517'
app.DDDBtag = 'dddb-20150724'

EventSelector().PrintFreq = 1000

from Configurables import IODataManager
IODataManager().DisablePFNWarning = True

from Configurables import HistogramPersistencySvc
HistogramPersistencySvc().OutputLevel = 5

# Configure from TCK to get the routing bits from there
from Configurables import ConfigCDBAccessSvc
accessSvc = ConfigCDBAccessSvc(File = '$HLTTCKROOT/config.cdb')

from Configurables import HltConfigSvc
HltConfigSvc().initialTCK = '0x{0:08x}'.format(hlt1_tck if not hlt2_tck else hlt2_tck)
HltConfigSvc().checkOdin = not hlt2_tck
HltConfigSvc().OutputLevel = 5
HltConfigSvc().ConfigAccessSvc = accessSvc.getFullName()
# Remove some modules that don't live in the LHCb project
# Empty the Hlt sequence to disable the HLT.
# Point the RoutingBitsWriter at a copy of the raw event
HltConfigSvc().ApplyTransformation = {".*LoKi.*/.*Factory.*" : {"Modules" : {r",[ ]*'LoKiTrigger.[a-z]*'" : ""},
                                                                "Lines"   : {r",[ ]*'import HltTracking.Hlt1StreamerConf'" : ""} },
                                      'GaudiSequencer/Hlt' : {"Members" : {"'.*'" : ""}},
                                      'HltRoutingBitsWriter/.*RoutingBitsWriter' : {'RawEventLocation' : {"DAQ/.*RawEvent" : "DAQ/CopyRawEvent"},
                                                                                    'UpdateExistingRawBank' : {"True" : "False"}}
                                     }
# Write these DecReports somewhere else so the decoder can run
HltConfigSvc().HltDecReportsLocations = ['Hlt1/EmptyDecReports']
ApplicationMgr().ExtSvc.insert(0, HltConfigSvc().getFullName())

topSeq = GaudiSequencer( "TopSequence" )

# Filter nanofied events if the file is HLT2 accepted
if hlt2_tck:
    from Configurables import HltRoutingBitsFilter
    nano_filter = HltRoutingBitsFilter('NonNanoFilter', RequireMask = [0x0, 0x0, 0x80000000])
    topSeq.Members += [nano_filter]

# Decode Hlt DecReports
from DAQSys.Decoders import DecoderDB
for dec in ("L0DUDecoder/L0DUFromRaw",
            "HltDecReportsDecoder/Hlt1DecReportsDecoder"):
    topSeq.Members.append(DecoderDB[dec].setup())
    # tell HltConfigSvc to leave the just explicitly configured decoders alone..
    HltConfigSvc().SkipComponent += [ dec.split('/')[-1] ]
print topSeq.Members

if hlt2_tck:
    topSeq.Members.append(DecoderDB["HltDecReportsDecoder/Hlt2DecReportsDecoder"].setup())
    HltConfigSvc().SkipComponent += [ 'Hlt2DecReportsDecoder' ]

from Configurables import HltRoutingBitsWriter
from Configurables import bankKiller
from Configurables import LHCb__RawEventCopy as RawEventCopy

copier = RawEventCopy("RawEventCopy", Destination = '/Event/DAQ/CopyRawEvent', DeepCopy = True)
killer = bankKiller('KillRoutingBits', BankTypes = ['HltRoutingBits'], DefaultIsKill = False,
                    RawEventLocations = ['DAQ/CopyRawEvent'])
writer = HltRoutingBitsWriter('{stream}{stage}RoutingBitsWriter'.format(stage = 'Hlt1' if not hlt2_tck else 'Hlt2',
                                                                        stream = args.stream.capitalize()))
topSeq.Members += [copier, killer, writer]

ApplicationMgr().TopAlg = [topSeq]

from Configurables import LoKiSvc
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
        print '{0} {1}: {2}'.format(odin.runNumber(), odin.eventNumber(), list(diff))
        error = True
        break

gaudi.stop()
gaudi.finalize()
gaudi.exit()

if not error:
    print 'PASSED'
else:
    print 'FAILED'
