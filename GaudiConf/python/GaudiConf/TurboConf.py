"""High level configuration for Turbo."""
from Configurables import LHCbConfigurableUser
from Configurables import DataOnDemandSvc
from Configurables import Gaudi__DataLink as DataLink
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
     }

    _propertyDocDct = {
       "DataType":    "Flag for backward compatibility with old data",
       "PersistReco": "Setup PersistReco (overrides some Turbo defaults)",
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

        mergeProtos = TESMergerProtoParticle("MergeProtos")
        mergeProtos.inputLocations = [
            packing.outputs["Hlt2LongProtos"],
            packing.outputs["Hlt2DownstreamProtos"],
        ]
        mergeProtos.outputLocation = '/Event/Hlt2/Protos/Charged'
        linkChargedProtos = DataLink('HltRecProtos',
                                     What=mergeProtos.outputLocation,
                                     Target='/Event/Rec/ProtoP/Charged')
        recProtos = GaudiSequencer("TurboProtosAsRec")
        recProtos.Members = [mergeProtos, linkChargedProtos]
        DataOnDemandSvc().AlgMap[linkChargedProtos.Target] = recProtos

        linkNeutralProtos = DataLink('HltRecNeutralProtos',
                                     What=packing.outputs["Hlt2NeutralProtos"],
                                     Target='/Event/Rec/ProtoP/Neutral')
        recNeutralProtos = GaudiSequencer("TurboNeutralProtosAsRec")
        recNeutralProtos.Members = [linkNeutralProtos]
        DataOnDemandSvc().AlgMap[linkNeutralProtos.Target] = recNeutralProtos

        linkPVs = DataLink('LinkHltPersistRecoPVs',
                           What=packing.outputs['Hlt2RecVertices'],
                           Target='/Event/Turbo/PrimaryWithTracks')
        DataOnDemandSvc().AlgMap[linkPVs.Target] = linkPVs

    def _register_pv_links(self, pv_source):
        linkPVs = DataLink('LinkHltVertexReportsPVs',
                           What=pv_source,
                           Target='/Event/Rec/Vertex/Primary')
        DataOnDemandSvc().AlgMap[linkPVs.Target] = linkPVs

    def __apply_configuration__(self):
        self._register_unpackers()
        pv_source = '/Event/Turbo/Primary'

        if self.getProp('PersistReco'):
            packing = PersistRecoPacking()
            self._register_pr_unpackers(packing)
            self._register_pr_links(packing)
            pv_source = '/Event/Turbo/PrimaryWithTracks'

        self._register_pv_links(pv_source)
