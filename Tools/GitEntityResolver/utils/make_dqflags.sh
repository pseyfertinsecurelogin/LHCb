#!/bin/bash
script_dir=$(cd $(dirname $0) ; pwd)
export PATH=${script_dir}:$PATH

name=DQFLAGS

make_git_conddb.py ${SQLITEDBPATH}/${name}.db ${SQLITEDBPATH}/../doc/release_notes.xml ${name}

cd ${name}
git remote add origin ssh://git@gitlab.cern.ch:7999/clemenci/${name}.git

git checkout dq-20161007
git branch -f master dq-20161007
git checkout master
git branch | grep branch- | xargs -r git branch -D

cat <<EOF
Now do:

  ( cd $(pwd) && git push --mirror origin )

EOF
