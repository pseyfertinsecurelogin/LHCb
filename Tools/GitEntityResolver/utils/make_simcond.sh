#!/bin/bash
###############################################################################
# (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
script_dir=$(cd $(dirname $0) ; pwd)
export PATH=${script_dir}:$PATH

name=SIMCOND

make_git_conddb.py --no-head ${SQLITEDBPATH}/${name}.db ${SQLITEDBPATH}/../doc/release_notes.xml ${name}

(
  cd ${name}
  git tag upgrade/sim-20120126-vc-md100 sim-20120126-vc-md100
  git tag upgrade/sim-20111111-vc-md100 sim-20111111-vc-md100
  git checkout sim-20111111-vc-md100
  git branch | grep branch- | xargs -r git branch -D
  git branch | grep dt- | xargs -r git branch -D
)

make_git_conddb.py --no-head --tag-prefix=upgrade/ ${SQLITEUPGRADEDBPATH}/${name}.db ${SQLITEUPGRADEDBPATH}/../doc/release_notes.xml ${name}

(
  cd ${name}
  git checkout upgrade/sim-20160927-vc-md100
  git branch | grep branch- | xargs -r git branch -D
  git branch | grep dt- | xargs -r git branch -D
)

cd ${name}
git remote add origin ssh://git@gitlab.cern.ch:7999/lhcb-conddb/${name}.git

git branch -f master sim-20161124-2-vc-md100
git branch -f vc-md100 sim-20161124-2-vc-md100
git branch -f vc-mu100 sim-20161124-2-vc-mu100
git branch -f upgrade/master upgrade/sim-20170210-vc-md100
git branch -f upgrade/vc-mu100 upgrade/sim-20170210-vc-mu100
git branch -f upgrade/vc-md100 upgrade/sim-20170210-vc-md100

# fix a duplication
git tag -f upgrade/sim-20161111-vc-mu100 upg-vc-mu100

git checkout master

cat <<EOF
Now do:

  ( cd $(pwd) && git push --mirror origin )

EOF
