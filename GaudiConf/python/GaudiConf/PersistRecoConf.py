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
"""Packing configuration for HLT persisted reconstruction."""
from collections import OrderedDict

from Gaudi.Configuration import *
from Configurables import PackProtoParticle, UnpackProtoParticle
from Configurables import PackTrack, UnpackTrack
from Configurables import PackVeloCluster, UnpackVeloCluster
from Configurables import PackSTCluster, UnpackSTCluster
from Configurables import DataPacking__Pack_LHCb__RichPIDPacker_ as PackRichPIDs
from Configurables import DataPacking__Unpack_LHCb__RichPIDPacker_ as UnpackRichPIDs
from Configurables import DataPacking__Pack_LHCb__MuonPIDPacker_ as PackMuonPIDs
from Configurables import DataPacking__Unpack_LHCb__MuonPIDPacker_ as UnpackMuonPIDs
from Configurables import PackRecVertex, UnpackRecVertex
from Configurables import DataPacking__Pack_LHCb__CaloClusterPacker_ as PackCaloClusters
from Configurables import DataPacking__Unpack_LHCb__CaloClusterPacker_ as UnpackCaloClusters
from Configurables import DataPacking__Pack_LHCb__CaloDigitPacker_ as PackCaloDigits
from Configurables import DataPacking__Unpack_LHCb__CaloDigitPacker_ as UnpackCaloDigits
from Configurables import DataPacking__Pack_LHCb__CaloAdcPacker_ as PackCaloAdcs
from Configurables import DataPacking__Unpack_LHCb__CaloAdcPacker_ as UnpackCaloAdcs
from Configurables import PackCaloHypo, UnpackCaloHypo

__all__ = ['PersistRecoPacking']

__author__ = "Rosen Matev"


def _location_2016_to_2015(location):
    return (location.replace("Long", "long").replace("Downstream",
                                                     "down").replace(
                                                         "Neutral", "neutral"))


def _od_rename(x, keymap):
    return OrderedDict((keymap.get(k, k), v) for k, v in x.items())


class PackingDescriptor(object):
    """Holds information of how and where to (un)pack objects.

    Attributes:
        name: Descriptive name of the object
        location: Packed object location
        packer: Algorithm used for packing
        unpacker: Algorithm used for unpacking

    """

    def __init__(self, name, location, packer, unpacker):
        self.name = name
        self.location = location
        self.packer = packer
        self.unpacker = unpacker

    def copy(self, **kwargs):
        new = PackingDescriptor(self.name, self.location, self.packer,
                                self.unpacker)
        for k, v in kwargs.items():
            setattr(new, k, v)
        return new


