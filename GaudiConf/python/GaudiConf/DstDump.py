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
## @file DstDump.py
#
#  Light version of DstDump module from Bender project to dump the content of (x,mu,s,r,...)DSTs
#
# Usage:
#
# @code
#
#    dst-dump-light [options] file1 [ file2 [ file3 [ file4 ....'
#
#  @endcode
#
#  @date   2011-10-24
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#
# =============================================================================
"""``Light'' version of DstDump module from Bender project to dump the content
of various (x,mu,s,r,...)DST files

Usage:

    dst-dump-light [options] file1 [ file2 [ file3 [ file4 ....'

"""
# =============================================================================
__author__ = 'Vanya BELYAEV Ivan.Belyaev@itep.ru'
__date__ = "2011-10-24"
__version__ = '$Revision$'
__all__ = ()  ## nothing to import
__all__ = ('dumpDst', )
# =============================================================================
## logging
# ============================================================================
import logging, ROOT


# =============================================================================
## get configured logger
#  @code
#  logger1 = getLogger ( 'LOGGER1' )
#  logger2 = getLogger ( 'LOGGER2' , level = logging.INFO )
#  @endcode
def getLogger(name,
              fmt='# %(name)-25s %(levelname)-7s %(message)s',
              level=logging.INFO,
              stream=None):
    """Get the proper logger
    >>> logger1 = getLogger ( 'LOGGER1' )
    >>> logger2 = getLogger ( 'LOGGER2' , level = logging.INFO )
    """
    #
    logger = logging.getLogger(name)
    logger.propagate = False
    ##logger.propagate =  True
    #
    while logger.handlers:
        logger.removeHandler(logger.handlers[0])
    #
    if not stream:
        import sys
        stream = sys.stdout

    lh = logging.StreamHandler(stream)
    fmt = logging.Formatter(fmt)
    lh.setFormatter(fmt)
    logger.addHandler(lh)
    #
    logger.setLevel(level)
    #
    return logger


# =============================================================================
if '__main__' == __name__: logger = getLogger('GaudiConf.DstDump')
else: logger = getLogger(__name__)
# =============================================================================

# =============================================================================
## table format
_fhdr_ = ' | %8s | %15s | %7s | %4s | %6s |'  ## header
_fmt1_ = ' | %8g | %7.1f+-%-6.1f | %-7.1f | %4d | %-6d |'
_fmt2_ = ' | %8d | %7d%8s | %7s | %4s | %6s |'


# =============================================================================
def formatItem(item):
    if item.Min() != item.Max() or 0 != item.rms() or 0 != item.meanErr():
        return _fmt1_ % (item.sum(), item.mean(), item.meanErr(), item.rms(),
                         long(item.Min()), long(item.Max()))
    else:
        return _fmt2_ % (long(item.sum().value()), long(item.mean().value()),
                         '', '', '', '')


# =============================================================================
## the function that prints the table
def print_them(objects, config, iEvent):
    """The function that prints the table
    """

    keys = objects.keys()
    keys.sort()

    length = 25
    for key in keys:
        length = max(length, len(key))
    length += 2 - 7

    _printMessage = []

    lline = ' +' + (length + 58) * '-' + '+'
    _printMessage += [lline]

    message = _fhdr_ % ('Total ', '     Mean     ', '  rms  ', 'min', ' max ')
    message = " | %s %s" % ('Location'.ljust(length), message)

    _printMessage += [message]
    _printMessage += [lline]

    for loc in keys:
        item = objects[loc]

        l = loc.replace('/Event/', '')

        if config.Zeroes:
            while item.nEntries() < _nevents_:
                item += 0

        ## the actual formating
        message = formatItem(item)

        message = " | %s %s" % (l.ljust(length), message)

        _printMessage += [message]

    _printMessage += [lline]
    _printMessage += ["   Analysed %d events" % iEvent]

    logger.info(100 * '*')
    print '\n\n\n'
    ofile = open(config.SummaryFile, 'w')
    for line in _printMessage:
        print line
        print >> ofile, line
    ofile.close()
    print '\n\n\n'
    logger.info(100 * '*')


