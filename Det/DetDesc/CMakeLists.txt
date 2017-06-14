################################################################################
# Package: DetDesc
################################################################################
gaudi_subdir(DetDesc v19r1)

gaudi_depends_on_subdirs(GaudiKernel
                         GaudiUtils
                         Kernel/LHCbMath
                         Kernel/VectorClass)

find_package(Boost)
find_package(ROOT COMPONENTS MathCore GenVector)

find_path(RANGES_V3_INCLUDE_DIR NAMES range/v3/all.hpp)
if(NOT RANGES_V3_INCLUDE_DIR)
  message(FATAL "required headers from range-v3 missing")
endif()

gaudi_add_library(DetDescLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS DetDesc
                  INCLUDE_DIRS ROOT Boost ${RANGES_V3_INCLUDE_DIR}
                  LINK_LIBRARIES ROOT GaudiKernel GaudiUtilsLib LHCbMathLib)

gaudi_add_dictionary(DetDesc
                     dict/DetDescDict.h
                     dict/DetDescDict.xml
                     INCLUDE_DIRS ROOT
                     LINK_LIBRARIES ROOT GaudiKernel GaudiUtilsLib LHCbMathLib DetDescLib
                     OPTIONS "-U__MINGW32__")
