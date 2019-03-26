# Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
# Author: Paul Seyfert <pseyfert@cern.ch>
#
# This software is distributed under the terms of the GNU General Public
# Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# In applying this licence, CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

from Configurables import MessageSvc
from Gaudi.Configuration import *
from Configurables import Gaudi__Examples__IntDataProducer as IntDataProducer
from Configurables import LHCb__Examples__Harvester as Harvester
from Configurables import LHCb__Examples__Sorter as Sorter
from Configurables import LHCb__Examples__Rater as Rater
from Configurables import LHCb__Examples__Cook as Cook

outputlevel = WARNING
# Application setup
app = ApplicationMgr()
# - Algorithms
app.TopAlg = [
    IntDataProducer(OutputLevel=outputlevel),
    Harvester(OutputLevel=outputlevel),
    Sorter(OutputLevel=outputlevel),
    Rater(OutputLevel=outputlevel),
    Cook(OutputLevel=outputlevel)
]

# - Events
app.EvtMax = 2
app.EvtSel = "NONE"
app.HistogramPersistency = "NONE"

MessageSvc().Format = "% F%80W%S%7W%R%T %0W%M"
