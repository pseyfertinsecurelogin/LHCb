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
