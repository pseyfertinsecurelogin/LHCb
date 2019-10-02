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
# Package: LoKiNumbers
################################################################################
gaudi_subdir(LoKiNumbers)

gaudi_depends_on_subdirs(GaudiPython
                         Phys/LoKiCore)

gaudi_add_dictionary(LoKiNumbers
                     dict/LoKiNumbersDict.h
                     dict/LoKiNumbers.xml
                     LINK_LIBRARIES LoKiCoreLib
                     OPTIONS " -U__MINGW32__ ")

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
