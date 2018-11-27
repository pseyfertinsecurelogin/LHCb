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
# @date 21/03/2018
# @author Philip Ilten
# Return the property dictionary for a given LHCb::ParticleProperty.
def properties(pid):
    return {
        'name': pid.name(),
        'threeCharge': int(round(pid.charge()*3)),
        'hasAnti': pid.selfcc()
        }

# Generate the PID dictionary and optionally archive.
def generate(dat = None):
    import PartProp.PartPropSvc
    from GaudiPython.Bindings import AppMgr 
    gaudi = AppMgr()
    ppsvc = gaudi.ppSvc()

    # Loop over the particles and add entries.
    pids = {}
    for pid in ppsvc:
        pids[pid.pid().pid()] = properties(pid)

    # Archive if requested.
    if dat:
        with open(dat, 'w') as dat: dat.write(repr(pids))
    return pids

# Load the PID dictionary.
try: pids = eval(file('ParticleProperty.dat').read())
except: pids = generate('ParticleProperty.dat')

