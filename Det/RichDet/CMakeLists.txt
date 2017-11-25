################################################################################
# Package: RichDet
################################################################################
gaudi_subdir(RichDet v17r7)

gaudi_depends_on_subdirs(Det/DetDescCnv
                         Rich/RichUtils)

find_package(Boost)
find_package(GSL)
find_package(Vc)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS}
                           ${Vc_INCLUDE_DIR})     

gaudi_add_library(RichDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS RichDet
                  INCLUDE_DIRS GSL Boost
                  LINK_LIBRARIES GSL Boost DetDescCnvLib RichUtils)

gaudi_add_module(RichDet
                 src/component/*.cpp
                 INCLUDE_DIRS GSL Boost
                 LINK_LIBRARIES GSL Boost DetDescCnvLib RichUtils RichDetLib)

gaudi_add_dictionary(RichDet
                     dict/RichDetDict.h
                     dict/RichDetDict.xml
                     INCLUDE_DIRS GSL Boost
                     LINK_LIBRARIES GSL Boost DetDescCnvLib RichUtils RichDetLib
                     OPTIONS " -Wno-undefined-var-template -U__MINGW32__ ")

# Note - Above -Wno-undefined-var-template is to work around an issue in Vc 1.3.0.
#        To be removed once Vc is updated.