# =============================================================================
## try to extract the data type, simulation flag and file extension (type)
#
#  @code
#
#    >>> file_names  = ...
#    >>> data_type, simulation,ext = dataType ( file_names )
#
#  @endcode
#
def dataType(files):
    """Extract the data type,
    simulation type and input type from file name(s)

    >>> file_names  = ...
    >>> data_type , simulation , ext = dataType ( file_names )

    """
    #
    if isinstance(files, str): files = [files]
    #
    dtype = ''
    simu = False
    ext = ''
    for f in files:
        #
        fu = f.upper()
        if 0 <= fu.find('COLLISION09'): dtype = '2009'
        elif 0 <= fu.find('COLLISION10'): dtype = '2010'
        elif 0 <= fu.find('COLLISION11'): dtype = '2011'
        elif 0 <= fu.find('COLLISION12'): dtype = '2012'
        elif 0 <= fu.find('COLLISION13'): dtype = '2012'  ## ATTENTION
        elif 0 <= fu.find('COLLISION15'): dtype = '2015'
        elif 0 <= fu.find('COLLISION16'): dtype = '2016'
        elif 0 <= fu.find('COLLISION17'): dtype = '2017'
        elif 0 <= fu.find('COLLISION18'):
            dtype = '2018'
            #
        elif 0 <= fu.find('STRIPPING13'):
            dtype = '2011'
        elif 0 <= fu.find('STRIPPING17'):
            dtype = '2011'
        elif 0 <= fu.find('STRIPPING15'):
            dtype = '2011'
        elif 0 <= fu.find('STRIPPING19'):
            dtype = '2012'
            #
        elif 0 <= fu.find('STRIPPING20R1'):
            dtype = '2011'
        elif 0 <= fu.find('STRIPPING20'):
            dtype = '2012'
            #
        elif 0 <= fu.find('STRIPPING21R1'):
            dtype = '2011'
        elif 0 <= fu.find('STRIPPING21'):
            dtype = '2012'
            #
        elif 0 <= fu.find('STRIPPING22'):
            dtype = '2015'
        elif 0 <= fu.find('STRIPPING23'):
            dtype = '2015'
        elif 0 <= fu.find('STRIPPING24'):
            dtype = '2015'
            #
        elif 0 <= fu.find('STRIPPING26'):
            dtype = '2016'
        elif 0 <= fu.find('STRIPPING28'):
            dtype = '2016'
        elif 0 <= fu.find('STRIPPING29'):
            dtype = '2017'
            #
        elif 0 <= fu.find('2K+10'):
            dtype = '2010'
        elif 0 <= fu.find('2K+11'):
            dtype = '2011'
        elif 0 <= fu.find('2K+12'):
            dtype = '2012'
        elif 0 <= fu.find('2K+13'):
            dtype = '2013'
        elif 0 <= fu.find('2K+15'):
            dtype = '2015'
        elif 0 <= fu.find('2K+16'):
            dtype = '2016'
        elif 0 <= fu.find('2K+17'):
            dtype = '2017'
        elif 0 <= fu.find('2K+18'):
            dtype = '2018'

        #
        if 0 <= fu.find('MC09') or 0 <= fu.find('MC/2009'):
            dtype = '2009'
            simu = True
        elif 0 <= fu.find('MC10') or 0 <= fu.find('MC/2010'):
            dtype = '2010'
            simu = True
        elif 0 <= fu.find('MC11') or 0 <= fu.find('MC/2011'):
            dtype = '2011'
            simu = True
        elif 0 <= fu.find('MC12') or 0 <= fu.find('MC/2012'):
            dtype = '2012'
            simu = True
        elif 0 <= fu.find('MC13') or 0 <= fu.find('MC/2013'):
            dtype = '2013'  ## ??
            simu = True
        elif 0 <= fu.find('MC15') or 0 <= fu.find('MC/2015'):
            dtype = '2015'
            simu = True
        elif 0 <= fu.find('MC16') or 0 <= fu.find('MC/2016'):
            dtype = '2016'
            simu = True
        elif 0 <= fu.find('MC17') or 0 <= fu.find('MC/2017'):
            dtype = '2017'
            simu = True
        elif 0 <= fu.find('MC18') or 0 <= fu.find('MC/2018'):
            dtype = '2018'
            simu = True
        elif 0 <= fu.find('MC/UPGRADE'):
            dtype = 'Upgrade'
            simu = True
        elif 0 <= fu.find('/MC/'):
            simu = True
        elif 0 <= fu.find('PYTHIA'):
            simu = True
        elif 0 <= fu.find('BCVEGPY'):
            simu = True

        flst = f.split()
        for _f in flst:
            _ff = _f.strip().strip('"').strip("'")
            _ff, qm, _ = _ff.partition('?')
            ## allow up to 5 symbols for exension
            first, dot, second = _ff.rpartition('.')
            if dot and 3 <= len(second) <= 5 and not ext: ext = second.upper()

    return dtype, simu, ext


