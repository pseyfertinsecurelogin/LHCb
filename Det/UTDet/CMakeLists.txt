################################################################################
# Package: UTDet
################################################################################
gaudi_subdir(UTDet v1r0)

gaudi_depends_on_subdirs(Det/DetDesc
                         Det/DetDescCnv
                         Kernel/LHCbKernel)

find_package(Boost)

gaudi_add_library(UTDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS UTDet
                  INCLUDE_DIRS Boost
                  LINK_LIBRARIES Boost DetDescLib DetDescCnvLib LHCbKernel)

gaudi_add_module(UTDet
                 src/Component/*.cpp
                 INCLUDE_DIRS Boost
                 LINK_LIBRARIES Boost DetDescLib DetDescCnvLib LHCbKernel UTDetLib)

gaudi_add_dictionary(UTDet
                     dict/UTDetDict.h
                     dict/UTDetDict.xml
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost DetDescLib DetDescCnvLib LHCbKernel UTDetLib
                     OPTIONS "-U__MINGW32__")

