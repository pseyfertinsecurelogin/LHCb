from xml.parsers.xmlproc import xmlval
from xml.parsers.xmlproc.xmlapp import PubIdResolver
import os

#================================================================================
class GODSrcParser (xmlval.Application):
#--------------------------------------------------------------------------------
    def __init__(self, filename):
        self.filename = filename
        #self.elemstack = []
        self.data = {}
        self.locator = 0
        self.cdata = ''
#--------------------------------------------------------------------------------
    def handle_doctype(self,root,pubID,sysID):
        pass
#--------------------------------------------------------------------------------
    def doc_start(self):
        print 'Parsing Source File ', self.filename,
#--------------------------------------------------------------------------------
    def doc_end(self):
        print '- Done'
#--------------------------------------------------------------------------------
    def handle_comment(self,data):
        pass
#--------------------------------------------------------------------------------
    def handle_data(self,data,start,end):
        self.cdata += data[start:end]
#--------------------------------------------------------------------------------
    def handle_start_tag(self,name,attr):
        currdic = self.data
        for stackelem in self.locator.get_elem_stack():
            currdic = currdic[stackelem][-1]
        if currdic.has_key(name):
            currdic[name].append({'attrs':attr})
        else:
            currdic[name] = [{'attrs':attr}]
#--------------------------------------------------------------------------------
    def handle_end_tag(self,name):
        if len(self.cdata):
            currdic = self.data
            for stackelem in self.locator.get_elem_stack():
                currdic = currdic[stackelem][-1]
            currdic[name][-1]['cont'] = self.cdata
            self.cdata = ''
#--------------------------------------------------------------------------------
    def handle_ignorable_data(self, data, start, end):
        pass
#--------------------------------------------------------------------------------
    def handle_pi(self, target, data):
        pass
#--------------------------------------------------------------------------------
    def set_entity_info(self,xmlver,enc,sddecl):
        pass
#--------------------------------------------------------------------------------
    def set_locator(self, locator):
        self.locator = locator

#================================================================================
class GODDbParser (xmlval.Application):
#--------------------------------------------------------------------------------
    def __init__(self, filename):
        self.filename = filename
        self.data = {}
#--------------------------------------------------------------------------------
    def doc_start(self):
        print 'Parsing Class Database ', self.filename,
#--------------------------------------------------------------------------------
    def doc_end(self):
        print '- Done'
#--------------------------------------------------------------------------------
    def handle_start_tag(self,name,attr):
        if name == 'class':
            self.data[attr['name']] = attr['file']

class GODPubIdResolver(PubIdResolver):
    """
    Implementation of PubIdResolver to be able to locate the DTD in a fixed
    directory instead of the same directory of the XML file.
    """
    def __init__(self, dtdpath=None):
        """
        Initialize the instance with the full path to the DTD file (gdd.dtd).
        """
        self.dtdpath = dtdpath
    def resolve_doctype_pubid(self, pubid, sysid):
        """
        Return the fixed path to the file 'gdd.dtd'.
        """
        if self.dtdpath:
            if pubid == "gdd.dtd" or sysid == "gdd.dtd":
                return self.dtdpath
        return sysid

#================================================================================
class xparser:
#--------------------------------------------------------------------------------
    def __init__(self, dtdPath=None):
        self.parser = xmlval.XMLValidator()
        self.dtdPath = dtdPath
#--------------------------------------------------------------------------------
    def parseSource(self,sourcefile):
        parser = xmlval.XMLValidator()
        godSrcParser = GODSrcParser(sourcefile)
        parser.set_application(godSrcParser)
        parser.set_pubid_resolver(GODPubIdResolver(self.dtdPath))
        parser.parse_resource(sourcefile)
        return godSrcParser.data['gdd'][0]
#--------------------------------------------------------------------------------
    def parseDB(self,dbfile):
        parser = xmlval.XMLValidator()
        godDbParser = GODDbParser(dbfile)
        parser.set_application(godDbParser)
        parser.set_pubid_resolver(GODPubIdResolver(self.dtdPath))
        parser.parse_resource(dbfile)
        return godDbParser.data
