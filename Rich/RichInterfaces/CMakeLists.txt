################################################################################
# Package: RichInterfaces
################################################################################
gaudi_subdir(RichInterfaces v1r0)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Det/RichDet
                         Rich/RichUtils)

gaudi_add_dictionary(RichInterfaces
                     dict/RichInterfacesDict.h
                     dict/RichInterfacesDict.xml
                     INCLUDE_DIRS AIDA Boost RichDet
                     LINK_LIBRARIES LHCbKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(RichInterfaces)
