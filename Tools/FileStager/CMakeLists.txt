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
gaudi_subdir(FileStager)

gaudi_depends_on_subdirs(GaudiKernel)

find_package(Boost COMPONENTS system regex iostreams thread filesystem)

gaudi_add_library(FileStagerFunctions
                  src/Lib/*.cpp
                  PUBLIC_HEADERS FileStager
                  INCLUDE_DIRS Boost
                  LINK_LIBRARIES Boost GaudiKernel)

gaudi_add_module(FileStager
                 src/Component/*.cpp
                 INCLUDE_DIRS Boost
                 LINK_LIBRARIES Boost GaudiKernel FileStagerFunctions)

gaudi_add_dictionary(FileStager
                     dict/FileStagerDict.h
                     dict/FileStagerDict.xml
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost GaudiKernel FileStagerFunctions)

# suppress compilation warnings from external package header files
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

# only for the applications
find_package(Boost COMPONENTS program_options)
# work around issue when using ${Boost_LIBRARIES} from BoostConfig.cmake
if(Boost_LIBRARIES MATCHES "Boost::system")
  set(Boost_LIBRARIES Boost::program_options)
endif()

gaudi_add_executable(garbage
                     src/app/garbage.cpp
                     LINK_LIBRARIES Boost FileStagerFunctions)

gaudi_add_executable(hash_filename
                     src/app/hash_filename.cpp
                     LINK_LIBRARIES Boost FileStagerFunctions)

gaudi_install_python_modules()

gaudi_add_test(QMTest QMTEST)
