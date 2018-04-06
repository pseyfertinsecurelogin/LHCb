################################################################################
# Package: PartProp
################################################################################
gaudi_subdir(PartProp v1r21)

gaudi_depends_on_subdirs(GaudiKernel
                         Kernel/LHCbMath)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})


gaudi_add_library(PartPropLib
                  src/*.cpp
                  PUBLIC_HEADERS Kernel
                  LINK_LIBRARIES GaudiKernel LHCbMathLib)

gaudi_add_module(PartProp
                 src/Components/*.cpp
                 LINK_LIBRARIES GaudiKernel LHCbMathLib PartPropLib)

gaudi_add_dictionary(PartProp
                     dict/PartPropDict.h
                     dict/PartProp.xml
                     LINK_LIBRARIES GaudiKernel LHCbMathLib PartPropLib
                     OPTIONS " -U__MINGW32__ ")

gaudi_install_python_modules()
gaudi_install_scripts()
gaudi_alias (dump_particle_properties
             dump_ParticleProperties) 

gaudi_add_test(QMTest QMTEST)
