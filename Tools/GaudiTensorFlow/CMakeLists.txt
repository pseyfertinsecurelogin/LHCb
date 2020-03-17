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

### Find Boost 
find_package (Boost) 

### Find GSL libraries
find_package(cppgsl)

### Find tensorflow
find_path(TENSORFLOW_INCLUDE_DIR NAMES tensorflow/c/c_api.h)
find_library(TENSORFLOW_LIBRARY NAMES tensorflow)
find_library(TENSORFLOW_FRAMEWORK_LIBRARY NAMES tensorflow_framework)
if(TENSORFLOW_INCLUDE_DIR AND TENSORFLOW_LIBRARY AND TENSORFLOW_FRAMEWORK_LIBRARY)
  message(STATUS "Found tensorflow: ${TENSORFLOW_INCLUDE_DIR}")
else()
  message(FATAL_ERROR "Cannot find tensorflow required files")
endif()
set(TENSORFLOW_LIBRARIES ${TENSORFLOW_LIBRARY} ${TENSORFLOW_FRAMEWORK_LIBRARY})


gaudi_add_library (GaudiTensorFlow
                    src/*.cpp
                    PUBLIC_HEADERS GaudiTensorFlow 
                    INCLUDE_DIRS Boost cppgsl ${TENSORFLOW_INCLUDE_DIR}
                    LINK_LIBRARIES ${TENSORFLOW_LIBRARIES} Boost
                  ) 

gaudi_add_executable(testTensor
                     tests/testTensor.cpp
                     LINK_LIBRARIES GaudiTensorFlow
                     )

gaudi_add_test(QMTest QMTEST)
