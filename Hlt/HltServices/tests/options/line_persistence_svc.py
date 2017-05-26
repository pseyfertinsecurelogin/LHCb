from itertools import product, combinations, chain
from Configurables import ApplicationMgr
from Configurables import HltLinePersistenceSvc
import GaudiPython

LHCb = GaudiPython.gbl.LHCb
ALL_RAWBANKS = [LHCb.RawBank.typeName(i) for i in range(LHCb.RawBank.LastType)]


svc = HltLinePersistenceSvc()
svc.Locations = {
    # Regular-turbo-like line
    'Hlt2FirstLineDecision': [
        '/Event/Hlt2FirstLine/Particles',
        '/Event/Hlt2FirstLine/Vertices',
    ],
    # Selective-persistence-like line
    'Hlt2SecondLineDecision': [
        '/Event/Hlt2SecondLine/Particles',
        '/Event/Hlt2SecondLine/Vertices',
        '/Event/Hlt2SecondLine/Extras',
        '/Event/SomeExternalLocation',
    ],
    # Persist-reco-like line
    'Hlt2ThirdLineDecision': [
        '/Event/Hlt2ThirdLine/Particles',
        '/Event/Hlt2ThirdLine/Vertices',
        '/Event/SomeExternalLocation',
        '/Event/OtherExternalLocation',
        '/Event/AnotherExternalLocation',
    ],
}
svc.TurboPPLines = ['Hlt2ThirdLineDecision']
svc.RawBankTypes = {
    # Lumi-like line
    'Hlt2FirstLineDecision': ['ODIN', 'HltLumiSummary', 'HltRoutingBits', 'DAQ'],
    # Turbo-like line
    'Hlt2SecondLineDecision': [
        'ODIN', 'HltLumiSummary', 'DAQ', 'DstData', 'HltRoutingBits',
        'HltDecReports', 'HltSelReports', 'HltVertexReports', 'HltTrackReports',
        'L0DU', 'L0Calo', 'L0PU', 'L0PUFull', 'L0Muon', 'L0CaloFull',
        'L0MuonCtrlAll', 'L0MuonProcCand', 'L0MuonProcData'],
    # Full-like line
    'Hlt2ThirdLineDecision': list(set(ALL_RAWBANKS) - set(['DstData'])),
}
ApplicationMgr().ExtSvc += [svc]


gaudi = GaudiPython.AppMgr()
gaudi.EvtSel = 'NONE'
gaudi.config()
gaudi.initialize()


LHCb = GaudiPython.gbl.LHCb
std = GaudiPython.gbl.std
# Make sure relevant types are instantiated, if not PyROOT segfaults!
std.vector(std.string)
std.set(std.string)
std.bitset(LHCb.RawBank.LastType)
std.vector(std.bitset(LHCb.RawBank.LastType))


def dummyDecReports(decisions):
    drs = LHCb.HltDecReports()
    for k, v in decisions.items():
        drs.insert(k, LHCb.HltDecReport(v))
    return drs


def std_set(iterable):
    x = std.set(std.string)()
    for i in iterable:
        x.insert(i)
    return x


def to_bitset(rawbanks):
    x = std.bitset(LHCb.RawBank.LastType)()
    for t in rawbanks:
        x.set(ALL_RAWBANKS.index(t))
    return x


def from_bitset(x):
    return set(LHCb.RawBank.typeName(i)
               for i, bit in enumerate(reversed(x.to_string()))
               if bit == '1')


# Get the service
hltsvc = gaudi.service('HltLinePersistenceSvc', 'ILinePersistenceSvc')


# Brute force test of all combinations of HDRs and line subsets
all_lines = sorted(svc.Locations.keys())
all_subsets = list(chain.from_iterable(
    combinations(all_lines, r) for r in range(len(all_lines) + 1)))
all_decisions = list(product(*([[False, True]]*3)))
for subset in all_subsets:
    lines = std_set(subset)
    for decs in all_decisions:
        decisions = dict(zip(all_lines, decs))
        hdr = dummyDecReports(decisions)

        expected = set(chain.from_iterable(
            svc.Locations[line] for line in subset if decisions[line]))
        result = set(hltsvc.locationsToPersist(hdr, lines))
        assert result == expected, ("hdr={} lines={} result={} expected={}"
                                    .format(decisions, subset, result, expected))
        # print 'Locations check {} == {}'.format(len(result), len(expected))

        expected = set(chain.from_iterable(
            svc.Locations[line] for line in subset
            if decisions[line] and line in svc.TurboPPLines))
        result = set(hltsvc.turboPPLocationsToPersist(hdr, lines))
        assert result == expected, ("hdr={} lines={} result={} expected={}"
                                    .format(decisions, subset, result, expected))
        # print 'TurboPPLocations check {} == {}'.format(len(result), len(expected))

        expected = set(chain.from_iterable(
            svc.RawBankTypes[line] for line in subset if decisions[line]))
        result = from_bitset(hltsvc.rawBanksToPersist(hdr, lines))
        assert result == expected, ("hdr={} lines={} result={} expected={}"
                                    .format(decisions, subset, result, expected))
        # print 'RawBankTypes check {} == {}'.format(len(result), len(expected))

# Test that extra lines in svc.Locations are not a problem
hdr = dummyDecReports({
    'Hlt2FirstLineDecision': True,
    'Hlt2SecondLineDecision': True})
lines = std_set(['Hlt2FirstLineDecision', 'Hlt2SecondLineDecision'])
result = set(hltsvc.locationsToPersist(hdr, lines))
assert result == set(svc.Locations['Hlt2FirstLineDecision'] +
                     svc.Locations['Hlt2SecondLineDecision'])

# Test failure if line is not in svc.Locations
hdr = dummyDecReports({
    'Hlt2SecondLineDecision': True,
    'Hlt2MissingLineDecision': True})
lines = std_set(['Hlt2FirstLineDecision', 'Hlt2MissingLineDecision'])
try:
    result = hltsvc.locationsToPersist(hdr, lines)
except Exception as e:
    assert 'Decision name not in Locations map.' in e.args[0]

# Test that extra lines in svc.RawBankTypes are not a problem
hdr = dummyDecReports({
    'Hlt2FirstLineDecision': True,
    'Hlt2SecondLineDecision': True})
lines = std_set(['Hlt2FirstLineDecision', 'Hlt2SecondLineDecision'])
result = from_bitset(hltsvc.rawBanksToPersist(hdr, lines))
assert result == set(svc.RawBankTypes['Hlt2FirstLineDecision'] +
                     svc.RawBankTypes['Hlt2SecondLineDecision'])

# Test failure if line is not in svc.RawBankTypes
hdr = dummyDecReports({
    'Hlt2SecondLineDecision': True,
    'Hlt2MissingLineDecision': True})
lines = std_set(['Hlt2FirstLineDecision', 'Hlt2MissingLineDecision'])
try:
    result = hltsvc.rawBanksToPersist(hdr, lines)
except Exception as e:
    assert 'Decision name not in RawBankTypes map.' in e.args[0]

print 'PASS'  # just a comforting message
