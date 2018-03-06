CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

#---------------------------------------------------------------
# Load macros and functions for Gaudi-based projects
find_package(GaudiProject)
#---------------------------------------------------------------

if(EXISTS ${CMAKE_SOURCE_DIR}/Online/RootCnv)
  set(override_subdirs RootCnv)
endif()

# hide warnings from some external projects
find_package(Boost)
find_package(ROOT)
find_package(CLHEP)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS}
                           ${CLHEP_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS})

# Declare project name and version
gaudi_project(LHCb v50r0
              USE Gaudi v30r1
              DATA Gen/DecFiles
                   Det/SQLDDDB VERSION v7r*
                   FieldMap
                   ParamFiles
                   PRConfig
                   RawEventFormat
                   TCK/HltTCK
		   TCK/L0TCK VERSION v5r*)
