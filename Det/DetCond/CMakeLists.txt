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
gaudi_subdir(DetCond)

gaudi_depends_on_subdirs(Det/DetDesc
                         GaudiAlg
                         Kernel/LHCbKernel)

gaudi_add_module(DetCond
                 src/*.cpp
                 LINK_LIBRARIES DetDescLib GaudiAlgLib LHCbKernel)

gaudi_install_python_modules()

# Prepare test Git CondDB overlay
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/test/DQFLAGS)
file(COPY tests/data/DQFLAGS/ DESTINATION test/DQFLAGS/)
execute_process(COMMAND git init -q ${CMAKE_CURRENT_BINARY_DIR}/test/DQFLAGS)

gaudi_add_test(QMTest QMTEST ENVIRONMENT TEST_DQFLAGS_ROOT=${CMAKE_CURRENT_BINARY_DIR}/test/DQFLAGS)
