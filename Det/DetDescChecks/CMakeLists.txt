################################################################################
# Package: DetDescChecks
################################################################################
gaudi_subdir(DetDescChecks)

gaudi_depends_on_subdirs(Det/DDDB
                         Det/DetDesc
                         Det/DetDescCnv
                         Det/DetDescSvc
                         Det/Magnet
                         GaudiAlg
                         GaudiConf
                         GaudiPython
                         GaudiSvc
                         RootHistCnv)

find_package(ROOT)
find_package(Boost)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(DetDescChecks
                 src/*.cpp
                 LINK_LIBRARIES Boost DetDescLib DetDescCnvLib GaudiAlgLib)

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
