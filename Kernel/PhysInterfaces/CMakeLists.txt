################################################################################
# Package: PhysInterfaces
################################################################################
gaudi_subdir(PhysInterfaces)

gaudi_depends_on_subdirs(GaudiAlg
                         Event/PhysEvent
                         GaudiKernel)

find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})

gaudi_add_library(PhysInterfacesLib
                  src/*.cpp
                  PUBLIC_HEADERS Kernel
                  LINK_LIBRARIES PhysEvent GaudiKernel GaudiAlgLib)

gaudi_add_dictionary(PhysInterfaces
                     dict/PhysInterfacesDict.h
                     dict/PhysInterfacesDict.xml
                     LINK_LIBRARIES PhysEvent GaudiKernel PhysInterfacesLib
                     OPTIONS "-U__MINGW32__")