_standardDescriptors = [
    # Charged proto particles
    PackingDescriptor(
        name='Hlt2LongProtos',
        location="/Event/Hlt2/pRec/Long/Protos",
        packer=PackProtoParticle,
        unpacker=UnpackProtoParticle),
    PackingDescriptor(
        name="Hlt2DownstreamProtos",
        location="/Event/Hlt2/pRec/Downstream/Protos",
        packer=PackProtoParticle,
        unpacker=UnpackProtoParticle),
    # RICH PIDs
    PackingDescriptor(
        name="Hlt2LongRichPIDs",
        location="/Event/Hlt2/pRec/Long/RichPIDs",
        packer=PackRichPIDs,
        unpacker=UnpackRichPIDs),
    PackingDescriptor(
        name="Hlt2DownstreamRichPIDs",
        location="/Event/Hlt2/pRec/Downstream/RichPIDs",
        packer=PackRichPIDs,
        unpacker=UnpackRichPIDs),
    # Muon PIDs
    PackingDescriptor(
        name="Hlt2LongMuonPIDs",
        location="/Event/Hlt2/pRec/Long/MuonIDs",
        packer=PackMuonPIDs,
        unpacker=UnpackMuonPIDs),
    PackingDescriptor(
        name="Hlt2DownstreamPIDMuonSegments",
        location="/Event/Hlt2/pRec/Downstream/PID/MuonSegments",
        packer=PackTrack,
        unpacker=UnpackTrack),
    # Vertices
    PackingDescriptor(
        name="Hlt2RecVertices",
        location="/Event/Hlt2/pRec/Vertex/PV3D",
        packer=PackRecVertex,
        unpacker=UnpackRecVertex),
    # Tracks
    PackingDescriptor(
        name="Hlt2LongTracks",
        location="/Event/Hlt2/pRec/Long/Tracks",
        packer=PackTrack,
        unpacker=UnpackTrack),
    PackingDescriptor(
        name="Hlt2DownstreamTracks",
        location="/Event/Hlt2/pRec/Downstream/Tracks",
        packer=PackTrack,
        unpacker=UnpackTrack),
    PackingDescriptor(
        name="Hlt2VeloPVTracks",
        location="/Event/Hlt2/pRec/VeloPV/Tracks",
        packer=PackTrack,
        unpacker=UnpackTrack),

    # Neutral proto particles
    PackingDescriptor(
        name="Hlt2NeutralProtos",
        location="/Event/Hlt2/pRec/Neutral/Protos",
        packer=PackProtoParticle,
        unpacker=UnpackProtoParticle),
    # Neutral CaloClusters
    PackingDescriptor(
        name="Hlt2CaloClusters",
        location="/Event/Hlt2/pRec/Neutral/CaloClusters",
        packer=PackCaloClusters,
        unpacker=UnpackCaloClusters),
    PackingDescriptor(
        name="Hlt2EcalSplitClusters",
        location="/Event/Hlt2/pRec/Neutral/EcalSplitClusters",
        packer=PackCaloClusters,
        unpacker=UnpackCaloClusters),
    # Neutral CaloHypos
    PackingDescriptor(
        name="Hlt2CaloElectronHypos",
        location="/Event/Hlt2/pRec/Neutral/Electrons",
        packer=PackCaloHypo,
        unpacker=UnpackCaloHypo),
    PackingDescriptor(
        name="Hlt2CaloPhotonHypos",
        location="/Event/Hlt2/pRec/Neutral/Photons",
        packer=PackCaloHypo,
        unpacker=UnpackCaloHypo),
    PackingDescriptor(
        name="Hlt2CaloMergedPi0Hypos",
        location="/Event/Hlt2/pRec/Neutral/MergedPi0s",
        packer=PackCaloHypo,
        unpacker=UnpackCaloHypo),
    PackingDescriptor(
        name="Hlt2CaloSplitPhotonHypos",
        location="/Event/Hlt2/pRec/Neutral/SplitPhotons",
        packer=PackCaloHypo,
        unpacker=UnpackCaloHypo),
]

standardDescriptors = {}
# Transform the 2016 descriptors into OrderedDict{name: descriptor}
# without changing all the lines above. Unset descriptor.name
standardDescriptors['2016'] = OrderedDict(
    (i.name, i.copy(name=None)) for i in _standardDescriptors)

# In 2015 packed locations were named slightly differently
standardDescriptors['2015'] = OrderedDict(
    (k, v.copy(location=_location_2016_to_2015(v.location)))
    for k, v in standardDescriptors['2016'].items())

# We have 2012 descriptors only for running the unit tests
standardDescriptors['2012'] = standardDescriptors['2016'].copy()

