'''
Utils for payloads manipulations in Git CondDBs.
'''
import os
import re
from hashlib import sha1
from GitCondDB.IOVs import get_iovs


SYSTEM_RE = re.compile(r'(SYSTEM\s+)(?:"|\')([^"\']+)(:?"|\')')


def fix_system_refs(data, root, path):
    '''
    helper to fix explicit references to conddb: with references to git:
    '''
    curr_dir = os.path.dirname(path)

    def repl(match):
        'replacement function'
        if match.group(2).startswith('conddb:/'):
            newpath = match.group(2)[8:]
        else:
            newpath = os.path.normpath(os.path.join(curr_dir, match.group(2)))
            newpath = os.path.relpath(newpath, root)
        return '{0}"git:/{1}"'.format(match.group(1), newpath)
    return (SYSTEM_RE.sub(repl, data)
            .replace('"Conditions/MainCatalog.xml',
                     '"conddb:/Conditions/MainCatalog.xml'))


def fix_lines_ends(data):
    '''
    Change \\r\\n to \\n and remove spaces at end of lines.
    '''
    return '\n'.join(l.rstrip() for l in data.splitlines()) + '\n'


def checksum(data):
    'compute SHA1 checksum of a string'
    s = sha1()
    s.update(data)
    return s.hexdigest()


def payload_filename(data):
    '''
    return the conventional filename for a payload
    '''
    return checksum(data)[:10]

class DeferredPayload(object):
    '''
    Helper to retrieve payload data only when needed.
    '''
    def __init__(self, repository, payload, tag):
        '''
        Prepare the instance with references to the data to extract.
        '''
        self._args = (repository, payload, tag)
        self._data = None
    def __str__(self):
        '''
        Return the actual payload data loading it from the repository if not
        done already.
        '''
        if self._data is None:
            from GitCondDB.GitAccess import gitOpen
            self._data = gitOpen(*self._args).read()
        return self._data
    def __repr__(self):
        'Instance representation.'
        return '{}({}, {}, {})'.format(
            self.__class__.__name__, *self._args
        )

def get_payloads(repository, path, since, until, tag='HEAD', deferred=True):
    '''
    Return a generator over all payloads at path in repository for the given
    IOV (since, until).

    Each element is (payload_data, (since, until)).

    If deferred is True, payload_data is an instance of DeferredPayload, to be
    converted to str when the content is needed.
    '''
    for_iov = (since, until)
    if deferred:
        def payload_data(payload):
            'helper to prepare the payload_data (deferred)'
            return DeferredPayload(repository, payload, tag)
    else:
        from GitCondDB.GitAccess import gitOpen
        def payload_data(payload):
            'helper to prepare the payload_data (immediate)'
            return gitOpen(repository, payload, tag).read()
    for payload, (since, until) in get_iovs(repository, path, tag,
                                            for_iov=for_iov):
        yield payload_data(payload), (since, until)

def get_payload(repository, path, timestamp, tag='HEAD'):
    '''
    Given a path, extract the payload valid at the given timestamp and return
    it with its IOV, as (payload, (since, until)).
    '''
    return get_payloads(repository, path,
                        timestamp, timestamp, tag='HEAD').next()
