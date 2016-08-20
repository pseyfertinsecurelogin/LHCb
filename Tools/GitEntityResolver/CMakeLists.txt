gaudi_subdir(GitEntityResolver)

gaudi_depends_on_subdirs(GaudiKernel
                         Kernel/LHCbKernel
                         Tools/XmlTools)

include(FindPkgConfig)
pkg_search_module(git2 libgit2)

find_package(Boost COMPONENTS filesystem)
# avoid warnings from Boost headers
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

if (git2_FOUND)
  gaudi_add_module(GitEntityResolver
                   src/GitEntityResolver.cpp
                   INCLUDE_DIRS git2 Boost
                   LINK_LIBRARIES XmlToolsLib git2 Boost)

  gaudi_install_headers(GitEntityResolver)

  gaudi_add_test(QMTest QMTEST
                 ENVIRONMENT GIT_TEST_REPOSITORY=${CMAKE_CURRENT_BINARY_DIR}/test_repo/DDDB
                             PYTHONPATH+=${CMAKE_CURRENT_SOURCE_DIR}/tests/python)
else()
  message(WARNING "libgit2 not found, cannot build GitEntityResolver")
endif()
