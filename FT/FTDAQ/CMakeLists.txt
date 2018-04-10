################################################################################
# Package: FTDAQ
################################################################################
gaudi_subdir(FTDAQ v1r8)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/FTEvent
                         Det/DetDesc
                         DAQ/DAQKernel
                         GaudiAlg)

find_package(Boost)

gaudi_add_module(FTDAQ
                 src/*.cpp
                 INCLUDE_DIRS Boost Event/FTEvent DAQ/DAQUtils Event/DigiEvent
                 LINK_LIBRARIES DAQEventLib DAQKernelLib DetDescLib GaudiAlgLib LHCbKernel RecEvent)

