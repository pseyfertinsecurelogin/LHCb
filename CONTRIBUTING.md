# General information

In addition to `master`, this project contains several officialy maintained branches, with names such as `XXX-patches`.
They are all protected, meaning that code cannot be pushed into them directly but only through merge requests (MRs).
This helps with the validation of code prior to making it available in the official branches for future releases.

## Available supported branches

- `master` branch: new developments and updates targeting run 3. Builds on current supported platforms against latest version of Gaudi

- `run2-patches` branch: new developments and updates targeting runs 1+2 analysis and/or reprocessing. Builds on current supported platfroms against latest version of Gaudi

- `2018-patches` branch: for 2018 incremental stripping (`S34r0pX`, `S35r0pX`, `S35r1pX`) and patches to Moore, Brunel (`Reco18`) and stripping in 2018 simulation workflows. Builds with gcc62 on slc6 and centos7 and with gcc7 on centos7

- `2017-patches` branch: for 2017 incremental stripping (`S29r2pX`, `S32r0pX`, `S33r1pX`) and patches to Moore, Brunel (`Reco17`) and stripping in 2017 simulation workflows. Builds with gcc49 on slc6 and with gcc62 on slc6 and centos7

- `2016-patches` branch: for 2016 incremental stripping (`S28r1pX`, `S30r2pX`, `S30r3pX`) and patches to Brunel (`Reco16`) and stripping in 2016 simulation workflows. Builds with gcc49 on slc6

- `hlt2016-patches` branch: for patches to Moore in 2016 simulation workflows. Builds with gcc49 on slc6

- `reco15-patches` branch: for patches to Brunel (`Reco15`) in 2015 simulation workflows. Builds with gcc49 on slc6

- `stripping24-patches` branch: for 2015 incremental stripping and patches to stripping in 2015 simulation workflows. Builds with gcc49 on slc6

- `reco14-patches` branch: for patches to Brunel (`Reco14`) in run 1 simulation workflows. Builds with gcc46 on slc5. Requires CMT

- `stripping21-patches` branch: for run 1 incremental stripping (`S21r0pX`, `S21r1pX`) and patches to stripping in run 1 simulation workflows. Builds with gcc49 on slc6

- `hlt2012-patches` branch: for patches to Moore in 2012 simulation workflows. Builds with gcc46 on slc5. Requires CMT

- `hlt2011-patches` branch: for patches to Moore in 2011 simulation workflows. Builds with gcc43 on slc5. Requires CMT

- `digi14-patches` branch: for patches to Boole in Sim09 simulation workflows. Builds with gcc49 on slc6

- `sim09-patches` branch: for patches to Gauss in Sim09 simulation workflows. Builds with gcc48 and gcc49 on slc6.


## Where to commit code to

- Bug fixes specific to a given processing should be committed to the corresponding `XXX-patches` branch.

- Any changes or fixes for Run 1 and Run 2 analysis (or re-reconstruction, re-stripping) should go to the `run2-patches` branch.
  They will then be propagated to `master` (if relevant also for Upgrade) by the applications managers. 
  Fixes also relevant to specific `XXX-patches` branches should be flagged as such, they will  be propagated by the applications managers. 

- Any changes specific to Upgrade should *only* got to `master`. When making a MR to `master`, please make it initially as WIP and add
a comment when it is ready to be tested. The release manager will then test it initially in one nightly slot and, if successful,
remove the WIP for general testing. 

In doubt, please get in touch before creating a MR.