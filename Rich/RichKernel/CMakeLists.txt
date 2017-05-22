################################################################################
# Package: RichKernel
################################################################################
gaudi_subdir(RichKernel v8r0)

gaudi_depends_on_subdirs(GaudiAlg
                         GaudiUtils
                         Kernel/LHCbKernel
                         Rich/RichUtils
                         Rich/RichInterfaces)

#message(STATUS "CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")
find_package(AIDA)
find_package(Boost)
find_package(VDT)

gaudi_add_library(RichKernelLib
                  src/lib/*.cpp
                  PUBLIC_HEADERS RichKernel
                  INCLUDE_DIRS AIDA Boost VDT RichUtils LHCbKernel
                  LINK_LIBRARIES Boost VDT GaudiAlgLib GaudiUtilsLib RichUtils LHCbKernel)

gaudi_add_module(RichKernel
                 src/component/*.cpp
                 INCLUDE_DIRS AIDA Boost RichUtils LHCbKernel
                 LINK_LIBRARIES Boost GaudiAlgLib GaudiUtilsLib LHCbKernel RichUtils RichKernelLib)

gaudi_install_python_modules()
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
