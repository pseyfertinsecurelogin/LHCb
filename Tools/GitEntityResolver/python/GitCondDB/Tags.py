'''
Utils to work on Git CondDB tags metadata.
'''
import os
import re
import logging
from collections import namedtuple

from .GitAccess import _git

TagInfo = namedtuple('TagInfo', ['author', 'date', 'info'])

def _parse_metadata(s):
    '''
    Parse and return the (first) YAML section (surrounded by '---' lines) in a
    multiline string.
    '''
    from itertools import dropwhile, takewhile, islice
    from yaml import load
    skipone = lambda iterable: islice(iterable, 1, None)
    not_marker = lambda l: l.rstrip() != '---'
    data = '\n'.join(takewhile(not_marker,
                               skipone(dropwhile(not_marker,
                                                 s.splitlines()))))
    if not data:
        return {}
    return load(data)

def getTagsInfos(repository):
    '''
    Return tag informations (name -> (author, date, metadata)) for the tags
    in the given repository.

    The metadata is extracted from a YAML section (surrounded by '---' lines) in
    the tag message.
    '''
    from collections import OrderedDict
    from dateutil.parser import parse as date_parse
    cmd = ['for-each-ref', '--python', '--sort=-creatordate',
           ('--format=(%(objecttype),%(refname:short),%(taggername)" "'
            '%(taggeremail),%(taggerdate:iso8601),'
           '%(authorname)" "%(authoremail),%(authordate:iso8601),%(contents)),'),
           'refs/tags/']

    # Using the named tuple to make sure we always match correctly the output
    # of the command.
    GitResult = namedtuple('GitResult',
                           ['objecttype', 'tag',
                            'taggerauthor', 'taggerdate',
                            'commitauthor', 'commitdate', 'content'])

    def _checkTagInfo(result):
        ''' Check the tags values and return the right ones '''
        if result.objecttype == "tag":
            return (result.taggerauthor, date_parse(result.taggerdate),
                    _parse_metadata(result.content))
        elif result.objecttype == "commit":
            return (result.commitauthor, date_parse(result.commitdate),
                    _parse_metadata(result.content))
        raise Exception("objectype %s is not handled" % result.objectype)
    
    return OrderedDict((g.tag, TagInfo(*_checkTagInfo(g)))
                       for g in 
                       [ GitResult(*gitresult)
                         for gitresult in
                         eval('[{}]'.format(_git(repository, cmd)
                                            .replace('\r', '\\r'))) ])
