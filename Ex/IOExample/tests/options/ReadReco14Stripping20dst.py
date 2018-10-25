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
from Configurables import IOTest
from GaudiConf.IOHelper import IOHelper

from Configurables import ApplicationMgr, createODIN
odin = createODIN()
ApplicationMgr().TopAlg += [ odin ]

IOTest( EvtMax = 5, LoadAll = True )

ioh = IOHelper('ROOT')
ioh.setupServices()
ioh.inputFiles(['PFN:Reco14-Stripping20.dst'])
