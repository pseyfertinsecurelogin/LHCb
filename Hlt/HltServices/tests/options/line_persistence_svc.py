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
import os
from itertools import product, combinations, chain
from Configurables import ApplicationMgr
from Configurables import HistogramPersistencySvc
from Configurables import HltLinePersistenceSvc
from Configurables import TCKLinePersistenceSvc
from Configurables import ConfigCDBAccessSvc, HltConfigSvc
import GaudiPython
import ROOT

HistogramPersistencySvc().OutputLevel = 5

execfile(
    os.path.expandvars('$HLTSERVICESROOT/tests/options/'
                       'line_persistence_svc_cfg.py'))
svcconf = configure_hlt_svc('TestHltLinePersistenceSvc')
ApplicationMgr().ExtSvc += [svcconf]

tcksvcconf = TCKLinePersistenceSvc()
tcksvcconf.ContainerMap = {
    '/Event/pRec/Protos': '/Event/Rec/Protos',
    '/Event/pRec/DoesNotExist': '/Event/Rec/DoesNotExist',
}
ApplicationMgr().ExtSvc += [tcksvcconf]

# cd LHCb/build.x86_64-centos7-gcc62-opt/Hlt/HltServices/tests_tmp/
#  ../../../../run ipython -i ../../../../Hlt/HltServices/tests/options/line_persistence_svc.py

# Configure the TCK
accessSvc = ConfigCDBAccessSvc(File='TCKData/config.cdb')
HltConfigSvc().initialTCK = '0x11291600'  # see assign_first_tck.qmt
HltConfigSvc().ConfigAccessSvc = accessSvc.getFullName()
ApplicationMgr().ExtSvc.insert(0, HltConfigSvc().getFullName())

gaudi = GaudiPython.AppMgr()
gaudi.config()
gaudi.initialize()

LHCb = GaudiPython.gbl.LHCb
# Taking `std` from `cppyy.gbl` (or `GaudiPython.gbl` which is equivalent) is broken in GCC9
# https://root-forum.cern.ch/t/incomplete-python-bindings-under-gcc9/37071
std = ROOT.std
# Make sure relevant types are instantiated, if not PyROOT segfaults!
std.vector(std.string)
std.set(std.string)
std.bitset(LHCb.RawBank.LastType)
std.vector(std.bitset(LHCb.RawBank.LastType))

ALL_RAWBANKS = [LHCb.RawBank.typeName(i) for i in range(LHCb.RawBank.LastType)]


def dummyDecReports(decisions):
    drs = LHCb.HltDecReports()
    drs.setConfiguredTCK(int(HltConfigSvc().initialTCK, 16))
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
    return set(
        LHCb.RawBank.typeName(i)
        for i, bit in enumerate(reversed(x.to_string())) if bit == '1')


def test_svc(svc, confLocations, confTurboPPLines, confRawBankTypes):
    # Brute force test of all combinations of HDRs and line subsets
    all_lines = sorted(confLocations.keys())
    all_subsets = list(
        chain.from_iterable(
            combinations(all_lines, r) for r in range(len(all_lines) + 1)))
    all_decisions = list(product(*([[False, True]] * 3)))
    for subset in all_subsets:
        lines = std_set(subset)
        for decs in all_decisions:
            decisions = dict(zip(all_lines, decs))
            hdr = dummyDecReports(decisions)

            expected = set(
                chain.from_iterable(
                    confLocations[line] for line in subset if decisions[line]))
            result = set(svc.locationsToPersist(hdr, lines))
            assert result == expected, (
                "hdr={} lines={} result={} expected={}".format(
                    decisions, subset, result, expected))
            # print 'Locations check {} == {}'.format(len(result), len(expected))

            expected = set(
                chain.from_iterable(
                    confLocations[line] for line in subset
                    if decisions[line] and line in confTurboPPLines))
            result = set(svc.turboPPLocationsToPersist(hdr, lines))
            assert result == expected, (
                "hdr={} lines={} result={} expected={}".format(
                    decisions, subset, result, expected))
            # print 'TurboPPLocations check {} == {}'.format(len(result), len(expected))

            expected = set(
                chain.from_iterable(confRawBankTypes[line] for line in subset
                                    if decisions[line]))
            result = from_bitset(svc.rawBanksToPersist(hdr, lines))
            assert result == expected, (
                "hdr={} lines={} result={} expected={}".format(
                    decisions, subset, result, expected))
            # print 'RawBankTypes check {} == {}'.format(len(result), len(expected))

    # Test that extra lines in confLocations are not a problem
    hdr = dummyDecReports({
        'Hlt2FirstLineDecision': True,
        'Hlt2SecondLineDecision': True
    })
    lines = std_set(['Hlt2FirstLineDecision', 'Hlt2SecondLineDecision'])
    result = set(svc.locationsToPersist(hdr, lines))
    assert result == set(confLocations['Hlt2FirstLineDecision'] +
                         confLocations['Hlt2SecondLineDecision'])

    # Test failure if line is not in confLocations
    hdr = dummyDecReports({
        'Hlt2SecondLineDecision': True,
        'Hlt2MissingLineDecision': True
    })
    lines = std_set(['Hlt2FirstLineDecision', 'Hlt2MissingLineDecision'])
    try:
        result = svc.locationsToPersist(hdr, lines)
    except Exception as e:
        assert 'Decision name not in Locations map.' in e.args[0]

    # Test that extra lines in confRawBankTypes are not a problem
    hdr = dummyDecReports({
        'Hlt2FirstLineDecision': True,
        'Hlt2SecondLineDecision': True
    })
    lines = std_set(['Hlt2FirstLineDecision', 'Hlt2SecondLineDecision'])
    result = from_bitset(svc.rawBanksToPersist(hdr, lines))
    assert result == set(confRawBankTypes['Hlt2FirstLineDecision'] +
                         confRawBankTypes['Hlt2SecondLineDecision'])

    # Test failure if line is not in confRawBankTypes
    hdr = dummyDecReports({
        'Hlt2SecondLineDecision': True,
        'Hlt2MissingLineDecision': True
    })
    lines = std_set(['Hlt2FirstLineDecision', 'Hlt2MissingLineDecision'])
    try:
        result = svc.rawBanksToPersist(hdr, lines)
    except Exception as e:
        assert 'Decision name not in RawBankTypes map.' in e.args[0]


# Test the HltLinePersistenceSvc
hltsvc = gaudi.service(svcconf.getFullName(), 'ILinePersistenceSvc')
test_svc(hltsvc, svcconf.Locations, svcconf.TurboPPLines, svcconf.RawBankTypes)

# Test the TCKLinePersistenceSvc
tcksvc = gaudi.service(tcksvcconf.getFullName(), 'ILinePersistenceSvc')
mappedLocations = {
    line: [tcksvcconf.ContainerMap.get(loc, loc) for loc in locs]
    for line, locs in svcconf.Locations.items()
}
test_svc(tcksvc, mappedLocations, svcconf.TurboPPLines, svcconf.RawBankTypes)

print 'PASS'  # just a comforting message
