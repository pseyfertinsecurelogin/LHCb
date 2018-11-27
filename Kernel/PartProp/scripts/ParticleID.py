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
# Return the property dictionary for a given LHCb::ParticleID.
def properties(pid):
    from PartProp.decorators import LHCb
    pid = LHCb.ParticleID(pid)
    return {
        'isSM'          : pid.isSM(),
        'isValid'       : pid.isValid(),
        'isMeson'       : pid.isMeson(),    
        'isBaryon'      : pid.isBaryon(),     
        'isDiQuark'     : pid.isDiQuark(),    
        'isHadron'      : pid.isHadron(),       
        'isLepton'      : pid.isLepton(),       
        'isNucleus'     : pid.isNucleus(),      
        'hasUp'         : pid.hasUp(),          
        'hasDown'       : pid.hasDown(),        
        'hasStrange'    : pid.hasStrange(),     
        'hasCharm'      : pid.hasCharm(),       
        'hasBottom'     : pid.hasBottom(),      
        'hasTop'        : pid.hasTop(),         
        'hasBottomPrime': pid.hasBottomPrime(), 
        'hasTopPrime'   : pid.hasTopPrime(), 
        'isQuark'       : pid.isQuark(),        
        'hasQuarks'     : pid.hasQuarks(),      
        'Z'             : pid.Z(),              
        'A'             : pid.A(),              
        'nLambda'       : pid.nLambda(),        
        'threeCharge'   : pid.threeCharge(),    
        'jSpin'         : pid.jSpin(),          
        'sSpin'         : pid.sSpin(),          
        'lSpin'         : pid.lSpin()
        }
    
# Generate the PID dictionary and optionally archive.
def generate(dat = None):
    import PythiaID, ParticleProperty
    pids = PythiaID.pids
    pids.update(ParticleProperty.pids)
        
    # Loop over the particles and update entries.
    for pid in pids: pids[pid] = properties(pid)
        
    # Archive if requested.
    if dat:
        with open(dat, 'w') as dat: dat.write(repr(pids))
    return pids

# Load the PID dictionary.
try: pids = eval(file('ParticleID.dat').read())
except: pids = generate('ParticleID.dat')

