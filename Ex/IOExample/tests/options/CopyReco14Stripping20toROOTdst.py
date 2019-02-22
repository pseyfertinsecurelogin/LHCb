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
# Test to ensure a Reco14-Stripping20 dst can be copied
from Configurables import IOTest
from GaudiConf.IOHelper import IOHelper
from PRConfig import TestFileDB

IOTest(EvtMax=5, LoadAll=True)

ioh = IOHelper()
ioh.setupServices()
ioh.outStream('PFN:Reco14-Stripping20.dst', "InputCopyStream")

TestFileDB.test_file_db["S20_stripped_test"].run()
