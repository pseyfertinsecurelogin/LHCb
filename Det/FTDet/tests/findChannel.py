# To run this script:
# lb-run LHCb bash
# python -i findChannel.py
#
from Gaudi.Configuration import *
from Configurables import LHCbApp, CondDB
from GaudiPython.Bindings import gbl, AppMgr
import GaudiPython

LHCbApp().DataType   = "Upgrade"
LHCbApp().Simulation = True
CondDB().Upgrade = True

appMgr = AppMgr(outputlevel=4)
det = appMgr.detSvc()
FT = det['/dd/Structure/LHCb/AfterMagnetRegion/T/FT']

# set an arbitrary point in global coordinates
point = gbl.Gaudi.XYZPoint(-2413.17,-1746.5,7848.61)

# get the corresponding mat
mat = FT.findFibreMat(point)
print mat

hit = gbl.LHCb.MCHit()
disp = gbl.Gaudi.XYZVector(0,0,0.1)
hit.setEntry(point)
hit.setDisplacement(disp)

FTPair = gbl.std.pair(gbl.LHCb.FTChannelID,'double')
ftpair = FTPair()

mat.calculateMeanChannel(hit,ftpair)
channel = ftpair.first
fraction = ftpair.second

print "Hit position = ",point.x(), point.y(),point.z()
print "Hit channel: layer=",channel.layer(),
print " module =",channel.module(),
print " mat =",channel.mat(),
print " sipm =",channel.sipmId(),
print " cell =",channel.sipmCell()
print "   fraction=",fraction

# Get the local point
localP = mat.geometry().toLocal(point)
print
print "In the local frame the position is ", localP.x(), localP.y(), localP.z()
