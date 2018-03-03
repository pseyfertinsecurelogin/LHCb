"""Test the DecisionBankKiller.

Here we test that DecisionBankKiller selects what we expect.

"""
from __future__ import print_function
import re
from pprint import pformat

from Gaudi.Configuration import DEBUG, VERBOSE, INFO
from Configurables import ApplicationMgr, EventSelector, GaudiSequencer
from Configurables import LHCbApp
from Configurables import HltRoutingBitsFilter
from Configurables import DecisionBankKiller
from Configurables import LHCb__RawEventCopy as RawEventCopy
from Configurables import IODataManager
from Configurables import HistogramPersistencySvc
from Configurables import LoKiSvc
from Configurables import LoKi__HDRFilter
from DAQSys.Decoders import DecoderDB
from Configurables import LHCbTimingAuditor, LHCbSequencerTimerTool, AuditorSvc
from PRConfig.TestFileDB import test_file_db

import GaudiPython

from Configurables import HltLinePersistenceSvc
import GaudiPython

LHCb = GaudiPython.gbl.LHCb
ALL_RAWBANKS = [LHCb.RawBank.typeName(i)
                for i in range(LHCb.RawBank.LastType)]
RAW_BANK_TYPES = [(i, LHCb.RawBank.typeName(i))
                  for i in range(LHCb.RawBank.LastType)]
DEFAULT_TURBO_BANKS = [
    'DstData',
    'HltDecReports', 'HltSelReports', 'HltVertexReports', 'HltTrackReports',
    'L0DU', 'L0Calo', 'L0PU', 'L0PUFull', 'L0Muon', 'L0CaloFull',
    'L0MuonCtrlAll', 'L0MuonProcCand', 'L0MuonProcData'
]


svc = HltLinePersistenceSvc()
# Banks for decisions that are not set here are set later (on the first event)
# to DEFAULT_TURBO_BANKS.
non_default_raw_bank_types = {
    # Full-like line
    # does not influence what is kept since it is filtered by the stream filter
    'Hlt2DiMuonBDecision': list(set(ALL_RAWBANKS) - set(['DstData'])),
    # Let's try to keep some Muon banks for a muon line and Rich for charm
    'Hlt2DiMuonBTurboDecision': DEFAULT_TURBO_BANKS + ['Muon'],
    'Hlt2CharmHadDspToPimPipPipTurboDecision': DEFAULT_TURBO_BANKS + ['Rich'],
    'Hlt2CharmHadD02KmPipTurboDecision': DEFAULT_TURBO_BANKS + ['Rich'],
    # Very important line, so let's keep everything
    'Hlt2CharmHadDpToKmPipPip_ForKPiAsymTurboDecision': ALL_RAWBANKS,
}
ApplicationMgr().ExtSvc.append(svc.getFullName())


# Setup the timing table
LHCbTimingAuditor('TIMER').addTool(LHCbSequencerTimerTool, name="TIMER")
ApplicationMgr().ExtSvc.append('AuditorSvc')
AuditorSvc().Auditors.append(LHCbTimingAuditor('TIMER'))
ApplicationMgr().AuditAlgorithms = True

# Setup the input data
test_file_db['HltDAQ-routingbits_full'].run(configurable=LHCbApp())
# LHCbApp().SkipEvents =
EventSelector().PrintFreq = 100

# Get rid of warnings
IODataManager().DisablePFNWarning = True
HistogramPersistencySvc().OutputLevel = 5
LoKiSvc().Welcome = False

# Filter nanofied events if the file is HLT2 accepted
content_filter = HltRoutingBitsFilter('RBFilter')
content_filter.RequireMask = [0x0, 0x0, 0x80000000]

decoder = DecoderDB['HltDecReportsDecoder/Hlt2DecReportsDecoder']

stream_filter = LoKi__HDRFilter('Hlt2Filter')
stream_filter.Location = decoder.listOutputs()[0]
stream_filter.Code = "HLT_TURBOPASS_RE('^Hlt2.*(?<!TurboCalib)Decision$')"

copyRaw = RawEventCopy()
copyRaw.Destination = "DAQ/RawEventOriginal"

killer = DecisionBankKiller('Killer')
killer.InputHltDecReportsLocation = decoder.listOutputs()[0]
killer.ILinePersistenceSvc = svc.getFullName()
killer.LineFilter.Code = stream_filter.Code
assert set(killer.AlwaysKeepBanks) == set(['ODIN', 'HltRoutingBits', 'DAQ'])

