#!/usr/bin/env python
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
# =============================================================================
# =============================================================================
## @file DetDescChecks/CheckOverlap.py
#  Helper file to illustrate the usage of Volume-Check algorithm
#  @see DetDesc::CheckOverlap
#  @author Vanya BELYAEV Icvan.Belyaev@nikhef.nl
# =============================================================================
"""
Helper file to illustrate the usage of Volume-Check algorithm
"""
# =============================================================================
__author__ = 'Vanya BELYAEV Icvan.Belyaev@nikhef.nl'
__version__ = '1.1'
# =============================================================================

from Gaudi.Configuration import *

from Configurables import DDDBConf
DDDBConf(DataType='2015')

from Configurables import DetDesc__VolumeCheck

check = DetDesc__VolumeCheck(
    'Check',
    #
    Shots3D=50000,
    ShotsXY=50000,
    ShotsYZ=50000,
    ShotsZX=50000,
    #
    Volume='/dd/Geometry/MagnetRegion/Magnet/Magnet')

from Configurables import ApplicationMgr

ApplicationMgr(TopAlg=[check], EvtSel="NONE", EvtMax=1)

# =============================================================================
if "__main__" == __name__:

    print __doc__
    print __author__
    print __version__

    from GaudiPython.Bindings import AppMgr

    gaudi = AppMgr()

    gaudi.run(1)

# =============================================================================
# The END
# =============================================================================
