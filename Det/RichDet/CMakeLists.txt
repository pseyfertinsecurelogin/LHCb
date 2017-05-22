################################################################################
# Package: RichDet
################################################################################
gaudi_subdir(RichDet v17r7)

gaudi_depends_on_subdirs(Det/DetDescCnv
                         Rich/RichUtils)

find_package(Boost)
find_package(GSL)

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
                     OPTIONS "-U__MINGW32__")

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
