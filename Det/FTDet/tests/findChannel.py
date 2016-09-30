# To run this script:
# lb-run LHCb bash
# python -i findChannel.py
#
import ROOT
from Gaudi.Configuration import *
from Configurables import LHCbApp, CondDB, DDDBConf
from GaudiPython.Bindings import gbl, AppMgr
import GaudiPython

LHCbApp().DataType   = "Upgrade"
LHCbApp().Simulation = True
CondDB().Upgrade = True

# use local xml files
# DDDBConf().DbRoot = "../DDDB/lhcb.xml"

appMgr = AppMgr(outputlevel=4)
det = appMgr.detSvc()
FT = det['/dd/Structure/LHCb/AfterMagnetRegion/T/FT']

# set an arbitrary point in global coordinates
point = gbl.Gaudi.XYZPoint(-2413.17,-1746.5,7848.61)

# get the corresponding module
module = FT.findModule(point)
print module

# Get the local point
localP = module.geometry().toLocal(point)

fraction = ROOT.Double(0.0) # needed fro pass-by-ref of doubles
channel = module.calculateChannelAndFrac(localP.x(),fraction)

print "Hit position = ",point.x(), point.y(),point.z()
print "Hit channel: station=",channel.station(),
print " layer=",channel.layer(),
print " quarter =",channel.quarter(),
print " module =",channel.module(),
print " sipm =",channel.sipm(),
print " channel =",channel.channel()
print "   fraction=",fraction
print
print "In the local frame the position is ", localP.x(), localP.y(), localP.z()
print
