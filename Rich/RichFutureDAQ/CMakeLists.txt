################################################################################
# Package: RichFutureDAQ
################################################################################
gaudi_subdir(RichFutureDAQ v1r0)

gaudi_depends_on_subdirs(Det/RichDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Rich/RichFutureKernel
                         Rich/RichDAQKernel
                         Rich/RichFutureUtils
                         Rich/RichUtils)

find_package(Boost)
find_package(Vc)

# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${Vc_INCLUDE_DIR})


gaudi_add_module(RichFutureDAQ
                 src/*.cpp
                 INCLUDE_DIRS Rich/RichFutureKernel Rich/RichUtils Rich/RichFutureUtils Rich/RichDAQKernel Event/DAQEvent Event/DAQEvent 
                 LINK_LIBRARIES RichDetLib DAQEventLib RichFutureKernel RichDAQKernel RichUtils RichFutureUtils)
