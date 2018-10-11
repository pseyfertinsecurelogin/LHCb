###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
from Configurables import LHCbApp
from PRConfig import TestFileDB
TestFileDB.test_file_db['2016-lb2l0gamma.strip.dst'].run(configurable=LHCbApp())

from Configurables import L0Conf, DataOnDemandSvc, ApplicationMgr
ApplicationMgr().ExtSvc += [ DataOnDemandSvc() ]
L0Conf( FullL0MuonDecoding = True , EnableL0DecodingOnDemand = True )

from Configurables import L0CaloFix2016
L0CaloFix2016( ApproximateMethod = False , Simulation = True , TCK = '0x160F' )

import GaudiPython
appMgr = GaudiPython.AppMgr()
evt = appMgr.evtsvc()

counter_electron = 0

for i in xrange( 1000 ) :
    appMgr.run(1)
    l0du = evt['Trig/L0/L0DUReport']
    if l0du.channelDecisionByName( "Electron" ):
        counter_electron += 1

print "Number of L0 electron decisions = ", counter_electron
