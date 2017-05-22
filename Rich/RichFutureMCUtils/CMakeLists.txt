################################################################################
# Package: RichFutureMCUtils
################################################################################
gaudi_subdir(RichFutureMCUtils v1r0)

gaudi_depends_on_subdirs(GaudiUtils
                         Kernel/LHCbKernel
                         Event/MCEvent
                         Event/RecEvent
                         Event/TrackEvent
                         Event/LinkerEvent
                         Kernel/Relations
                         Rich/RichFutureKernel
                         Rich/RichFutureUtils)

#find_package(AIDA)
#find_package(Boost)
#find_package(VDT)
#find_package(ROOT COMPONENTS Vc)

gaudi_add_library(RichFutureMCUtilsLib
                  src/lib/*.cpp
                  PUBLIC_HEADERS RichFutureMCUtils
                  INCLUDE_DIRS Kernel/LHCbKernel Rich/RichFutureKernel Rich/RichUtils Event/MCEvent Event/RecEvent Event/TrackEvent 
                  LINK_LIBRARIES LinkerEvent RelationsLib RichFutureKernel GaudiUtilsLib LHCbKernel RichUtils RecEvent MCEvent TrackEvent )

gaudi_add_module(RichFutureMCUtils
                 src/component/*.cpp
                 INCLUDE_DIRS Kernel/LHCbKernel Rich/RichFutureKernel Rich/RichUtils Event/MCEvent Event/RecEvent Event/TrackEvent 
                 LINK_LIBRARIES LinkerEvent RelationsLib RichFutureKernel RichFutureMCUtilsLib GaudiUtilsLib LHCbKernel RichUtils RecEvent MCEvent TrackEvent )
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
