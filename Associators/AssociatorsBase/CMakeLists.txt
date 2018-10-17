################################################################################
# Package: AssociatorsBase
################################################################################
gaudi_subdir(AssociatorsBase v1r0)

gaudi_depends_on_subdirs(Event/LinkerEvent
                         Event/MCEvent
                         Kernel/Relations
                         GaudiAlg
                         GaudiKernel)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_install_headers(Associators)

gaudi_install_python_modules()
