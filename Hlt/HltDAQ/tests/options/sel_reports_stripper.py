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
"""Test the HltSelReportsStripper and related data flow.

The realistic data flow where HltSelReportsStripper will be used is
1. HltSelReportsDecoder
2. HltSelReportsStripper
3. bankKiller
4. HltSelReportsWriter

Here we test that HltSelReportsStripper selects what we expect, that it
correctly clones all the HltSummaryObjects and that the writing and the
subsequent decoding gives identical objects to those in the original
SelReports.

"""
from __future__ import print_function
import math

from Gaudi.Configuration import DEBUG, VERBOSE, INFO
from Configurables import ApplicationMgr, EventSelector, GaudiSequencer
from Configurables import LHCbApp
from Configurables import HltRoutingBitsFilter
from Configurables import bankKiller
from Configurables import LHCb__RawEventCopy as RawEventCopy
from Configurables import IODataManager
from Configurables import HistogramPersistencySvc
from Configurables import HltSelReportsStripper
from Configurables import HltSelReportsWriter
from DAQSys.Decoders import DecoderDB
from Configurables import LHCbTimingAuditor, LHCbSequencerTimerTool, AuditorSvc
from PRConfig.TestFileDB import test_file_db

import GaudiPython

LEVEL = INFO

# Setup the timing table
LHCbTimingAuditor('TIMER').addTool(LHCbSequencerTimerTool, name="TIMER")
ApplicationMgr().ExtSvc.append('AuditorSvc')
AuditorSvc().Auditors.append(LHCbTimingAuditor('TIMER'))
ApplicationMgr().AuditAlgorithms = True

# Setup the input data
test_file_db['HltDAQ-routingbits_turbo'].run(configurable=LHCbApp())
# LHCbApp().SkipEvents =
EventSelector().PrintFreq = 100

# Get rid of warnings
IODataManager().DisablePFNWarning = True
HistogramPersistencySvc().OutputLevel = 5

# Filter nanofied events if the file is HLT2 accepted
content_filter = HltRoutingBitsFilter('RBFilter')
content_filter.RequireMask = [0x0, 0x0, 0x80000000]

decoder = DecoderDB['HltSelReportsDecoder/Hlt2SelReportsDecoder']
decdecoder = DecoderDB['HltDecReportsDecoder/Hlt2DecReportsDecoder'].setup()
decoder.Properties['OutputLevel'] = LEVEL

stripper = HltSelReportsStripper()
stripper.InputHltSelReportsLocation = decoder.Outputs[
    "OutputHltSelReportsLocation"]
stripper.OutputHltSelReportsLocation = 'Hlt2/SelReportsFiltered'
stripper.OutputHltObjectSummariesLocation = (
    str(stripper.OutputHltSelReportsLocation) + '/Candidates')
stripper.SelectionNames = [
    'Hlt2DiMuonBDecision',
    'Hlt2Topo2BodyDecision',
    'Hlt2RecSummary',
]
stripper.OutputLevel = LEVEL

copyRaw = RawEventCopy()
copyRaw.Destination = "DAQ/RawEventOriginal"

killer = bankKiller("Hlt2SelRepKiller")
killer.BankTypes = ["HltSelReports"]
killer.DefaultIsKill = False
# HltSelReportsWriter::kSourceID_Hlt2 << HltSelReportsWriter::kSourceID_BitShift
killer.KillSourceID = 2 << 13
# HltSelReportsWriter::kSourceID_MajorMask
killer.KillSourceIDMask = 0xE000

writer = HltSelReportsWriter('Hlt2SelReportsWriter')
writer.SourceID = 2
writer.DecReports = str(decdecoder.OutputHltDecReportsLocation)
writer.SelReports = str(stripper.OutputHltSelReportsLocation)
writer.ObjectSummaries = str(stripper.OutputHltObjectSummariesLocation)
writer.OutputLevel = LEVEL

decoder2 = decoder.clone('HltSelReportsDecoder/SecondDecoder')
# decoder2.Inputs = {'RawEventLocations': ['DAQ/RawEvent']}
decoder2.Outputs = {
    'OutputHltSelReportsLocation': 'Hlt2/SelReportsFilteredFromRaw',
    'OutputHltObjectSummariesLocation':
    'Hlt2/SelReportsFilteredFromRaw/Candidates'
}
decoder2.Properties['OutputLevel'] = LEVEL

