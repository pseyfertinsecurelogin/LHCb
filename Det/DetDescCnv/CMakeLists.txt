################################################################################
# Package: DetDescCnv
################################################################################
gaudi_subdir(DetDescCnv v4r19)

gaudi_depends_on_subdirs(Det/DetDesc
                         GaudiKernel
                         Tools/XmlTools)

find_package(XercesC)
find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS}
                           ${ROOT_INCLUDE_DIRS}
                           ${XERCESC_INCLUDE_DIRS})

gaudi_add_library(DetDescCnvLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS DetDescCnv
                  INCLUDE_DIRS XercesC
                  LINK_LIBRARIES XercesC DetDescLib GaudiKernel XmlToolsLib)

gaudi_add_module(DetDescCnv
                 src/component/*.cpp
                 INCLUDE_DIRS XercesC
                 LINK_LIBRARIES XercesC DetDescLib GaudiKernel XmlToolsLib DetDescCnvLib)

