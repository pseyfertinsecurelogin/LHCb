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
# Package: CondDBUI
################################################################################
gaudi_subdir(CondDBUI)

gaudi_depends_on_subdirs(Det/DDDB)

# Needed by LCG build of Qt with GCC 6
foreach(_lib_dir ${std_library_path})
  set(ENV{LD_LIBRARY_PATH} "${_lib_dir}:$ENV{LD_LIBRARY_PATH}")
endforeach()

find_package(Qt5 REQUIRED COMPONENTS Gui)
find_package(Freetype REQUIRED)

find_package(pygraphics)
find_package(pytools)
find_library(xkbcommon_lib NAMES libxkbcommon.so.0)
if(xkbcommon_lib)
  get_filename_component(xkbcommon_dir "${xkbcommon_lib}" DIRECTORY)
  gaudi_env(PREPEND LD_LIBRARY_PATH "${xkbcommon_dir}")
endif()

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
")
install(FILES ${CMAKE_BINARY_DIR}/python/CondDBUI/Browser/__version__.py
        DESTINATION python/CondDBUI/Browser)

# Special target to prepare only what is needed to run the CondDBBrowser
add_custom_target(CondDBBrowser)
add_dependencies(CondDBBrowser CondDBUI.Browser.Resources
                               CondDBUI.Browser.UI)

# if(TARGET MergedConfDB)
#   add_dependencies(CondDBBrowser MergedConfDB)
# endif()
