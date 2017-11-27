import os
from GaudiTesting.BaseTest import normalizeExamples, RegexpReplacer, LineSkipper, SortGroupOfLines, BlockSkipper

gitCondDBFixes = (RegexpReplacer(when='Detector description database:',
                                 orig='conddb:/', repl='git:/') +
                  LineSkipper(['CORAL Connection Retrial',
                               'INFO Connected to database',
                               'INFO opening Git repository',
                               'INFO using commit']))

preprocessor = (
    normalizeExamples +
    RegexpReplacer(  # normalize full path to DBASE or PARAM
        orig = r'/[^ :]+/(DBASE|PARAM)/',
        repl = r'\1/') +
    RegexpReplacer(  # hide release directories
        when = "cern.ch",
        orig = r'/afs/cern.ch/lhcb/software/(DEV/nightlies|releases)/|/cvmfs/lhcb.cern.ch/lib/lhcb',
        repl = r'') +
    RegexpReplacer(  # hide data package versions
        orig = r'((DBASE|PARAM)/([^/]+/)?[^/]+)/v[0-9]+r[0-9]+(p[0-9]+)?/',
        repl = r'\1/vXrYpZ/') +
    RegexpReplacer(when = "at 0x########L",
        orig = r'0x########L',
        repl = r'0x########') +
    RegexpReplacer(when = "Connected to database",
        orig = r'ONLINE[_-][0-9 ]{1,6}(.*)"[^"]*/([0-9A-Z_]{1,8})"',
        repl = r'ONLINE_xxxxxx\1"\2"') +
    RegexpReplacer(when = "Connected to database",
        orig = r'"[^"]*/([0-9A-Z_]{1,8})"',
        repl = r'"\1"') +
    LineSkipper(["EventSelector        INFO Stream:EventSelector.DataStreamTool",
                 "INFO Using TAG", "TimingAuditor.T", "RootDBase.open",
                 "INFO Opened magnetic field file",
                 "INFO Connecting to database",
                 "INFO Disconnect from database after being idle",
                 "INFO Memory has changed from",
                 "INFO Memory change after pool release",
                 "Memory for the event exceeds 3*sigma",
                 "Mean 'delta-memory' exceeds 3*sigma",
                 "INFO  'CnvServices':",
                 "DEBUG Property ['Name': Value] =  'IsIOBound':False",
                 "#properties =",
                 "VERBOSE ServiceLocatorHelper::service: found service AlgExecStateSvc",
                 "Run numbers generated from 0 every 0 events",
                 "############ CONFIGURING RawEventFormatConf!! ###############",
                 "INFO  resolving alias TCK/0x",
                 "SUCCESS Number of counters : "
                ],
      regexps = [r"DEBUG Property \['Name': Value\] =  '(Timeline|(Extra|Data)(In|Out)puts)'",
                ]) +
    # Functional framework related
    BlockSkipper("ForwardSchedule...   INFO Data Dependencies for Algorithms:",
                 "ForwardSchedule...   INFO No unmet INPUT data dependencies were found") +
    LineSkipper(["HiveSlimEventLo...   INFO",
                 "ThreadPoolSvc        INFO no thread init tools attached",
                 "AlgResourcePool      INFO",
                 "ForwardSchedule...   INFO",
                ]) +
    RegexpReplacer(when = "Histograms converted successfully according to request",
        orig = r'"HiveSlimEventLo..."',
        repl = r'"EventLoopMgr      "') +
    # Grouping
    SortGroupOfLines(r'.*SUCCESS (Exceptions/Errors/Warnings/Infos Statistics :| #WARNINGS   =| #ERRORS   =|List of booked \dD histograms in directory).*')
   )

from DDDB.Configuration import GIT_CONDDBS
if GIT_CONDDBS:
    preprocessor = preprocessor + gitCondDBFixes
