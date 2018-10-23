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
# Package: DetCondExample
################################################################################
gaudi_subdir(DetCondExample v11r16)

gaudi_depends_on_subdirs(Det/DetCond
                         Det/DetDesc
                         Det/DetDescCnv
                         Det/DetDescSvc
                         GaudiAlg
                         GaudiConf
                         GaudiKernel
                         GaudiSvc
                         Tools/CondDBEntityResolver
                         Tools/XmlTools)

find_package(MySQL)
find_package(Oracle)
find_package(SQLite)

find_package(COOL)
include_directories(SYSTEM ${COOL_INCLUDE_DIRS})

gaudi_add_executable(DetCondExample
                     src/*.cpp
                     ${CMAKE_SOURCE_DIR}/GaudiConf/src/GaudiMain.cpp
                     LINK_LIBRARIES DetCondLib DetDescLib DetDescCnvLib GaudiAlgLib GaudiKernel XmlToolsLib)

gaudi_env(SET CONDDBCONNECTIONSTRING sqlite_file:CONDDB.db/CONDDB
          SET CONDDBCONNECTIONSTRING1 sqlite_file:CONDDB1.db/CONDDB1
          SET CONDDBCONNECTIONSTRING2 sqlite_file:CONDDB2.db/CONDDB2
          SET CONDITIONS_PATH conddb:/Conditions/MainCatalog.xml
          SET DAQCONNECTIONSTRING sqlite_file:DAQ.db/DAQ
          SET DAQFOLDERNAME DAQ
          SET DETCONDEXAMPLE_XMLDDDB \${DETCONDEXAMPLEROOT}/XMLDDDB)

