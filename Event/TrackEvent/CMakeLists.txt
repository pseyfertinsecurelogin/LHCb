################################################################################
# Package: TrackEvent
################################################################################
gaudi_subdir(TrackEvent v6r7)

gaudi_depends_on_subdirs(GaudiObjDesc
                         Kernel/LHCbKernel)

find_package(Boost)
find_package(GSL)
find_package(LLVM REQUIRED)

include_directories(${LLVM_INCLUDE_DIRS})

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_unit_test(test_deref tests/src/test_deref.cpp
                    LINK_LIBRARIES GaudiKernel TrackEvent
                    TYPE Boost)

gaudi_add_library(TrackEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS GSL Boost LLVM
                  LINK_LIBRARIES GSL Boost LHCbKernel LLVM)

god_build_dictionary(xml/*.xml
                     EXTEND dict/lcgDict.h
                     INCLUDE_DIRS GSL Boost LLVM
                     LINK_LIBRARIES GSL Boost LHCbKernel TrackEvent LLVM)

