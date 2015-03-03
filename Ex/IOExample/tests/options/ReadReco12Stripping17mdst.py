from Configurables import IOTest
from GaudiConf.IOHelper import IOHelper

IOTest( EvtMax = 5, LoadAll = True, DataContent = 'MDST' )

ioh = IOHelper('ROOT')
ioh.setupServices()
ioh.inputFiles(['PFN:Reco12-Stripping17.mdst'])
