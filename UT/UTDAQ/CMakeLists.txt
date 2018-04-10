################################################################################
# Package: UTDAQ
################################################################################
gaudi_subdir(UTDAQ v1r0)

gaudi_depends_on_subdirs(DAQ/DAQUtils
                         Det/DetDesc
                         Det/STDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/RecEvent
                         GaudiAlg
                         GaudiKernel
                         Kernel/LHCbKernel
                         ST/STKernel
                         ST/STTELL1Event
                         Si/SiDAQ)


find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_library(UTDAQLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS UTDAQ
                  INCLUDE_DIRS Boost Event/RawEvent DAQ/DAQUtils Event/DigiEvent Si/SiDAQ
                  LINK_LIBRARIES DetDescLib STDetLib DAQEventLib RecEvent GaudiAlgLib GaudiKernel LHCbKernel STKernelLib STTELL1Event)
