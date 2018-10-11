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
# Package: XMLSummaryKernel
################################################################################
gaudi_subdir(XMLSummaryKernel)

gaudi_depends_on_subdirs(GaudiConf
                         GaudiKernel
                         GaudiUtils
                         Kernel/XMLSummaryBase)

find_package(Boost COMPONENTS regex)
find_package(PythonLibs)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

find_package(Xrootd) # runtime, only for the tests
# FIXME: workaround for bug https://sft.its.cern.ch/jira/browse/ROOT-6516
set_property(GLOBAL APPEND PROPERTY INCLUDE_PATHS ${XROOTD_INCLUDE_DIRS})

gaudi_add_module(XMLSummaryKernel
                 src/*.cpp
                 INCLUDE_DIRS Boost PythonLibs
                 LINK_LIBRARIES Boost PythonLibs GaudiKernel GaudiUtilsLib)

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
