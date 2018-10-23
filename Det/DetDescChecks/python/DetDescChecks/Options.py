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
def LoadDDDBTest(datatype, sim=False, loctags=True, paths=None):
    '''
    Configure the application for the LoadDDDB test (load whole detector
    description).
    '''
    from Configurables import (ApplicationMgr, LoadDDDB, DDDBConf, CondDB,
                               GaudiSequencer)

    # Configure the detector description
    DDDBConf(DataType = datatype,
             Simulation = sim,
             IgnoreHeartBeat = True,
             EnableRunStampCheck = False)
    # Set up conditions database configuration (ignore checks for up-to-date)
    CondDB(Upgrade = datatype == "Upgrade",
           LatestGlobalTagByDataTypes = [datatype],
           LatestLocalTagsByDataType  = [datatype] if loctags else [])
    # Set up application
    main = GaudiSequencer(MeasureTime=True)
    if not paths:
        main.Members = [ LoadDDDB() ]
    else:
        main.Members = [ LoadDDDB('loader_%d' % i, Node = path)
                        for i, node in enumerate(paths) ]

    app = ApplicationMgr()
    app.EvtSel = "NONE"
    app.EvtMax = 1
    app.TopAlg = [ main ]

    # Ensure that the first event time is the one used at initialize
    # (based on the datatype).
    from Gaudi.Configuration import appendPostConfigAction
    def bindFakeEventTime():
        from Configurables import EventClockSvc, FakeEventTime
        ecs = EventClockSvc()
        ecs.addTool(FakeEventTime, "FakeEventTime")
        ecs.FakeEventTime.StartTime = ecs.getProp("InitialTime")
    appendPostConfigAction(bindFakeEventTime)
