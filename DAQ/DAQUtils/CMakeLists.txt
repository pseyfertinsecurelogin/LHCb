################################################################################
# Package: DAQUtils
################################################################################
gaudi_subdir(DAQUtils v1r14)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/HltEvent
                         Event/RecEvent
                         GaudiAlg
                         GaudiKernel
                         GaudiSvc
                         Phys/LoKiHlt
			 DAQ/DAQKernel)

find_package(RELAX)
find_package(Boost)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


gaudi_add_module(DAQUtils
                 src/*.cpp
                 INCLUDE_DIRS RELAX
                 LINK_LIBRARIES RELAX DAQEventLib DAQKernelLib HltEvent RecEvent GaudiAlgLib GaudiKernel LoKiHltLib)

gaudi_install_headers(Event)


gaudi_add_test(QMTest QMTEST)
