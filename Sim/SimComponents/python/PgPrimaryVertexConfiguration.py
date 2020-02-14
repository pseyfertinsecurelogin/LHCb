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
from Gaudi.Configuration import *
from GaudiKernel.SystemOfUnits import mm

"""
See https://its.cern.ch/jira/browse/LHCBGAUSS-1287
Note: Fit model for that study is not the same as the
fit model used in the code PGPrimaryVertex. We therefore use
the fact that the mean value of the triple gaussian there is fixed
to convert the fractions. We then use this to calculate the used values
for the on diagonal parts of the covariance matrix
"""

PgPrimaryVertexConfiguration={
    '2011':{
        'MagUp':
        {'X':{'mu1':-1.64321e-04*mm,
              'mu2':-2.07447e-05*mm,
              'mu3':-1.59372e-04*mm,
              'sigma1': 2.14809e-02 *mm,
              'sigma2': 9.28222e-03 *mm,
               'sigma3':5.53627e-02 *mm,
              'f1': 0.4086,
              'f2': 0.7969},
         'Y':{'mu1':-3.54945e-05 *mm,
              'mu2':-1.92725e-05 *mm,
              'mu3' :-7.60356e-05 *mm,
              'sigma1': 2.12752e-02 *mm,
              'sigma2': 9.42666e-03 *mm,
              'sigma3':5.50516e-02 *mm,
              'f1': 0.4035,
              'f2': 0.8039},
         'Z':{'mu1': 1.92948e-03*mm,
              'mu2':-6.39393e-05*mm,
              'mu3':4.43561e-04*mm,
              'sigma1': 2.93170e-01*mm,
              'sigma2': 4.58140e-02*mm,
              'sigma3':1.10641e-01*mm,
              'f1': 0.1628,
              'f2': 0.5597},
         'CovXX':4.36969844565514146e-04,
         'CovYY':4.09792105425419665e-04,
         'CovZZ':1.85588016359800601e-02,
        },
        'MagDown':
        {'X':{'mu1':-1.64321e-04*mm,
              'mu2':-2.07447e-05*mm,
              'mu3':-1.59372e-04*mm,
              'sigma1': 2.14809e-02 *mm,
              'sigma2': 9.28222e-03 *mm,
              'sigma3':5.53627e-02 *mm,
              'f1': 0.4086,
              'f2': 0.7969},
         'Y':{'mu1':-3.54945e-05 *mm,
              'mu2':-1.92725e-05 *mm,
              'mu3' :-7.60356e-05 *mm,
              'sigma1': 2.12752e-02 *mm,
              'sigma2': 9.42666e-03 *mm,
              'sigma3':5.50516e-02 *mm,
              'f1': 0.4035,
              'f2': 0.8039},
         'Z':{'mu1': 1.92948e-03*mm,
              'mu2':-6.39393e-05*mm,
              'mu3':4.43561e-04*mm,
              'sigma1': 2.93170e-01*mm,
              'sigma2': 4.58140e-02*mm,
              'sigma3':1.10641e-01*mm,
              'f1': 0.1628,
              'f2': 0.5597},
         'CovXX':4.36969844565514146e-04,
         'CovYY':4.09792105425419665e-04,
         'CovZZ':1.85588016359800601e-02,
        },
    },
    '2012':{
        'MagUp':
        {'X':{'mu1':-1.64321e-04*mm,
              'mu2':-2.07447e-05*mm,
              'mu3':-1.59372e-04*mm,
              'sigma1': 2.14809e-02 *mm,
              'sigma2': 9.28222e-03 *mm,
              'sigma3':5.53627e-02 *mm,
              'f1': 0.4086,
              'f2': 0.7969},
         'Y':{'mu1':-3.54945e-05 *mm,
              'mu2':-1.92725e-05 *mm,
              'mu3' :-7.60356e-05 *mm,
              'sigma1': 2.12752e-02 *mm,
              'sigma2': 9.42666e-03 *mm,
              'sigma3':5.50516e-02 *mm,
              'f1': 0.4035,
              'f2': 0.8039},
         'Z':{'mu1': 1.92948e-03*mm,
              'mu2':-6.39393e-05*mm,
              'mu3':4.43561e-04*mm,
              'sigma1': 2.93170e-01*mm,
              'sigma2': 4.58140e-02*mm,
              'sigma3':1.10641e-01*mm,
              'f1': 0.1628,
              'f2': 0.5597},
         'CovXX':4.36969844565514146e-04,
         'CovYY':4.09792105425419665e-04,
         'CovZZ':1.85588016359800601e-02,
        },
        'MagDown':
        {'X':{'mu1':-1.64321e-04*mm,
              'mu2':-2.07447e-05*mm,
              'mu3':-1.59372e-04*mm,
              'sigma1': 2.14809e-02 *mm,
              'sigma2': 9.28222e-03 *mm,
              'sigma3':5.53627e-02 *mm,
              'f1': 0.4086,
              'f2': 0.7969},
         'Y':{'mu1':-3.54945e-05 *mm,
              'mu2':-1.92725e-05 *mm,
              'mu3' :-7.60356e-05 *mm,
              'sigma1': 2.12752e-02 *mm,
              'sigma2': 9.42666e-03 *mm,
              'sigma3':5.50516e-02 *mm,
              'f1': 0.4035,
              'f2': 0.8039},
         'Z':{'mu1': 1.92948e-03*mm,
              'mu2':-6.39393e-05*mm,
              'mu3':4.43561e-04*mm,
              'sigma1': 2.93170e-01*mm,
              'sigma2': 4.58140e-02*mm,
              'sigma3':1.10641e-01*mm,
              'f1': 0.1628,
              'f2': 0.5597},
         'CovXX':4.36969844565514146e-04,
         'CovYY':4.09792105425419665e-04,
         'CovZZ':1.85588016359800601e-02,
        }
    },
    '2015':{
        'MagUp':
        {"X":{'mu1':-3.29e-5 *mm,
              'mu2':-3.29e-5 *mm,
        'mu3':-3.29e-5 *mm,
              'sigma1': 0.01109 *mm,
              'sigma2': 0.04393 *mm,
              'sigma3':0.02087 *mm,
              'f1': 0.71440,
              "f2": 0.02266236},
         "Y":{'mu1':1.33e-5*mm,
              'mu2':1.33e-5*mm,
              'mu3':1.33e-5*mm,
              'sigma1':0.01063*mm,
              'sigma2':0.04060*mm,
              'sigma3':0.01919*mm,
              'f1':0.69546,
              "f2": 0.0239124808,
         },
         "Z":{'mu1': -1.9e-4*mm,
              'mu2': -1.9e-4*mm,
              'mu3': -1.9e-4*mm,
              'sigma1':0.37696*mm,
              'sigma2':0.13423*mm,
              'sigma3':0.06089*mm,
              'f1':0.01556,
              "f2": 0.2192938544,
         },
         "CovXX":(0.71440 * (0.01109 *mm)**2 + 0.02266236 * (0.04393*mm)**2 + (1-0.71440-0.02266236)*(0.02087*mm)**2),
         "CovYY":(0.69546 * (0.01063*mm)**2 + (0.0239124808 * 0.04060*mm)**2 + ((1-0.69546-0.0239124808)*0.01919*mm)**2),
         "CovZZ":(0.01556* (0.37696*mm)**2 + 0.2192938544 * (0.13423*mm)**2 + (1-0.69546-0.2192938544)*(0.06089*mm)**2),
        },
        'MagDown':
        {"X":{'mu1':4.05e-5*mm,
              'mu2':4.05e-5*mm,
              'mu3':4.05e-5*mm,
              'sigma1':0.01082*mm,
              'sigma2':0.04187*mm,
              'sigma3':0.02012*mm,
              'f1':0.70373,
              "f2": 0.0253370104},
         "Y":{'mu1':-3.32e-5*mm,
              'mu2':-3.32e-5*mm,
              'mu3':-3.32e-5*mm,
              'sigma1': 0.01041*mm,
              'sigma2': 0.04072*mm,
              'sigma3': 0.01930*mm,
              'f1':0.70820,
              "f2": 0.022888791999999998},
         "Z":{'mu1':-1.8e-4*mm,
              'mu2':-1.8e-4*mm,
              'mu3':-1.8e-4*mm,
              'sigma1':0.38345*mm,
              'sigma2':0.13362*mm,
              'sigma3':0.06049*mm,
              'f1':0.01368,
              "f2": 0.2072159688},
         "CovXX":0.70373*(0.01082*mm)**2+0.0253370104*(0.04187*mm)**2 + (1-0.70373-0.0253370104)*(0.02012*mm)**2,
         "CovYY":0.70820*(0.01041*mm)**2 +0.022888791999999998*(0.04072*mm)**2 +(1-0.70820-0.022888791999999998)*(0.01930*mm)**2,
         "CovZZ":0.01368*(0.38345*mm)**2 + 0.2072159688*(0.13362*mm)**2 + (1-0.01368-0.2072159688)*(0.06049*mm)**2,
        },
    },
    '2016':{
        'MagUp' :
        {"X":{'mu1':1.46e-5*mm,
              'mu2':1.46e-5*mm,
              'mu3':1.46e-5*mm,
              'sigma1':0.02246*mm,
              'sigma2':0.04501*mm,
              'sigma3':0.01176*mm,
              'f1':0.39575,
              "f2": 0.0842626625
        },
         "Y":{'mu1':-5.39e-5*mm,
              'mu2':-5.39e-5*mm,
              'mu3':-5.39e-5*mm,
              'sigma1':0.02129*mm,
              'sigma2':0.04236*mm,
              'sigma3':0.01130*mm,
              'f1':0.39621,
              "f2": 0.0802618047
         },
         "Z":{'mu1':2.3e-4*mm,
              'mu2':2.3e-4*mm,
              'mu3':2.3e-4*mm,
              'sigma1':0.42375*mm,
              'sigma2':0.07201*mm,
              'sigma3':0.17024*mm,
              'f1':0.03295,
        "f2": 0.7111202175
         },
         "CovXX":0.39575*(0.02246*mm)**2 + 0.0842626625*(0.04501*mm)**2 + (1-0.39575-0.0842626625)*(0.01176*mm)**2,
         "CovYY":0.39621*(0.02129*mm)**2 + 0.0802618047*(0.04236*mm)**2 + (1-0.39621-0.0802618047)*(0.01130*mm)**2,
         "CovZZ":0.03295*(0.42375*mm)**2 + 0.7111202175*(0.07201*mm)**2 + (1-0.03295-0.7111202175)*(0.17024*mm)**2
        },
        'MagDown' :
        {"X":{'mu1':-5.16e-5*mm,
              'mu2':-5.16e-5*mm,
              'mu3':-5.16e-5*mm,
              'sigma1':0.02377*mm,
              'sigma2':0.04692*mm,
              'sigma3':0.01221*mm,
              'f1':0.35419,
              "f2": 0.0710778486,
        },
         "Y":{'mu1':1.76e-5*mm,
              'mu2':1.76e-5*mm,
              'mu3':1.76e-5*mm,
              'sigma1':0.01160*mm,
              'sigma2':0.04330*mm,
              'sigma3':0.02243*mm,
              'f1':0.56669,
              "f2": 0.069979565
         },
         "Z":{'mu1':8.9e-4*mm,
              'mu2':8.9e-4*mm,
              'mu3':8.9e-4*mm,
              'sigma1':0.16142*mm,
              'sigma2':0.40333*mm,
              'sigma3':0.06978*mm,
              'f1':0.27633,
              "f2": 0.0359881091
         },
         "CovXX":0.35419*(0.02377*mm)**2 + 0.0710778486*(0.04692*mm)**2 + (1-0.35419-0.0710778486)*(0.01221*mm)**2,
         "CovYY":0.56669*(0.01160*mm)**2 + 0.069979565*(0.04330*mm)**2 + (1-0.56669-0.069979565)*(0.02243*mm)**2,
         "CovZZ":0.27633*(0.16142*mm)**2 + 0.0359881091*(0.40333*mm)**2 + (1-0.27633-0.0359881091)*(0.06978*mm)**2
        },
    },
    '2017':{
        'MagUp' :
        {"X":{'mu1':-3.343e-5*mm,
              'mu2':-3.343e-5*mm,
              'mu3':-3.343e-5*mm,
              'sigma1':0.01238*mm,
              'sigma2':0.04589*mm,
              'sigma3':0.02336*mm,
              'f1':0.50859,
              "f2": 0.0891270317
        },
         "Y":{'mu1':4.15e-5*mm,
              'mu2':4.15e-5*mm,
              'mu3':4.15e-5*mm,
              'sigma1':0.01223*mm,
              'sigma2':0.04545*mm,
              'sigma3':0.02425*mm,
              'f1':0.58180,
              "f2": 0.064386072,
         },
         "Z":{'mu1':2.0e-4*mm,
              'mu2':2.0e-4*mm,
              'mu3':2.0e-4*mm,
              'sigma1':0.42427*mm,
              'sigma2':0.07200*mm,
              'sigma3':0.16817*mm,
              'f1':0.03839,
              "f2": 0.6670496248
         },
         "CovXX":0.50859*(0.01238*mm)**2 +0.0891270317*(0.04589*mm)**2 + (1-0.50859-0.0891270317)*(0.02336*mm)**2,
         "CovYY":0.58180*(0.01223*mm)**2 + 0.064386072*(0.04545*mm)**2 + (1-0.58180-0.064386072)*(0.02425*mm)**2,
         "CovZZ":0.03839*(0.42427*mm)**2 + 0.6670496248*(0.07200*mm)**2 +(1-0.03839-0.6670496248)*(0.16817*mm)**2
        },
        'MagDown' :
        {"X":{'mu1':-7.42e-5*mm,
              'mu2':-7.42e-5*mm,
              'mu3':-7.42e-5*mm,
              'sigma1':0.01232*mm,
              'sigma2':0.04692*mm,
              'sigma3':0.02418*mm,
              'f1':0.53837,
              "f2": 0.07661211479999999
        },
         "Y":{'mu1':0.93e-5*mm,
              'mu2':0.93e-5*mm,
              'mu3':0.93e-5*mm,
              'sigma1':0.01180*mm,
              'sigma2':0.04433*mm,
              'sigma3':0.02295*mm,
              'f1':0.54446,
              "f2": 0.07347860199999999
         },
         "Z":{'mu1':-2.6e-4,
              'mu2':-2.6e-4,
              'mu3':-2.6e-4,
              'sigma1':0.41223*mm,
              'sigma2':0.06970*mm,
              'sigma3':0.16299*mm,
              'f1':0.03819,
              "f2": 0.659994022
         },
         "CovXX":0.53837*(0.01232*mm)**2 +0.07661211479999999*(0.04692*mm)**2 +(1-0.53837-0.07661211479999999)*(0.02418*mm)**2,
         "CovYY":0.54446*(0.01180*mm)**2 + 0.07347860199999999*(0.04433*mm)**2 + (1-0.54446-0.07347860199999999)*(0.02295*mm)**2,
         "CovZZ":0.03819*(0.41223*mm)**2 + 0.659994022*(0.06970*mm)**2 + (1-0.03819-0.659994022)*(0.16299*mm)**2
        },
    },
    '2018':{
        'MagUp' :
        {"X":{'mu1':-7.97e-5*mm,
              'mu2':-7.97e-5*mm,
              'mu3':-7.97e-5*mm,
              'sigma1':0.01230*mm,
              'sigma2':0.04682*mm,
              'sigma3':0.02420*mm,
              'f1':0.54123,
              "f2": 0.0773577974
        },
         "Y":{'mu1':-3.34e-5*mm,
              'mu2':-3.34e-5*mm,
              'mu3':-3.34e-5*mm,
              'sigma1':0.01187*mm,
              'sigma2':0.04410*mm,
              'sigma3':0.02308*mm,
              'f1':0.54574,
              "f2": 0.0760067832
         },
         "Z":{'mu1':-1.6e-4*mm,
              'mu2':-1.6e-4*mm,
              'mu3':-1.6e-4*mm,
              'sigma1':0.42426*mm,
              'sigma2':0.07209*mm,
              'sigma3':0.17011*mm,
              'f1':0.03745,
              "f2": 0.6760181160000001
         },
         "CovXX":0.54123*(0.01230*mm)**2 + 0.0773577974*(0.04682*mm)**2 + (1-0.54123-0.0773577974)*(0.02420*mm)**2,
         "CovYY":0.54574*(0.01187*mm)**2 + 0.0760067832*(0.04410*mm)**2 + (1-0.54574-0.0760067832)*(0.02308*mm)**2,
         "CovZZ":0.03745*(0.42426*mm)**2 + 0.6760181160000001*(0.07209*mm)**2 + (1-0.03745-0.6760181160000001)*(0.17011*mm)**2
        },
        'MagDown' :
        {"X":{'mu1':-7.92e-5*mm,
              'mu2':-7.92e-5*mm,
              'mu3':-7.92e-5*mm,
              'sigma1':0.01249*mm,
              'sigma2':0.04630*mm,
              'sigma3':0.02393*mm,
              'f1':0.5408,
              "f2": 0.08132432000000002
        },
         "Y":{'mu1':-5.89e-5*mm,
              'mu2':-5.89e-5*mm,
              'mu3':-5.89e-5*mm,
              'sigma1':0.01223*mm,
              'sigma2':0.04540*mm,
              'sigma3':0.02381*mm,
              'f1':0.57869,
              "f2": 0.0645236265
         },
         "Z":{'mu1':1.5e-4*mm,
              'mu2':1.5e-4*mm,
              'mu3':1.5e-4*mm,
              'sigma1':0.41754*mm,
              'sigma2':0.07009*mm,
              'sigma3':0.16409*mm,
              'f1':0.03805,
              "f2": 0.6650633715000001,
         },
         "CovXX":0.5408*(0.01249*mm)**2 + 0.08132432000000002*(0.04630*mm)**2 + (1-0.5408-0.08132432000000002)*(0.02393*mm)**2,
         "CovYY":0.57869*(0.01223*mm)**2 + 0.0645236265*(0.04540*mm)**2 + (1-0.57869-0.0645236265)*(0.02381*mm)**2,
         "CovZZ":0.03805*(0.41754*mm)**2 + 0.6650633715000001*(0.07009*mm)**2 + (1-0.03805-0.6650633715000001)*(0.16409*mm)**2,
        },
    },
}
