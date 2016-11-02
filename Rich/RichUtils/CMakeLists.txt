################################################################################
# Package: RichUtils
################################################################################
gaudi_subdir(RichUtils v1r0)

gaudi_depends_on_subdirs(Event/DAQEvent
	                 GaudiUtils
                         Kernel/LHCbKernel)

find_package(AIDA)
find_package(Boost)
find_package(VDT)

#find_package(ROOT COMPONENTS Vc)

gaudi_add_library(RichUtils
                  src/*.cpp
                  PUBLIC_HEADERS RichUtils
                  INCLUDE_DIRS AIDA Boost VDT Kernel/LHCbKernel Event/DAQEvent
                  LINK_LIBRARIES Boost VDT DAQEventLib GaudiUtilsLib LHCbKernel)

gaudi_add_dictionary(RichUtils
                     dict/RichUtilsDict.h
                     dict/RichUtilsDict.xml
                     INCLUDE_DIRS AIDA Boost
                     LINK_LIBRARIES Boost DAQEventLib GaudiUtilsLib LHCbKernel RichUtils
                     OPTIONS "-U__MINGW32__")