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
## @file
#  Small script forcing a time-out in the access to the 
from Gaudi.Configuration  import *
from Configurables import CondDB, CondDBAccessSvc, DDDBConf

DDDBConf()

DDDB = CondDBAccessSvc("DDDB")
DDDB.ConnectionTimeOut = 5

CondDB().Tags["DDDB"] = "dddb-20171030-2"

#MessageSvc(OutputLevel = ERROR)

import GaudiPython
app = GaudiPython.AppMgr()
app.initialize()
app.start()

import time
app.detSvc()["/dd"] # access the DB
print "TEST ===> start"
time.sleep(6) # wait enough
print "TEST ===> end"
