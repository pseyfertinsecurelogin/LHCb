'''
Utils for payloads manipulations in Git CondDBs.
'''
import os
import re
from hashlib import sha1


SYSTEM_RE = re.compile(r'(SYSTEM\s+)(?:"|\')([^"\']+)(:?"|\')')


def fix_system_refs(data, fullpath, path):
    '''
    helper to fix explicit references to conddb: with references to git:
    '''
    curr_dir = os.path.dirname(path)

    def repl(match):
        'replacement function'
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
