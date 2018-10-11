################################################################################
# Package: CaloFutureDAQ
################################################################################
gaudi_subdir(CaloFutureDAQ v4r39)

gaudi_depends_on_subdirs(CaloFuture/CaloFutureUtils
                         DAQ/DAQUtils
                         DAQ/DAQKernel
                         Det/CaloDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/L0Event
                         Event/RecEvent
                         GaudiAlg)

find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(CaloFutureDAQ
                 src/*.cpp
                 INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent
                 LINK_LIBRARIES CaloFutureUtils CaloDetLib DAQEventLib DAQKernelLib L0Event RecEvent GaudiAlgLib)

gaudi_add_dictionary(CaloFutureDAQ
                     dict/CaloFutureDAQDict.h
                     dict/CaloFutureDAQDict.xml
                     INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent
                     LINK_LIBRARIES CaloFutureUtils CaloDetLib DAQEventLib L0Event RecEvent GaudiAlgLib
                     OPTIONS "-U__MINGW32__ -DBOOST_DISABLE_ASSERTS")

gaudi_install_headers(CaloFutureDAQ)
gaudi_install_python_modules()

