# Test to copy a POOL digi file to a POOL digi file
from Configurables import IOTest
from GaudiConf.IOHelper import IOHelper

IOTest( EvtMax = 5, DataContent = "DIGI", WithMC = True )

ioh = IOHelper('POOL','POOL')
ioh.setupServices()
ioh.inputFiles(['PFN:rfio:/castor/cern.ch/user/c/cattanem/Boole/v19r2/30000000-100ev-20090407-MC09.digi'])
ioh.outStream('PFN:POOL.digi',"InputCopyStream")
