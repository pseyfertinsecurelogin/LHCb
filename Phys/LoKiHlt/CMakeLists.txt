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
################################################################################
# Package: LoKiHlt
################################################################################
gaudi_subdir(LoKiHlt v3r22)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/HltEvent
                         Event/L0Event
                         GaudiPython
                         Kernel/HltInterfaces
                         Phys/LoKiCore
                         Phys/LoKiNumbers)

find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

gaudi_add_library(LoKiHltLib
                  src/*.cpp
                  PUBLIC_HEADERS LoKi
                  LINK_LIBRARIES DAQEventLib HltEvent L0Event HltInterfaces LoKiCoreLib)

gaudi_add_module(LoKiHlt
                 src/Components/*.cpp
                 LINK_LIBRARIES DAQEventLib HltEvent L0Event HltInterfaces LoKiCoreLib LoKiHltLib)

gaudi_add_dictionary(LoKiHlt
                     dict/LoKiHltDict.h
                     dict/LoKiHlt.xml
                     LINK_LIBRARIES DAQEventLib HltEvent L0Event HltInterfaces LoKiCoreLib LoKiHltLib
                     OPTIONS " -U__MINGW32__ ")

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
