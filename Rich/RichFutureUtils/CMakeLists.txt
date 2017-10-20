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
                     OPTIONS "-U__MINGW32__")

target_link_libraries( RichFutureUtils     "${Vc_LIB_DIR}/libVc.a" )
#target_link_libraries( RichFutureUtilsDict "${Vc_LIB_DIR}/libVc.a" )
