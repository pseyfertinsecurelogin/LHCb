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

# This uses the upstream SOAContainer repository as submodule.

# In the submodules directory, the .gaudi_project_ignore files disables
# automatic discovery of CMakeLists.txt files, which is why the
# submodules/SOAContainer/CMakeLists.txt file needs to be added explicitly here.
# We do not want to automatically discover any of the other CMakeLists.txt
# files in the subdirectories therein (therefore disabling in the first place).

add_subdirectory(submodules/SOAContainer)
