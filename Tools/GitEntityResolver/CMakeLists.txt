###############################################################################
# (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
gaudi_subdir(GitEntityResolver)

gaudi_depends_on_subdirs(GaudiKernel
                         Kernel/LHCbKernel
                         Tools/XmlTools)


set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
find_package(PkgConfig)
# make sure pkg-config looks for packages on the system too
list(APPEND CMAKE_PREFIX_PATH /usr)
pkg_check_modules(git2 libgit2)


find_package(Boost COMPONENTS filesystem)
find_package(XercesC)
# avoid warnings from Boost and XercesC headers
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS} ${XercesC_INCLUDE_DIRS})

if (git2_FOUND)
  include_directories(${git2_INCLUDE_DIRS})
  gaudi_add_module(GitEntityResolver
                   src/GitEntityResolver.cpp
                   INCLUDE_DIRS Boost
                   LINK_LIBRARIES XmlToolsLib Boost)
  target_link_libraries(GitEntityResolver ${git2_LDFLAGS})
  if(git2_LIBRARY_DIRS)
    gaudi_env(PREPEND LD_LIBRARY_PATH ${git2_LIBRARY_DIRS})
  endif()

  gaudi_install_headers(GitEntityResolver)

  gaudi_install_scripts()

  gaudi_add_test(QMTest QMTEST
                 ENVIRONMENT GIT_TEST_REPOSITORY=${CMAKE_CURRENT_BINARY_DIR}/test_repo/DDDB
                             PYTHONPATH+=${CMAKE_CURRENT_SOURCE_DIR}/tests/python)

else()
  message(FATAL_ERROR "libgit2 not found, cannot build GitEntityResolver")
endif()

gaudi_install_python_modules()
gaudi_add_test(python COMMAND nosetests --with-doctest -v
                              ${CMAKE_CURRENT_SOURCE_DIR}/python)
gaudi_add_test(scripts COMMAND nosetests --with-doctest -v
                               ${CMAKE_CURRENT_SOURCE_DIR}/scripts
                               ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_scripts
                       ENVIRONMENT PYTHONPATH+=${CMAKE_CURRENT_SOURCE_DIR}/scripts)
