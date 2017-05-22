################################################################################
# Package: RichFutureDAQ
################################################################################
gaudi_subdir(RichFutureDAQ v1r0)

gaudi_depends_on_subdirs(Det/RichDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Rich/RichFutureKernel
                         Rich/RichDAQKernel
                         Rich/RichUtils)

gaudi_add_module(RichFutureDAQ
                 src/*.cpp
                 INCLUDE_DIRS Rich/RichFutureKernel Event/DAQEvent Event/DAQEvent RichUtils RichDAQKernel
                 LINK_LIBRARIES RichDetLib DAQEventLib RichFutureKernel RichDAQKernel)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
