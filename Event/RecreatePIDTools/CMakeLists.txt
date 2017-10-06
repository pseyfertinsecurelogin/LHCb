################################################################################
# Package: RecreatePIDTools
################################################################################
gaudi_subdir(RecreatePIDTools v1r9)

gaudi_depends_on_subdirs(Event/RecEvent
                         GaudiAlg)

find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(RecreatePIDTools
                 src/*.cpp
                 LINK_LIBRARIES RecEvent GaudiAlgLib)

