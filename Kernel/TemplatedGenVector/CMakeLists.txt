################################################################################
# Package: TemplatedGenVector
################################################################################
gaudi_subdir(TemplatedGenVector v1r0)

gaudi_depends_on_subdirs(GaudiPolicy)

gaudi_install_headers(TemplatedGenVector)

gaudi_add_library(TemplatedGenVectorLib
                  src/*.cxx
                  PUBLIC_HEADERS TemplatedGenVector
                  INCLUDE_DIRS TemplatedGenVector)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