# =============================================================================
## try to extract more information from file names
def hasInFile(files, patterns):
    """Check the presence of certain (uppercased) patterns in the file name(s)
    - return the first found pattern
    """
    #
    if isinstance(files, str): files = [files]
    if isinstance(patterns, str): patterns = [patterns]
    #
    upatterns = [p.upper() for p in patterns]
    for f in files:
        fu = f.upper()
        for p in upatterns:
            if p and 0 <= fu.find(p): return p
    return False


# =============================================================================
## Configure node killing agents for uDST processing
#  @author VanyaBELYAEV Ivan.Belyaev@itep.ru
#  @date   2011-10-05
def killDAQ(nodes=['/Event/DAQ', '/Event/pRec']):
    """Configure node killing agents for uDST processing
    """
    #
    from Configurables import EventNodeKiller
    killer = EventNodeKiller("KillDAQ")

    for node in nodes:
        if node in killer.Nodes: continue
        killer.Nodes.append(node)

    from Configurables import ApplicationMgr
    a = ApplicationMgr()
    a.TopAlg = [killer] + a.TopAlg
    logger.info("Add killer agent for: %s" % killer.Nodes)


# ======================================================================================
## Configure various uDst settings from RootInTES
#
#  @code
#  from PhysConf.MicroDST import uDstConf
#  uDstConf ( '/Event/Leptonic' )
#  @endcode
#
#  ``KillNodes'': list of nodes to be killed.
#  If not specified explicitely, for the regular uDST the following nodes will be removed:
#  - '/Event/DAQ'
#  - '/Event/pRec'
#
#  @param  rootInTes  Root-In-TES for this uDST
#  @param  killNodes  list of nodes to be killed
#
#  @author VanyaBELYAEV Ivan.Belyaev@itep.ru
#  @date   2011-10-05
def uDstConf(rootInTes, killNodes=None):
    """Various settings for mDst

    >>>  from PhysConf.MicroDST import uDstConf
    >>> uDstConf ( '/Event/Leptonic' )

    ``KillNodes'': list of nodes to be killed.
    If not specified explicitely, for the regular uDST following the nodes will be removed:
    - '/Event/DAQ'
    - '/Event/pRec'
    """
    #
    if killNodes is None:
        ## for TURBO streams no need to kill nodes
        #  but they need to be killed for regular uDST
        killNodes = () if '/TURBO' in rootInTes.upper() else ('/Event/DAQ',
                                                              '/Event/pRec')
    elif isinstance(killNodes, str):
        killNodes = (killNodes, )
    elif not isinstance(killNodes, (list, tuple)):
        logger.error('Invalid type of ``killNodes'
                     ':%s/%s' % (killNodes, type(killNodes)))

    #
    ## Never use gfal ...
    #
    from Configurables import Gaudi__IODataManager as IODataManager
    IODataManager("IODataManager").UseGFAL = False
    logger.info("Disable GFAL")

    logger.info("Reconfigure uDST to use    RootInTES='%s'" % rootInTes)
    from Configurables import DataOnDemandSvc
    dod = DataOnDemandSvc()
    #
    ## extra configuration:
    #
    ## 0. Kill Event/DAQ
    #
    if killNodes:
        from Gaudi.Configuration import appendPostConfigAction

        def _killDAQ_():
            return killDAQ(nodes=killNodes)

        appendPostConfigAction(_killDAQ_)
        logger.info("Configure node-killer agent for %s " % str(killNodes))
    #
    ## 1. Copy/Link ODIN, RawEvent and RecSummary
    #
    from Configurables import Gaudi__DataLink as Link
    rawEvt = Link(
        'LinkRawEvent',
        What='/Event/Trigger/RawEvent',
        Target=rootInTes + 'DAQ/RawEvent')
    odin = Link(
        'LinkODIN', What='/Event/DAQ/ODIN', Target=rootInTes + 'DAQ/ODIN')
    summary = Link(
        'LinkSummary',
        What='/Event/Rec/Summary',
        Target=rootInTes + 'Rec/Summary')
    header = Link(
        'LinkHeader',
        What='/Event/Rec/Header',
        Target=rootInTes + 'Rec/Header')
    reports = Link(
        'LinkStripReports',
        What='/Event/Strip/Phys/DecReports',
        Target=rootInTes + 'Strip/Phys/DecReports')

    dod.AlgMap[rawEvt.Target] = rawEvt
    dod.AlgMap[odin.Target] = odin
    dod.AlgMap[summary.Target] = summary
    dod.AlgMap[header.Target] = header
    dod.AlgMap[reports.Target] = reports

    hlt1_reports = Link(
        'LinkHlt1Reports',
        What=rootInTes + 'Hlt1/DecReports',
        Target='/Event/Hlt1/DecReports')

    hlt2_reports = Link(
        'LinkHlt2Reports',
        What=rootInTes + 'Hlt2/DecReports',
        Target='/Event/Hlt2/DecReports')

    dod.AlgMap[hlt1_reports.Target] = hlt1_reports
    dod.AlgMap[hlt2_reports.Target] = hlt2_reports

    logger.info(
        "Configure ODIN,Raw,Summary&Reports RootInTES='%s'" % rootInTes)
    rawEventLoc = rootInTes + 'DAQ/RawEvent'
    from DAQSys.Decoders import DecoderDB
    from DAQSys.DecoderClass import decodersForBank
    for d in decodersForBank(DecoderDB, "L0PU"):
        d.overrideInputs(rawEventLoc)


