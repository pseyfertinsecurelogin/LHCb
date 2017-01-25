#!/bin/bash
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
git remote add origin ssh://git@gitlab.cern.ch:7999/clemenci/ONLINE.git

cat <<EOF
Now do:

  cd $(pwd) && git push --mirror origin

EOF
