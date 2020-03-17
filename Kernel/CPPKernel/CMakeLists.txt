###############################################################################
# (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
################################################################################
# Package: CPPKernel
################################################################################
gaudi_subdir(CPPKernel)

find_package(Boost)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_path(CPP_GSL_INCLUDE_DIR NAMES gsl/span)
if(NOT CPP_GSL_INCLUDE_DIR)
       message(FATAL "required headers from C++ GSL missing")
endif()

gaudi_install_headers(Kernel)

gaudi_add_test(QMTest QMTEST)

gaudi_add_unit_test(test_STLExtensions tests/src/test_STLExtensions.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
