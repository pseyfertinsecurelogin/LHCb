################################################################################
# Package: L0HC
################################################################################
gaudi_subdir(L0HC v1r0)

gaudi_depends_on_subdirs(DAQ/DAQUtils
			 DAQ/DAQKernel	
                         Det/DetDesc
                         Event/DAQEvent
                         Event/DigiEvent
                         GaudiAlg
                         Kernel/LHCbKernel

#Calo/CaloDAQ
#Calo/CaloUtils
                         Event/LinkerEvent
                         DAQ/DAQKernel
                         L0/L0Base)

find_package(AIDA)

gaudi_add_module(L0HC
                 src/*.cpp
                 INCLUDE_DIRS AIDA HC/HCDAQ
                 LINK_LIBRARIES DetDesc DAQEvent DigiEvent GaudiAlg LHCbKernel LinkerEvent L0Base DAQKernelLib)

gaudi_env(SET L0HCOPTS \${L0HCROOT}/options)

