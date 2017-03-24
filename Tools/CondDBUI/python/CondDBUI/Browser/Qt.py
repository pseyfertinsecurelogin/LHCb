# Wrapper around different PyQt versions
try:
    from __version__ import __qt_version__
except ImportError:
    __qt_version__ = '4'

if __qt_version__ == '4':
    # use the same API as PyQt5
    import sip
    sip.setapi('QDate', 2)
    sip.setapi('QDateTime', 2)
    sip.setapi('QString', 2)
    sip.setapi('QTextStream', 2)
    sip.setapi('QTime', 2)
    sip.setapi('QUrl', 2)
    sip.setapi('QVariant', 2)
    from PyQt4 import Qt
elif __qt_version__ == '5':
    from PyQt5 import Qt
else:
    raise ImportError('unsupprted version of Qt: {0}'.format(__qt_version__))

import sys
sys.modules[__name__] = Qt
