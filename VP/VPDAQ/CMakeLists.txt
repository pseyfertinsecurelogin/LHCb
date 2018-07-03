################################################################################
# Package: VPDAQ
################################################################################
gaudi_subdir(VPDAQ v2r8)

gaudi_depends_on_subdirs(Det/VPDet
                         DAQ/DAQUtils
			 DAQ/DAQKernel	
                         Event/DAQEvent
                         Event/DigiEvent
                         VP/VPKernel 
                         GaudiAlg
                         Kernel/LHCbKernel)

find_package(AIDA)
find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(VPDAQ
                 src/*.cpp
                 INCLUDE_DIRS AIDA Event/DigiEvent Pr/PrKernel
                 LINK_LIBRARIES VPDetLib TrackEvent DAQEventLib DAQKernelLib GaudiAlgLib LHCbKernel)

