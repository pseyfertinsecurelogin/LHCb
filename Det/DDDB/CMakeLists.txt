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
# Package: DDDB
################################################################################
gaudi_subdir(DDDB v1r84)

gaudi_depends_on_subdirs(Det/DetCond
                         Tools/CondDBEntityResolver)

find_package(Oracle)
find_package(SQLite)

gaudi_install_python_modules()

