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
# Package: DetDescSvc
################################################################################
gaudi_subdir(DetDescSvc v4r1)

gaudi_depends_on_subdirs(Det/DetDesc
                         GaudiAlg
                         GaudiKernel)

find_package(Boost)

# Needed to be able to include IXmlParserSvc.h
gaudi_depends_on_subdirs(Tools/XmlTools)
find_package(XercesC)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS})

find_package(OpenSSL)
if (NOT OPENSSL_CRYPTO_LIBRARY)
  message(FATAL_ERROR "libcrypto is required")
endif()

gaudi_add_module(DetDescSvc
                 src/*.cpp
                 INCLUDE_DIRS Boost XercesC OpenSSL
                 LINK_LIBRARIES Boost XercesC DetDescLib GaudiAlgLib GaudiKernel ${OPENSSL_CRYPTO_LIBRARY})

gaudi_add_unit_test(test_RunChangeHandler
                    tests/src/test_RunChangeHandler.cpp src/RunChangeHandlerSvc.cpp
                    INCLUDE_DIRS OpenSSL
                    LINK_LIBRARIES XercesC DetDescLib ${OPENSSL_CRYPTO_LIBRARY}
                    TYPE Boost)
