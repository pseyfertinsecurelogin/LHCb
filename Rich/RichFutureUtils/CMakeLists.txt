################################################################################
# Package: RichFutureUtils
################################################################################
gaudi_subdir(RichFutureUtils v1r0)

gaudi_depends_on_subdirs(GaudiUtils
                         Kernel/LHCbKernel
                         Rich/RichUtils)

#find_package(AIDA)
#find_package(Boost)
#find_package(VDT)
#find_package(ROOT COMPONENTS Vc)

gaudi_add_library(RichFutureUtils
                  src/*.cpp
                  PUBLIC_HEADERS RichFutureUtils
                  INCLUDE_DIRS Kernel/LHCbKernel Rich/RichUtils
                  LINK_LIBRARIES GaudiUtilsLib LHCbKernel RichUtils)

gaudi_add_dictionary(RichFutureUtils
                     dict/RichFutureUtilsDict.h
                     dict/RichFutureUtilsDict.xml
                     INCLUDE_DIRS AIDA Boost
                     LINK_LIBRARIES GaudiUtilsLib LHCbKernel RichUtils RichFutureUtils
                     OPTIONS "-U__MINGW32__")
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
