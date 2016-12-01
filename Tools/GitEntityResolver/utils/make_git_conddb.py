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
from datetime import datetime
from clean_iovs import process as clean_iovs

IOV_MIN = 0
IOV_MAX = 0x7fffffffffffffff


SYSTEM_RE = re.compile(r'(SYSTEM\s+)(?:"|\')([^"\']+)(:?"|\')')


def fix_system_refs(data, fullpath, path):
    curr_dir = os.path.dirname(path)

    def repl(match):
        if match.group(2).startswith('conddb:/'):
            newpath = match.group(2)[8:]
        else:
            fullpath = os.path.normpath(os.path.join(curr_dir,
                                                     match.group(2)))
            newpath = os.path.relpath(fullpath, path)
        return '{0}"git:/{1}"'.format(match.group(1), newpath)
    return (SYSTEM_RE.sub(repl, data)
            .replace('"Conditions/MainCatalog.xml',
                     '"conddb:/Conditions/MainCatalog.xml'))


def checksum(data):
    s = sha1()
    s.update(data)
    return s.hexdigest()


def extract_tags_infos(notes, partition):
    # Analyze relese notes
    ns = {'lhcb': 'http://lhcb.cern.ch'}
    notes = ET.parse(notes)

    committers = dict((el.find('lhcb:name', ns).text,
                       '{0} <{1}>'.format(el.find('lhcb:name', ns).text,
                                          el.find('lhcb:email', ns).text
                                            .replace('__AT__', '@')))
                      for el in notes.findall('./lhcb:maintainer', ns))

    tags_xpath = ("./lhcb:global_tag/lhcb:partition/"
                  "[lhcb:name='{0}']/..").format(partition)
    return dict((el.find('lhcb:tag', ns).text,
                 (
                    # we use a dummy time
                    el.find('lhcb:date', ns).text + 'T00:00:00',
                    committers.get(el.find('lhcb:contributor', ns).text),
                    el.find(("lhcb:partition/[lhcb:name='{0}']/"
                             "lhcb:base").format(partition), ns).text,
                 ))
                for el in notes.findall(tags_xpath, ns))

def fix_lines_ends(data):
    '''
    Change \\r\\n to \\n and remove spaces at end of lines.
    '''
    return '\n'.join(l.rstrip() for l in data.splitlines()) + '\n'

