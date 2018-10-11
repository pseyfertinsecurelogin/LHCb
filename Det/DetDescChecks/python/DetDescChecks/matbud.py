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
# To run this script:
# > lb-run LHCb/latest bash -f
# > python matbud.py

import os
import getopt, sys
from ctypes import POINTER, c_int, cast, pythonapi
from ROOT import *
from Gaudi.Configuration import *
from Configurables import LHCbApp, CondDB
from GaudiPython.Bindings import gbl, AppMgr
import GaudiPython
from GaudiKernel import SystemOfUnits as unit
from math import *

# BEGIN CONFIG
z1 = 7620. * unit.mm
z2 = 9439. * unit.mm
eta_min = 1.5
eta_max = 5.5
version = '0.1'
contact = 'nis.meinert@cern.ch'
# END CONFIG

def isInteractiveMode():
    if os.environ.get('PYTHONINSPECT'):
        return True
    iflag_ptr = cast(pythonapi.Py_InteractiveFlag, POINTER(c_int))
    return iflag_ptr.contents.value != 0
interactiveMode = isInteractiveMode()

print ' __  __    _  _____ ____  _   _ ____      _       '
print '|  \/  |  / \|_   _| __ )| | | |  _ \  __| |_   _ '
print '| |\/| | / _ \ | | |  _ \| | | | | | |/ _` | | | |'
print '| |  | |/ ___ \| | | |_) | |_| | |_| | (_| | |_| |'
print '|_|  |_/_/   \_\_| |____/ \___/|____/ \__,_|\__, |'
print '                                            |___/ '

if interactiveMode:
    vtmp = '(v{0} - interactive mode)\n'
else:
    vtmp = '(v{0})\n'
print vtmp.format(version)
print 'In case of bugs or questions, contact: {0}\n'.format(contact)

try:
    opts = getopt.getopt(sys.argv[1:], 'hd:p:', ['help', 'dddb=', 'persist=', 'z1=', 'z2=', 'eta1=', 'eta2='])
except getopt.GetoptError as e:
    print str(e)
    usage()
    sys.exit(1)

def usage():
    print 'To run this script:'
    print '> lb-run LHCb/latest bash -f'
    print '> python matbud.py'
    print ' or interactively via:'
    print '> python -i matbud.py'
    print '\nAvailable options are:'
    print '    -h, --help:                                      Print this dialog'
    print '    -d<PATH TO lhcb.xml>, --dddb=<PATH TO lhcb.xml>: Set path to DDDB/lhcb.xml or DDDB*.db'
    print '    -p<file.root>, --persist=<file.root>:            Save histograms in <file.root>'
    print '    --z1=<z low>:                                    Start value for integration in mm (default {0})'.format(z1)
    print '    --z2=<z high>:                                   End value for integration in mm (default {0})'.format(z2)
    print '    --eta1=<eta low>:                                Lower eta boundary (default {0})'.format(eta_min)
    print '    --eta2=<eta high>:                               Upper eta boundary (default {0})'.format(eta_max)

dddbroot = None
ofname = None
for opt, arg in opts[0]:
    if opt in ('-h', '--help'):
        usage()
        sys.exit()
    elif opt in ('-d', '--dddb'):
        dddbroot = arg
    elif opt in ('-p', '--persist'):
        ofname = arg
    elif opt in ('--z1'):
        z1 = float(arg) * unit.mm
    elif opt in ('--z2'):
        z2 = float(arg) * unit.mm
    elif opt in ('--eta1'):
        eta_min = float(arg)
    elif opt in ('--eta2'):
        eta_max = float(arg)
        
if not interactiveMode and ofname is None:
    usage()
    exit(1)

assert z1 < z2
assert eta_min < eta_max

print 'Integrate from z={0}mm to z={1}mm and eta={2} to eta={3}'.format(z1, z2, eta_min, eta_max)
print 'Read detector configuration from \'{0}\''.format(dddbroot)
if not interactiveMode:
    print 'Save histograms in \'{0}\''.format(ofname)
else:
    print 'You are running in interactive mode. Histograms will not be stored automatically!'


LHCbApp().DataType   = "Upgrade"
LHCbApp().Simulation = True
CondDB().Upgrade = True

if dddbroot is not None:
    dddbroot = dddbroot.strip()
    assert os.path.isfile(dddbroot), 'No such file \'{0}\''.format(dddbroot)

    if dddbroot.endswith('.xml'):
        from Configurables import DDDBConf
        DDDBConf().DbRoot = dddbroot
    else:
        assert dddbroot.endswith('.db'), 'Unsupported format of DDDB'
        CondDB().addLayer(dbFile = dddbroot, dbName = "DDDB")
else:
    print 'Using default DDDB'

from Configurables import MessageSvc
MessageSvc().Format = '% F%40W%S%7W%R%T %0W%M'

appMgr = AppMgr(outputlevel = 4)
appMgr.ExtSvc += ['TransportSvc']
det = appMgr.detSvc()

transSvc = appMgr.service('TransportSvc', 'ITransportSvc')

# define intersections
std = GaudiPython.gbl.std
Interval = std.pair('double', 'double')
Intersection = std.pair(Interval,'const Material*')
Intersections = std.vector(Intersection)
intersepts = Intersections()
tickMin = 0.
tickMax = 1.