standardDescriptors['2017'] = standardDescriptors['2016'].copy()
standardDescriptors['2017'].update([
    (i.name, i) for i in [
        # Track clusters
        PackingDescriptor(
            name="Hlt2VeloClusters",
            location="/Event/Hlt2/pRec/Velo/Clusters",
            packer=PackVeloCluster,
            unpacker=UnpackVeloCluster),
        PackingDescriptor(
            name="Hlt2TTClusters",
            location="/Event/Hlt2/pRec/TT/Clusters",
            packer=PackSTCluster,
            unpacker=UnpackSTCluster),
        PackingDescriptor(
            name="Hlt2ITClusters",
            location="/Event/Hlt2/pRec/IT/Clusters",
            packer=PackSTCluster,
            unpacker=UnpackSTCluster),
        # CaloDigits
        PackingDescriptor(
            name="Hlt2VeloClusters",
            location="/Event/Hlt2/pRec/Velo/Clusters",
            packer=PackVeloCluster,
            unpacker=UnpackVeloCluster),
        PackingDescriptor(
            name="Hlt2TTClusters",
            location="/Event/Hlt2/pRec/TT/Clusters",
            packer=PackSTCluster,
            unpacker=UnpackSTCluster),
        PackingDescriptor(
            name="Hlt2ITClusters",
            location="/Event/Hlt2/pRec/IT/Clusters",
            packer=PackSTCluster,
            unpacker=UnpackSTCluster),
        PackingDescriptor(
            name="Hlt2CaloEcalDigits",
            location="/Event/Hlt2/pRec/Neutral/EcalDigits",
            packer=PackCaloDigits,
            unpacker=UnpackCaloDigits),
        PackingDescriptor(
            name="Hlt2CaloHcalDigits",
            location="/Event/Hlt2/pRec/Neutral/HcalDigits",
            packer=PackCaloDigits,
            unpacker=UnpackCaloDigits),
        PackingDescriptor(
            name="Hlt2CaloPrsDigits",
            location="/Event/Hlt2/pRec/Neutral/PrsDigits",
            packer=PackCaloDigits,
            unpacker=UnpackCaloDigits),
        PackingDescriptor(
            name="Hlt2CaloSpdDigits",
            location="/Event/Hlt2/pRec/Neutral/SpdDigits",
            packer=PackCaloDigits,
            unpacker=UnpackCaloDigits),
        # CaloAdcs
        PackingDescriptor(
            name="Hlt2CaloEcalAdcs",
            location="/Event/Hlt2/pRec/Neutral/EcalAdcs",
            packer=PackCaloAdcs,
            unpacker=UnpackCaloAdcs),
        PackingDescriptor(
            name="Hlt2CaloHcalAdcs",
            location="/Event/Hlt2/pRec/Neutral/HcalAdcs",
            packer=PackCaloAdcs,
            unpacker=UnpackCaloAdcs),
        PackingDescriptor(
            name="Hlt2CaloPrsAdcs",
            location="/Event/Hlt2/pRec/Neutral/PrsAdcs",
            packer=PackCaloAdcs,
            unpacker=UnpackCaloAdcs),
        PackingDescriptor(
            name="Hlt2CaloSpdAdcs",
            location="/Event/Hlt2/pRec/Neutral/SpdAdcs",
            packer=PackCaloAdcs,
            unpacker=UnpackCaloAdcs)
    ]
])
standardDescriptors['2017'] = _od_rename(
    standardDescriptors['2017'], {
        'Hlt2DownstreamPIDMuonSegments': 'Hlt2MuonPIDSegments',
        'Hlt2LongMuonPIDs': 'Hlt2MuonPIDs'
    })
standardDescriptors['2018'] = standardDescriptors['2017'].copy()

standardOutputs = {}

