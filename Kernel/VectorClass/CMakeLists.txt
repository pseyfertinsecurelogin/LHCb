################################################################################
# Package: VectorClass
################################################################################
gaudi_subdir(VectorClass v1r26)

gaudi_depends_on_subdirs(GaudiPolicy)

gaudi_install_headers(VectorClass)

gaudi_add_library(VectorClassLib
                  src/ranvec1.cpp
                  PUBLIC_HEADERS VectorClass
                  INCLUDE_DIRS VectorClass)

# Disable some warnings
set_property(SOURCE src/ranvec1.cpp PROPERTY COMPILE_FLAGS " -Wno-overflow -Wno-sign-compare -Wno-strict-aliasing " )
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
