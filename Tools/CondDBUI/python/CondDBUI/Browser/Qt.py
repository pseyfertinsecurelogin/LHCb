# Wrapper around different PyQt versions
from PyQt4 import Qt

import sys
sys.modules[__name__] = Qt
