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
# Package: DetSys
################################################################################
gaudi_subdir(DetSys)

gaudi_depends_on_subdirs(Det/BcmDet
                         Det/CaloDet
                         Det/CaloDetXmlCnv
                         Det/DDDB
                         Det/DetCond
                         Det/DetDesc
                         Det/DetDescCnv
                         Det/DetDescSvc
                         Det/Magnet
                         Det/MuonDet
                         Det/OTDet
                         Det/RichDet
                         Det/STDet
                         Det/VeloDet
                         Tools/XmlTools)

