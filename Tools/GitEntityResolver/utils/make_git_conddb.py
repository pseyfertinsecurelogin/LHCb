#!/usr/bin/env python
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
'''
Small script to convert a DDDB COOL/SQLite database to a Git repository.
'''

import os
import logging
import re
import yaml
from optparse import OptionParser
from xml.etree import ElementTree as ET
from subprocess import check_output
from datetime import datetime
from GitCondDB.IOVs import clean_iovs, IOV_MIN, IOV_MAX
from GitCondDB.Payload import *


def extract_tags_infos(notes, partition):
    '''extract tags informations from a release_notes.xml file

    Return a dictionary {'tagname': (datestring, author, basetag,
                                     datatype_branches)}'''
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

    class DTCollector(object):
        '''
        helper to collect data type for the tags

        only data types not yet assigned are taken into account for a given
        tag element (tags are sorted from most recent to oldest)
        '''
        def __init__(self):
            self.assigned_datatypes = set()
        def __call__(self, el):
            branches = []
            for dt in [dt.text for dt in el.findall('lhcb:type', ns)]:
                if dt not in self.assigned_datatypes:
                    self.assigned_datatypes.add(dt)
                    branches.append('dt-' + dt)
            return branches
    datatype_branches = DTCollector()

    return dict((el.find('lhcb:tag', ns).text,
                 (
                     # we use a dummy time
                     el.find('lhcb:date', ns).text + 'T00:00:00',
                     committers.get(el.find('lhcb:contributor', ns).text),
                     el.find(("lhcb:partition/[lhcb:name='{0}']/"
                              "lhcb:base").format(partition), ns).text,
                     datatype_branches(el),
                     [dt.text for dt in el.findall('lhcb:type', ns)]
                 ))
                for el in notes.findall(tags_xpath, ns))


