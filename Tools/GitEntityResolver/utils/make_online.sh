#!/bin/bash
###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
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

names=$(ls ${SQLITEDBPATH} | grep ONLINE | sed 's/.db//')
last=$(echo $names | awk '{print $NF}')
first=$(echo $names | awk '{print $1}')

for name in $names ; do

if [ "$name" = "$last" ] ; then
  clean_iovs_opt=--clean-iovs
else
  clean_iovs_opt=--no-clean-iovs
fi
echo "======= $name ==================================================="
make_git_conddb.py $clean_iovs_opt --append --always-iovs ${SQLITEDBPATH}/${name}.db ${SQLITEDBPATH}/../doc/release_notes.xml ONLINE

# merge all commits into one
if [ "$name" != "$first" ] ; then
  echo "-> squash with previous commit"
  (
  cd ONLINE
  git reset --quiet --soft HEAD~
  git commit --quiet --amend -C HEAD
  )
else
  (
  cd ONLINE
  git commit --quiet --amend -m "initial version"
  )
fi

done

cd ONLINE

echo "generate the list of valid runs"
grep -r RunNumber Conditions/Online/LHCb/RunStamp.xml | sed -n 's/.*>\([0-9]\+\)<.*/\1/p' | sort -n > Conditions/Online/valid_runs.txt.tmp
(seq 0 $(head -1 Conditions/Online/valid_runs.txt.tmp) ; cat Conditions/Online/valid_runs.txt.tmp) | uniq > Conditions/Online/valid_runs.txt
rm -f Conditions/Online/valid_runs.txt.tmp
git add Conditions/Online/valid_runs.txt
git rm -r Conditions/Online/LHCb/RunStamp.xml
# get the time of the latest Tick (heart beat) to use it as commit time
export GIT_COMMITTER_DATE=$(python -c "from CondDBUI import CondDB; print int(CondDB('sqlite_file:${SQLITEDBPATH}/${name}.db/ONLINE').getPayloadList('/Conditions/Online/LHCb/Tick', 9223372036854775806L, 9223372036854775807L)[0][1]/1e9)")
git commit --quiet --amend --date ${GIT_COMMITTER_DATE} -C HEAD

git remote add origin ssh://git@gitlab.cern.ch:7999/lhcb-conddb/ONLINE.git

cat <<EOF
Now do:

  cd $(pwd) && git push --mirror origin

EOF
