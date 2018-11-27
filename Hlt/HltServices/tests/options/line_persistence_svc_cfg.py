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
def configure_hlt_svc(name=None):
    from Configurables import HltLinePersistenceSvc
    import GaudiPython

    LHCb = GaudiPython.gbl.LHCb
    ALL_RAWBANKS = [LHCb.RawBank.typeName(i)
                    for i in range(LHCb.RawBank.LastType)]

    svc = HltLinePersistenceSvc(name) if name else HltLinePersistenceSvc()
    svc.Locations = {
        # Regular-turbo-like line
        'Hlt2FirstLineDecision': [
            '/Event/Hlt2FirstLine/Particles',
            '/Event/Hlt2FirstLine/Vertices',
            '/Event/pRec/Protos',
        ],
        # Selective-persistence-like line
        'Hlt2SecondLineDecision': [
            '/Event/Hlt2SecondLine/Particles',
            '/Event/Hlt2SecondLine/Vertices',
            '/Event/Hlt2SecondLine/Extras',
            '/Event/SomeExternalLocation',
            '/Event/pRec/Protos',
        ],
        # Persist-reco-like line
        'Hlt2ThirdLineDecision': [
            '/Event/Hlt2ThirdLine/Particles',
            '/Event/Hlt2ThirdLine/Vertices',
            '/Event/SomeExternalLocation',
            '/Event/OtherExternalLocation',
            '/Event/AnotherExternalLocation',
            '/Event/pRec/Protos',
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

    return svc
