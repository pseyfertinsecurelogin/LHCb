#!/usr/bin/env python
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
# =============================================================================
## Useful utilities for accesssing L0 algorithms as they are used in L0Conf
#  @author Julien Cogan
#  @date 2010-02-10
# =============================================================================
"""
Trivial module with few helper utilities to access the L0 algorithms as
they are used in L0Conf
"""

# =============================================================================
# Emulators
# =============================================================================

L0CaloAlgName        = "L0Calo"
L0MuonAlgName        = "L0Muon"
L0PileUpAlgName      = "L0PuVeto"
L0HCAlgName          = "L0HC"
L0DUAlgName          = "L0DU"

## L0Calo algorithm
def emulateL0Calo(suffix=''):
    """ Returns the algorithm to emulate the L0Calo."""
    from Configurables import L0CaloAlg
    return L0CaloAlg(L0CaloAlgName+suffix)

## L0Calo sequence
def emulateL0CaloSeq(suffix=''):
    """ Returns a list with the algorithm to emulate the L0Calo."""
    return [ emulateL0Calo(suffix) ]

## L0Muon algorithm
def emulateL0Muon(suffix=''):
    """ Returns the algorithm to emulate the L0Muon."""
    from Configurables import L0MuonAlg
    return L0MuonAlg(L0MuonAlgName+suffix)

## L0Muon sequence
def emulateL0MuonSeq(suffix=''):
    """ Returns a list with the algorithm to emulate the L0Muon."""
    return [ emulateL0Muon(suffix) ]

## L0Pileup algorithm
def emulateL0PileUp(suffix=''):
    """ Returns the algorithm to emulate the L0PileUp."""
    from Configurables import PuVetoAlg
    return PuVetoAlg(L0PileUpAlgName+suffix)

## L0Pileup sequence
def emulateL0PileUpSeq(suffix=''):
    """ Returns a list with the algorithm to emulate the L0PileUp."""
    return [ emulateL0PileUp(suffix) ]

## L0HC algorithm
def emulateL0HC(suffix=''):
    """ Returns the algorithm to emulate the L0HC."""
    from Configurables import L0HCAlg 
    return L0HCAlg(L0HCAlgName+"_emulate_"+suffix)
    
## L0DU algorithm
def emulateL0DU(suffix=''):
    """ Returns the algorithm to emulate the L0DU."""
    from Configurables import L0DUAlg
    return L0DUAlg(L0DUAlgName+suffix)
    
## L0DU sequence
def emulateL0DUSeq(suffix=''):
    """ Returns a list with the algorithm to emulate the L0DU."""
    return [ emulateL0DU(suffix) ]

# =============================================================================
# Decoding
# =============================================================================

L0CaloFromRawAlgName = "L0CaloFromRaw"
L0MuonFromRawAlgName = "L0MuonFromRaw"
L0DUFromRawAlgName   = "L0DUFromRaw"

## L0Calo algorithm
def decodeL0Calo(suffix=''):
    """ Returns the algorithm to decode the L0Calo data."""
    from Configurables import L0CaloCandidatesFromRaw 
    return L0CaloCandidatesFromRaw(L0CaloFromRawAlgName+suffix)

## L0Calo sequence
def decodeL0CaloSeq(suffix=''):
    """ Returns a list with the algorithm to decode the L0Calo data."""
    return [ decodeL0Calo(suffix) ] 
     
## L0Muon algorithm
def decodeL0Muon(suffix=''):
    """ Returns the algorithm to decode the L0Muon data."""
    from Configurables import L0MuonCandidatesFromRaw
    return L0MuonCandidatesFromRaw(L0MuonFromRawAlgName+suffix)

## L0Muon sequence
def decodeL0MuonSeq(suffix=''):
    """ Returns a list with the algorithm to decode the L0Muon data."""
    return [ decodeL0Muon(suffix) ] 

## L0DU algorithm
def decodeL0DU(suffix='',WriteProcData = False, WriteOnTES = True):
    """ Returns the algorithm to decode the L0DU data."""
    if ( WriteProcData or not WriteOnTES ) :
       from Configurables import L0DUFromRawAlg
       decoding = L0DUFromRawAlg(L0DUFromRawAlgName+suffix)
       decoding.WriteProcData = WriteProcData
       decoding.WriteOnTES = WriteOnTES
       return decoding
    else :
       from Configurables import L0DUDecoder
       return L0DUDecoder(L0DUFromRawAlgName+suffix)

## L0DU sequence
def decodeL0DUSeq(suffix=''):
    """ Returns a list with the algorithm to decode the L0DU data."""
    return [ decodeL0DU(suffix) ]

# =============================================================================
# Monitoring
# =============================================================================

## L0Calo algorithm
def monitorL0Calo(suffix=''):
    """ Returns the algorithm to monitor L0Calo data."""
    from Configurables import L0CaloMonit
    return L0CaloMonit("L0CaloMonit"+suffix)

## L0Muon algorithm
def monitorL0Muon(suffix=''):
    """ Returns the algorithm to monitor L0Muon data."""
    from Configurables import L0MuonOnlineMonitor
    return L0MuonOnlineMonitor("L0MuonOnlineMonitor"+suffix)

## L0DU algorithm
def monitorL0DU(suffix=''):
    """ Returns the algorithm to monitor L0DU data."""
    from Configurables import L0DUReportMonitor
    return L0DUReportMonitor("L0DUReportMonitor"+suffix)


