// redefined anyway in features.h by _GNU_SOURCE
#undef _XOPEN_SOURCE
#undef _POSIX_C_SOURCE

#include "Python.h"
#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#include "CondDBUIHelper/Helpers.h"
