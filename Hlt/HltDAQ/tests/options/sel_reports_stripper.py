from Configurables import ApplicationMgr, LHCbApp, GaudiSequencer
from Configurables import HltRoutingBitsFilter
from Configurables import IODataManager
from Configurables import HistogramPersistencySvc
from Configurables import HltSelReportsStripper
from DAQSys.Decoders import DecoderDB

# Setup the input data
from PRConfig.TestFileDB import test_file_db
test_file_db['HltDAQ-routingbits_turbo'].run(configurable=LHCbApp())
IODataManager().DisablePFNWarning = True
HistogramPersistencySvc().OutputLevel = 5

topSeq = GaudiSequencer("TopSequence")
ApplicationMgr().TopAlg = [topSeq]

# Filter nanofied events if the file is HLT2 accepted
content_filter = HltRoutingBitsFilter(
    'RBFilter', RequireMask=[0x0, 0x0, 0x80000000])
topSeq.Members.append(content_filter)


decoder = DecoderDB['HltSelReportsDecoder/Hlt2SelReportsDecoder']
topSeq.Members.append(decoder.setup())

stripper = HltSelReportsStripper()
stripper.Input = decoder.listOutputs()[0]
stripper.Output = 'Hlt2/SelReportsFiltered'
stripper.Lines = [
    'Hlt2DiMuonBDecision',
    'Hlt2Topo2BodyDecision',
    'Hlt2RecSummary',
]
topSeq.Members.append(stripper)


import GaudiPython
gaudi = GaudiPython.AppMgr()
TES = gaudi.evtsvc()
RawBank = GaudiPython.gbl.LHCb.RawBank

# for i in range(2):
while True:
    gaudi.run(1)
    raw = TES['DAQ/RawEvent']
    if not raw:  # no more events
        break

    inp = TES[str(stripper.Input)]
    out = TES[str(stripper.Output)]
    if not inp:  # we didn't have SelReports to process
        continue

    result = set(out.selectionNames())
    expected = set(inp.selectionNames()) & set(stripper.Lines)
    assert result == expected, (
        'The filtered SelReports {} do not match the expectation {}'
        .format(result, expected))

    for name, report in out.selReports().items():
        original = inp.selReport(name)
        assert str(report) == str(original), (
            'The copied SelReport is not the same')
