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