topSeq = GaudiSequencer("TopSequence")
topSeq.Members = [
    content_filter,
    copyRaw,
    decoder.setup(),
    decdecoder,
    stripper,
    killer,
    writer,
    decoder2.setup(),
]
ApplicationMgr().TopAlg = [topSeq]

gaudi = GaudiPython.AppMgr()
TES = gaudi.evtsvc()
RawBank = GaudiPython.gbl.LHCb.RawBank


def bank_size(raw):
    """Return the size of the first HltSelReports bank for HLT2."""
    for b in raw.banks(RawBank.HltSelReports):
        if b.sourceID() >> 13 & 7 == 2:
            return b.size()
    return -1


def iter_vector(v):
    for i in range(v.size()):
        yield v[i]


def safe_str(x):
    """Return repr(x) while checking there is no precision loss."""
    s = repr(x)
    assert float(s) == x or math.isnan(x)
    return s


def hos_to_pod(hos):
    """Convert HltObjectSummary to plain python objects."""
    return {
        'summarizedObjectCLID':
        hos.summarizedObjectCLID(),
        'numericalInfo':
        {k: safe_str(v)
         for k, v in hos.numericalInfo().iteritems()},
        'lhcbIDs': [x.lhcbID() for x in iter_vector(hos.lhcbIDs())],
        'substructure':
        [hos_to_pod(sub) for sub in iter_vector(hos.substructure())],
        'substructureExtended':
        [hos_to_pod(sub) for sub in iter_vector(hos.substructureExtended())],
    }


def lhcbids(hos):
    ids = [(hos['summarizedObjectCLID'], tuple(hos['lhcbIDs']))]
    ids.extend(sum((lhcbids(sub) for sub in hos['substructure']), []))
    ids.extend(sum((lhcbids(sub) for sub in hos['substructureExtended']), []))
    return ids


def assert_equal(expected, result, msg):
    if result != expected:
        import pprint
        import difflib
        diff = difflib.ndiff(
            pprint.pformat(expected).splitlines(),
            pprint.pformat(result).splitlines(),
        )
        full_msg = ('{}\n    diff:\n{}'.format(msg, '\n'.join(diff)))
        raise AssertionError(full_msg)


# for i in range(100):
while True:
    gaudi.run(1)
    raw2 = TES['DAQ/RawEvent']
    if not raw2:  # no more events
        break

    raw = TES['DAQ/RawEventOriginal']
    inp = TES[str(stripper.InputHltSelReportsLocation)]
    out = TES[str(stripper.OutputHltSelReportsLocation)]
    out2 = TES[decoder2.Outputs["OutputHltSelReportsLocation"]]
    inpc = TES[str(stripper.InputHltSelReportsLocation) + '/Candidates']
    outc = TES[str(stripper.OutputHltSelReportsLocation) + '/Candidates']
    out2c = TES[decoder2.Outputs["OutputHltObjectSummariesLocation"]]
    if not inp:  # we didn't have SelReports to process
        continue

    # Some stats
    print('N selections (input, stripped, persisted):      ', inp.size(),
          out.size(), out2.size())
    print('N HltObjectSummary (input, stripped, persisted):', inpc.size(),
          outc.size(), out2c.size())
    print('Hlt2 SelReports bank size (input, stripped):    ', bank_size(raw),
          bank_size(raw2))

    # Check the selection logic (we keep only the requested reports)
    result = set(out.selectionNames())
    expected = set(inp.selectionNames()) & set(stripper.SelectionNames)
    assert_equal(
        expected, result,
        'The stripped SelReports do not match the expectation'.format(
            result, expected))

    # Check the deep copying of HltSummaryObjects (i.e. that the cloned
    # reports are the same as the originals)
    for name, report in out.selReports().items():
        original = inp.selReport(name)
        assert_equal(
            hos_to_pod(original), hos_to_pod(report),
            'The copied SelReport is not the same')

    # Check that the persistence works (the SelReports which are written
    # and decoding are the same as the originals)
    for name, report in out2.selReports().items():
        original = inp.selReport(name)
        assert_equal(
            hos_to_pod(original), hos_to_pod(report),
            'The persisted SelReport is not the same')
