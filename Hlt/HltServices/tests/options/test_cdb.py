import sys, os, re
import subprocess
import argparse
import inspect

parser = argparse.ArgumentParser(usage = 'usage: %(prog)s file')

parser.add_argument("--tck", type = str, dest = "tck", default = "",
                    help = "What TCK")

args = parser.parse_args()

from PRConfig.TestFileDB import test_file_db
files = test_file_db["HltServices-close_cdb_file"].filenames

if not args.tck:
    # Use inspect's stack to get the file of the current frame: us
    script_dir = os.path.expandvars('$HLTDAQROOT/tests/options')
    cmd = ['python', os.path.join(script_dir, 'get_tck.py'), '--odin', files[0]]
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
        print 'Could not determine TCK from {}'.format(files[0])
        print 'FAILED'
        sys.exit(-1)

    if not hlt1_tck:
        print o
        print "Could not determine TCK from {}, it's 0".format(files[0])
        print 'FAILED'
        sys.exit(-1)
else:
    tck = int(args.tck, 16)
    hlt1_tck, hlt2_tck = (0, 0)
    if (tck & (1 << 28)) == (1 << 28):
        hlt1_tck = tck
    else:
        hlt2_tck = tck

        
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
close_incident = 'TEST_CLOSE'
accessSvc = ConfigCDBAccessSvc(File = '$HLTTCKROOT/config.cdb', CloseIncident = close_incident)

from Configurables import HltConfigSvc
HltConfigSvc().initialTCK = '0x{0:08x}'.format(hlt1_tck if not hlt2_tck else hlt2_tck)
HltConfigSvc().checkOdin = True
HltConfigSvc().OutputLevel = 5
HltConfigSvc().ConfigAccessSvc = accessSvc.getFullName()

# Remove some modules that don't live in the LHCb project
# Empty the Hlt sequence to disable the HLT.
# Point the RoutingBitsWriter at a copy of the raw event
HltConfigSvc().ApplyTransformation = {'GaudiSequencer/Hlt' : {"Members" : {"'.*'" : ""}}}

# Run HltConfigSvc first
ApplicationMgr().ExtSvc.insert(0, HltConfigSvc().getFullName())

topSeq = GaudiSequencer( "TopSequence" )
from Configurables import createODIN
topSeq.Members += [createODIN()]

# Decode Hlt DecReports
from DAQSys.Decoders import DecoderDB
for dec in ("L0DUFromRawAlg/L0DUFromRaw",):
    topSeq.Members.append(DecoderDB[dec].setup())

ApplicationMgr().TopAlg = [topSeq]

from Configurables import LoKiSvc
LoKiSvc().Welcome = False

IOHelper("MDF").inputFiles(files)

def open_files():
    pid = os.getpid()
    proc_path = '/proc/{}/fd'.format(pid)
    return sorted([os.path.realpath(os.path.join(proc_path, f)) for f in os.listdir(proc_path)])

from GaudiPython.Bindings import AppMgr, gbl, InterfaceCast
gaudi = AppMgr()
gaudi.initialize()

def failed(lines):
    gaudi.finalize()
    gaudi.exit()
    for line in lines:
        print line
    print 'FAILED'
    sys.exit(-1)

# Convenience
incSvc = InterfaceCast(gbl.IIncidentSvc)(gaudi.service('IncidentSvc').getInterface())
TES = gaudi.evtSvc()
cdb_file = os.path.realpath(gaudi.service(accessSvc.getFullName()).File)

if cdb_file not in open_files():
    failed(['CDB file not in open files after initialization'])

incSvc.fireIncident(gbl.Incident("GP", close_incident))

if cdb_file in open_files():
    failed(['CDB file not closed after firing close incident {}'.format(close_incident)])

while True:
    gaudi.run(1)
    if not TES['/Event']:
        failed(['No reconfiguration occured, end of events'])

    odin = TES['DAQ/ODIN']
    if odin.triggerConfigurationKey() != hlt1_tck:
        break

gaudi.stop()

if cdb_file in open_files():
    failed(['CDB file not closed after stop'])

gaudi.finalize()
gaudi.exit()

print 'PASSED'
