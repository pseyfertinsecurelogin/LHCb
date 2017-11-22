################################################################################
# Package: RichDAQ
################################################################################
gaudi_subdir(RichDAQ v4r0)

gaudi_depends_on_subdirs(Det/RichDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Rich/RichKernel
                         Rich/RichDAQKernel
                         Rich/RichUtils)

find_package(Vc)

include_directories(SYSTEM ${Vc_INCLUDE_DIR})

gaudi_add_module(RichDAQ
                 src/*.cpp
                 INCLUDE_DIRS Event/DAQEvent Event/DAQEvent RichUtils RichDAQKernel
                 LINK_LIBRARIES RichDetLib DAQEventLib RichKernelLib RichDAQKernel)
