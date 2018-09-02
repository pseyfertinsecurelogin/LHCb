################################################################################
# Package: HLTScheduler
################################################################################
gaudi_subdir(HLTScheduler)

gaudi_depends_on_subdirs(
                         GaudiCommonSvc
                         GaudiCoreSvc
                         GaudiHive
                         GaudiAlg
                         GaudiKernel)

find_package(Boost)
find_package(TBB)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${TBB_INCLUDE_DIRS})

gaudi_add_module(HLTScheduler
                 *.cpp
                 INCLUDE_DIRS Boost HLTScheduler
                 LINK_LIBRARIES Boost GaudiAlgLib GaudiKernel)

#gaudi_add_unit_test(utestTrackingCoder
#                    src/utest/utestTrackingCoder.cpp
#                    LINK_LIBRARIES GaudiKernel HltEvent HLTSchedulerLib
#                    TYPE Boost)
#
gaudi_add_test(QMTest QMTEST)
