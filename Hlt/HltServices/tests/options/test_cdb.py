###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
import sys
import os
import subprocess
import argparse
import re

from GaudiConf import IOHelper
from Gaudi.Configuration import EventSelector, ApplicationMgr
from Configurables import LHCbApp, GaudiSequencer
from PRConfig.TestFileDB import test_file_db
from Configurables import IODataManager
from Configurables import HistogramPersistencySvc
from Configurables import ConfigCDBAccessSvc
from Configurables import HltConfigSvc
from Configurables import LoKiSvc

parser = argparse.ArgumentParser(usage='usage: %(prog)s')

parser.add_argument("--tck", type=str, dest="tck", default="",
                    help="What TCK")

args = parser.parse_args()


def tcks(tck):
    tck = int(tck, 16)
    hlt1_tck, hlt2_tck = (0, 0)
    if (tck & (1 << 28)) == (1 << 28):
        hlt1_tck = tck
    else:
        hlt2_tck = tck
    return hlt1_tck, hlt2_tck


files = test_file_db["HltServices-close_cdb_file"].filenames
expr = re.compile(r'0x[a-f\d]{8}')
m = expr.search(os.path.split(files[0])[-1])

if not args.tck and m:
    hlt1_tck, hlt2_tck = tcks(m.group(0))
elif not args.tck:
    # Use inspect's stack to get the file of the current frame: us
    script_dir = os.path.expandvars('$HLTDAQROOT/tests/options')
    cmd = ['python', os.path.join(
        script_dir, 'get_tck.py'), '--odin', files[0]]
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
        print 'Could not determine TCK from {}'.format(files[0])
        print 'FAILED'
        sys.exit(-1)

    if not hlt1_tck:
        print o
        print "Could not determine TCK from {}, it's 0".format(files[0])
        print 'FAILED'
        sys.exit(-1)
else:
    hlt1_tck, hlt2_tck = tcks(args.tck)

# General configuration
app = LHCbApp()
app.EvtMax = -1
app.DataType = '2016'
app.CondDBtag = 'cond-20160517'
app.DDDBtag = 'dddb-20150724'

EventSelector().PrintFreq = 1000

IODataManager().DisablePFNWarning = True

HistogramPersistencySvc().OutputLevel = 5

# Configure from TCK to get the routing bits from there
close_incident = 'TEST_CLOSE'
accessSvc = ConfigCDBAccessSvc(
    File='TCKData/config.cdb', CloseIncident=close_incident)

HltConfigSvc().initialTCK = '0x{0:08x}'.format(
    hlt1_tck if not hlt2_tck else hlt2_tck)
HltConfigSvc().checkOdin = True
HltConfigSvc().OutputLevel = 5
HltConfigSvc().ConfigAccessSvc = accessSvc.getFullName()

# Run HltConfigSvc first
ApplicationMgr().ExtSvc.insert(0, HltConfigSvc().getFullName())

topSeq = GaudiSequencer("TestSequence")
ApplicationMgr().TopAlg = [topSeq]

LoKiSvc().Welcome = False

IOHelper("MDF").inputFiles(files)


def open_files():
    pid = os.getpid()
    proc_path = '/proc/{}/fd'.format(pid)
    of = os.listdir(proc_path)
    return sorted([os.path.realpath(os.path.join(proc_path, f)) for f in of])

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
incSvc = InterfaceCast(gbl.IIncidentSvc)(
    gaudi.service('IncidentSvc').getInterface())
TES = gaudi.evtSvc()
cdb_file = os.path.realpath(gaudi.service(accessSvc.getFullName()).File)

if cdb_file not in open_files():
    failed(['CDB file not in open files after initialization'])

incSvc.fireIncident(gbl.Incident("GP", close_incident))

if cdb_file in open_files():
    failed(['CDB file not closed after '
            'firing close incident {}'.format(close_incident)])

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
