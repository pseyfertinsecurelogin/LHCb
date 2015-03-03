# Test to copy a POOL dst file to a ROOT dst file
from Configurables import IOTest
from GaudiConf.IOHelper import IOHelper

IOTest( EvtMax = 5 )

ioh = IOHelper('POOL','ROOT')
ioh.setupServices()
ioh.inputFiles(['PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid//lhcb/LHCb/Collision11/DIMUON.DST/00010348/0000/00010348_00000579_1.dimuon.dst?svcClass=lhcbdisk'])
ioh.outStream('PFN:ROOT-Reco09.dst',"InputCopyStream")
