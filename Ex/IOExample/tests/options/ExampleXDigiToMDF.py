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
from Configurables import LHCbApp, CondDB
from Configurables import LHCb__MDFWriter as MDFWriter
from Gaudi.Configuration import ApplicationMgr
from PRConfig import TestFileDB


app = LHCbApp()
app.EvtMax = 10
app.Simulation = True
app.DataType = "Upgrade"
CondDB().Upgrade = True


mdf_writer = MDFWriter('MDFWriter', Compress=0, ChecksumType=1, GenerateMD5=True,
                       Connection='file://testmdf.mdf')

ApplicationMgr().TopAlg = [mdf_writer]


TestFileDB.test_file_db["upgrade-baseline-FT61-digi"].run()

# if you dont' want to use the TestFileDB use the IOHelper
# from GaudiConf import IOHelper
# IOHelper('ROOT').inputFiles([list of paths to input files])
