################################################################################
# Package: GaudiConf
################################################################################
gaudi_subdir(GaudiConf v19r20)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         L0/L0DU
                         DAQ/RawEventCompat
                         Event/EventPacker
                         )

set_property(DIRECTORY PROPERTY CONFIGURABLE_USER_MODULES
             GaudiConf.SimConf GaudiConf.DigiConf GaudiConf.CaloPackingConf
             GaudiConf.DstConf GaudiConf.TurboConf)
gaudi_install_python_modules()
gaudi_install_scripts()

# GAUDI_QMTEST_CLASS overwrites the default python class used for LHCb tests
# Default is QMTTest, we use here an LHCb extension : LHCbTest
gaudi_env(SET STDOPTS \${GAUDICONFROOT}/options
          SET GAUDI_QMTEST_MODULE GaudiConf.QMTest.LHCbTest
          SET GAUDI_QMTEST_CLASS LHCbTest)

gaudi_add_test(QMTest QMTEST)
