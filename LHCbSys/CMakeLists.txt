################################################################################
# Package: LHCbSys
################################################################################

# Turn off threading for a number of external components that otherwise
# would decide how many threads to use for themselves.

# OpenBLAS
gaudi_env(SET OPENBLAS_NUM_THREADS 1)
# XGBoost
gaudi_env(SET OMP_THREAD_LIMIT 1)
