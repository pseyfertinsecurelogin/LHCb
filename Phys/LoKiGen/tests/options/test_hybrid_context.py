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
from Gaudi.Configuration import DEBUG
from Configurables import ApplicationMgr, LoKiSvc, HistogramPersistencySvc
from Configurables import LoKi__GenFilter as GenFilter

# TODO why is this still needed? (taken from LHCbApp)
from Configurables import AlgContextSvc
AlgContextSvc(BypassIncidents=True)

# Get rid of warnings
HistogramPersistencySvc().OutputLevel = 5
LoKiSvc().Welcome = False

algorithm = GenFilter(
    'Algorithm',
    Preambulo=[
        """
# Check that GSOURCE is decorated correctly
print "Algorithm, GSOURCE =", GSOURCE
assert "Algorithm" in repr(GSOURCE)
f1 = GSOURCE('/Event/Location') == True
"""
    ],
    OutputLevel=DEBUG)

app = ApplicationMgr()
app.TopAlg = [algorithm]