standardOutputs['2016'] = {
    "Hlt2LongProtos":
    "/Event/Hlt2/Long/Protos",
    "Hlt2DownstreamProtos":
    "/Event/Hlt2/Downstream/Protos",
    "Hlt2LongRichPIDs":
    "/Event/Hlt2/TrackFitted/Long/PID/RICH" +
    "/electronmuonpionkaonprotondeuteronbelowThreshold/Rich1GasRich2GasLong",
    "Hlt2DownstreamRichPIDs":
    "/Event/Hlt2/TrackFitted/Downstream/PID/RICH" +
    "/electronmuonpionkaonprotondeuteronbelowThreshold/Rich1GasRich2GasDownstream",
    "Hlt2LongMuonPIDs":
    "/Event/Hlt2/PID/Muon",
    "Hlt2DownstreamPIDMuonSegments":
    "/Event/Hlt2/TrackFitted/Downstream/PID/MuonSegments",
    "Hlt2LongTracks":
    "/Event/Hlt2/TrackFitted/Long",
    "Hlt2DownstreamTracks":
    "/Event/Hlt2/TrackFitted/Downstream",
    "Hlt2VeloPVTracks":
    "/Event/Hlt2/TrackFitted/VeloPV",
    "Hlt2RecVertices":
    "/Event/Hlt2/Vertex/PV3D",
    "Hlt2NeutralProtos":
    "/Event/Hlt2/Neutral/Protos",
    "Hlt2CaloClusters":
    "/Event/Hlt2/Calo/EcalClusters",
    "Hlt2EcalSplitClusters":
    "/Event/Hlt2/PID/CALO/Calo/EcalSplitClusters",
    "Hlt2CaloElectronHypos":
    "/Event/Hlt2/PID/CALO/Calo/Electrons",
    "Hlt2CaloPhotonHypos":
    "/Event/Hlt2/PID/CALO/Calo/Photons",
    "Hlt2CaloMergedPi0Hypos":
    "/Event/Hlt2/PID/CALO/Calo/MergedPi0s",
    "Hlt2CaloSplitPhotonHypos":
    "/Event/Hlt2/PID/CALO/Calo/SplitPhotons",
}

# In 2015 the RICH locations were different
standardOutputs['2015'] = standardOutputs['2016'].copy()
standardOutputs['2015']["Hlt2LongRichPIDs"] = (
    "/Event/Hlt2/TrackFitted/Long/PID/RICH"
    "/electronmuonpionkaonprotonbelowThreshold/Rich1GasRich2GasLong")
standardOutputs['2015']["Hlt2DownstreamRichPIDs"] = (
    "/Event/Hlt2/TrackFitted/Downstream/PID/RICH"
    "/electronmuonpionkaonprotonbelowThreshold/Rich1GasRich2GasDownstream")

# We have 2012 descriptors only for running the unit tests
standardOutputs['2012'] = standardOutputs['2016'].copy()

# In 2017 and 2018 we unpack into a prefixed location /Event/Turbo
standardOutputs["2017"] = {
    "Hlt2LongProtos": "/Event/Turbo/Long/Protos",
    "Hlt2DownstreamProtos": "/Event/Turbo/Downstream/Protos",
    "Hlt2LongRichPIDs": "/Event/Turbo/PID/Rich/Long",
    "Hlt2DownstreamRichPIDs": "/Event/Turbo/PID/Rich/Downstream",
    "Hlt2MuonPIDs": "/Event/Turbo/PID/Muon",
    "Hlt2MuonPIDSegments": "/Event/Turbo/Track/Best/Muon",
    "Hlt2LongTracks": "/Event/Turbo/Track/Best/Long",
    "Hlt2DownstreamTracks": "/Event/Turbo/Track/Best/Downstream",
    "Hlt2VeloPVTracks": "/Event/Turbo/Track/FittedVeloInPV",
    "Hlt2VeloClusters": "/Event/Turbo/Raw/Velo/Clusters",
    "Hlt2TTClusters": "/Event/Turbo/Raw/TT/Clusters",
    "Hlt2ITClusters": "/Event/Turbo/Raw/IT/Clusters",
    "Hlt2RecVertices": "/Event/Turbo/Vertex/PV3D",
    "Hlt2NeutralProtos": "/Event/Turbo/Neutral/Protos",
    "Hlt2CaloClusters": "/Event/Turbo/PID/Calo/EcalClusters",
    "Hlt2EcalSplitClusters": "/Event/Turbo/PID/Calo/EcalSplitClusters",
    "Hlt2CaloElectronHypos": "/Event/Turbo/PID/Calo/Electrons",
    "Hlt2CaloPhotonHypos": "/Event/Turbo/PID/Calo/Photons",
    "Hlt2CaloMergedPi0Hypos": "/Event/Turbo/PID/Calo/MergedPi0s",
    "Hlt2CaloSplitPhotonHypos": "/Event/Turbo/PID/Calo/SplitPhotons",
    "Hlt2CaloEcalDigits": "/Event/Turbo/Raw/Ecal/Digits",
    "Hlt2CaloHcalDigits": "/Event/Turbo/Raw/Hcal/Digits",
    "Hlt2CaloPrsDigits": "/Event/Turbo/Raw/Prs/Digits",
    "Hlt2CaloSpdDigits": "/Event/Turbo/Raw/Spd/Digits",
    "Hlt2CaloEcalAdcs": "/Event/Turbo/Raw/Ecal/Adcs",
    "Hlt2CaloHcalAdcs": "/Event/Turbo/Raw/Hcal/Adcs",
    "Hlt2CaloPrsAdcs": "/Event/Turbo/Raw/Prs/Adcs",
    "Hlt2CaloSpdAdcs": "/Event/Turbo/Raw/Spd/Adcs"
}
standardOutputs['2018'] = standardOutputs['2017'].copy()

