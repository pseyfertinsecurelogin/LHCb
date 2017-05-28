from GaudiConf import IOHelper
from Gaudi.Configuration import EventSelector, ApplicationMgr
from Configurables import LHCbApp, GaudiSequencer
from PRConfig.TestFileDB import test_file_db
from Configurables import IODataManager
from Configurables import HistogramPersistencySvc
from Configurables import ConfigCDBAccessSvc
from Configurables import HltConfigSvc

# General configuration
app = LHCbApp()
app.DataType = '2016'
app.CondDBtag = 'cond-20160517'
app.DDDBtag = 'dddb-20150724'

files = test_file_db["HltServices-close_cdb_file"].filenames
IOHelper("MDF").inputFiles(files)

IODataManager().DisablePFNWarning = True
HistogramPersistencySvc().OutputLevel = 5

ConfigCDBAccessSvc(File='TCKData/config.cdb')
HltConfigSvc().initialTCK = '0x11291600'
ApplicationMgr().ExtSvc.insert(0, HltConfigSvc().getFullName())


from GaudiPython.Bindings import AppMgr
gaudi = AppMgr()
gaudi.initialize()
gaudi.start()
gaudi.run(1)

TES = gaudi.evtsvc()
dr = TES['/Event/Hlt1/DecReports']
taskID = dr.taskID()
print ('binaryId = {}, hostInstrSetLevel = {}'
       .format(taskID & 0xffff, taskID >> 16))

assert taskID != 0xffffffff
