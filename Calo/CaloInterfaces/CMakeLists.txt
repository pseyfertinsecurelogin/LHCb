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

gaudi_add_dictionary(CaloInterfaces
                     dict/CaloInterfacesDict.h
                     dict/CaloInterfacesDict.xml
                     INCLUDE_DIRS Event/DigiEvent
                     LINK_LIBRARIES CaloDetLib L0Event RecEvent GaudiKernel LHCbKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(CaloInterfaces)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
