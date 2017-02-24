#!/bin/bash
script_dir=$(cd $(dirname $0) ; pwd)
export PATH=${script_dir}:$PATH

name=LHCBCOND

make_git_conddb.py ${SQLITEDBPATH}/${name}.db ${SQLITEDBPATH}/../doc/release_notes.xml ${name}

cd ${name}
git remote add origin ssh://git@gitlab.cern.ch:7999/lhcb-conddb/${name}.git

git checkout cond-20170120-1
git branch | grep branch- | xargs -r git branch -D

for bt in dt-2008:head-20090402 dt-2009:head-20110622-Reco10 \
          dt-2010:cond-20150527 dt-2011:cond-20150409 \
          dt-2012:cond-20150409-1 dt-2013:cond-20150409-1 \
          dt-2015:cond-20170120 dt-2016:cond-20170120-1 \
          dt-2017:cond-20170120-1 \
          dt-DC06:DC06-20081002 \
          dt-Reco09:head-20110512-Reco09 \
          master:cond-20170120-1 ; do
    git branch -f ${bt/:/ }
done

git checkout master

cat <<EOF
Now do:

  ( cd $(pwd) && git push --mirror origin )

EOF
