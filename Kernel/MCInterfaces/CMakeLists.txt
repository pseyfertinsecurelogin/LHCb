################################################################################
# Package: MCInterfaces
################################################################################
gaudi_subdir(MCInterfaces v1r19)

gaudi_depends_on_subdirs(Event/GenEvent
                         Event/MCEvent
                         GaudiKernel
                         Kernel/LHCbKernel)

gaudi_add_dictionary(MCInterfaces
                     dict/MCInterfacesDict.h
                     dict/MCInterfacesDict.xml
                     LINK_LIBRARIES TrackEvent GenEvent MCEvent GaudiKernel LHCbKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(MCInterfaces)

