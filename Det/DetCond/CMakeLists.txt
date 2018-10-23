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
# Package: DetCond
################################################################################
gaudi_subdir(DetCond v12r47)

gaudi_depends_on_subdirs(Det/DetDesc
                         GaudiAlg
                         GaudiKernel
                         Kernel/LHCbKernel)

find_package(Boost COMPONENTS system thread filesystem)
find_package(COOL COMPONENTS CoolKernel CoolApplication)
find_package(CORAL COMPONENTS CoralBase CoralKernel RelationalAccess)
find_package(ROOT)

include_directories(SYSTEM ${CORAL_INCLUDE_DIRS} ${COOL_INCLUDE_DIRS}
                           ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_library(DetCondLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS DetCond
                  INCLUDE_DIRS Boost COOL CORAL
                  LINK_LIBRARIES Boost COOL CORAL DetDescLib GaudiAlgLib GaudiKernel LHCbKernel)

gaudi_add_module(DetCond
                 src/component/*.cpp
                 tests/src/*.cpp
                 INCLUDE_DIRS Boost COOL CORAL
                 LINK_LIBRARIES Boost COOL CORAL DetDescLib GaudiAlgLib GaudiKernel LHCbKernel DetCondLib)

gaudi_add_dictionary(DetCond
                     src/dict/DetCondDict.h
                     src/dict/DetCondDict.xml
                     INCLUDE_DIRS Boost COOL CORAL
                     LINK_LIBRARIES Boost COOL CORAL DetDescLib GaudiAlgLib GaudiKernel LHCbKernel DetCondLib
                     OPTIONS "-U__MINGW32__")

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST ENVIRONMENT NO_GIT_CONDDB=1)