# ============================================================================
## further suppress printout (suitbale doe "dst-dump"-like scripts
def totalSilence(lst=[
        'RootCnvSvc', 'IOManagerSvc', 'RootHistSvc', 'LHCb::RawDataCnvSvc',
        'HcalDet.Quality', 'EcalDet.Quality', 'MagneticFieldSvc',
        'PropertyConfigSvc', 'ToolSvc.L0DUConfig', 'ToolSvc.L0CondDBProvider',
        'L0MuonFromRaw', 'IntegrateBeamCrossing'
],
                 dod=True):

    from Configurables import MessageSvc, DataOnDemandSvc, ToolSvc
    from Configurables import Gaudi__RootCnvSvc as RootCnvSvc
    from Configurables import Gaudi__IODataManager as IODataManager
    from Configurables import LHCb__RawDataCnvSvc as RawDataCnvSvc

    msg = MessageSvc()
    msg.OutputLevel = 5

    ToolSvc(OutputLevel=5)
    RootCnvSvc("RootCnvSvc", OutputLevel=6)
    RawDataCnvSvc(OutputLevel=6)

    IODataManager('IODataManager', OutputLevel=6, AgeLimit=1, UseGFAL=False)

    if dod:
        DataOnDemandSvc(Dump=True)
    else:
        DataOnDemandSvc(Dump=False, OutputLevel=6)
        msg.setError += ['DataOnDemandSvc']

    msg.setError += lst


