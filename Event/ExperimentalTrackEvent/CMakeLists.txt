################################################################################
# Package: ExperimentalTrackEvent
################################################################################
gaudi_subdir(ExperimentalTrackEvent v1r0)

find_package(Boost)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Event/TrackEvent)

gaudi_add_library(ExperimentalTrackEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event Boost
                  LINK_LIBRARIES LHCbKernel Boost TrackEvent)
