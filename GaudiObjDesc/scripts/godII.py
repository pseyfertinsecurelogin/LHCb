#!/usr/bin/env python
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
import os
import sys
# Ensure we can find the GaudiObjDesc module even if the PYTHONPATH does not
# have it. (Assume the standard relative locations of the modules)
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(__file__)), 'python'))

from GaudiObjDesc.godII import godII

g = godII(sys.argv)
g.doit()
