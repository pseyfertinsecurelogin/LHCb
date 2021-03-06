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
## @file PartProp/Check.py
#  the trivial example to inspect the particle data table from the data base
#  @author Vanya BELYAEV vanya@nikhef.nl
#  @date  2009-03-06
# =============================================================================
"""
The trivial example to inspect the particle data table from the data base
"""
# =============================================================================
__author__ = "Vanya BELYAEV vanya@nikhef.nl"
__verison__ = " "
# =============================================================================

from Gaudi.Configuration import *
from Configurables import LHCbApp, LHCb__ParticlePropertySvc, ApplicationMgr

## access to DBASE:
LHCbApp(
    DDDBtag='',  # HEAD
    CondDBtag='')  # HEAD

## let application manager to know about the service:
ApplicationMgr().ExtSvc += [LHCb__ParticlePropertySvc()]

## switch to GaudiPython
from GaudiPython.Bindings import AppMgr
from GaudiKernel.SystemOfUnits import micrometer
import PartProp.Service

# instantiate the actual (C++) application manager
gaudi = AppMgr()

# switch off all algorithms
gaudi.TopAlg = []

# initialize everything properly
gaudi.run(1)

# get the actual (C++) particle properties service
ppsvc = gaudi.ppSvc()

# finally: play with the service

# get all a self-conjugated particles
print ' Self-charge conjugated particles '
selfcc = ppsvc.get(lambda x: x.selfcc())
print selfcc

# set self-charge conjugated conjugated mesons:
print ' CC-neutral mesons'
mesonscc = ppsvc.get(lambda x: (x.selfcc() and x.isMeson()))
print mesonscc

# set self-charge conjugated conjugated mesons:
print ' CC-neutral beauty mesons'
mesonscc = ppsvc.get(lambda x: (x.isMeson() and x.hasBottom()))
print mesonscc

# get "stable" particles ( lifetime in excess of 1 micrometer
print ' STABLE particles (c*tau> 1 um)'
stable = ppsvc.get(lambda x: (x.ctau() > micrometer))
print stable
