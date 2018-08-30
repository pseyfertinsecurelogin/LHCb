################################################################################
# Package: MuonDet
################################################################################
gaudi_subdir(MuonDet v10r0)

gaudi_depends_on_subdirs(Det/DetDescCnv
                         GaudiAlg
                         Kernel/LHCbKernel
                         Muon/MuonKernel)

find_package(ROOT)
find_package(Boost)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS})

gaudi_add_library(MuonDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS MuonDet
                  LINK_LIBRARIES DetDescCnvLib GaudiAlgLib LHCbKernel MuonKernelLib)

gaudi_add_module(MuonDet
                 src/component/*.cpp
                 LINK_LIBRARIES DetDescCnvLib GaudiAlgLib LHCbKernel MuonKernelLib MuonDetLib)

gaudi_add_dictionary(MuonDet
                     dict/MuonDetDict.h
                     dict/MuonDetDict.xml
                     LINK_LIBRARIES DetDescCnvLib GaudiAlgLib LHCbKernel MuonKernelLib MuonDetLib
                     OPTIONS "-U__MINGW32__")

