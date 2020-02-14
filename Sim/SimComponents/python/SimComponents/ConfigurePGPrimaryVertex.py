###############################################################################
# (c) Copyright 2020 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
from PgPrimaryVertexConfiguration import PgPrimaryVertexConfiguration as PGPVcfg
from Configurables import PGPrimaryVertex


def ConfigurePGPrimaryVertex(DataType, MagPol):
    if not MagPol in ["MagUp", "MagDown"]:
        #RETURN ERROR
        raise ValueError('Polarity %s not in ["MagUp","MagDown"]' % MagPol)
    if not DataType in ["2011", "2012", "2015", "2016", "2017", "2018"]:
        #RETURN ERROR
        raise ValueError(
            'DataType %s not in ["2011","2012","2015","2016","2017","2018"]' %
            DataType)
    pgpv = PGPrimaryVertex()
    #X Configuration
    pgpv.GaussResolutionX1CentralValue = PGPVcfg[DataType][MagPol]['X']['mu1']
    pgpv.GaussResolutionX2CentralValue = PGPVcfg[DataType][MagPol]['X']['mu2']
    pgpv.GaussResolutionX3CentralValue = PGPVcfg[DataType][MagPol]['X']['mu3']
    pgpv.GaussResolutionX1Width = PGPVcfg[DataType][MagPol]['X']['sigma1']
    pgpv.GaussResolutionX2Width = PGPVcfg[DataType][MagPol]['X']['sigma2']
    pgpv.GaussResolutionX3Width = PGPVcfg[DataType][MagPol]['X']['sigma3']
    pgpv.GaussResolutionX1fraction = PGPVcfg[DataType][MagPol]['X']['f1']
    pgpv.GaussResolutionX2fraction = PGPVcfg[DataType][MagPol]['X']['f2']

    #Y Configuration
    pgpv.GaussResolutionY1CentralValue = PGPVcfg[DataType][MagPol]['Y']['mu1']
    pgpv.GaussResolutionY2CentralValue = PGPVcfg[DataType][MagPol]['Y']['mu2']
    pgpv.GaussResolutionY3CentralValue = PGPVcfg[DataType][MagPol]['Y']['mu3']
    pgpv.GaussResolutionY1Width = PGPVcfg[DataType][MagPol]['Y']['sigma1']
    pgpv.GaussResolutionY2Width = PGPVcfg[DataType][MagPol]['Y']['sigma2']
    pgpv.GaussResolutionY3Width = PGPVcfg[DataType][MagPol]['Y']['sigma3']
    pgpv.GaussResolutionY1fraction = PGPVcfg[DataType][MagPol]['Y']['f1']
    pgpv.GaussResolutionY2fraction = PGPVcfg[DataType][MagPol]['Y']['f2']

    #Z Configuration
    pgpv.GaussResolutionZ1CentralValue = PGPVcfg[DataType][MagPol]['Z']['mu1']
    pgpv.GaussResolutionZ2CentralValue = PGPVcfg[DataType][MagPol]['Z']['mu2']
    pgpv.GaussResolutionZ3CentralValue = PGPVcfg[DataType][MagPol]['Z']['mu3']
    pgpv.GaussResolutionZ1Width = PGPVcfg[DataType][MagPol]['Z']['sigma1']
    pgpv.GaussResolutionZ2Width = PGPVcfg[DataType][MagPol]['Z']['sigma2']
    pgpv.GaussResolutionZ3Width = PGPVcfg[DataType][MagPol]['Z']['sigma3']
    pgpv.GaussResolutionZ1fraction = PGPVcfg[DataType][MagPol]['Z']['f1']
    pgpv.GaussResolutionZ2fraction = PGPVcfg[DataType][MagPol]['Z']['f2']

    #covariance
    pgpv.CovarianceXX = PGPVcfg[DataType][MagPol]['CovXX']
    pgpv.CovarianceYY = PGPVcfg[DataType][MagPol]['CovYY']
    pgpv.CovarianceZZ = PGPVcfg[DataType][MagPol]['CovZZ']

    return pgpv
