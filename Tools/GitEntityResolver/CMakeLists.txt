gaudi_subdir(GitEntityResolver)

gaudi_depends_on_subdirs(GaudiKernel
                         Det/DetCond
                         Tools/XmlTools)

include(FindPkgConfig)
pkg_search_module(git2 libgit2)

if (git2_FOUND)
  gaudi_add_module(GitEntityResolver
                   src/GitEntityResolver.cpp
                   INCLUDE_DIRS git2
                   LINK_LIBRARIES DetCondLib XmlToolsLib git2)

  #gaudi_add_test(QMTest QMTEST)
else()
  message(WARNING "libgit2 not found, cannot build GitEntityResolver")
endif()
