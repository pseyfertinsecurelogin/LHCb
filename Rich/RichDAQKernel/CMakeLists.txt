################################################################################
# Package: RichDAQKernel
################################################################################
gaudi_subdir(RichDAQKernel v1r0)

gaudi_depends_on_subdirs(GaudiKernel
                         Event/DAQEvent
                         Rich/RichUtils)

gaudi_add_library(RichDAQKernel
                  src/*.cpp
                  PUBLIC_HEADERS RichDAQKernel
                  INCLUDE_DIRS Event/DAQEvent GaudiKernel RichUtils
                  LINK_LIBRARIES RichUtils LHCbKernel)
