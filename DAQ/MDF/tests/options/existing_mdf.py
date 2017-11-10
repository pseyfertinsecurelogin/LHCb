from __future__ import print_function
import os

from GaudiConf import IOExtension, IOHelper
from Configurables import (
    LHCb__MDFWriter as MDFWriter,
    LHCbApp,
    EventSelector,
    IODataManager,
    HistogramPersistencySvc,
)
from PRConfig.TestFileDB import test_file_db
import GaudiPython

FILENAME = 'existing.mdf'
EVTMAX = 100
EXISTING_SIZE = EVTMAX * 200 * 1024

# Write a dummy file that should be larged than the MDFWriter output
with open(FILENAME, "wb") as f:
    f.write(bytearray([0xff] * EXISTING_SIZE))

# Setup the input data
test_file_db['HltDAQ-routingbits_full'].run(configurable=LHCbApp())
EventSelector().PrintFreq = 100

# Get rid of warnings
IODataManager().DisablePFNWarning = True
HistogramPersistencySvc().OutputLevel = 5

# Setup the MDF writer
writer = MDFWriter('Writer', Connection='file://' + FILENAME)
IOHelper("MDF", "MDF").outStream(FILENAME, writer, writeFSR=False)

gaudi = GaudiPython.AppMgr()
gaudi.run(EVTMAX)
gaudi.stop()
gaudi.finalize()

size = os.stat(FILENAME).st_size
print('Existing file size:', EXISTING_SIZE)
print('Current file size:', size)
assert size < EXISTING_SIZE, 'Existing file was not truncated'
