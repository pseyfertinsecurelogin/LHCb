################################################################################
# Package: HltDAQ
################################################################################
gaudi_subdir(HltDAQ v4r28)

gaudi_depends_on_subdirs(Det/DetCond
                         Det/DetDesc
                         Event/DAQEvent
                         Event/HltEvent
                         Event/LumiEvent
                         Event/EventPacker
                         GaudiAlg
                         GaudiKernel
                         GaudiObjDesc
                         Hlt/HltServices
                         Kernel/HltInterfaces
                         Phys/LoKiHlt
			             DAQ/DAQKernel)

find_package(AIDA)
find_package(Boost COMPONENTS iostreams filesystem)

include(GaudiObjDesc)

god_build_headers(xml/*.xml
                  DESTINATION HltDAQ)

gaudi_add_library(HltDAQLib
                 src/lib/*.cpp
                 PUBLIC_HEADERS HltDAQ
                 INCLUDE_DIRS Boost AIDA
                 LINK_LIBRARIES Boost DetCondLib DetDescLib DAQEventLib DAQKernelLib HltEvent GaudiAlgLib GaudiKernel HltInterfaces LoKiHltLib)

gaudi_add_module(HltDAQ
                 src/component/*.cpp
                 INCLUDE_DIRS Boost AIDA HltDAQ
                 LINK_LIBRARIES HltDAQLib Boost DetCondLib DetDescLib DAQEventLib DAQKernelLib HltEvent GaudiAlgLib GaudiKernel HltInterfaces LoKiHltLib)

gaudi_add_unit_test(utestTrackingCoder
                    src/utest/utestTrackingCoder.cpp
                    LINK_LIBRARIES GaudiKernel HltEvent HltDAQLib
                    TYPE Boost)

gaudi_add_test(QMTest QMTEST)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
