###############################################################################
# (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
################################################################################
# Package: GaudiTensorFlow
################################################################################

gaudi_subdir(GaudiTensorFlow)
gaudi_depends_on_subdirs(GaudiAlg)

set(TENSORFLOW_REPOSITORY
    # https://storage.googleapis.com/tensorflow/libtensorflow
    http://lhcb-rpm.web.cern.ch/lhcb-rpm/misc/libtensorflow
    CACHE STRING "HTTP repository for tensorflow" )

set(TENSORFLOW_TAG "1.12.0" CACHE STRING "Version of TensorFlow binaries" )
set(TENSORFLOW_DEVICE  "cpu" CACHE STRING "Device used by TensorFlow (either 'cpu' or 'gpu')")
set(TENSORFLOW_SHA1 "961de4ddd7010df09b080ebf1d937ba1be4a1d5c" CACHE STRING "SHA1 checksum of the TensorFlow package")

# helper variables (mostly for readability)
set(TF_NAME libtensorflow-${TENSORFLOW_DEVICE}-linux-x86_64-${TENSORFLOW_TAG})
set(TF_TAR ${TF_NAME}.tar.gz)
set(TF_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TF_NAME})
set(TF_TARFILE ${CMAKE_CURRENT_BINARY_DIR}/${TF_TAR})

file(MAKE_DIRECTORY ${TF_BIN_DIR})

set(TENSORFLOW_LIBRARIES
    ${TF_BIN_DIR}/lib/libtensorflow_framework.so
    ${TF_BIN_DIR}/lib/libtensorflow.so)

if(EXISTS ${TF_TARFILE})
  file(SHA1 ${TF_TARFILE} TF_TARFILE_SHA1)
endif()
if(NOT TF_TARFILE_SHA1 STREQUAL TENSORFLOW_SHA1)
  message(STATUS "Downloading ${TENSORFLOW_REPOSITORY}/${TF_TAR}")
  file(DOWNLOAD ${TENSORFLOW_REPOSITORY}/${TF_TAR}
       ${TF_TARFILE}
       EXPECTED_HASH SHA1=${TENSORFLOW_SHA1})
endif()
message(STATUS "Unpacking ${TF_TAR}")
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${TF_TARFILE}
                WORKING_DIRECTORY ${TF_BIN_DIR})

### Find Boost 
find_package (Boost) 

### Find GSL libraries
find_package (GSL)
find_path(CPP_GSL_INCLUDE_DIR NAMES gsl/span)
if(NOT CPP_GSL_INCLUDE_DIR)
  message(FATAL "required headers from C++ GSL missing")
endif()

include_directories(${TF_BIN_DIR}/include)
gaudi_add_library (GaudiTensorFlow
                    src/*.cpp
                    PUBLIC_HEADERS GaudiTensorFlow 
                    INCLUDE_DIRS Boost GSL ${CPP_GSL_INCLUDE_DIR}
                    LINK_LIBRARIES
                    ${TENSORFLOW_LIBRARIES} Boost GSL 
                  ) 

install(FILES     ${TENSORFLOW_LIBRARIES}                DESTINATION lib     OPTIONAL)
install(DIRECTORY ${TF_BIN_DIR}/include/tensorflow DESTINATION include OPTIONAL)


gaudi_add_executable(testTensor
                     tests/testTensor.cpp
                     LINK_LIBRARIES
                     GaudiTensorFlow
                    )
add_dependencies (testTensor GaudiTensorFlow)


gaudi_add_test(QMTest QMTEST)
