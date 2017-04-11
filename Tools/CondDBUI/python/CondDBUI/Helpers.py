import cppyy.gbl as _gbl
_gbl.gSystem.Load('libCondDBUIHelperLib.so')
_gbl.gInterpreter.Declare('#include "CondDBUIHelper/Helpers.h"')
Helpers = _gbl.CondDBUI.Helpers
