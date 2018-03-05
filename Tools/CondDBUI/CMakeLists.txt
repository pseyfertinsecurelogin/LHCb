################################################################################
# Package: CondDBUI
################################################################################
gaudi_subdir(CondDBUI v3r36)

gaudi_depends_on_subdirs(Det/DDDB Tools/CondDBEntityResolver)

# Needed by LCG build of Qt with GCC 6
foreach(_lib_dir ${std_library_path})
  set(ENV{LD_LIBRARY_PATH} "${_lib_dir}:$ENV{LD_LIBRARY_PATH}")
endforeach()

find_package(Boost)
find_package(COOL COMPONENTS CoolKernel CoolApplication)
find_package(CORAL COMPONENTS CoralBase CoralKernel RelationalAccess)
find_package(PythonLibs)

# Force Qt5 whichever is the default
set(DESIRED_QT_VERSION 5)

if(DESIRED_QT_VERSION STREQUAL "5")
  find_package(Qt5 COMPONENTS Gui)
else()
  find_package(Qt)
endif()
message(STATUS "CondDBBrowser using Qt${DESIRED_QT_VERSION}")

find_package(ROOT)
find_package(XercesC)
find_package(pygraphics)
find_package(pytools)

include_directories(SYSTEM ${CORAL_INCLUDE_DIRS} ${COOL_INCLUDE_DIRS}
                           ${PYTHON_INCLUDE_DIRS})

gaudi_add_library(CondDBUIHelperLib src/helper/*.cpp
                  INCLUDE_DIRS PythonLibs COOL CORAL
                  LINK_LIBRARIES PythonLibs COOL CORAL
                  PUBLIC_HEADERS CondDBUIHelper)

# gen_pyqt_* functions are provided by 'pygraphics'
gen_pyqt_resource(CondDBUI.Browser.Resources CondDBUI/Browser qt_resources/*.qrc)
gen_pyqt_uic(CondDBUI.Browser.UI CondDBUI/Browser qt_resources/*.ui)

gaudi_install_python_modules()
gaudi_install_scripts()

execute_process(COMMAND date -I OUTPUT_VARIABLE today OUTPUT_STRIP_TRAILING_WHITESPACE)
# This must be done after 'gaudi_install_python_modules'
file(WRITE ${CMAKE_BINARY_DIR}/python/CondDBUI/Browser/__version__.py
"# generated file, do not edit
__versionNumber__ = '${subdir_version}'
__versionDate__ = '${today}'
__qt_version__ = '${DESIRED_QT_VERSION}'
")
install(FILES ${CMAKE_BINARY_DIR}/python/CondDBUI/Browser/__version__.py
        DESTINATION python/CondDBUI/Browser)

# Special target to prepare only what is needed to run the CondDBBrowser
add_custom_target(CondDBBrowser)
add_dependencies(CondDBBrowser CondDBUI.Browser.Resources
                               CondDBUI.Browser.UI
                               CondDBUIHelperLib)
if(TARGET CondDBCompression)
  add_dependencies(CondDBBrowser CondDBCompression)
endif()
if(TARGET MergedCondDB)
  add_dependencies(CondDBBrowser MergedCondDB)
endif()
