################################################################################
# Package: MCAssociators
################################################################################
gaudi_subdir(MCAssociators v1r6)

gaudi_depends_on_subdirs(Event/LinkerEvent
                         Event/MCEvent
                         Event/PhysEvent
                         GaudiAlg
                         GaudiKernel)

find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})


gaudi_add_dictionary(MCAssociators
                     dict/MCAssociatorsDict.h
                     dict/MCAssociatorsDict.xml
                     LINK_LIBRARIES LinkerEvent MCEvent PhysEvent GaudiAlgLib GaudiKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(Kernel)

