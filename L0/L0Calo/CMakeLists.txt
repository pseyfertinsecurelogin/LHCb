################################################################################
# Package: L0Calo
################################################################################
gaudi_subdir(L0Calo v11r16)

# hide warnings from some external projects
find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_depends_on_subdirs(Calo/CaloDAQ
                         Calo/CaloUtils
                         Event/LinkerEvent
                         DAQ/DAQKernel
                         L0/L0Base)

find_package(AIDA)

gaudi_add_module(L0Calo
                 src/*.cpp
                 INCLUDE_DIRS AIDA Calo/CaloDAQ
                 LINK_LIBRARIES CaloUtils LinkerEvent L0Base DAQKernelLib)

gaudi_install_python_modules()

gaudi_env(SET L0CALOOPTS \${L0CALOROOT}/options)

gaudi_add_test(QMTest QMTEST)