def _nodes_(self, forceload=False, node=''):
    """A simple tool to obtain list of nodes in Data Store

    Usage:

    >>> evtSvc = gaudi.evtSvc()
    >>> nodes = evtSvc.nodes('Rec')
    >>> for node in nodes :
    >>>  print node
    >>> nodes = evtSvc.nodes('Rec', forceload = True)
    >>> for node in nodes :
    >>>  print node
    """
    nodelist = []
    if type(node) is str:
        root = self.retrieveObject(node)
        if root: node = root.registry()
        else: return nodelist
    if node.object():
        if self.leaves(node).size() > 0:
            for l in self.leaves(node):
                if forceload:
                    try:
                        temp = self[l.identifier()]
                    except:
                        temp = self[l.identifier()]
                nodelist += self.nodes(forceload, l)
        else:
            nodelist.append(node.identifier())
    else:
        nodelist.append(node.identifier())
    return nodelist


# =============================================================================
## stripped down verison of the function from BenderTools.DstExplorer moodule
def configure(config):
    """Configure the application from parser data
    - stripped down version of the function from BEnderTools.DstExplorer moodule
    """
    #
    if config.OutputLevel <= 3 and not config.Quiet:
        _vars = vars(config)
        _keys = _vars.keys()
        _keys.sort()
        logger.info('Configuration:')
        for _k in _keys:
            logger.info('  %15s : %-s ' % (_k, _vars[_k]))

    if config.Quiet:
        config.OutputLevel = max(config.OutputLevel, 4)
        logger.info('set OutputLevel to be %s ' % config.OutputLevel)

    output_level = config.OutputLevel
    if 6 <= output_level: logging.disable(logging.FATAL - 1)
    elif 5 <= output_level: logging.disable(logging.ERROR - 1)
    elif 4 <= output_level: logging.disable(logging.WARNING - 1)
    elif 3 <= output_level: logging.disable(logging.INFO - 1)
    elif 2 <= output_level: logging.disable(logging.DEBUG - 1)

    ## redefine output level for 'quiet'-mode
    if config.OutputLevel > 5:
        config.OutputLevel = 5
        logger.info('set OutputLevel to be %s ' % config.OutputLevel)

    if config.OutputLevel < 0:
        config.OutputLevel = 0
        logger.info('set OutputLevel to be %s ' % config.OutputLevel)

    from Configurables import EventSelector
    es = EventSelector(OutputLevel=config.OutputLevel)
    if config.OutputLevel >= 5: es.PrintFreq = 1000
    elif config.OutputLevel >= 4: es.PrintFreq = 500
    elif config.OutputLevel >= 3: es.PrintFreq = 100

    #
    # some sanity actions:
    #
    config.RootInTES = config.RootInTES.strip()
    config.files = [i.strip() for i in config.files if i.strip()]

    #
    ## get the file type for the file extension
    #
    files = [i for i in config.files]

    if not files and not config.ImportOptions:
        raise AttributeError('No data files are specified!')

    from Configurables import EventSelector
    from Gaudi.Configuration import importOptions
    from GaudiConf import IOHelper
    ifiles = EventSelector().Input
    if ifiles:
        io = IOHelper()
        ii = []
        for i in ifiles:
            ii.append(io.undressFile(i))
        ifiles = ii

    if not files and not ifiles:
        raise AttributeError('No data files are specified!')

    ## append command line files to the IO helper
    io = IOHelper()
    io.inputFiles(files, clear=False)

    allfiles = files + ifiles

    ## try to get some information from the file names

    ## get some info from file names/extensision
    dtype, simu, ext = None, None, None
    ## try to get information form input file names (via command line)
    dtype, simu, ext = dataType(allfiles)
    if not simu:
        if 'DST' == ext and hasInFile(files, 'AllStreams.dst'):
            simu = True
            logger.info("Simulation flag is activated 'ALLSTREAMS.DST'")
        elif 'LDST' == ext and hasInFile(files, 'AllStreams.ldst'):
            simu = True
            logger.info("Simulation flag is activated 'ALLSTREAMS.LDST'")
        elif 'MDST' == ext and hasInFile(files, 'AllStreams.mdst'):
            simu = True
            logger.info("Simulation flag is activated 'ALLSTREAMS.MDST'")
            logger.debug('DataType,Simu&extension:"%s",%s,"%s" (from files)' %
                         (dtype, simu, ext))

    #
    if ext.upper() in ('GEN', 'XGEN', 'LDST') and not simu: simu = True

    if dtype and dtype != config.DataType:
        logger.info(
            'Redefine DataType from  %s to %s ' % (config.DataType, dtype))
        config.DataType = dtype

    if simu and not config.Simulation:
        logger.info(
            'Redefine Simulation from  %s to %s ' % (config.Simulation, simu))
        config.Simulation = simu

    if config.Simulation and config.Lumi:
        logger.info('suppress Lumi for Simulated data')
        config.Lumi = False

    ## summary information (when available)
    from Configurables import LHCbApp
    LHCbApp().XMLSummary = 'summary.xml'

    lhcb = LHCbApp(
        DataType=config.DataType,
        Simulation=config.Simulation,
        OutputLevel=config.OutputLevel)
    #
    ## DB tags
    #
    tags_set = False
    if config.DDDBtag and config.CondDBtag:
        if not config.NoTagsFromDATA:
            config.NoTagsFromDATA = True
            logger.debug(
                'DDDB/CONDDB Tags are given explicitely, disable extraction from data'
            )
        app = LHCbApp(DDDBtag=config.DDDBtag, CondDBtag=config.CondDBtag)
        tags_set = True
        logger.info('Use DDDBtag   : %s ' % app.DDDBtag)
        logger.info('Use CONDDBtag : %s ' % app.CondDBtag)
    elif config.DDDBtag:
        logger.warning(
            'DDDB   tag is specified but CondDB tag it not specified!')
        app = LHCbApp(DDDBtag=config.DDDBtag)
        logger.info('Use DDDBtag   : %s ' % app.DDDBtag)
    elif config.CondDBtag:
        logger.warning(
            'CondDB tag is specified but DDDB   tag it not specified!')
        app = LHCbApp(CondDBtag=config.CondDBtag)
        logger.info('Use CONDDBtag : %s ' % app.CondDBtag)

    #
    ## try to use the latest available tags, if not specified from the command line
    #
    latest = False
    if not tags_set          and \
       not config.Simulation and \
       not config.DDDBtag    and \
       not config.CondDBtag  and config.DataType in ( '2010' ,
                                                      '2011' ,
                                                      '2012' ,
                                                      '2013' ,
                                                      '2015' ,
                                                      '2016' ,
                                                      '2017' ,
                                                      '2018' ) :
        from Configurables import CondDB
        CondDB(LatestGlobalTagByDataType=config.DataType)
        logger.info('Use latest global tag for data type %s' % config.DataType)
        latest = True

    if 'UPGRADE' == config.DataType.upper():
        from Configurables import CondDB
        db = CondDB(Upgrade=True)
        logger.info('Use Upgrade partition of CondDB')

    if config.IgnoreDQFlags:
        logger.info('DataQuality flags will be ignored')
        lhcb.IgnoreDQFlags = config.IgnoreDQFlags

    if config.MicroDST or 'mdst' == ext or 'MDST' == ext or 'uDST' == ext:
        logger.info('Define input type as micro-DST')
        if not config.RootInTES:
            logger.error('Root-in-TES is not specified for MDST  data!')
        else:
            uDstConf(config.RootInTES)

    from Configurables import EventClockSvc, CondDB
    EventClockSvc(EventTimeDecoder="FakeEventTime")
    logger.debug('Use fake event time decoder')
    CondDB(IgnoreHeartBeat=True)
    logger.debug('Ignore Heart-beat')

    totalSilence()


