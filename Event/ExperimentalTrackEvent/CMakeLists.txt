################################################################################
# Package: ExperimentalTrackEvent
################################################################################
gaudi_subdir(ExperimentalTrackEvent v1r0)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Kernel/SOAContainer
                         Event/TrackEvent)

gaudi_add_library(ExperimentalTrackEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS Boost ROOT SOAContainer
                  LINK_LIBRARIES LHCbKernel Boost TrackEvent)
