#!/usr/bin/env python
'''
Small script to convert a DDDB COOL/SQLite database to a Git repository.
'''

import os
import sys
import re
import time
from optparse import OptionParser
from xml.etree import ElementTree as ET
from subprocess import check_output, STDOUT

def get_tags_from_notes(notes):
    '''
    Return the list of DDDB global tags in a release_notes.xml file, from the oldest
    to the newest.
    '''
    ns = {'lhcb': 'http://lhcb.cern.ch'}
    notes = ET.parse(notes)
    xpath = "./lhcb:global_tag/lhcb:partition/[lhcb:name='DDDB']/../lhcb:tag"
    return [el.text for el in notes.findall(xpath, ns)][-1::-1]


def fix_system_refs(path):

    patt = re.compile(r'(SYSTEM\s+)(?:"|\')([^"\']+)(:?"|\')')

    def make_repl(curr_dir):
        def _repl(match):
            fullpath = os.path.normpath(os.path.join(curr_dir, match.group(2)))
            newpath = os.path.relpath(fullpath, path)
            return '{0}"git:/{1}"'.format(match.group(1), newpath)
        return _repl

    for root, dirs, files in os.walk(path):
        if '.git' in dirs:
            dirs.remove('.git')

        repl = make_repl(root)
        for name in files:
            with open(os.path.join(root, name)) as f:
                data = f.read()
            newdata = patt.sub(repl, data)
            if newdata != data:
                with open(os.path.join(root, name), 'w') as f:
                    f.write(newdata)

    top = os.path.join(path, 'lhcb.xml')
    if os.path.exists(top):
        with open(top) as f:
            data = f.read()
        with open(top, 'w') as f:
            f.write(data.replace('"Conditions/MainCatalog.xml', '"conddb:/Conditions/MainCatalog.xml'))


def main():

    parser = OptionParser(usage='%prog [options] dbfile notes-xml repo_dir')

    opts, (dbfile, notes, repo_dir) = parser.parse_args()

    cool_url = 'sqlite_file:{0}/DDDB'.format(dbfile)

    tags_to_copy = get_tags_from_notes(notes)

    #print cool_url
    #print '\n'.join(tags_to_copy)

    timestamp = '%d' % (time.time() * 1E9)

    if os.path.exists(repo_dir):
        print 'ERROR: directory %s already exist, please, remove it' % repo_dir
        sys.exit(1)

    print 'Initialize repository'
    check_output(['git', 'init', repo_dir])
    print 'processing %d tags' % len(tags_to_copy)
    for count, tag in enumerate(tags_to_copy, 1):
        if len(os.listdir(repo_dir)) > 1:
            print 'cleaning up'
            check_output(['git', 'rm', '-r', '.'], cwd=repo_dir)

        print 'dumping tag %s (%d/%d)' % (tag, count, len(tags_to_copy))
        check_output(['dump_db_to_files.py', '-c', cool_url,
                      '-d', repo_dir, '-T', tag, '-t', timestamp],
                     stderr=STDOUT)

        if len(os.listdir(repo_dir)) <= 1:
            print 'WARNING: empty tag %s, ignored' % tag
            continue

        print 'fixing paths'
        fix_system_refs(repo_dir)

        print 'updating repository'
        check_output(['git', 'add', '.'], cwd=repo_dir)
        if check_output(['git', 'status', '--porcelain'], cwd=repo_dir).strip():
            check_output(['git', 'commit', '-m', tag], cwd=repo_dir)
        else:
            print 'no changes in %s' % tag
        check_output(['git', 'tag', tag], cwd=repo_dir)

    print 'compacting repository'
    check_output(['git', 'gc'], cwd=repo_dir)


if __name__ == '__main__':
    main()
