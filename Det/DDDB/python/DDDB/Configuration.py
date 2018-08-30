"""
High level configuration tools for the detector description.
"""
__author__ = "Marco Clemencic <Marco.Clemencic@cern.ch>"

import re
from Gaudi.Configuration import *
from Configurables import ( CondDBEntityResolver,
                            XmlCnvSvc,
                            XmlParserSvc )


GIT_CONDDBS = {}

# GitEntityResolver may not be available
import Configurables
if ('EntityResolverDispatcher' in Configurables.__all__ and
        'GitEntityResolver' in Configurables.__all__):
    from Configurables import EntityResolverDispatcher, GitEntityResolver
    # look for git DBs
    def _gitconddbpath():
        from os.path import join, isdir
        from itertools import ifilter, imap, chain

        def pathenv(name):
            from os import environ as env, pathsep
            return env.get(name, '').split(pathsep)

        def deduplicate(iterable):
            used = set()
            for i in iterable:
                if i not in used:
                    used.add(i)
                    yield i

        user_path = pathenv('GITCONDDBPATH')
        main_path = imap(lambda p: join(p, 'git-conddb'),
                         pathenv('CMAKE_PREFIX_PATH') +
                         pathenv('CMTPROJECTPATH'))

        return ifilter(isdir, deduplicate(chain(user_path, main_path)))

    for p in _gitconddbpath():
        for dbname in [f.split('.')[0] for f in os.listdir(p)
                       if re.match(r'^[A-Z0-9_]+(\.git)?$', f)]:
            if dbname not in GIT_CONDDBS:
                if os.path.isdir(os.path.join(p, dbname, '.git')):
                    GIT_CONDDBS[dbname] = os.path.join(p, dbname)
                elif os.path.isdir(os.path.join(p, dbname + '.git')):
                    GIT_CONDDBS[dbname] = os.path.join(p, dbname + '.git')
    # allow forcing COOL CondDB if Git CondDBs are found
    if 'NO_GIT_CONDDB' in os.environ:
        GIT_CONDDBS = {}


from DetCond.Configuration import CondDB
from datetime import datetime, timedelta
import xmlrpclib

__all__ = ["DDDBConf"]

