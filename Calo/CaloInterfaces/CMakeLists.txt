################################################################################
# Package: CaloInterfaces
################################################################################
gaudi_subdir(CaloInterfaces v8r9)

gaudi_depends_on_subdirs(Det/CaloDet
                         Event/DigiEvent
                         Event/L0Event
                         Event/RecEvent
                         GaudiKernel
                         Kernel/LHCbKernel)

find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})

gaudi_add_dictionary(CaloInterfaces
                     dict/CaloInterfacesDict.h
                     dict/CaloInterfacesDict.xml
                     INCLUDE_DIRS Event/DigiEvent
                     LINK_LIBRARIES CaloDetLib L0Event RecEvent GaudiKernel LHCbKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(CaloInterfaces)
