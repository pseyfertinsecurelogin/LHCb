################################################################################
# Package: VectorClass
################################################################################
gaudi_subdir(VectorClass v1r26)

gaudi_depends_on_subdirs(GaudiPolicy)

gaudi_add_library(VectorClassLib
                  src/ranvec1.cpp
                  NO_PUBLIC_HEADERS
                  INCLUDE_DIRS VectorClass)

gaudi_install_headers(VectorClass NO_TEST)
# Disable some warnings
set_property(SOURCE src/ranvec1.cpp PROPERTY COMPILE_FLAGS " -Wno-overflow -Wno-sign-compare -Wno-strict-aliasing " )
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
