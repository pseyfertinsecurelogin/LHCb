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
# Package: MicroDst
################################################################################
gaudi_subdir(MicroDst v1r3)

gaudi_depends_on_subdirs(Event/PhysEvent
                         GaudiKernel
                         GaudiPython)

gaudi_add_library(MicroDstLib
                  src/*.cpp
                  PUBLIC_HEADERS Kernel
                  LINK_LIBRARIES PhysEvent GaudiKernel GaudiPythonLib)

gaudi_add_dictionary(MicroDst
                     dict/MicroDstDict.h
                     dict/MicroDstDict.xml
                     LINK_LIBRARIES PhysEvent GaudiKernel GaudiPythonLib MicroDstLib
                     OPTIONS "-U__MINGW32__")

gaudi_install_python_modules()

