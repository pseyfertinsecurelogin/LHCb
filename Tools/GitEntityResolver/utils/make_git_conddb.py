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
from hashlib import sha1

from CondDBUI import CondDB
from PyCool import cool

IOV_MIN = 0
IOV_MAX = 0x7fffffffffffffff


def fix_system_refs(path):

    patt = re.compile(r'(SYSTEM\s+)(?:"|\')([^"\']+)(:?"|\')')

    def make_repl(curr_dir):
        def _repl(match):
            if match.group(2).startswith('conddb:/'):
                newpath = match.group(2)[8:]
            else:
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
            f.write(data)


def checksum(data):
    s = sha1()
    s.update(data)
    return s.hexdigest()


def main():

    parser = OptionParser(usage='%prog [options] dbfile repo_dir')

    opts, (dbfile, repo_dir) = parser.parse_args()

    name = os.path.splitext(os.path.basename(dbfile))[0]
    if 'ONLINE' in name:
        name = 'ONLINE'
    cool_url = 'sqlite_file:{0}/{1}'.format(dbfile, name)

    db = CondDB(cool_url)
    root_node = db.getCOOLNode('/')

    # collect all tags
    tags_to_copy = db.getTagList('/')
    head_tag = [t for t in tags_to_copy if t.name == 'HEAD'][0]
    tags_to_copy = [t for t in tags_to_copy if t.name != 'HEAD']
    tags_to_copy.sort(key=lambda t:
                      root_node.tagInsertionTime(t.name)
                               .coralTimeStamp().total_nanoseconds())
    # tags_to_copy = tags_to_copy[-5:]
    tags_to_copy.append(head_tag)

    # FIXME: extract tag metadata from release notes

    if os.path.exists(repo_dir):
        print 'ERROR: directory %s already exist, please, remove it' % repo_dir
        sys.exit(1)

    single_version_nodes = set(n for n in db.getAllNodes()
                               if db.isSingleVersionFolder(n))
    print 'Initialize repository'
    check_output(['git', 'init', repo_dir])
    print 'processing %d tags' % len(tags_to_copy)
    for count, tag in enumerate(tags_to_copy, 1):
        tag = tag.name
        if len(os.listdir(repo_dir)) > 1:
            print 'cleaning up'
            check_output(['git', 'rm', '-r', '.'], cwd=repo_dir)

        print 'dumping tag %s (%d/%d)' % (tag, count, len(tags_to_copy))
        # FIXME: are single version nodes considered?
        nodes = db.findNodesWithTag(tag) + list(single_version_nodes)
        for node in nodes:
            # print node
            data = db.getPayloadList(node, IOV_MIN, IOV_MAX, None,
                                     tag if node not in single_version_nodes else 'HEAD')
            for payload, since, until, channel, insertion_time in data:
                for key in payload:
                    if not payload[key]:  # skip empty entries
                        continue
                    if key == 'data':
                        path = repo_dir + node
                    else:
                        path = (repo_dir +
                                '{0}/{1}@{2}'.format(os.path.dirname(node),
                                                     key,
                                                     os.path.basename(node)))
                    if channel or path.endswith('MagnetScale.xml'):
                        path = path + ':{0}'.format(channel)
                    if since == IOV_MIN and until == IOV_MAX:
                        if not os.path.exists(os.path.dirname(path)):
                            os.makedirs(os.path.dirname(path))
                        with open(path, 'w') as f:
                            f.write(payload[key])
                    else:
                        if not os.path.exists(path):
                            os.makedirs(path)
                        value_id = checksum(payload[key])[:10]
                        if not os.path.exists(os.path.join(path, value_id)):
                            with open(os.path.join(path, value_id), 'w') as f:
                                f.write(payload[key])
                        with open(os.path.join(path, 'IOVs'), 'a') as iovs:
                            iovs.write('%d %s\n' % (since, value_id))

            # cn = db.getCOOLNode(node)
            # channel_names = dict(cn.listChannelsWithNames())
            # data =
            # local_tag = cn.resolveTag(tag)
            # for obj in cn.browseObjects(0, 0x7fffffffffffffff,
            #                             ALL_CHANNELS, local_tag):
            #     ch = channel_names.get(obj.channelId(), obj.channelId())
            #     if ch not in channels:
            #         channels[ch] = {'IOVs': {},
            #                         'values': {}}
            #     data = channels[ch]
            #     key = checksum()
            #     data['IOVs'][ibj]

        print 'fixing paths'
        fix_system_refs(repo_dir)

        print 'updating repository'
        check_output(['git', 'add', '.'], cwd=repo_dir)
        if check_output(['git', 'status', '--porcelain'],
                        cwd=repo_dir).strip():
            check_output(['git', 'commit', '-m', tag], cwd=repo_dir)
        else:
            print 'no changes in %s' % tag
        check_output(['git', 'tag', tag], cwd=repo_dir)

    print 'compacting repository'
    check_output(['git', 'gc'], cwd=repo_dir)


if __name__ == '__main__':
    main()
