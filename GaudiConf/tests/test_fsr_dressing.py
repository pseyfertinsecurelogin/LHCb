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
from GaudiConf import IOHelper

def testType(ioh):
    
    from Configurables import RecordStream
    
    RecordStream('FileRecord').Output="DATAFILE='FishyStuff.dst' TYP='NOTYPE' OPT='REC'"
    
    RecordStream('FileRecord2').Output="DATAFILE='FishyStuff.dst' TYP='NOTYPE' OPT='REC'"
    
    RecordStream('FileRecord3').Output="DATAFILE='FishyStuff.dst' TYP='POOL_ROOTTREE' OPT='REC'"
    
    RecordStream('FileRecord4').Output="DATAFILE='FishyStuff.dst' SVC='RootCnvSvc' OPT='REC'"
    
    RecordStream('FileRecord5').Output="DATAFILE='FishyStuff.dst' SVC='FileRecordCnvSvc' OPT='REC'"
    
    RecordStream("RubbishRecord").EvtDataSvc="CrappyNonexistingService"
    
    ioh.debugIO()
    
    ioh.convertStreams()
    
    ioh.debugIO()

persistencies=["ROOT","MDF"]

for Persistency in persistencies:
    print "====================="
    print Persistency
    testType(IOHelper(Persistency,Persistency))
    

print "====================="
