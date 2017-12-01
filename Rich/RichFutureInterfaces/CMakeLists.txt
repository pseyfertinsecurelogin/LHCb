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
                     OPTIONS " -U__MINGW32__ -Wno-undefined-var-template ")

# Note - Above -Wno-undefined-var-template is to work around an issue in Vc 1.3.0.
#        To be removed once Vc is updated.

gaudi_install_headers(RichFutureInterfaces)