# We need to register the locations of (non-reconstructed) data
# that is referenced by the some of the packed objects.
_externalLocations = [
    '/Event/Raw/Ecal/Digits',
    '/Event/Raw/Prs/Digits',
    '/Event/Raw/Spd/Digits',
]
externalLocations = {}
# We have 2012 descriptors only for running the unit tests
externalLocations['2012'] = list(_externalLocations)
externalLocations['2015'] = list(_externalLocations)
externalLocations['2016'] = list(externalLocations['2015'])
# Digits are handled explicitly in 2017 and 2018
externalLocations['2017'] = externalLocations['2018'] = []


class PersistRecoPacking(object):
    """Collection of packed object descriptors.

    Provides useful methods for dealing with the PersistReco packed objects.

    Note:
        This can be upgraded to an LHCbConfigurableUser if the need arises.

    """

    def __init__(self,
                 datatype,
                 descriptors=standardDescriptors,
                 inputs={},
                 outputs=standardOutputs):
        """Collection of packed object descriptors.

        Args:
            descriptors: Dict of the form
                {datatype: <OrderedDict of PackingDescriptor objects>}.
            inputs: Dict of the form {descriptor_name: input_location}.
                Used for configuration of packing.
            outputs: Dict of the form
                {datatype: {descriptor_name: output_location}}.
                Used for configuration of unpacking.
        """
        self._descriptors = descriptors[datatype]
        self.inputs = inputs
        self.outputs = outputs[datatype]
        self.external = externalLocations[datatype]

    def packedLocations(self):
        """Return a list with the packed object locations."""
        return [d.location for d in self._descriptors.values()]

    def externalLocations(self):
        """Return the list of external object locations."""
        return self.external

    def unpackers(self):
        """Return the list of unpacking algorithms."""
        algs = []
        for name, d in self._descriptors.items():
            alg = d.unpacker('Unpack' + name)
            alg.InputName = d.location
            alg.OutputName = self.outputs[name]
            algs.append(alg)
        return algs

    def packedToOutputLocationMap(self):
        """Return the dict {packed_location: output_location}."""
        m = {
            d.location: self.outputs[name]
            for name, d in self._descriptors.items()
        }
        m.update({x: x for x in self.external})
        return m

    def packers(self, identifier=''):
        """Return the list of packing algorithms."""
        algs = []
        for name, d in self._descriptors.items():
            alg = d.packer('Pack' + name + identifier)
            alg.InputName = self.inputs[name]
            alg.OutputName = d.location
            algs.append(alg)
        return algs

    def inputToPackedLocationMap(self):
        """Return the dict {input_location: packed_location}."""
        m = {
            self.inputs[name]: d.location
            for name, d in self._descriptors.items()
        }
        m.update({x: x for x in self.external})
        return m
