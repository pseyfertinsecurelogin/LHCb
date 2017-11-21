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

find_package(Vc)
include_directories(SYSTEM ${Vc_INCLUDE_DIR})

gaudi_add_library(RichFutureMCUtilsLib
                  src/lib/*.cpp
                  PUBLIC_HEADERS RichFutureMCUtils
                  INCLUDE_DIRS Kernel/LHCbKernel Rich/RichFutureKernel Rich/RichUtils Event/MCEvent Event/RecEvent Event/TrackEvent 
                  LINK_LIBRARIES LinkerEvent RelationsLib RichFutureKernel GaudiUtilsLib LHCbKernel RichUtils RecEvent MCEvent TrackEvent )

target_link_libraries( RichFutureMCUtilsLib "${Vc_LIB_DIR}/libVc.a" )

gaudi_add_module(RichFutureMCUtils
                 src/component/*.cpp
                 INCLUDE_DIRS Kernel/LHCbKernel Rich/RichFutureKernel Rich/RichUtils Event/MCEvent Event/RecEvent Event/TrackEvent 
                 LINK_LIBRARIES LinkerEvent RelationsLib RichFutureKernel RichFutureMCUtilsLib GaudiUtilsLib LHCbKernel RichUtils RecEvent MCEvent TrackEvent )

target_link_libraries( RichFutureMCUtils "${Vc_LIB_DIR}/libVc.a" )
