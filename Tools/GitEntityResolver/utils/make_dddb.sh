#!/bin/bash
script_dir=$(cd $(dirname $0) ; pwd)
export PATH=${script_dir}:$PATH

name=DDDB

make_git_conddb.py ${SQLITEDBPATH}/${name}.db ${SQLITEDBPATH}/../doc/release_notes.xml ${name}

(
  cd ${name}
  git checkout dddb-20150724
  git branch -f master dddb-20150724
  git tag upgrade/head-20120316 head-20120316
  git checkout upgrade/head-20120316
  git branch | grep branch- | xargs -r git branch -D
)

make_git_conddb.py --tag-prefix=upgrade/ ${SQLITEUPGRADEDBPATH}/${name}.db ${SQLITEUPGRADEDBPATH}/../doc/release_notes.xml ${name}

(
  cd ${name}
  git checkout upgrade/dddb-20161201
  git branch -f upgrade upgrade/dddb-20161201
  git branch | grep branch- | xargs -r git branch -D
)

cd ${name}
git remote add origin ssh://git@gitlab.cern.ch:7999/lhcb-conddb/${name}.git

git checkout master

cat <<EOF
Now do:

  ( cd $(pwd) && git push --mirror origin )

EOF
