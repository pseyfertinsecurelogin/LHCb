################################################################################
# Package: RichFutureKernel
################################################################################

gaudi_subdir(RichFutureKernel v1r0)

gaudi_depends_on_subdirs(GaudiAlg
                         GaudiUtils
                         Kernel/LHCbKernel
                         Rich/RichUtils
                         Rich/RichInterfaces)

find_package(AIDA)
find_package(Boost)
find_package(VDT)

gaudi_add_library(RichFutureKernel
                  src/*.cpp
                  PUBLIC_HEADERS RichFutureKernel
                  INCLUDE_DIRS AIDA Boost VDT RichUtils LHCbKernel
                  LINK_LIBRARIES Boost VDT GaudiAlgLib GaudiUtilsLib RichUtils LHCbKernel)

gaudi_install_python_modules()
