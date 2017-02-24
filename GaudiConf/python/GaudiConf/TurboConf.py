"""High level configuration for Turbo."""
from os.path import join

from Configurables import LHCbConfigurableUser
from Configurables import DataOnDemandSvc
from PersistRecoConf import PersistRecoPacking

__author__ = "Sean Benson, Rosen Matev"

__all__ = [
    'TurboConf'
]


class TurboConf(LHCbConfigurableUser):
    """Configure writing/packing/unpacking for Turbo."""

    __slots__ = {
        "DataType":    "",
        "PersistReco": False,
        "RootInTES": "/Event/Turbo"
     }

    _propertyDocDct = {
       "DataType":    "Flag for backward compatibility with old data",
       "PersistReco": "Setup PersistReco (overrides some Turbo defaults)",
       "RootInTES": "Where to link the unpacked PersistReco objects under"
    }

    __used_configurables__ = [
    ]

    @classmethod
    def optItemList(cls):
        return [
            "/Event/Turbo/pPhys/Particles#99",
            "/Event/Turbo/pPhys/Vertices#99",
            "/Event/Turbo/pPhys/RecVertices#99",
            "/Event/Turbo/pPhys/Relations#99",
            "/Event/Turbo/pPhys/PP2MCPRelations#99",
            "/Event/Turbo/pRec/Track/Custom#99",
            "/Event/Turbo/pRec/Muon/CustomPIDs#99",
            "/Event/Turbo/pRec/Rich/CustomPIDs#99",
            "/Event/Turbo/pRec/neutrals/Clusters#99",
            "/Event/Turbo/pRec/neutrals/Hypos#99",
            "/Event/Turbo/pRec/ProtoP/Custom#99",
        ]

    def _register_unpackers(self):
        """Set up DataOnDemandSvc to unpack reconstruction information."""
        from Configurables import DataPacking__Unpack_LHCb__CaloClusterPacker_ as UnpackCaloClusters
        from Configurables import UnpackCaloHypo as UnpackCaloHypos

        clustersRep = UnpackCaloClusters(
            name="UnpackCaloClustersRep",
            InputName="/Event/Turbo/pRec/neutral/Clusters",
            OutputName="Turbo/CaloClusters")
        hyposRep = UnpackCaloHypos(
            name="UnpackCaloHyposRep",
            InputName="/Event/Turbo/pRec/neutral/Hypos",
            OutputName="Turbo/CaloHypos")
        for alg in [clustersRep, hyposRep]:
            DataOnDemandSvc().AlgMap[alg.OutputName] = alg

    def _register_pr_unpackers(self, packing):
        """Set up DataOnDemandSvc to unpack reconstruction information."""
        for alg in packing.unpackers():
            DataOnDemandSvc().AlgMap[alg.OutputName] = alg

    def _register_pr_links(self, packing):
        """Set up DataOnDemandSvc to create links to standard rec locations."""
        from Configurables import TESMerger_LHCb__ProtoParticle_ as TESMergerProtoParticle
        from Configurables import TESMerger_LHCb__Track_ as TESMergerTrack
        from Configurables import GaudiSequencer
        from Configurables import Gaudi__DataLink as DataLink

        rootintes = self.getProp("RootInTES")

        mergeProtos = TESMergerProtoParticle("MergeProtos")
        mergeProtos.inputLocations = [
            packing.outputs["Hlt2LongProtos"],
            packing.outputs["Hlt2DownstreamProtos"],
        ]
        mergeProtos.outputLocation = '/Event/Hlt2/Protos/Charged'
        DataOnDemandSvc().AlgMap[mergeProtos.outputLocation] = mergeProtos

        mergeTracks = TESMergerTrack("MergeTracks")
        mergeTracks.inputLocations = [
            packing.outputs["Hlt2LongTracks"],
            packing.outputs["Hlt2DownstreamTracks"],
        ]
        mergeTracks.outputLocation = '/Event/Hlt2/TrackFitted/Charged'
        DataOnDemandSvc().AlgMap[mergeTracks.outputLocation] = mergeTracks

        linkChargedProtos = DataLink('HltRecProtos',
                                     What=mergeProtos.outputLocation,
                                     Target=join(rootintes, 'Rec/ProtoP/Charged'))
        DataOnDemandSvc().AlgMap[linkChargedProtos.Target] = linkChargedProtos

        linkTracks = DataLink('HltRecTracks',
                              What=mergeTracks.outputLocation,
                              Target=join(rootintes, 'Rec/Track/Best'))
        DataOnDemandSvc().AlgMap[linkTracks.Target] = linkTracks

        linkNeutralProtos = DataLink('HltRecNeutralProtos',
                                     What=packing.outputs["Hlt2NeutralProtos"],
                                     Target=join(rootintes, 'Rec/ProtoP/Neutrals'))
        recNeutralProtos = GaudiSequencer("TurboNeutralProtosAsRec")
        recNeutralProtos.Members = [linkNeutralProtos]
        DataOnDemandSvc().AlgMap[linkNeutralProtos.Target] = recNeutralProtos

        linkPVs = DataLink('LinkHltPersistRecoPVs',
                           What=packing.outputs['Hlt2RecVertices'],
                           Target=join(rootintes, 'Rec/Vertex/Primary'))
        DataOnDemandSvc().AlgMap[linkPVs.Target] = linkPVs

    def __apply_configuration__(self):
        self._register_unpackers()

        if self.getProp("DataType")=="2015":
            # do not neet decoder interference if using 2015 data
            from DAQSys.Decoders import DecoderDB
            from DAQSys.DecoderClass import Decoder
            Decoder("HltPackedDataDecoder/Hlt2PackedDataDecoder",active=False,conf=DecoderDB)

        if self.getProp('PersistReco'):
            packing = PersistRecoPacking()
            
            self._register_pr_unpackers(packing)
            self._register_pr_links(packing)
            
            # account for name change in 2015
            if self.getProp("DataType")=="2015":
                unpackers = packing.unpackers()
                for alg in unpackers:
                    # Fix packed location
                    origLoc = alg.InputName
                    newLoc=origLoc.replace("Long","long")
                    newLoc1=newLoc.replace("Downstream","down")
                    newLoc2=newLoc1.replace("Neutral","neutral")
                    alg.InputName=newLoc2
                    # Point to correct RichPID location
                    origLoc_rpid = alg.OutputName
                    newLoc_rpid = origLoc_rpid.replace("deuteron","")
                    alg.OutputName = newLoc_rpid
                    if origLoc_rpid!=newLoc_rpid:
                        DataOnDemandSvc().AlgMap[alg.OutputName] = alg

