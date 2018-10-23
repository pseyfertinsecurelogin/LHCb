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
# Package: GaudiObjDesc
################################################################################
gaudi_subdir(GaudiObjDesc v11r24)

gaudi_depends_on_subdirs(GaudiKernel)

# required for nosetests and lxml
find_package(LibXml2)
find_package(pytools)
find_package(Boost)
find_package(ROOT)

include_directories(SYSTEM ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

gaudi_install_python_modules()
gaudi_install_scripts()

gaudi_install_cmake_modules()
# note: gaudi_install_cmake_modules only install .cmake files
install(FILES cmake/god_generate_deps.py
        DESTINATION cmake)

#install(FILES xml_files/gdd.dtd DESTINATION data/GaudiObjDesc)
gaudi_install_resources(xml_files/gdd.dtd)

file(GLOB templates templates/*.tpl)
gaudi_install_resources(${templates} DESTINATION templates)

gaudi_add_library(GaudiObjDescLib *.cpp
                  LINK_LIBRARIES GaudiKernel
                  PUBLIC_HEADERS GaudiObjDesc)

gaudi_add_unit_test(test_allocatorReleaser ../tests/src/test_allocatorReleaser.cpp
                    LINK_LIBRARIES GaudiObjDescLib)

gaudi_add_test(nosetests
               COMMAND nosetests -v --with-doctest
                       ${CMAKE_CURRENT_SOURCE_DIR}/python)

# No need to call QMTest, because all the tests are already described here
# gaudi_add_test(QMTest QMTEST)
