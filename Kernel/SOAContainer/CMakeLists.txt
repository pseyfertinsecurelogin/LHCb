###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
################################################################################
# Package: SOAContainer
################################################################################
gaudi_subdir(SOAContainer v1r0)
set_property(DIRECTORY PROPERTY INSTALLS_LOCAL_HEADERS TRUE)

# This uses the upstream SOAContainer repository as submodule.

# In the submodules directory, the .gaudi_project_ignore files disables
# automatic discovery of CMakeLists.txt files, which is why the
# submodules/SOAContainer/CMakeLists.txt file needs to be added explicitly here.
# We do not want to automatically discover any of the other CMakeLists.txt
# files in the subdirectories therein (therefore disabling in the first place).

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/submodules/SOAContainer/CMakeLists.txt")
  set(SOAContainer_disable_tests ON)
  set(SOAContainer_header_destination ${CMAKE_INSTALL_PREFIX}/include/SOAContainer)
  add_subdirectory(submodules/SOAContainer)
else()
  message(FATAL_ERROR
"SOAContainer submodule file not found
from the directory: ${CMAKE_SOURCE_DIR}
you possibly need to run: `git submodule update --init --recursive`

see also https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Dealing_with_submodules_when_cha")
endif()
