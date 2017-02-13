################################################################################
# Package: RichFutureInterfaces
################################################################################
gaudi_subdir(RichFutureInterfaces v1r0)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Rich/RichFutureUtils)

gaudi_add_dictionary(RichFutureInterfaces
                     dict/RichFutureInterfacesDict.h
                     dict/RichFutureInterfacesDict.xml
                     INCLUDE_DIRS AIDA Boost
                     LINK_LIBRARIES LHCbKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(RichFutureInterfaces)