def decreport_pass(name, report):
    """Return whether the decreport would pass the filter above."""
    return (bool(report.decision()) and
            report.executionStage() & 0x80 == 0x80 and
            re.match(r'^Hlt2.*(?<!TurboCalib)Decision$', name))


topSeq = GaudiSequencer("TopSequence")
topSeq.Members = [
    content_filter,
    decoder.setup(),
    stream_filter,
    copyRaw,
    killer,
]
ApplicationMgr().TopAlg = [topSeq]


gaudi = GaudiPython.AppMgr()
TES = gaudi.evtsvc()
RawBank = GaudiPython.gbl.LHCb.RawBank


def rawbank_sizes(rawevent):
    """Return (name, size) for each raw bank type."""
    def size(i):
        return sum(bank.totalSize() for bank in rawevent.banks(i))
    return {name: size(i) for i, name in RAW_BANK_TYPES if size(i) > 0}


def assert_equal(expected, result, msg):
    if result != expected:
        import pprint
        import difflib
        diff = difflib.ndiff(
            pprint.pformat(expected).splitlines(),
            pprint.pformat(result).splitlines(),
        )
        full_msg = ('{}\n    diff:\n{}'
                    .format(msg, '\n'.join(diff)))
        raise AssertionError(full_msg)

# Trick! Disable the killer until we can configure the svc after the first event
svc_instance = gaudi.service(svc.getFullName(), 'IProperty')
killer_instance = gaudi.algorithm(killer.getFullName())
killer_instance.Enable = False
first_event = True
raw_bank_types = non_default_raw_bank_types

all_decisions = []
all_requests = []

# for i in range(100):
while True:
    gaudi.run(1)
    raw2 = TES['DAQ/RawEvent']
    if not raw2:  # no more events
        break

    raw = TES['DAQ/RawEventOriginal']
    hdr = TES[str(killer.InputHltDecReportsLocation)]
    if not hdr or not raw:  # we didn't have DecReports or filter didn't pass
        continue

    if first_event:
        # print(svc_instance.getProperty('RawBankTypes').toString())
        types = {name: DEFAULT_TURBO_BANKS for name in hdr.decisionNames()
                 if 'Turbo' in name}
        types.update(non_default_raw_bank_types)
        raw_bank_types = types
        svc_instance.setProperty('RawBankTypes', repr(types))
        # Now that we've configured the service, enable the killer
        killer_instance.Enable = True
        first_event = False
        # don't do any check on the first event
        continue

    decisions = {name: (report.decision(), decreport_pass(name, report))
                 for name, report in hdr.decReports()}
    all_decisions.append(decisions)

    sizes = rawbank_sizes(raw)
    sizes2 = rawbank_sizes(raw2)

    # # Some stats
    fired = set(name for name, d in hdr.decReports() if d.decision())
    print('              ',fired)
    # print('Original raw event banks:', sorted(sizes.items()))
    # print('Modified raw event banks:', sorted(sizes2.items()))

    # Check that what we keep matches the expectation
    requests = [set(raw_bank_types[name]) for name, report in hdr.decReports()
                if decreport_pass(name, report)]
    requests += [set(killer.AlwaysKeepBanks)]
    requests = set.union(*requests)
    all_requests.append(requests)
    result = sizes2
    expected = {banktype: size for banktype, size in sizes.items()
                if banktype in requests}
    print(sizes2.keys())
    assert_equal(expected, result,
                 'The kept banks {} do not match the expectation {}.'
                 .format(result, expected))

# Based on the fake test case defined by non_default_raw_bank_types, and the
# stream filter, we expect to see 5 combinations of possible set of requested
# raw banks. Given enough input data we have to see all 5.
combinations_tested = set(map(frozenset, all_requests))
if len(combinations_tested) != 5:
    raise RuntimeError('Only these combinations were tested, expected {}:\n{}'
                       .format(5, pformat(combinations_tested)))

# make sure we see at least one of each line alone
# expected = set(non_default_raw_bank_types.keys())
# result = (expected &
#           set([name for name, (dec, passes) in all_decisions.items() if passes]))
# assert_equal(expected, result,
#              'Not all non-default lines were seen: {}'.format(expected))
