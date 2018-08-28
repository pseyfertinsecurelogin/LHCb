# General information

The project contains several active and maintained branches, such as the `201X-patches` branches.
They are all protected, meaning that code cannot be pushed into them directly but only through merge requests (MRs).
This helps with the validation of code prior to making it available in the official branches for future releases.

## Where to commit code to

- Bug fixes for 2018 data-taking should be committed as MRs to `2018-patches`. They will then be propagated to the
`run2-patches` branch  (and master if appropriate) by the applications managers.

- Any other changes for Run 1 and Run 2 analysis (or re-reconstruction, re-stripping) should go to the `run2-patches` branch.
  They will then be propagated to `master` (if relevant also for Upgrade) by the applications managers.

- Any changes specific to Upgrade should *only* got to `master`. When making a MR to `master`, please make it initially as WIP and add
a comment when it is ready to be tested. The release manager will then tet it initially in one nightly slot and, if successful,
remove the WIP for general testing 

-  Things may be different for bug fixes to old and/or specific versions of applications, in which case it is probably best to discuss 
unless you know exactly what you are doing.

In doubt, please get in touch before creating a MR.