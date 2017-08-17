################################################################################
# Package: XmlTools
################################################################################
gaudi_subdir(XmlTools v6r9)

gaudi_depends_on_subdirs(GaudiKernel
                         Kernel/LHCbKernel)

find_package(Boost)
find_package(ROOT)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS}
                                                 ${XERCESC_INCLUDE_DIRS})

gaudi_add_library(XmlToolsLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS XmlTools
                  INCLUDE_DIRS XercesC
                  LINK_LIBRARIES XercesC GaudiKernel)

gaudi_add_module(XmlTools
                 src/component/*.cpp
                 INCLUDE_DIRS XercesC
                 LINK_LIBRARIES XercesC GaudiKernel XmlToolsLib)

gaudi_add_dictionary(XmlTools
                     dict/XmlToolsDict.h
                     dict/XmlToolsDict.xml
                     INCLUDE_DIRS XercesC
                     LINK_LIBRARIES XercesC GaudiKernel XmlToolsLib
                     OPTIONS "-U__MINGW32__")

