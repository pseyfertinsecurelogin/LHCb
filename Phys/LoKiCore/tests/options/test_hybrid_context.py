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
from Gaudi.Configuration import DEBUG
from Configurables import ApplicationMgr, LHCbApp, LoKiSvc, HistogramPersistencySvc
from Configurables import LoKi__VoidFilter as VoidFilter

# TODO why is this still needed? (taken from LHCbApp)
from Configurables import AlgContextSvc
AlgContextSvc(BypassIncidents=True)

# Get rid of warnings
HistogramPersistencySvc().OutputLevel = 5
LoKiSvc().Welcome = False

inner = VoidFilter(
    'Inner',
    Preambulo=[
        """
# Check that CONTAINS is decorated correctly
print "Inner, CONTAINS =", CONTAINS
assert "Inner" in repr(CONTAINS)
f1 = CONTAINS('/Event/Location') > 0
    """
    ],
    Code="f1",
    OutputLevel=DEBUG)

algorithm = VoidFilter(
    'Algorithm',
    Preambulo=[
        """
# Check that CONTAINS is decorated correctly
print "Algorithm, CONTAINS =", CONTAINS
assert "Algorithm" in repr(CONTAINS)
f1 = CONTAINS('/Event/Location') > 0

# Check that COUNTER, which inherits from CONTAINS, is decorated correctly
print "Algorithm COUNTER =", COUNTER
assert "Algorithm" in repr(COUNTER)
f2 = COUNTER('/Event/Location', 'counter')

from LoKiHlt.algorithms import *
# the following will initialize Inner inside of Algorithm's initialization
functor = execute('Inner')

# Check there is no leak after Inner is initialized
assert "Algorithm" in repr(CONTAINS)
"""
    ],
    Code="functor",
    OutputLevel=DEBUG)

app = ApplicationMgr()
# `inner` is in the list just to make it known to Gaudi, but is otherwise
# initialized by `algorithm`'s initialize as seen by the DEBUG messages
app.TopAlg = [algorithm, inner]
