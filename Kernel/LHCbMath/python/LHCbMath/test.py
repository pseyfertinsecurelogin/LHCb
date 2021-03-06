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
# -*- coding: utf-8 -*-
# =============================================================================
## @file
#  Test-file for various "with error" objects
#  @see Gaudi::Math::ValueWithError
#  @see Gaudi::Math::Point3DWithError
#  @see Gaudi::Math::Vector3DWithError
#  @see Gaudi::Math::LorentzVectorWithError
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
# =============================================================================

import LHCbMath.Types

from GaudiPython.Bindings import gbl as cpp

Gaudi = cpp.Gaudi

p1 = Gaudi.XYZPoint(0, 1, 2)
print '3D-point:       ', p1

v1 = Gaudi.XYZVector(2, 1, 0)

lv1 = Gaudi.LorentzVector(2, 1, 0, 3)

l1 = Gaudi.Math.XYZLine(p1, v1)

pl1 = Gaudi.Plane3D(1, 1, 1, 0)

print '3D-point       (x,y,z)           :', p1
print '3D-vector      (x,y,z)           :', v1
print 'Lorentz-vector (px,py,pz;E)      :', lv1
print 'Line           (point,direction) :', l1
print 'Plane          (point,normal)    :', pl1

l2 = Gaudi.Vector2(100, 200)
l3 = Gaudi.Vector3(100, 200, 300)
l4 = Gaudi.Vector4(100, 200, 300, 400)
l5 = Gaudi.Vector5(100, 200, 300, 400, 500)

print 'SVector<2>                       :', l2
print 'SVector<3>                       :', l3
print 'SVector<4>                       :', l4
print 'SVector<5>                       :', l5

ve1 = Gaudi.Math.ValueWithError(10, 1)
vp1 = Gaudi.Math.Point3DWithError(Gaudi.XYZPoint(1, 2, 3))
vv1 = Gaudi.Math.Vector3DWithError(Gaudi.XYZVector(2, 3, 4))
vl1 = Gaudi.Math.LorentzVectorWithError(Gaudi.LorentzVector(2, 3, 4, 5))

print 'ValueWithError                   :', ve1, ve1 + 10.1, 13.5 * ve1
print 'Point3DWithError                 :', vp1, 24.5 * vp1
print 'Vector3DWithError                :', vv1, vv1 / 33
print 'LorentzVectorWithError           :', vl1, vl1 * 3.14

v3 = Gaudi.Math.SVector3WithError()
v4 = Gaudi.Math.SVector4WithError()

print 'SVector3WithError                :', v3, vp1.asVector()
print 'SVector4WithError                :', v4, vl1.asVector()

pnt1 = Gaudi.XYZPoint(-1, -2, -3)
pnt2 = Gaudi.XYZPoint(1, -2, -3)
line1 = Gaudi.Math.XYZLine(Gaudi.XYZPoint(0, 1, 2), Gaudi.XYZVector(1, 1, 1))
line2 = Gaudi.Math.XYZLine(Gaudi.XYZPoint(1, 3, 0), Gaudi.XYZVector(1, -1, 2))
plane1 = Gaudi.Math.Plane3D(0, 1, 2, 3)
plane2 = Gaudi.Math.Plane3D(1, 8, 9, 0)
plane3 = Gaudi.Math.Plane3D(4, 5, 6, -1)

print ' line  : intersect          : ', line1.intersect(plane1)
print ' plane : intersect two      : ', plane1.line(plane2)
print ' plane : intersect three    : ', plane1.point(plane2, plane3)
print ' plane : intersect two      : ', plane1.intersect(plane2)
print ' plane : intersect three    : ', plane1.intersect(plane2, plane3)
print ' line  : impactParameter    : ', line1.impactParameter(pnt1)
print ' point : impactParameter    : ', pnt1.impactParameter(line1)
print ' line  : distance           : ', line1.distance(line2)
print ' line  : closestPoints      : ', line1.closestPoints(line2)
print ' line  : closestPointParams : ', line1.closestPointParams(line2)
print ' line  : closestPoint       : ', line1.closestPoint(pnt2)
print ' point : closestPoint       : ', pnt2.closestPoint(line1)
print ' line  : closestPointParam  : ', line1.closestPointParam(pnt1)
print ' point : closestPointParam  : ', pnt1.closestPointParam(line1)
print ' line  : parallel           : ', line1.parallel(line2)

print ' Binomial eff: 3/4   : ', Gaudi.Math.binomEff(3, 4)
print ' Binomial eff: 1/100 : ', Gaudi.Math.binomEff(1, 100)
print ' Binomial eff: 1/100 : ', Gaudi.Math.binomEff(
    1, 100).toString("( %.3f +- %.3f )")

mtrx1 = Gaudi.SymMatrix3x3()
mtrx1[0, 0] = 0.01
mtrx1[1, 1] = 0.02
mtrx1[2, 2] = 0.03

mtrx2 = Gaudi.SymMatrix4x4()
mtrx2[0, 0] = 0.04
mtrx2[1, 1] = 0.04
mtrx2[2, 2] = 0.04
mtrx2[3, 3] = 0.04

pars = Gaudi.Math.ParticleParams(
    Gaudi.XYZPoint(1, 1, 2), Gaudi.LorentzVector(10, 10, 10, 20), 10, mtrx1,
    mtrx2, 0.25, Gaudi.Matrix4x3(), Gaudi.Vector3(), Gaudi.Vector4())

print 'ParticleParams ', pars

print 'Before mass-constrained fit:', pars
fitted, chi2 = pars.fitMass(5.0)
mom = fitted.momentum()
print 'Mass-constrained fit:', fitted, chi2, mom.M()
print ' ', mom.sigma2Mass2(), mom.sigma2Mass(), mom.sigmaMass()

print mom.cov2(0, 0), mom.cov2(1, 1), mom.cov2(2, 2), mom.cov2(3, 3)

fitted2, chi2 = fitted.fitMass(5.0)
mom = fitted2.momentum()
print 'Mass-constrained fit:', fitted2, chi2, mom.M()
print ' ', mom.sigma2Mass2(), mom.sigma2Mass(), mom.sigmaMass()

print 'symmatrix   \n', mom.cov2()

print 'sym  matrix \n', pars.momCovMatrix()
print 'sym  matrix \n', pars.posCovMatrix()
print 'corr matrix \n', pars.momPosCov()

# =============================================================================
# The  END
# =============================================================================
