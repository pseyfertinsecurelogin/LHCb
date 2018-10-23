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
from GaudiConf import IOHelper


def testthisioh(ioh):
    print 'isRootSupported? ', ioh.isRootSupported()

    print '- filedressing tests'
    
    ifi='PFN:itest.txt'
    dressedI=ioh.dressFile(ifi,"I")
    
    print dressedI
    
    undressedI=ioh.undressFile(dressedI)
    
    if ifi!=undressedI:
        print dressedI,"!=",undressedI
    
    ofi='PFN:otest.txt'
    dressedO=ioh.dressFile(ofi,"O")
    
    print dressedO
    
    undressedO=ioh.undressFile(dressedO)
    
    if ofi!=undressedO:
        print dressedO,"!=",undressedO

    print "- input assignment tests"
    
    ioh.inputFiles(['test1.blah','test2.blah'])
    from Gaudi.Configuration import EventSelector
    print EventSelector().Input
    
    print ioh.selectorString()
    for setPersistency in [True, False]:
        print ioh.helperString(setPersistency=setPersistency)

    import commands
    
    print commands.getstatusoutput('python -c "'+ioh.selectorString().replace('"','\\"')+'"')
    
    for setPersistency in [True, False]:
        print commands.getstatusoutput("python -c '"+ioh.helperString(setPersistency=setPersistency)+"'")
    
    print "- change service tests"
    
    ioh.changeServices()
    print ioh.activeServices()
    

persistencies=[None, "ROOT","MDF"]
if IOHelper().isPoolSupported():
    persistencies.append("POOL")

for persistency in persistencies:
    print '============================='
    print persistency
    print '============================='

    print "- input conversion tests "
    #preload with MDF
    iohm=IOHelper("MDF","MDF")
    iohm.inputFiles(['IAMdf.mdf','IALSOAMdf.blah'])
    
    ioh=IOHelper(persistency, persistency)
    ioh.convertSelector()
    
    from Gaudi.Configuration import EventSelector
    print EventSelector().Input
    print ioh.selectorString()
    print ioh.helperString()
    
    EventSelector().Input=[]
    
    testthisioh(ioh)