class DDDBConf(ConfigurableUser):
    """
    ConfigurableUser for the configuration of the detector description.
    """
    __slots__ = { "DbRoot"    : "git:/lhcb.xml" if "DDDB" in GIT_CONDDBS else "conddb:/lhcb.xml",
                  "DataType"  : "2012",
                  "Simulation": False,
                  "AutoTags"  : False,
                  "InitialTime" : "Safe",
                  "OnlineMode" : False,
                  "IgnoreHeartBeat": False,
                  "EnableRunStampCheck": False
                   }
    _propertyDocDct = {
                       'DbRoot' : """ Root file of the detector description """,
                       'DataType' : """ Symbolic name for the data type. Allowed values: ["2018", "2017", "2016", "2015", "2013", "2012", "2011", "2010", "2009","2008","Upgrade"] """,
                       'Simulation' : """ Boolean flag to select the simulation or real-data configuration """,
                       'AutoTags'  : """ Perform automatic resolution of CondDB tags """,
                       'InitialTime' : """ How to set the initial time. None/'Safe' uses a list of dummy times for each year and sets that time. 'Now' uses the current time. Sepcifying a number assumes that is a time in utc.""",
                       'OnlineMode' : """ To use to run online jobs (Monitoring, ...) """,
                       'IgnoreHeartBeat': """ Disable check on latest update of ONLINE CondDB """,
                       'EnableRunStampCheck': """ Enable the check for run stamp (valid data for the run) """
                       }

    __used_configurables__ = [ CondDB ]

    def __apply_configuration__(self):
        """
        Converts the high-level information passed as properties into low-level configuration.
        """
        ##########################################################################
        # Detector Transient Store Configuration
        ##########################################################################
        # Notes:
        #   - DetDbLocation must use '/' because it is a URL (never use '\')
        #   - DetStorageType:
        #     + 7 => XML
        #     + 9 => pure CondDB (remove "conddb:" from URL)
        ##########################################################################
        detDataSvc = DetectorDataSvc( UsePersistency = True,
                                      DetDbRootName = "dd",
                                      DetDbLocation = self.getProp("DbRoot"),
                                      DetStorageType = 7 )

        using_git = self.getProp("DbRoot").startswith('git:')
        ##########################################################################
        # XML Conversion Service configuration
        ##########################################################################
        # Notes:
        #   - AllowGenericConversion triggers generic detector element conversion
        #     Useful in the case of visualization or in a case the required user
        #     defined detector element converters are somehow not available.
        ##########################################################################
        xmlCnvSvc = XmlCnvSvc(AllowGenericConversion = True)

        # fine tune the defaults for heart-beat and run-stamp checks
        ignore_hb = self.getProp("OnlineMode") or self.getProp("Simulation")
        self._properties["IgnoreHeartBeat"].setDefault(ignore_hb)
        self._properties["EnableRunStampCheck"].setDefault(not ignore_hb)

        if using_git:
            if self.getProp("Simulation"):
                resolvers = [
                    GitEntityResolver('GitDDDB', Ignore="Conditions/.*"),
                    GitEntityResolver('GitSIMCOND'),
                ]
            else:
                resolvers = [
                    GitEntityResolver('GitDDDB', Ignore="Conditions/.*"),
                    GitEntityResolver('GitLHCBCOND', Ignore="Conditions/(Online|DQ).*"),
                    GitEntityResolver('GitONLINE', Ignore="Conditions/DQ.*",
                                      LimitToLastCommitTime=not self.getProp("IgnoreHeartBeat")),
                    GitEntityResolver('GitDQFLAGS', Ignore="Conditions/Online.*"),
                ]
            # set PathToRepository if needed and available
            for r in resolvers:
                r.PathToRepository = (r.PathToRepository if r.isPropertySet('PathToRepository')
                                      else GIT_CONDDBS.get(r.name()[11:], ''))
            # keep only Git resolvers that can be actually used
            resolvers = [ r for r in resolvers if r.PathToRepository ]

            # Leave the outputLevel at INFO even if global level is different
            for r in resolvers:
                if not r.isPropertySet('OutputLevel') : r.setProp( 'OutputLevel', INFO )
                
            # add failover to COOL CondDB
            resolvers.append(EntityResolverDispatcher('FallbackResolver',
                                                      EntityResolvers=[CondDBEntityResolver()],
                                                      Mappings=[(r'^git:', 'conddb:')]))
            resolver = EntityResolverDispatcher(EntityResolvers=resolvers,
                                                Mappings=[(r'^conddb:', 'git:')])
        else:
            resolver = CondDBEntityResolver()

        xmlParserSvc = XmlParserSvc(
                                    # Set the maximum number of files to be put in the
                                    # cache of the XmlCnvSvc
                                    MaxDocNbInCache = 15,

                                    # Change the behavior of the cache. A 0 value makes
                                    # it FIFO while a bigger value tends to make it keep
                                    # only reused files.
                                    CacheBehavior = 3,

                                    # Tell to the XmlParserSvc how to resolve entities
                                    # in the CondDB
                                    EntityResolver = resolver
                                    )

        ##########################################################################
        # Use the XmlCnvSvc in the persistency service for the detector description
        DetectorPersistencySvc( CnvServices = [ xmlCnvSvc ] )


        ##########################################################################
        # DetetectorDataSvc must be external, to finalize after ToolSvc
        ApplicationMgr().ExtSvc.insert(0, detDataSvc)

        # Propagate the properties to the used Configurables
        self.propagateProperties()

        dataType = self.getProp("DataType")
        if not dataType in self.__data_types_handlers__:
            raise ValueError("Invalid value %r for property DetDesc().DataType."
                             " (allowed: %r)"% (dataType,
                                                self.__data_types_handlers__.keys()))
        if dataType in [ "Upgrade" ]:
            sim = self.getProp("Simulation")
            if not sim:
                log.info("%s data is _always_ simulation", dataType )

        if not self.getProp('AutoTags'):
            # calls the specific configuration function for the requested data type
            self.__data_types_handlers__[dataType](self)
            # by default, use the latest DQFLAGS tag for the requested data type
            # (unless already set by a data type handler)
            if not self.getProp("Simulation") and dataType not in ("Upgrade"):
                self.__set_tag__(["DQFLAGS"], "<latest:{0}>".format(dataType))
        else:
            log.info("Ariadne driven configuration requested for CondDB")
            datatype = self.getProp("DataType")
            cond_type = 'LHCBCONDTag' if not self.getProp("Simulation") else 'SIMCONDTag'
            question = {'detector_type': datatype, 'DDDBTag': None, cond_type: None, 'DQFLAGSTag': None}
            self.__auto_tags_conf__(question, criterion = 'latest_LHCBCOND_DDDB')

        # Get particle properties table from condDB
        from Configurables import LHCb__ParticlePropertySvc
        LHCb__ParticlePropertySvc(
           ParticlePropertiesFile = '{protocol}///param/ParticleTable.txt'.format(
               protocol = 'git:' if using_git else 'conddb:'
           )
        )

    def __auto_tags_conf__(self, question, criterion):
        """ Automatic configuration of CondDB tags through the Ariadne system """

        log.debug("Connecting to Ariadne XMLRPC gateway...")
        proxy = xmlrpclib.ServerProxy("http://ariadne-lhcb.cern.ch/xmlrpc/", allow_none = True)

        log.info("Querying Ariadne for a configuration thread...")
        log.debug("Question to Ariadne: %s"%question)
        response = proxy.aquery(question, criterion)
        log.info("Got response from Ariadne, configuring CondDB..")
        log.debug("Response from Ariadne is: %s" %question)

        cond_type = 'LHCBCONDTag' if not self.getProp("Simulation") else 'SIMCONDTag'
        for p in ['DDDB', cond_type.rstrip('Tag'), 'DQFLAGS']:
            tag = response.get(p + 'Tag')
            if not tag:
                raise RuntimeError("No tag found in Ariadne's response for partition %s"%p)
            CondDB().Tags[p] = tag
            log.info("Ariadne driven configuration: Tag '%s' is set for partition '%s'" %(tag, p))


    def __set_tag__(self, partitions, tag):
        cdb = CondDB()
        online = False
        if self.isPropertySet( "OnlineMode" ):
            if self.getProp( "OnlineMode" ):
                online = True
        for p in partitions:
            if p not in cdb.Tags:
                cdb.Tags[p] = tag
                if online:
                    log.info( "Using default tag %s for partition %s", tag, p)
                else:
                    log.warning("Using default tag %s for partition %s", tag, p)
            elif cdb.Tags[p].upper() == "DEFAULT" :
                cdb.Tags[p] = tag
                if online:
                    log.info("Default tag requested for partition %s (using %s)", p, tag )
                else:
                    log.warning("Default tag requested for partition %s (using %s)", p, tag )

    def __set_init_time__(self, utcDatetime):
        """
        Configure the initialization time,
        depends on the value of self.IntitialTime
        Default: using the lower between the proposed time and the current time
        'Now' : use the higher between the current time and the proposed time
        dateime : use this datetime
        """
        #default situation
        if self.getProp("InitialTime").lower()=="safe" or not len(self.getProp("InitialTime")):
            #default situation
            utcDatetime = min(datetime.utcnow(), utcDatetime)
        elif self.getProp("InitialTime").lower()=="now":
            #Moore!
            utcDatetime = max(datetime.utcnow(), utcDatetime)
        else:
            raise TypeError("DDDBConf.InitialTime, I don't have a good way to convert "+self.getProp("InitialTime")+" to a datetime, please use 'Now' or 'Safe'")
        from Configurables import EventClockSvc
        ecs = EventClockSvc()
        # do not overwrite already set values
        if not ecs.isPropertySet("InitialTime"):
            dt = utcDatetime - datetime(1970, 1, 1, 0)
            ns = (dt.days * 24 * 60 * 60 + dt.seconds) * 1000000000
            ecs.InitialTime = ns
        else:
            t = datetime(1970, 1, 1, 0) + timedelta(seconds=ecs.InitialTime/1000000000)
            log.warning("EventClockSvc().InitialTime already set to %s UTC (requested %s UTC)",
                        t.isoformat(), utcDatetime.isoformat())

    def __2018_conf__(self):
        """
        Default configuration for 2018 data
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726-3" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20170724" )
            self.__set_tag__(["CALIBOFF"], "head-2015604" )
            # set initialization time to a safe default
            self.__set_init_time__(datetime(2018, 07, 16, 4, 00))  # during run 195042

    def __2017_conf__(self):
        """
        Default configuration for 2017 data
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726-3" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20170724" )
            self.__set_tag__(["CALIBOFF"], "head-2015604" )
            # set initialization time to a safe default
            self.__set_init_time__(datetime(2017, 11, 26, 10, 30)) # End of fill 6417

    def __2016_conf__(self):
        """
        Default configuration for 2016 data
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726-3" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20170325" )
            self.__set_tag__(["CALIBOFF"], "head-2015604" )
            # set initialization time to a safe default
            self.__set_init_time__(datetime(2016, 12, 5, 5, 3)) # End of fill 5575

    def __2015_conf__(self):
        """
        Default configuration for 2015 data
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726-3" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20171211" )
            self.__set_tag__(["CALIBOFF"], "head-2015604" )
            self.__set_init_time__(datetime(2015, 12, 13, 12, 8)) # End of fill 4720

    def __2013_conf__(self):
        """
        Default configuration for 2013 data (pA run) (and MonteCarlo for DDDB)
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726-2" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20150409-1"   )
            self.__set_init_time__(datetime(2013, 2, 14, 6, 25)) # End of fill 3564

    def __2012_conf__(self):
        """
        Default configuration for 2012 data (and MonteCarlo for DDDB)
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726-2" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20150409-1"   )
            self.__set_init_time__(datetime.utcfromtimestamp(1355694590)) # End of fill 3453

    def __2011_conf__(self):
        """
        Default configuration for 2011 data (and MonteCarlo for DDDB)
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726-1" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20150409"   )
            self.__set_init_time__(datetime.utcfromtimestamp(1319991087)) # End of fill 2267

    def __2010_conf__(self):
        """
        Default configuration for 2010 data (and MonteCarlo for DDDB)
        """
        # Set the tags
        self.__set_tag__(["DDDB"],     "dddb-20180726" )
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "cond-20150527" )
            self.__set_tag__(["DQFLAGS"], "tt-20110126")
            self.__set_init_time__(datetime.utcfromtimestamp(1288505611)) # End of fill 1459

    def __2009_conf__(self):
        """
        Default configuration for 2009 data and corresponding MonteCarlo
        """
        # Set the tags
        self.__set_tag__(["DDDB"], "head-20101206")
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "head-20110614")
            self.__set_tag__(["DQFLAGS"], "tt-20110126")
            self.__set_init_time__(datetime(2009, 12, 31, 21, 0)) # 31/12/2009 21:00
        else:
            self.__set_tag__(["SIMCOND"], "MC-20101026-vc15mm-md100")

    def __2008_conf__(self):
        """
        Default configuration for 2008 data and corresponding MonteCarlo
        """
        # Set the tags
        self.__set_tag__(["DDDB"], "head-20101003")
        if not self.getProp("Simulation"):
            self.__set_tag__(["LHCBCOND"], "head-20101010")
            self.__set_tag__(["DQFLAGS"], "tt-20110126")
            self.__set_init_time__(datetime(2008, 12, 31, 21, 0)) # 31/12/2008 21:00
        else:
            self.__set_tag__(["SIMCOND"], "sim-20090212")

    def __Upgrade_conf__(self):
        """
        Default database for Upgrade MonteCarlo production and analysis
        """
        # Need also to change connection string to DDDB
        CondDB().PartitionConnectionString = {"DDDB":"sqlite_file:$SQLITEDBPATH/DDDB_upgrade.db/DDDB"}

    __data_types_handlers__ =  { "2018": __2018_conf__,
                                 "2017": __2017_conf__,
                                 "2016": __2016_conf__,
                                 "2015": __2015_conf__,
                                 "2013": __2013_conf__,
                                 "2012": __2012_conf__,
                                 "2011": __2011_conf__,
                                 "2010": __2010_conf__,
                                 "2009": __2009_conf__,
                                 "2008": __2008_conf__,
                                 "Upgrade": __Upgrade_conf__ }
