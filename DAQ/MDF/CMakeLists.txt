################################################################################
# Package: MDF
################################################################################
gaudi_subdir(MDF v3r45)

gaudi_depends_on_subdirs(Event/DAQEvent
                         GaudiKernel)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})


gaudi_add_library(MDFLib
                  src/*.cpp
                  PUBLIC_HEADERS MDF
                  INCLUDE_DIRS ROOT
                  LINK_LIBRARIES ROOT GaudiKernel DAQEventLib)

gaudi_add_module(MDF
                 components/*.cpp
                 INCLUDE_DIRS ROOT
                 LINK_LIBRARIES ROOT GaudiKernel DAQEventLib MDFLib)

gaudi_add_test(QMTest QMTEST)