# =============================================================================
## dump it !
#  @date   2011-10-24
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
def dumpDst(config):
    ##

    logger.info(100 * '*')

    configure(config)

    if 1000 < config.nEvents:
        logger.warning('Large number of events is required to process %s ' %
                       config.nEvents)

    #
    ## decorate some   classes
    #

    ## 1) iDataSvc
    import GaudiPython.Bindings
    GaudiPython.Bindings.iDataSvc.nodes = _nodes_
    ## 2) packed classes
    import ROOT
    LHCb = ROOT.LHCb

    for i in [
            ## RC
        (LHCb.PackedCaloHypos, 'hypos'),
        (LHCb.PackedRecVertices, 'vertices'),
        (LHCb.PackedTwoProngVertices, 'vertices'),
        (LHCb.PackedParticles, 'data'),
        (LHCb.PackedVertices, 'data'),
        (LHCb.PackedMuonPIDs, 'data'),
        (LHCb.PackedRichPIDs, 'data'),
        (LHCb.PackedProtoParticles, 'protos'),
        (LHCb.PackedTracks, 'tracks'),
        (LHCb.PackedClusters, 'clusters'),
        (LHCb.PackedDecReport, 'reports'),
        (LHCb.PackedRelations, 'relations'),
        (LHCb.PackedWeightedRelations, 'relations'),
        (LHCb.PackedFlavourTags, 'data'),
        (LHCb.PackedCaloClusters, 'data'),
        (LHCb.PackedCaloHypos, 'hypos'),
        (LHCb.PackedMuonPIDs, 'data'),
        (LHCb.PackedRelatedInfoRelations, 'data'),
            ##
            ## MC
        (LHCb.PackedMCParticles, 'mcParts'),
        (LHCb.PackedMCVertices, 'mcVerts'),
        (LHCb.PackedMCCaloHits, 'data'),
        (LHCb.PackedMCHits, 'data')
    ]:

        t = i[0]
        m = i[1]

        if not hasattr(t, 'size'):
            t.get_data = getattr(t, m)
            t.__len__ = lambda s: s.get_data().size()
            t.size = lambda s: len(s)

        # =============================================================================
        ## Number of banks in RawEvent
        #  @code
        #  raw_event = ...
        #  print ' total: ', raw_event.nBanks()
        #  print ' Muon : ', raw_event.nBanks( LHCb.RawBank.Muon )
        #  @endcode
        def _re_nbanks_(rawevt, bank_type=None):
            """Number of banks in RawEvent
            >>> raw_event = ...
            >>> print ' total: ', raw_event.nBanks()
            >>> print ' Muon : ', raw_event.nBanks( LHCb.RawBank.Muon )
            """

            if isinstance(bank_type, int):
                return len(rawevt.banks(bank_type))

            _nb = 0
            for _bt in range(0, 100):
                _bn = LHCb.RawBank.typeName(_bt)
                if 'Undefined_name' == _bn: continue
                _banks = rawevt.banks(_bt)
                _nb += len(_banks)
            return _nb

        LHCb.RawEvent._len__ = _re_nbanks_

    #
    ## instantiate the application manager
    #
    gaudi = GaudiPython.Bindings.AppMgr()

    import LHCbMath.Types
    #
    ## copy the lines from Juan's script check_dst_contents.py
    #
    evtSvc = gaudi.evtSvc()

    import ROOT, cppyy
    SE = ROOT.StatEntity  ##  well, it is a class frpm Gaudi

    _nevents_ = 0

    def addEntry(dct, key, val):
        if not dct.has_key(key): dct[key] = SE()
        se = dct[key]
        se += float(val)

    dodSvc = gaudi.service('DataOnDemandSvc')

    nSelEvents = {}
    nObjects = {}

    root = config.RootInTES
    if not root:
        root = '/Event'
        logger.warning('Use "%s" as root, could be non-efficient' % root)

    iEvent = 0
    while True:

        sc = gaudi.run(1)
        _nevents_ += 1

        if sc.isFailure(): break
        #
        if not evtSvc['/Event']: break
        ##
        if 0 < config.nEvents <= iEvent: break
        iEvent += 1

        #
        nodes = evtSvc.nodes(node=root, forceload=True)
        if not nodes:
            logger.warning("No nodes are selected for Root:'%s'" % root)

        nodes = set(nodes)
        links = set()
        dods = set()

        #
        ## explore the regular nodes
        #
        for loc in nodes:
            loc0 = loc
            data = evtSvc[loc]
            loc = loc[:7] + ' ' + loc[7:]
            if not data:
                addEntry(nSelEvents, loc, 0)
                addEntry(nObjects, loc, 0)
            elif type(data) == ROOT.DataObject:
                continue
            else:
                #
                if hasattr(data, 'size'): addEntry(nObjects, loc, data.size())
                elif hasattr(data, '__len__'):
                    addEntry(nObjects, loc, len(data))
                else:
                    addEntry(nObjects, loc, 1)
                #
                ## collect the links (if needed)
                if config.FollowLinks:
                    lnks = data.links()
                    for l in lnks:
                        links.add(l)
                    if '/pRec/DecReport' in loc0:
                        links.add(loc0.replace('/pRec/', '/Rec/'))
        #
        ## follow the links? Useful for packed (u)DST
        #
        if config.FollowLinks:
            links = links - nodes
            for loc in links:

                if config.RootInTES and not config.RootInTES in ('/Event',
                                                                 '/Event/'):
                    if not config.RootInTES in loc: continue

                data = evtSvc[loc]
                loc = loc[:7] + '*' + loc[7:]
                if data is None: continue
                elif type(data) == cpp.DataObject: continue
                elif not data:
                    addEntry(nSelEvents, loc, 0)
                    addEntry(nObjects, loc, 0)
                elif hasattr(data, 'size'):
                    addEntry(nObjects, loc, data.size())
                elif hasattr(data, '__len__'):
                    addEntry(nObjects, loc, len(data))
                else:
                    addEntry(nObjects, loc, 1)

        #
        ## explore locations known for DOD
        #
        if config.DataOnDemand:

            for k in dodSvc.AlgMap.keys():
                dods.add(k)
            for k in dodSvc.NodeMap.keys():
                obj = dodSvc.NodeMap[k]
                if 'DataObject' == obj: continue
                dods.add(k)

            dods = dods - nodes
            dods = dods - links

            for loc in dods:

                if config.RootInTES:
                    if not config.RootInTES in loc: continue

                if not config.Simulation:
                    if 'MC' in loc: continue
                    if 'Prev' in loc: continue
                    if 'Next' in loc: continue

                data = evtSvc[loc]
                loc = loc[:7] + '+' + loc[7:]
                if not data:
                    addEntry(nSelEvents, loc, 0)
                    addEntry(nObjects, loc, 0)
                elif type(data) == cpp.DataObject:
                    continue
                else:
                    #
                    if hasattr(data, 'size'):
                        addEntry(nObjects, loc, data.size())
                    elif hasattr(data, '__len__'):
                        addEntry(nObjects, loc, len(data))
                    else:
                        addEntry(nObjects, loc, 1)

        if 1 < config.DumpFrequency and 0 < iEvent and 0 == iEvent % config.DumpFrequency:
            print_them(nObjects, config, iEvent + 1)

    ## print the final table
    print_them(nObjects, config, iEvent)


# =============================================================================
if '__main__' == __name__:

    logger.info(100 * '*')
    logger.info(__doc__)
    logger.info(100 * '*')
    logger.info(' Author  : %s ' % __author__)
    logger.info(' Version : %s ' % __version__)
    logger.info(' Date    : %s ' % __date__)
    logger.info(100 * '*')

    dumpDst()

    logger.info(100 * '*')

# =============================================================================
# The END
# =============================================================================
