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

#### these lines for the public_header_build_test
# gaudi_get_required_include_dirs(mylocaltmp2 GaudiAlgLib)
# gaudi_resolve_include_dirs(mylocaltmp ${mylocaltmp2})
# gaudi_resolve_link_libraries(interfacelibs GaudiAlgLib)
# foreach(dir ${mylocaltmp2})
#   include_directories(${dir})
# endforeach()
# # REQUIRED_INCLUDE_DIRS

# gaudi_install_headers(Associators)
gaudi_install_headers_with_deps(Associators
  INCLUDE_DIRS GaudiAlgLib 
  LINK_LIBRARIES GaudiAlgLib)
# # message(STATUS "got as dirs back: ${mylocaltmp2}")
# # message(STATUS "got as dirs back: ${mylocaltmp}")
# if(TARGET AssociatorsBase_Associators_headers)
#   set_target_properties(AssociatorsBase_Associators_headers PROPERTIES REQUIRED_INCLUDE_DIRS "${mylocaltmp}")
#   set_target_properties(AssociatorsBase_Associators_headers PROPERTIES REQUIRED_LIBRARIES "${interfacelibs}")
# endif()

gaudi_install_python_modules()
