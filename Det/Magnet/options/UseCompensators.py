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
from Gaudi.Configuration import *

from Configurables import MagneticFieldSvc
# MBXW compensator
mbxwh = MagneticFieldSvc("MBXWHFieldSvc")
# MBXWS correctors
mbxwsL = MagneticFieldSvc("MBXWSLFieldSvc")
mbxwsR = MagneticFieldSvc("MBXWSRFieldSvc")

mbxwh.UseConditions = False
mbxwsL.UseConditions = False
mbxwsR.UseConditions = False

mbxwh.FieldMapFiles = ["$FIELDMAPROOT/cdf/compensator.MBXWH.001.cdf"]
mbxwsL.FieldMapFiles = ["$FIELDMAPROOT/cdf/compensator.MBXWS.1L8.001.cdf"]
mbxwsR.FieldMapFiles = ["$FIELDMAPROOT/cdf/compensator.MBXWS.1R8.001.cdf"]

mbxwsL.ForcedSignedCurrentScaling = -1.

from Configurables import MultipleMagneticFieldSvc
MultipleMagneticFieldSvc().MagneticFieldServices += ["MagneticFieldSvc/MBXWHFieldSvc", \
                                                     "MagneticFieldSvc/MBXWSLFieldSvc", \
                                                     "MagneticFieldSvc/MBXWSRFieldSvc"]