def main():
    start_time = datetime.now()

    parser = OptionParser(usage='%prog [options] dbfile notes-xml repo_dir')

    parser.add_option('--tag-prefix',
                      help='prefix to use for the generated tags')

    parser.set_defaults(tag_prefix='')

    opts, (dbfile, notes, repo_dir) = parser.parse_args()

    name = os.path.splitext(os.path.basename(dbfile))[0]
    if 'ONLINE' in name:
        name = 'ONLINE'
    cool_url = 'sqlite_file:{0}/{1}'.format(dbfile, name)

    print 'parsing release notes...'
    tags_infos = extract_tags_infos(notes, name)

    print 'loading CondDBUI...'
    from CondDBUI import CondDB
    db = CondDB(cool_url)
    root_node = db.getCOOLNode('/')

    def tag_time(t):
        t = int(root_node.tagInsertionTime(t)
                .coralTimeStamp().total_nanoseconds() * 1.e-9)
        return str(datetime.fromtimestamp(t))

    # collect all tags
    print 'retrieving tags list...'
    tags_to_copy = db.getTagList('/')
    print 'found', len(tags_to_copy), 'tags'
    head_tag = [t for t in tags_to_copy if t.name == 'HEAD'][0]
    tags_to_copy = [t for t in tags_to_copy if t.name in tags_infos]
    tags_to_copy.sort(key=lambda t:
                      root_node.tagInsertionTime(t.name)
                               .coralTimeStamp().total_nanoseconds())
    # tags_to_copy = tags_to_copy[-5:]
    tags_to_copy.append(head_tag)

    if os.path.exists(repo_dir):
        print 'retrieve existing tags in', repo_dir
        existing_tags = set(check_output(['git', 'tag'], cwd=repo_dir).splitlines())
        tags_to_copy = [t for t in tags_to_copy if (opts.tag_prefix + t.name) not in existing_tags]
    else:
        print 'initialize repository'
        existing_tags = set()
        check_output(['git', 'init', repo_dir])

    single_version_nodes = set(n for n in db.getAllNodes()
                               if db.isSingleVersionFolder(n))
    tip_tags = set()  # tags not overridden (good for branches)
    print 'processing %d tags' % len(tags_to_copy)
    for count, tag in enumerate(tags_to_copy, 1):
        # print datetime.now()
        tag = tag.name
        print 80 * '-'
        print 'processing tag %s (%d/%d)' % (tag, count, len(tags_to_copy))

        if tag in tags_infos:
            date, author, base = tags_infos[tag]
            base = opts.tag_prefix + base
        else:
            # default author
            author = 'Marco Clemencic <marco.clemencic@cern.ch>'
            date = tag_time(tag)
            base = 'HEAD'

        if base in existing_tags:
            print 'get base tag %s' % (base,)
            check_output(['git', 'checkout', base], cwd=repo_dir)
        tip_tags.add(opts.tag_prefix + tag)
        if base in tip_tags:
            tip_tags.remove(base)

        if len(os.listdir(repo_dir)) > 1:
            print 'cleaning up'
            check_output(['git', 'rm', '-r', '.'], cwd=repo_dir)

        print 'dumping content of %s' % (tag,)
        if tag != 'HEAD':
            nodes = db.findNodesWithTag(tag) + list(single_version_nodes)
        else:
            nodes = db.getAllNodes()
        for node in nodes:
            # print node
            data = db.getPayloadList(node, IOV_MIN, IOV_MAX, None,
                                     tag if node not in
                                     single_version_nodes else 'HEAD')
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
                    if channel or (
                          os.path.basename(path) in ('MagnetScale.xml',
                                                     'RichHpdSurface.xml',
                                                     'HpdLogVol.xml')
                          ):
                        path = path + ':{0}'.format(channel)
                    value = fix_system_refs(payload[key], path, node)
                    value = fix_lines_ends(value)
                    if since == IOV_MIN and until == IOV_MAX:
                        if not os.path.exists(os.path.dirname(path)):
                            os.makedirs(os.path.dirname(path))
                        with open(path, 'w') as f:
                            f.write(value)
                    else:
                        if not os.path.exists(path):
                            os.makedirs(path)
                        value_id = checksum(value)[:10]
                        if not os.path.exists(os.path.join(path, value_id)):
                            with open(os.path.join(path, value_id), 'w') as f:
                                f.write(value)
                        with open(os.path.join(path, 'IOVs'), 'a') as iovs:
                            iovs.write('%d %s\n' % (since, value_id))

        clean_iovs(repo_dir)
        print 'updating repository'
        check_output(['git', 'add', '.'], cwd=repo_dir)
        if check_output(['git', 'status', '--porcelain'],
                        cwd=repo_dir).strip():
            os.environ['GIT_COMMITTER_DATE'] = date
            check_output(['git', 'commit', '-m', tag,
                          '--author', author, '--date', date],
                         cwd=repo_dir)
        else:
            print 'no changes in %s' % tag
        if tag != 'HEAD':
            check_output(['git', 'tag', opts.tag_prefix + tag], cwd=repo_dir)
            existing_tags.add(opts.tag_prefix + tag)

    print 80 * '-'
    print 'creating branches for tip tags'
    for i, tag in enumerate(sorted(tip_tags)):
        if tag.endswith('HEAD'):
            continue
        print 'branch-%d -> %s' % (i, tag)
        check_output(['git', 'branch', 'branch-%d' % i, tag], cwd=repo_dir)
    print 'compacting repository'
    check_output(['git', 'gc'], cwd=repo_dir)

    print 'completed in', (datetime.now() - start_time)

if __name__ == '__main__':
    main()
