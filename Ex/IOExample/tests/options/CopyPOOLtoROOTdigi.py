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
# Test to copy a POOL digi file to a ROOT digi file
from Configurables import IOTest
from GaudiConf.IOHelper import IOHelper
from PRConfig import TestFileDB

IOTest( EvtMax = 5, DataContent = "DIGI", WithMC = True, LoadAll = True )

ioh = IOHelper()
ioh.setupServices()
#ioh.inputFiles(['PFN:castor:/castor/cern.ch/user/c/cattanem/Boole/v19r2/30000000-100ev-20090407-MC09.digi'])
ioh.outStream('PFN:ROOT.digi',"InputCopyStream")

TestFileDB.test_file_db["MC09-pool-digi"].run()
