################################################################################
# Package: RichFutureUtils
################################################################################
gaudi_subdir(RichFutureUtils v1r0)

gaudi_depends_on_subdirs(GaudiUtils
                         Kernel/LHCbKernel
                         Rich/RichUtils)

find_package(ROOT)
find_package(Vc)

include_directories(SYSTEM ${ROOT_INCLUDE_DIRS} ${Vc_INCLUDE_DIR})

gaudi_add_library(RichFutureUtils
                  src/*.cpp
                  PUBLIC_HEADERS RichFutureUtils
                  INCLUDE_DIRS Kernel/LHCbKernel Rich/RichUtils
                  LINK_LIBRARIES GaudiUtilsLib LHCbKernel RichUtils)

gaudi_add_dictionary(RichFutureUtils
                     dict/RichFutureUtilsDict.h
                     dict/RichFutureUtilsDict.xml
                     INCLUDE_DIRS AIDA Boost
                     LINK_LIBRARIES GaudiUtilsLib LHCbKernel RichUtils RichFutureUtils
                     OPTIONS " -Wno-undefined-var-template -U__MINGW32__ ")

# Note - Above -Wno-undefined-var-template is to work around an issue in Vc 1.3.0.
#        To be removed once Vc is updated.

target_link_libraries( RichFutureUtils     "${Vc_LIB_DIR}/libVc.a" )

