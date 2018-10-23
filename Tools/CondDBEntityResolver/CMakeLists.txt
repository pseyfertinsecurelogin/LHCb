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
# Package: CondDBEntityResolver
################################################################################
gaudi_subdir(CondDBEntityResolver v5r10)

gaudi_depends_on_subdirs(GaudiKernel
                         Det/DetCond
                         Tools/XmlTools)

find_package(Boost COMPONENTS python)
find_package(ROOT COMPONENTS Core)
find_package(PythonLibs)

find_package(COOL)
include_directories(SYSTEM ${COOL_INCLUDE_DIRS})

gaudi_add_module(CondDBEntityResolver
                 src/component/*.cpp
                 LINK_LIBRARIES DetCondLib XmlToolsLib)

gaudi_add_python_module(CondDBCompression
                          src/component/CondDBCompression.cpp
                          src/python/module.cpp
                        LINK_LIBRARIES ${Boost_PYTHON_LIBRARY} ROOT GaudiKernel
                        INCLUDE_DIRS Boost ROOT PythonLibs)

gaudi_add_test(QMTest QMTEST)
