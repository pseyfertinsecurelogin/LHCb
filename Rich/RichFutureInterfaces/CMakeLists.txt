################################################################################
# Package: RichFutureInterfaces
################################################################################
gaudi_subdir(RichFutureInterfaces v1r0)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Rich/RichUtils
                         Rich/RichFutureUtils)

find_package(Vc)

include_directories(SYSTEM ${Vc_INCLUDE_DIR})

gaudi_add_dictionary(RichFutureInterfaces
                     dict/RichFutureInterfacesDict.h
                     dict/RichFutureInterfacesDict.xml
                     INCLUDE_DIRS AIDA Boost
                     LINK_LIBRARIES LHCbKernel RichFutureUtils RichUtils
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(RichFutureInterfaces)
