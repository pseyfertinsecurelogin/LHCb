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
from Gaudi.Configuration import *
import os 
from Configurables import MagneticFieldSvc
from Configurables import MultipleMagneticFieldSvc

side = "R"
magnets = ["Q1", "Q2a", "Q2b", "Q3", "D1"]
files = ["Q1.MQXA.1R8", "Q2a.MQXB.2R8", "Q2b.MQXB.2R8",
         "Q3.MQXA.3R8", "D1.MBX.4R8"]
for magnet, file in zip(magnets, files):
  fieldSvc = MagneticFieldSvc(magnet + side + "FieldSvc")
  fieldSvc.UseConditions = False
  path = os.path.join("$FIELDMAPROOT", "cdf", file + ".3500GeV.cdf")
  fieldSvc.FieldMapFiles = [path]
  svcName = "MagneticFieldSvc/" + magnet + side + "FieldSvc"
  MultipleMagneticFieldSvc().MagneticFieldServices += [svcName]

