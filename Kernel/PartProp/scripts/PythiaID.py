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
# @date 21/03/2018
# @author Philip Ilten
# Return the property dictionary for a given Pythia8::ParticleDataEntry.
def properties(pid, sgn = 1):
    return {
        'name': pid.name(sgn),
        'threeCharge': pid.chargeType(sgn),
        'hasAnti': pid.hasAnti(),
        'isLepton': pid.isLepton(),
        'isQuark': pid.isQuark(),
        'isDiQuark': pid.isDiquark(),
        'isHadron': pid.isHadron(),
        'isMeson': pid.isMeson(),
        'isBaryon': pid.isBaryon(),
        'jSpin': pid.spinType()
        }

# Generate the PID dictionary and optionally archive.
def generate(dat = None):
    import pythia8
    pythia = pythia8.Pythia('', False)

    # Loop over the particles.
    pid, pids = 1, {}
    while pid != 0:
        pids[pid] = properties(
            pythia.particleData.particleDataEntryPtr(pid))
        if pids[pid]['hasAnti']: pids[-pid] = properties(
            pythia.particleData.particleDataEntryPtr(pid), -1)
        pid = pythia.particleData.nextId(abs(pid))

    # Archive if requested.
    if dat:
        with open(dat, 'w') as dat: dat.write(repr(pids))
    return pids

# Load the Pythia dictionary.
try: pids = eval(file('PythiaID.dat').read())
except: pids = generate('PythiaID.dat')
