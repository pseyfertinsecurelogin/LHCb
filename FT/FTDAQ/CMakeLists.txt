################################################################################
# Package: FTDAQ
################################################################################
gaudi_subdir(FTDAQ v1r8)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/FTEvent
                         DAQ/DAQKernel
                         Det/DetDesc
                         Det/FTDet
                         GaudiAlg)

find_package(Boost)

gaudi_add_module(FTDAQ
                 src/*.cpp
                 INCLUDE_DIRS Boost Event/FTEvent
                 LINK_LIBRARIES DAQEventLib DAQKernelLib GaudiAlgLib DetDescLib FTDAQLib)

gaudi_add_library(FTDAQLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS FTDAQ
                  INCLUDE_DIRS Boost Event/RawEvent
                  LINK_LIBRARIES DAQEventLib)