def main():
    'script logic'
    start_time = datetime.now()

    parser = OptionParser(usage='%prog [options] dbfile [notes-xml] repo_dir')

    parser.add_option('--tag-prefix',
                      help='prefix to use for the generated tags')
    parser.add_option('--clean-iovs',
                      action='store_true', dest='clean_iovs',
                      help='partition IOVs after the dump (default)')
    parser.add_option('--no-clean-iovs',
                      action='store_false', dest='clean_iovs',
                      help='do not partition IOVs after the dump')
    parser.add_option('--append',
                      action='store_true',
                      help='do not clean the destination dir before dump')
    parser.add_option('--always-iovs',
                      action='store_true',
                      help='create IOVs file for sigle IOV in folder')
    parser.add_option('--partition-payloads',
                      action='store_true',
                      help='store payloads in separate folders (by first '
                           '2 chars of the name)')
    parser.add_option('--no-head',
                      action='store_false', dest='do_head',
                      help='do not process HEAD tag')
    parser.add_option('--no-force-branches',
                      action='store_true',
                      help='do not force creation of datatype branches')
    parser.add_option('-t', '--tag',
                      action='append', dest='tags',
                      help='tags to extract if notes-xml is not provided')
    parser.add_option('-T', '--head-tag-name')
    parser.add_option('--name',
                      help='name of the COOL database in the SQLite file '
                      '(default: the file name without extension)')

    parser.set_defaults(tag_prefix='',
                        clean_iovs=True,
                        append=False,
                        always_iovs=False,
                        partition_payloads=False,
                        do_head=True,
                        tags=[],
                        head_tag_name='HEAD')

    opts, args = parser.parse_args()

    logging.basicConfig(level=logging.DEBUG)

    if len(args) == 3:
        (dbfile, notes, repo_dir) = args
        if opts.tags:
            parser.error('tags cannot be specified in conjunction to a notes '
                         'xml')
    elif len(args) == 2:
        (dbfile, repo_dir) = args
        notes = None
        if not opts.tags and not opts.do_head:
            parser.error('no notes-xml nor list of tags provided')
        if 'HEAD' in opts.tags:  # HEAD is treated in a special way
            opts.do_head = True
            opts.tags.remove('HEAD')
    else:
        parser.error('wrong number of arguments')

    name = opts.name or os.path.splitext(os.path.basename(dbfile))[0]
    if 'ONLINE' in name:
        name = 'ONLINE'
    cool_url = 'sqlite_file:{0}/{1}'.format(dbfile, name)

    if notes is None:
        tags_infos = dict((opts.tags[i], ('1970-01-01T00:00:00',
                                          'nobody <nobody@no.where>',
                                          '' if not i else opts.tags[i - 1],
                                          [],
                                          []))
                          for i in range(len(opts.tags)))
    else:
        print 'parsing release notes...'
        tags_infos = extract_tags_infos(notes, name)

    print 'loading CondDBUI...'
    from CondDBUI import CondDB
    db = CondDB(cool_url)
    root_node = db.getCOOLNode('/')

    def tag_time(t):
        'convert tag instertion time to ISO string'
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
    if opts.do_head:
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
            date, author, base, branches, datatypes = tags_infos[tag]
            base = opts.tag_prefix + base
        else:
            # default author
            author = 'Marco Clemencic <marco.clemencic@cern.ch>'
            date = tag_time(tag)
            base = 'HEAD'
            branches = []
            datatypes = []

        if base in existing_tags:
            print 'get base tag %s' % (base,)
            check_output(['git', 'checkout', base], cwd=repo_dir)
        tip_tags.add(opts.tag_prefix + tag)
        if base in tip_tags:
            tip_tags.remove(base)

        if opts.append:
            # flatten IOVs before appending
            clean_iovs(repo_dir, partition=False)
        elif len(os.listdir(repo_dir)) > 1:
            # if not appending and the dest dir is not empty, clean it up
            print 'cleaning up'
            check_output(['git', 'rm', '-r', '.'], cwd=repo_dir)


        print 'dumping content of %s' % (tag,)
        if tag != 'HEAD':
            nodes = db.findNodesWithTag(tag) + list(single_version_nodes)
        else:
            nodes = db.getAllNodes()
        for node in nodes:
            if os.path.basename(node) in ('InstLumi', 'Tick'):
                continue  # ignore this condition object
            # print node
            data = db.getPayloadList(node, IOV_MIN, IOV_MAX, None,
                                     tag if node not in
                                     single_version_nodes else 'HEAD')
            for payload, since, until, channel, _ in data:
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
                    if (channel or
                            os.path.basename(path) in ('MagnetScale.xml',
                                                       'RichHpdSurface.xml',
                                                       'HpdLogVol.xml')):
                        path = path + ':{0}'.format(channel)
                    value = fix_system_refs(payload[key], path, node)
                    value = fix_lines_ends(value)
                    if (since == IOV_MIN and until == IOV_MAX) and not opts.always_iovs:
                        if not os.path.exists(os.path.dirname(path)):
                            os.makedirs(os.path.dirname(path))
                        with open(path, 'w') as f:
                            f.write(value)
                    else:
                        if not os.path.exists(path):
                            os.makedirs(path)
                        value_id = payload_filename(value)
                        if opts.partition_payloads:
                            value_id = os.path.join(value_id[:2], value_id)
                        if not os.path.exists(os.path.join(path, value_id)):
                            if opts.partition_payloads:
                                if not os.path.exists(os.path.join(path, value_id[:2])):
                                    os.makedirs(os.path.join(path, value_id[:2]))
                            with open(os.path.join(path, value_id), 'w') as f:
                                f.write(value)
                        with open(os.path.join(path, 'IOVs'), 'a') as iovs:
                            iovs.write('%d %s\n' % (since, value_id))

        if opts.clean_iovs:
            clean_iovs(repo_dir)
        print 'updating repository'
        check_output(['git', 'add', '.'], cwd=repo_dir)
        if check_output(['git', 'status', '--porcelain'],
                        cwd=repo_dir).strip():
            os.environ['GIT_COMMITTER_DATE'] = date
            check_output(['git', 'commit', '-m',
                          opts.head_tag_name if tag == 'HEAD' else tag,
                          '--author', author, '--date', date],
                         cwd=repo_dir)
        else:
            print 'no changes in %s' % tag
        if tag != 'HEAD':
            env = dict(os.environ)
            env['GIT_COMMITTER_DATE'] = date
            env['GIT_COMMITTER_NAME'], env['GIT_COMMITTER_EMAIL'] = \
                re.match(r'(.*) <(.*)>', author).groups()
            check_output(['git', 'tag', '-m',
                          '---\n{}---\n'
                          .format(yaml.dump({
                              'datatypes': [int(dt) if dt.isdigit() else str(dt)
                                            for dt in datatypes]})),
                          opts.tag_prefix + tag],
                         cwd=repo_dir, env=env)
            existing_tags.add(opts.tag_prefix + tag)
            for branch in branches:
                print 'creating branch', branch, '(from %s)' % (opts.tag_prefix + tag)
                cmd = ['git', 'branch', '-f', branch, opts.tag_prefix + tag]
                if opts.no_force_branches:
                    cmd.remove('-f')
                check_output(cmd, cwd=repo_dir)

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
