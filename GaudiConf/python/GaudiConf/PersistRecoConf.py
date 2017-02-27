"""Packing configuration for HLT persisted reconstruction."""
from Gaudi.Configuration import *
from Configurables import PackProtoParticle, UnpackProtoParticle
from Configurables import PackTrack, UnpackTrack
from Configurables import DataPacking__Pack_LHCb__RichPIDPacker_ as PackRichPIDs
from Configurables import DataPacking__Unpack_LHCb__RichPIDPacker_ as UnpackRichPIDs
from Configurables import DataPacking__Pack_LHCb__MuonPIDPacker_ as PackMuonPIDs
from Configurables import DataPacking__Unpack_LHCb__MuonPIDPacker_ as UnpackMuonPIDs
from Configurables import PackRecVertex, UnpackRecVertex
from Configurables import DataPacking__Pack_LHCb__CaloClusterPacker_ as PackCaloClusters
from Configurables import DataPacking__Unpack_LHCb__CaloClusterPacker_ as UnpackCaloClusters
from Configurables import PackCaloHypo, UnpackCaloHypo

__all__ = [
    'PersistRecoPacking'
]

__author__ = "Rosen Matev"


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


standardDescriptors = [
    # Charged proto particles
    PackingDescriptor(
        name='Hlt2LongProtos',
        location="/Event/Hlt2/pRec/Long/Protos",
        packer=PackProtoParticle, unpacker=UnpackProtoParticle),
    PackingDescriptor(
        name="Hlt2DownstreamProtos",
        location="/Event/Hlt2/pRec/Downstream/Protos",
        packer=PackProtoParticle, unpacker=UnpackProtoParticle),
    # RICH PIDs
    PackingDescriptor(
        name="Hlt2LongRichPIDs",
        location="/Event/Hlt2/pRec/Long/RichPIDs",
        packer=PackRichPIDs, unpacker=UnpackRichPIDs),
    PackingDescriptor(
        name="Hlt2DownstreamRichPIDs",
        location="/Event/Hlt2/pRec/Downstream/RichPIDs",
        packer=PackRichPIDs, unpacker=UnpackRichPIDs),
    # Muon PIDs
    PackingDescriptor(
        name="Hlt2LongMuonPIDs",
        location="/Event/Hlt2/pRec/Long/MuonIDs",
        packer=PackMuonPIDs, unpacker=UnpackMuonPIDs),
    PackingDescriptor(
        name="Hlt2DownstreamPIDMuonSegments",
        location="/Event/Hlt2/pRec/Downstream/PID/MuonSegments",
        packer=PackTrack, unpacker=UnpackTrack),
    # Tracks
    PackingDescriptor(
        name="Hlt2LongTracks",
        location="/Event/Hlt2/pRec/Long/Tracks",
        packer=PackTrack, unpacker=UnpackTrack),
    PackingDescriptor(
        name="Hlt2DownstreamTracks",
        location="/Event/Hlt2/pRec/Downstream/Tracks",
        packer=PackTrack, unpacker=UnpackTrack),
    PackingDescriptor(
        name="Hlt2VeloPVTracks",
        location="/Event/Hlt2/pRec/VeloPV/Tracks",
        packer=PackTrack, unpacker=UnpackTrack),
    # Vertices
    PackingDescriptor(
        name="Hlt2RecVertices",
        location="/Event/Hlt2/pRec/Vertex/PV3D",
        packer=PackRecVertex, unpacker=UnpackRecVertex),

    # Neutral proto particles
    PackingDescriptor(
        name="Hlt2NeutralProtos",
        location="/Event/Hlt2/pRec/Neutral/Protos",
        packer=PackProtoParticle, unpacker=UnpackProtoParticle),
    # Neutral CaloClusters
    PackingDescriptor(
        name="Hlt2CaloClusters",
        location="/Event/Hlt2/pRec/Neutral/CaloClusters",
        packer=PackCaloClusters, unpacker=UnpackCaloClusters),
    PackingDescriptor(
        name="Hlt2EcalSplitClusters",
        location="/Event/Hlt2/pRec/Neutral/EcalSplitClusters",
        packer=PackCaloClusters, unpacker=UnpackCaloClusters),
    # Neutral CaloHypos
    PackingDescriptor(
        name="Hlt2CaloElectronHypos",
        location="/Event/Hlt2/pRec/Neutral/Electrons",
        packer=PackCaloHypo, unpacker=UnpackCaloHypo),
    PackingDescriptor(
        name="Hlt2CaloPhotonHypos",
        location="/Event/Hlt2/pRec/Neutral/Photons",
        packer=PackCaloHypo, unpacker=UnpackCaloHypo),
    PackingDescriptor(
        name="Hlt2CaloMergedPi0Hypos",
        location="/Event/Hlt2/pRec/Neutral/MergedPi0s",
        packer=PackCaloHypo, unpacker=UnpackCaloHypo),
    PackingDescriptor(
        name="Hlt2CaloSplitPhotonHypos",
        location="/Event/Hlt2/pRec/Neutral/SplitPhotons",
        packer=PackCaloHypo, unpacker=UnpackCaloHypo),
]

