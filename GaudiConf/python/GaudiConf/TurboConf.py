"""High level configuration for Turbo."""
import logging as log
from os.path import join, split

from Configurables import LHCbConfigurableUser
from Configurables import DataOnDemandSvc
import PersistRecoConf

__author__ = "Sean Benson, Rosen Matev"

__all__ = [
    'TurboConf'
]


class TurboConf(LHCbConfigurableUser):
    """Configure writing/packing/unpacking for Turbo."""

    __slots__ = {
        "DataType":    "",
        "Simulation": False,
        "PersistReco": False,
        "RunPackedDataDecoder": False,
        "RunPersistRecoUnpacking": False,
        "RootInTES": "/Event/Turbo"
     }

    _propertyDocDct = {
       "DataType":    "Flag for backward compatibility with old data",
       "Simulation": "Flag set to True when running over simulated data",
       "PersistReco": "Setup PersistReco (overrides some Turbo defaults)",
       "RunPackedDataDecoder": "If True, decode the raw data bank containing PersistReco data",
       "RunPersistRecoUnpacking": "If True, unpack the PersistReco containers",
       "RootInTES": "Where to link the unpacked PersistReco objects under"
    }

    __used_configurables__ = [
        "DecodeRawEvent",  # we need to modify a decoder, so apply this after
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

    def _register_pr_links(self, packing, rootintes, datatype, simulation):
        """Set up DataOnDemandSvc to create links to standard rec locations."""
        from Configurables import TESMergerProtoParticle
        from Configurables import TESMergerTrack
        from Configurables import GaudiSequencer
        from Configurables import Gaudi__DataLink as DataLink

        mergedProtosLoc = 'Hlt2/Protos/Charged'
        mergeProtos = TESMergerProtoParticle("MergeProtos")
        mergeProtos.inputLocations = [
            packing.outputs["Hlt2LongProtos"],
            packing.outputs["Hlt2DownstreamProtos"],
        ]
        mergeProtos.outputLocation = join(rootintes, mergedProtosLoc)
        DataOnDemandSvc().AlgMap[mergeProtos.outputLocation] = mergeProtos

        mergedTracksLoc = 'Hlt2/TrackFitted/Charged'
        mergeTracks = TESMergerTrack("MergeTracks")
        mergeTracks.inputLocations = [
            packing.outputs["Hlt2LongTracks"],
            packing.outputs["Hlt2DownstreamTracks"],
        ]
        mergeTracks.outputLocation = join(rootintes, mergedTracksLoc)
        DataOnDemandSvc().AlgMap[mergeTracks.outputLocation] = mergeTracks

        linkChargedProtos = DataLink('HltRecProtos',
                                     What=str(mergeProtos.outputLocation),
                                     Target=join(rootintes, 'Rec/ProtoP/Charged'))
        DataOnDemandSvc().AlgMap[linkChargedProtos.Target] = linkChargedProtos

        linkTracks = DataLink('HltRecTracks',
                              What=str(mergeTracks.outputLocation),
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
        
        # 2016 MC is a bit special, as the relations tables are made for the
        # merged protos container in Tesla and this is *expected* to be under
        # /Event, not /Event/Turbo, so we have to make sure the container exists
        # there
        if simulation and datatype == 2016:
            assert rootintes.startswith('/Event/Turbo')
            linkMergedProtos = DataLink('LinkHltMergedProtos',
                                        What=mergeProtos.outputLocation,
                                        Target=join('/Event', mergedProtosLoc)
                                        )
            DataOnDemandSvc().AlgMap[linkMergedProtos.Target] = linkMergedProtos
            
            # Just in case, we do the same for the merged tracks
            linkMergedTracks = DataLink('LinkHltMergedTracks',
                                        What=mergeTracks.outputLocation,
                                        Target=join('/Event', mergedTracksLoc)
                                        )
            DataOnDemandSvc().AlgMap[linkMergedTracks.Target] = linkMergedTracks

    def _register_tes_root_links(self, rootintes):
        """Link /Event/<stream>/Turbo/p{Phys,Rec} to under /Event/<stream>.

        This works around a feature of the dynamic unpackers configured in
        DstConf, which struggle with RootInTES settings that are two levels
        deep (like /Event/<stream>/Turbo, rather than /Event/Turbo).
        """
        from Configurables import Gaudi__DataLink as DataLink
        from GaudiKernel.Constants import ERROR

        rootinstream, _ = split(rootintes)
        if rootinstream in ['/Event', '/Event/Turbo']:
            return

        # OutputLevel=ERROR to suppress WARNINGs when the 'What' doesn't exist
        link_pPhys = DataLink('LinkTurbopPhys',
                              What=join(rootintes, 'pPhys'),
                              Target=join(rootinstream, 'pPhys'),
                              OutputLevel=ERROR)
        link_pRec = DataLink('LinkTurbopRec',
                              What=join(rootintes, 'pRec'),
                              Target=join(rootinstream, 'pRec'),
                              OutputLevel=ERROR)
        DataOnDemandSvc().AlgMap[link_pPhys.Target] = link_pPhys
        DataOnDemandSvc().AlgMap[link_pRec.Target] = link_pRec

    def _register_raw_event_links(self):
        """Link /Event/DAQ/RawEvent to /Event/Trigger/RawEvent.

        For 2015 and 2016 Tesla output, the raw event is not split, and so the
        trigger raw bank is in DAQ/RawEvent. In DaVinci, the trigger raw bank
        is expected to be in Trigger/RawEvent, so create a symlink.
        """
        from Configurables import Gaudi__DataLink as DataLink 
        link_daq = DataLink('LinkDAQRawEvent',
                            What='/Event/DAQ/RawEvent',
                            Target='/Event/Trigger/RawEvent')
        DataOnDemandSvc().AlgMap[link_daq.Target] = link_daq

    def _persistrecopacking(self, datatype, rootintes):
        """Return configured PersistRecoPacking object.

        The PersistRecoPacking configuration always assumes that the packed
        containers are immediately under /Event, and will be unpacked to
        locations immediately under /Event/Turbo.
        Since 2017, the Turbo streaming implementation changed such that
        packed containers are now under stream-specific locations (i.e.
        /Event/<stream>/Turbo) after Tesla.
        The unpacked containers should go under the same root.

        This means:
            * For 2016, the same unpackers are used in Tesla and DaVinci; and
            * For >= 2017, Tesla and DaVinci have dedicated unpackers.
        Tesla configures its own (un)packers for >= 2017.
        """
        tes_root = '/Event'
        turbo_root = join(tes_root, 'Turbo')
        unified_datatypes = [2017, 2018]
        if datatype in unified_datatypes:
            packed_root = unpacked_root = rootintes
        else:
            packed_root = tes_root
            unpacked_root = turbo_root

        packing_descriptors = PersistRecoConf.standardDescriptors
        packing_outputs = PersistRecoConf.standardOutputs
        for dt in unified_datatypes:
            dt = str(dt)
            packing_descriptors[dt] = {
                k: d.copy(location=d.location.replace(tes_root, packed_root))
                for k, d in packing_descriptors[dt].items()
            }
            packing_outputs[dt] = {
                k: v.replace(turbo_root, unpacked_root)
                for k, v in packing_outputs[dt].items()
            }
        return PersistRecoConf.PersistRecoPacking(
            str(datatype),
            descriptors=packing_descriptors,
            outputs=packing_outputs
        )


    def _check_configuration(self):
        """Try to catch mis-configurations and warn the user."""
        datatype = self.getProp('DataType')
        persistreco = self.getProp('PersistReco')
        decode = self.getProp('RunPackedDataDecoder')
        unpack = self.getProp('RunPersistRecoUnpacking')

        try:
            datatype = int(datatype)
        except ValueError:
            log.error('TurboConf is not compatible with non-integer DataTypes')

        if datatype < 2015:
            log.warning('Turbo is not available for Run 1 data')

        if datatype == 2015 and (persistreco or decode or unpack):
            log.warning('PersistReco is not available for 2015 data')

        if datatype >= 2017 and persistreco:
            log.warning((
                'TurboConf().PersistReco should not be set for >= 2017 data\n'
                'DaVinci jobs should set DaVinci().Turbo instead, otherwise '
                'use RunPackedDataDecoder and/or RunPersistRecoUnpacking'
            ))

        if persistreco and (decode or unpack):
            log.warning((
                'TurboConf().PersistReco should not be True together with '
                'RunPackedDataDecoder and/or RunPersistRecoUnpacking'
            ))

    def __apply_configuration__(self):
        self._check_configuration()

        datatype = int(self.getProp('DataType'))
        simulation = self.getProp('Simulation')
        rootintes = self.getProp('RootInTES')
        persistreco = self.getProp('PersistReco')
        decode = self.getProp('RunPackedDataDecoder')
        unpack = self.getProp('RunPersistRecoUnpacking')

        if decode or persistreco:
            from DAQSys.Decoders import DecoderDB
            from DAQSys.DecoderClass import Decoder

            # Remove standard decoder
            decoder = DecoderDB.pop("HltPackedDataDecoder/Hlt2PackedDataDecoder")
            assert not decoder.wasUsed(), ('Hlt2PackedDataDecoder was '
                                           'aready setup, cannot remove!')
            # In 2017 the decoder is only used in Tesla (not DaVinci), where we
            # don't need the special RootInTES gymnastics implemented in
            # _persistrecopacking
            # For 2016 it's used in Tesla *and* DaVinci, but we also don't need
            # the special configuration there, so can always use the default
            packing = PersistRecoConf.PersistRecoPacking(str(datatype))
            decoder = Decoder(
                "HltPackedDataDecoder/Hlt2PackedDataDecoder",
                active=True, banks=["DstData"],
                inputs={"RawEventLocations": None},
                outputs=packing.packedLocations(),
                properties={"ContainerMap":
                            packing.packedToOutputLocationMap()},
                conf=DecoderDB
            )
            decoder.setup()

        packing = self._persistrecopacking(datatype, rootintes)
        # CALO objects are treated specially in 2015 and 2016
        if datatype < 2017:
            self._register_unpackers()
            self._register_raw_event_links()
        if unpack or persistreco:
            self._register_pr_unpackers(packing)
            self._register_pr_links(packing, rootintes, datatype, simulation)
            self._register_tes_root_links(rootintes)