def getRadiationAndInteractionLength(intersepts):
    fracAbsLength_rad = 0.
    fracAbsLength_int = 0.

    for i in intersepts:
        
        # this is not the total dist, but normalized to 'totalDist'
        # 'totalDist' is the length of vector v, filled in
        # TransportSvc::intersections(_, v, _, _, intersepts)
        dist = (i.first.second - i.first.first)

        material = i.second
        fracAbsLength_rad += dist / material.radiationLength()
        fracAbsLength_int += dist / material.absorptionLength()

    return (fracAbsLength_rad, fracAbsLength_int)


def getAbsRadiationAndInteractionLength(point, vector):
    totalDist = vector.R()
    transSvc.intersections(point, vector, tickMin, tickMax, intersepts)

    return [x * totalDist for x in getRadiationAndInteractionLength(intersepts)] 


def fill(getPointAndVector, hRad, hInt):
    assert hRad.GetNbinsX() == hInt.GetNbinsX()
    assert hRad.GetNbinsY() == hInt.GetNbinsY()

    nxbins = hRad.GetNbinsX()
    nybins = hRad.GetNbinsY()

    for i in range(1, nxbins + 1):
        x = hRad.GetXaxis().GetBinCenter(i) 

        for j in range(1, nybins + 1):
            y = hRad.GetYaxis().GetBinCenter(j)

            point, vector = getPointAndVector(x, y)
            absRad, absInt = getAbsRadiationAndInteractionLength(point, vector)
    
            hRad.Fill(x, y, absRad)
            hInt.Fill(x, y, absInt)


def radialFill_car(x, y):
    point = GaudiPython.gbl.Gaudi.XYZPoint(x, y, z1)
    vector = GaudiPython.gbl.Gaudi.XYZVector(x*(z2-z1)/z1, y*(z2-z1)/z1, z2-z1)

    return (point, vector)


def radialFill_pol(phi, eta):
    fx, fy = getFxFyFromPhiAndEta(phi, eta)

    point = GaudiPython.gbl.Gaudi.XYZPoint(fx*z1, fy*z1, z1)
    vector = GaudiPython.gbl.Gaudi.XYZVector(fx*(z2-z1), fy*(z2-z1), z2-z1)

    return (point, vector)


def horizontalFill(x, y):
    point = GaudiPython.gbl.Gaudi.XYZPoint(x, y, z1)
    vector = GaudiPython.gbl.Gaudi.XYZVector(0., 0., z2-z1)

    return (point, vector)


def getFxFyFromPhiAndEta(phi, eta):

    # beware: 90 degrees tilt to separate left-right
    phirad = (phi+90.)/180. * pi

    theta = 2. * atan(exp(-eta))

    fx = tan(theta) * cos(phirad)
    fy = tan(theta) * sin(phirad)

    return (fx, fy)


if not interactiveMode:
    of = TFile(ofname, 'recreate')

def round_mm2cm(x): return round(x, -1)
fxmax, fymax = getFxFyFromPhiAndEta(270., eta_min)[0], getFxFyFromPhiAndEta(0., eta_min)[1]
xmax, ymax = map(lambda x: round_mm2cm(x*z1), (fxmax, fymax))

#cartitleAppx = ' (shooting from (0,0,0) towards (x,y,{0}mm), between #it{z}=%3d and %3d mm)' % (z1,z1,z2)
poltitleAppx = ' (between #it{z}=%3d and %3d mm, #phi=0 corresponds to 12 o\'clock)' % (z1, z2)
hortitleAppx = ' (between #it{z}=%3d and %3d mm)' % (z1, z2)

#hRad_car = TH2D('radiation_cartesian', 'Radiation length' + cartitleAppx + ';#it{x}[mm];#it{y}[mm]',
#                200, -xmax, xmax, 200, -ymax, ymax)
#
#hInt_car = TH2D('interaction_cartesian', 'Interaction length' + cartitleAppx + ';#it{x}[mm];#it{y}[mm]',
#                200, -xmax, xmax, 200, -ymax, ymax)

hRad_pol = TH2D('radiation_polar', 'Radiation length' + poltitleAppx + ';#phi(#circ);#eta',
                2*72, -180, 180, 2*80, eta_min, eta_max)

hInt_pol = TH2D('interaction_polar', 'Interaction length' + poltitleAppx + ';#phi(#circ);#eta',
                2*72, -180, 180, 2*80, eta_min, eta_max)

hRad_hor = TH2D('radiation_horizontal', 'Radiation length' + hortitleAppx + ';#it{x}[mm];#it{y}[mm]',
                200, -xmax, xmax, 200, -ymax, ymax)

hInt_hor = TH2D('interaction_horizontal', 'Interaction length' + hortitleAppx + ';#it{x}[mm];#it{y}[mm]',
                200, -xmax, xmax, 200, -ymax, ymax)

#fill(radialFill_car, hRad_car, hInt_car);
fill(radialFill_pol, hRad_pol, hInt_pol);
fill(horizontalFill, hRad_hor, hInt_hor);

if not interactiveMode:
    of.Write()
    of.Close()
else:
    print 'Created the following histograms:'
    #print ' - hRad_car: radiation length in cartesian coordinates'
    #print ' - hInt_car: interaction length in cartesian coordinates'
    print ' - hRad_pol: radiation length in polar coordinates'
    print ' - hInt_pol: interaction length in polar coordinates'
    print ' - hRad_hor: radiation length for horizontally shooting'
    print ' - hInt_hor: interaction length for horizontally shooting'
    print '\nThese histograms are available via their respective names, e.g.'
    print '> hRad_pol.Draw(\"colz\")'