standardOutputs = {
    "Hlt2LongProtos":                "/Event/Hlt2/Long/Protos",
    "Hlt2DownstreamProtos":          "/Event/Hlt2/Downstream/Protos",
    "Hlt2LongRichPIDs":
        "/Event/Hlt2/TrackFitted/Long/PID/RICH" +
        "/electronmuonpionkaonprotondeuteronbelowThreshold/Rich1GasRich2GasLong",
    "Hlt2DownstreamRichPIDs":
        "/Event/Hlt2/TrackFitted/Downstream/PID/RICH" +
        "/electronmuonpionkaonprotondeuteronbelowThreshold/Rich1GasRich2GasDownstream",
    "Hlt2LongMuonPIDs":              "/Event/Hlt2/PID/Muon",
    "Hlt2DownstreamPIDMuonSegments": "/Event/Hlt2/TrackFitted/Downstream/PID/MuonSegments",
    "Hlt2LongTracks":                "/Event/Hlt2/TrackFitted/Long",
    "Hlt2DownstreamTracks":          "/Event/Hlt2/TrackFitted/Downstream",
    "Hlt2VeloPVTracks":              "/Event/Hlt2/TrackFitted/VeloPV",
    "Hlt2RecVertices":               "/Event/Hlt2/Vertex/PV3D",
    "Hlt2NeutralProtos":             "/Event/Hlt2/Neutral/Protos",
    "Hlt2CaloClusters":              "/Event/Hlt2/Calo/EcalClusters",
    "Hlt2EcalSplitClusters":         "/Event/Hlt2/PID/CALO/Calo/EcalSplitClusters",
    "Hlt2CaloElectronHypos":         "/Event/Hlt2/PID/CALO/Calo/Electrons",
    "Hlt2CaloPhotonHypos":           "/Event/Hlt2/PID/CALO/Calo/Photons",
    "Hlt2CaloMergedPi0Hypos":        "/Event/Hlt2/PID/CALO/Calo/MergedPi0s",
    "Hlt2CaloSplitPhotonHypos":      "/Event/Hlt2/PID/CALO/Calo/SplitPhotons",
}


# We need to register the locations of (non-reconstructed) data
# that is referenced by the some of the packed objects.
externalLocations = [
    '/Event/Raw/Ecal/Digits',
    '/Event/Raw/Prs/Digits',
    '/Event/Raw/Spd/Digits',
]


class PersistRecoPacking(object):
    """Collection of packed object descriptors.

    Provides useful methods for dealing with the PersistReco packed objects.

    Note:
        This can be upgraded to an LHCbConfigurableUser if the need arises.

    """
    def __init__(self, descriptors=standardDescriptors,
                 inputs={}, outputs=standardOutputs):
        """Collection of packed object descriptors.

        Args:
            descriptors: List of PackingDescriptor objects.
            inputs: Dict of the form {descriptor_name: input_location}.
                Used for configuration of packing.
            outputs: Dict of the form {descriptor_name: output_location}.
                Used for configuration of unpacking.
        """
        self._descriptors = descriptors
        names = [d.name for d in descriptors]
        self.inputs = {k: inputs[k] for k in inputs if k in names}
        self.outputs = {k: outputs[k] for k in outputs if k in names}
        self.external = externalLocations

    def packedLocations(self):
        """Return a list with the packed object locations."""
        return [d.location for d in self._descriptors]

    def externalLocations(self):
        """Return the list of external object locations."""
        return self.external

    def unpackers(self):
        """Return the list of unpacking algorithms."""
        algs = []
        for d in self._descriptors:
            alg = d.unpacker('Unpack' + d.name)
            alg.InputName = d.location
            alg.OutputName = self.outputs[d.name]
            algs.append(alg)
        return algs

    def packedToOutputLocationMap(self):
        """Return the dict {packed_location: output_location}."""
        m = {d.location: self.outputs[d.name] for d in self._descriptors}
        m.update({x: x for x in self.external})
        return m

    def packers(self):
        """Return the list of packing algorithms."""
        algs = []
        for d in self._descriptors:
            alg = d.packer('Pack' + d.name)
            alg.InputName = self.inputs[d.name]
            alg.OutputName = d.location
            algs.append(alg)
        return algs

    def inputToPackedLocationMap(self):
        """Return the dict {input_location: packed_location}."""
        m = {self.inputs[d.name]: d.location for d in self._descriptors}
        m.update({x: x for x in self.external})
        return m
