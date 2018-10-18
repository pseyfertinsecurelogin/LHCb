################################################################################
# Package: EmptyAssociators
################################################################################
gaudi_subdir(EmptyAssociators v1r0)

gaudi_depends_on_subdirs(Associators/AssociatorsBase Event/TrackEvent)

gaudi_add_module(EmptyAssociators src/foo.cpp
  LINK_LIBRARIES AssociatorsBase_Associators_headers)
# gaudi_add_module(EmptyAssociators src/foo.cpp
#   INCLUDE_DIRS Associators/AssociatorsBase GaudiAlgLib
#   LINK_LIBRARIES GaudiAlgLib)
