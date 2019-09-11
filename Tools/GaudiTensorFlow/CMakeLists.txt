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

gaudi_subdir(GaudiTensorFlow v1r0)
gaudi_depends_on_subdirs(GaudiAlg)

set(TENSORFLOW_REPOSITORY https://storage.googleapis.com/tensorflow/libtensorflow
    CACHE STRING "HTTP repository for tensorflow" )

set(TENSORFLOW_TAG "1.12.0" CACHE STRING "Version of TensorFlow binaries" )
set(TENSORFLOW_DEVICE  "cpu" CACHE STRING "Device used by TensorFlow (either 'cpu' or 'gpu')")

# directory for the unpack fo TensorFlow deployment kit
set(TF_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/libtensorflow-${TENSORFLOW_DEVICE}-linux-x86_64-${TENSORFLOW_TAG})
file(MAKE_DIRECTORY ${TF_BIN_DIR})

set(TENSORFLOW_LIBRARIES
    ${TF_BIN_DIR}/lib/libtensorflow_framework.so
    ${TF_BIN_DIR}/lib/libtensorflow.so)

add_custom_command(OUTPUT libtensorflow-${TENSORFLOW_DEVICE}-linux-x86_64-${TENSORFLOW_TAG}.tar.gz
  COMMAND curl -L -o libtensorflow-${TENSORFLOW_DEVICE}-linux-x86_64-${TENSORFLOW_TAG}.tar.gz ${TENSORFLOW_REPOSITORY}/libtensorflow-${TENSORFLOW_DEVICE}-linux-x86_64-${TENSORFLOW_TAG}.tar.gz
)
add_custom_command(OUTPUT ${TENSORFLOW_LIBRARIES}
  COMMAND tar xfC libtensorflow-${TENSORFLOW_DEVICE}-linux-x86_64-${TENSORFLOW_TAG}.tar.gz ${TF_BIN_DIR}
  DEPENDS libtensorflow-${TENSORFLOW_DEVICE}-linux-x86_64-${TENSORFLOW_TAG}.tar.gz
)

### Find Boost 
find_package (Boost) 

### Find GSL libraries
find_package (GSL)
find_path(CPP_GSL_INCLUDE_DIR NAMES gsl/span)
if(NOT CPP_GSL_INCLUDE_DIR)
  message(FATAL "required headers from C++ GSL missing")
endif()



add_custom_target(TensorFlow DEPENDS ${TENSORFLOW_LIBRARIES})
include_directories(${TF_BIN_DIR}/include)
gaudi_add_library (GaudiTensorFlow
                    src/*.cpp
                    PUBLIC_HEADERS GaudiTensorFlow 
                    INCLUDE_DIRS Boost GSL ${CPP_GSL_INCLUDE_DIR}
                    LINK_LIBRARIES
                    ${TENSORFLOW_LIBRARIES} Boost GSL 
                  ) 
add_dependencies (GaudiTensorFlow TensorFlow)

install(FILES     ${TENSORFLOW_LIBRARIES}                DESTINATION lib     OPTIONAL)
install(DIRECTORY ${TF_BIN_DIR}/include/tensorflow DESTINATION include OPTIONAL)


gaudi_add_executable(testTensor
                     tests/testTensor.cpp
                     LINK_LIBRARIES
                     GaudiTensorFlow
                    )
add_dependencies (testTensor GaudiTensorFlow)


gaudi_add_test(QMTest QMTEST)
