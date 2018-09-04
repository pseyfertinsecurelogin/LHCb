################################################################################
# Package: UTTELL1Event
################################################################################
gaudi_subdir(UTTELL1Event v1r12)

gaudi_depends_on_subdirs(GaudiObjDesc
                         Kernel/LHCbKernel
                         UT/UTKernel)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_library(UTTELL1Event
                  src/*.cpp
                  NO_PUBLIC_HEADERS
                  LINK_LIBRARIES LHCbKernel UTKernelLib)

god_build_dictionary(xml/*.xml
                     LINK_LIBRARIES LHCbKernel UTKernelLib UTTELL1Event)

