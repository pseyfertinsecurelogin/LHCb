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
# To run this script:
# lb-run LHCb bash
# python -i findChannel.py
#
import ROOT
from Gaudi.Configuration import *
from Configurables import LHCbApp, CondDB
from GaudiPython.Bindings import gbl, AppMgr
import GaudiPython

LHCbApp().DataType = "Upgrade"
LHCbApp().Simulation = True
CondDB().Upgrade = True

# use local xml files
#from Configurables import DDDBConf
#DDDBConf().DbRoot = "../DDDB/lhcb.xml"

# use database slice
CondDB().addLayer(
    dbFile="/eos/lhcb/wg/SciFi/Custom_Geoms_Upgrade/databases/DDDB_FT61.db",
    dbName="DDDB")

appMgr = AppMgr(outputlevel=4)
det = appMgr.detSvc()
FT = det['/dd/Structure/LHCb/AfterMagnetRegion/T/FT']

# set an arbitrary point in global coordinates
point = gbl.Gaudi.XYZPoint(-2485.27, -1207.75, 7848.65)

# get the corresponding mat
mat = FT.findMat(point)
print mat

# Get the local point
localP = mat.geometry().toLocal(point)

fraction = ROOT.Double(0.0)  # needed for pass-by-ref of doubles
channel = mat.calculateChannelAndFrac(localP.x(), fraction)

print "Hit position = ", point.x(), point.y(), point.z()
print "Hit channel: station=", channel.station(),
print " layer=", channel.layer(),
print " quarter =", channel.quarter(),
print " module =", channel.module(),
print " mat =", channel.mat(),
print " sipm =", channel.sipm(),
print " channel =", channel.channel()
print "   fraction=", fraction
print
print "In the local frame the position is ", localP.x(), localP.y(), localP.z()
print
