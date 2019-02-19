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
"""
High level configuration tools for LHCb applications
"""
__version__ = "$Id: $"
__author__ = "Marco Cattaneo <Marco.Cattaneo@cern.ch>"

from os import environ
from Gaudi.Configuration import *
from LHCbKernel.Configuration import *
from Configurables import (DDDBConf)
from Configurables import (XMLSummary)
from multiprocessing import cpu_count


class LHCbApp(LHCbConfigurableUser):
    __slots__ = {
        "EvtMax": -1,
        "SkipEvents": 0,
        "DataType": "2012",
        "Detectors": [],
        "LocalDataTypes": [],
        "DDDBtag": "",
        "CondDBtag": "",
        "DQFLAGStag": "",
        "Simulation": False,
        "Monitors": [],
        "OutputLevel": INFO,
        "TimeStamp": False,
        "XMLSummary": None,
        "Persistency": None,
        "IgnoreDQFlags": True,
        "EnableHive": False,
        "ThreadPoolSize": cpu_count(),
        "Scheduler": "AvalancheScheduler",
        "OnlineMode": False
    }

    _propertyDocDct = {
        'EvtMax':
        """ Maximum number of events to process """,
        'SkipEvents':
        """ Number of events to skip """,
        'DataType':
        """ Data type, see DDDBConf for allowed values. Default '2009' """,
        "Detectors":
        """ Maximal list of detector components used in the simulation """,
        "LocalDataTypes":
        """ Set of Local DataTypes uses to set DB """,
        'DDDBtag':
        """ Tag for DDDB. Default as set in DDDBConf for DataType """,
        'CondDBtag':
        """ Tag for CondDB. Default as set in DDDBConf for DataType """,
        "DQFLAGStag":
        """ Tag for DQFLAGS. Default as set in DDDBConf for DataType """,
        'Simulation':
        """ Flag to indicate usage of simulation conditions """,
        'Monitors':
        """ List of monitors to execute """,
        'OutputLevel':
        """ The printout level to use (default INFO) """,
        'TimeStamp':
        """ Flag to add a time stamp to messages (default False) """,
        'XMLSummary':
        """ Add an XML summary file, default None """,
        'Persistency':
        """ Overwrite the default persistency with something else. """,
        'IgnoreDQFlags':
        """ If False, process only events with good DQ. Default is True (process all events)""",
        'EnableHive':
        """ If True, use HiveEventLoopMgr (deafult False) """,
        'ThreadPoolSize':
        """ If EnableHive is set, size of the thread pool (default cpu_count) """,
        "Scheduler":
        "Choose between AvalancheScheduler, HLTEventLoopMgr, HLTControlFlowMgr",
        'OnlineMode':
        """ Set to True for online jobs like monitoring. Default is False """
    }

    __used_configurables__ = [DDDBConf, XMLSummary]

    __nominalDetectors = [
        'velo',
        'puveto',
        'rich1',
        'rich2',
        'rich',
        'tt',
        'it',
        'ot',
        'spd',
        'prs',
        'ecal',
        'hcal',
        'muon',
        'magnet',
        'tr'  # Added "Tr" -> "tr" to remove extra warnings when running Boole, Brunel, etc.
    ]

    __dtMapping = {
        "velo": "Velo",
        "puveto": "PuVeto",
        "muon": "Muon",
        "muonnom1": "MuonNoM1",
        "it": "IT",
        "ot": "OT",
        "tt": "TT",
        "ut": "UT",
        "ft": "FT",
        "vp": "VP",
        "rich": "Rich",
        "richpmt": "RichPmt",
        "rich1": "Rich1",
        "rich2": "Rich2",
        "rich1pmt": "Rich1Pmt",
        "rich2pmt": "Rich2Pmt",
        "rich1horiz": "Rich1Horiz",
        "magnet": "Magnet",
        "compact": "Compact",
        "sl": "SL",
        "ot": "OT"
    }

    def createDetList(self):
        detList = []
        if hasattr(self, "Detectors"):
            import string
            for det in self.getProp("Detectors"):
                detList.append(det.lower())
        return detList

    def upgradeDetectors(self):
        upgradeDetectorList = []
        if self.isPropertySet("Detectors"):
            tmpUpgradeDetectorList = [
                det for det in self.createDetList()
                if det not in self.__nominalDetectors
            ]
            for det in tmpUpgradeDetectorList:
                if self.__dtMapping.has_key(det):
                    upgradeDetectorList.append(self.__dtMapping[det])
                else:
                    log.warning("Unknown Upgrade detector: %s" % (det))
        return upgradeDetectorList

    def knownMonitors(self):
        return ["SC", "FPE"]

    def knownAuditors(self):
        return ["NameAuditor", "MemoryAuditor", "ChronoAuditor"]

    def defineDB(self):
        # Delegate handling of properties to DDDBConf
        self.setOtherProps(DDDBConf(),
                           ["Simulation", "DataType", "OnlineMode"])
        # Set CondDB tags if given, otherwise use default defined in DDDBConf
        from Configurables import CondDB
        if hasattr(self, "DDDBtag"):
            CondDB().Tags["DDDB"] = self.getProp("DDDBtag")
        if hasattr(self, "CondDBtag"):
            if not self.getProp("Simulation"):
                CondDB().Tags["LHCBCOND"] = self.getProp("CondDBtag")
            else:
                CondDB().Tags["SIMCOND"] = self.getProp("CondDBtag")
        if hasattr(self, "DQFLAGStag"):
            CondDB().Tags["DQFLAGS"] = self.getProp("DQFLAGStag")
        # Set up a time decoder for real data (Simulation uses FakeEventTime)
        if not self.getProp("Simulation"):
            from Configurables import EventClockSvc
            ecs = EventClockSvc()
            # do not overwrite already set values
            if not ecs.isPropertySet("EventTimeDecoder"):
                ecs.EventTimeDecoder = "OdinTimeDecoder"

    def defineEvents(self):
        SkipEvents = self.getProp("SkipEvents")
        if SkipEvents > 0:
            if hasattr(EventSelector(), "FirstEvent"):
                log.warning(
                    "EventSelector().FirstEvent and LHCBApp().SkipEvents both defined, using LHCbApp().SkipEvents"
                )
            EventSelector().FirstEvent = SkipEvents + 1

        # Delegate handling to ApplicationMgr configurable
        self.setOtherProps(ApplicationMgr(), ["EvtMax"])

        isSim = self.getProp("Simulation")
        if not isSim:
            ignoreDQ = self.getProp("IgnoreDQFlags")
            if not ignoreDQ:
                from Configurables import DQFilterSvc
                ApplicationMgr().ExtSvc.append(DQFilterSvc())

    def evtMax(self):
        if hasattr(ApplicationMgr(), "EvtMax") and not hasattr(self, "EvtMax"):
            return ApplicationMgr().getProp("EvtMax")
        else:
            return self.getProp("EvtMax")

    def skipEvents(self):
        if hasattr(EventSelector(),
                   "FirstEvent") and not hasattr(self, "SkipEvents"):
            return EventSelector().getProp("FirstEvent") - 1
        else:
            return self.getProp("SkipEvents")

    def defineMonitors(self):
        for prop in self.getProp("Monitors"):
            if prop not in self.knownMonitors():
                if prop in self.knownAuditors():
                    from Configurables import AuditorSvc
                    AuditorSvc().Auditors.append(prop)
                    theConf = getConfigurable(prop)
                    theConf.Enable = True
                else:
                    raise RuntimeError("Unknown monitor '%s'" % prop)
        if "SC" in self.getProp("Monitors"):
            ApplicationMgr().StatusCodeCheck = True
            # Ensure output is always kept
            getConfigurable("StatusCodeSvc").OutputLevel = INFO
        if "FPE" in self.getProp("Monitors"):
            #importOptions( "$STDOPTS/FPEAudit.opts" )
            from Configurables import AuditorSvc
            ApplicationMgr().ExtSvc += ['AuditorSvc']
            AuditorSvc().Auditors += ["FPEAuditor"]

    def defineXMLSum(self):
        if hasattr(self, "XMLSummary"):
            self.setOtherProps(XMLSummary(), ["XMLSummary"])

    def defineOutput(self):
        # Message service
        msgSvc = getConfigurable("MessageSvc")

        # Modify printout defaults
        if self.isPropertySet("OutputLevel"):
            level = self.getProp("OutputLevel")
            # Set the level
            msgSvc.OutputLevel = level
            getConfigurable("ToolSvc").OutputLevel = level
            # Information to be kept
            getConfigurable("EventSelector").OutputLevel = INFO
            getConfigurable("TimingAuditor").OutputLevel = INFO
            getConfigurable("EventLoopMgr").OutputLevel = INFO

        if self.getProp("TimeStamp"):
            # add a time stamp to remaining messages
            msgSvc.Format = "%u % F%18W%S%7W%R%T %0W%M"
            msgSvc.timeFormat = "%Y-%m-%d %H:%M:%S UTC"

    def definePersistency(self):
        '''Use IOHelper to set up the tes and IO services etc.'''
        persistency = None
        if hasattr(self, "Persistency"):
            persistency = self.getProp("Persistency")
        if self.isPropertySet("OnlineMode"):
            if self.getProp("OnlineMode"):
                persistency = "MDF"
        # Set up TES and I/O services
        from GaudiConf.IOHelper import IOHelper
        IOHelper(persistency, persistency).setupServices()

    def setupHive(self):
        '''Enable Hive event loop manager'''
        from Configurables import HiveWhiteBoard
        whiteboard = HiveWhiteBoard("EventDataSvc")
        if not whiteboard.isPropertySet('EventSlots'):
            whiteboard.EventSlots = 10
        ApplicationMgr().ExtSvc.insert(0, whiteboard)
        if self.getProp("Scheduler") == "HLTEventLoopMgr":
            from Configurables import HLTEventLoopMgr
            eventloopmgr = HLTEventLoopMgr()
            self.propagateProperty('ThreadPoolSize', eventloopmgr)
        elif self.getProp("Scheduler") == "HLTControlFlowMgr":
            from Configurables import HLTControlFlowMgr
            eventloopmgr = HLTControlFlowMgr()
            self.propagateProperty('ThreadPoolSize', eventloopmgr)
        elif self.getProp("Scheduler") == "AvalancheScheduler":
            from Configurables import HiveSlimEventLoopMgr, AvalancheSchedulerSvc
            scheduler = AvalancheSchedulerSvc()
            scheduler.CheckDependencies = True
            eventloopmgr = HiveSlimEventLoopMgr(SchedulerName=scheduler)
            self.propagateProperty('ThreadPoolSize', scheduler)
        else:
            raise RuntimeError("Unknown scheduler '%s'" % scheduler)
        ApplicationMgr().EventLoop = eventloopmgr

        from Configurables import UpdateManagerSvc
        UpdateManagerSvc().WithoutBeginEvent = True

    def __apply_configuration__(self):
        if self.getProp("EnableHive"): self.setupHive()
        self.defineDB()
        self.defineEvents()
        self.defineMonitors()
        self.defineXMLSum()
        self.defineOutput()
        self.definePersistency()

        # Restore the default behaviour of AlgContextSvc, which was changed in
        # Gaudi v29r0. This is needed in order to be able to determine the
        # parent of public tools during intitialize (mostly for LoKi functors).
        from Configurables import AlgContextSvc
        AlgContextSvc(BypassIncidents=True)
