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
# Test of UnpackTrack on SDST contaning a track with more than 65k LHCbIDs
from PRConfig import TestFileDB
from Gaudi.Configuration import ApplicationMgr
from Configurables import DumpTracks, UnpackTrack
from GaudiConf.IOHelper import IOHelper

upkTr = UnpackTrack()
ApplicationMgr().TopAlg += [ upkTr, DumpTracks() ]

ioh = IOHelper('ROOT')
ioh.setupServices()
ioh.inputFiles(['PFN:root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/swtest/lhcb/swtest/Reco10-sdst-10events/00011652_00000001_1-evt-18641to18650.sdst'])


#TestFileDB.test_file_db["Reco10-sdst-10events"].run()
