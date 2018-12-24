###############################################################################
# (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
## @file MainWindow.py
#  Class for the main window.
#
#  Definition of the browser MainWindow class.

from PyQt5.Qt import (Qt, QObject, pyqtSlot, pyqtSignal, QDateTime, QSettings,
                      QSize, QPoint, PYQT_VERSION_STR, qVersion, QApplication,
                      QMainWindow, QMessageBox, QHeaderView, QLabel, QIcon,
                      QMenu, QAction, QTextDocument, QTextCursor, QStyle,
                      QStyleFactory)

from Ui_MainWindow import Ui_MainWindow

from CondDBUI import openDB

from Models import *
from Dialogs import *

from Utils import parentpath
from CondDBUI.Browser.Models import tagsGlobalCache

import os


## Class containing the logic of the application.
class MainWindow(QMainWindow, Ui_MainWindow):
    # Signals
    changedPath = pyqtSignal(str)
    changedPathChannel = pyqtSignal(str, 'unsigned int')
    databaseOpen = pyqtSignal(bool)
    databaseOpenReadOnly = pyqtSignal(bool)
    openedDB = pyqtSignal(object)

    ## Constructor.
    #  Initialises the base class and defines some internal structures.
    def __init__(self, parent=None, flags=Qt.Widget):
        # Base class constructor.
        super(MainWindow, self).__init__(parent, flags)
        # Application and organization names
        app = QApplication.instance()
        self.appName = str(app.applicationName())
        self.appOrg = str(app.organizationName())
        # Preconfigured databases
        self.defaultDatabases = {}  # action
        # Icons for the model (property)
        self._icons = None
        # Current selected (path, channel) in the database
        self._path = (None, None)
        # The database
        self.db = None
        # Current connection string. Needed because the copy in
        # self.db has the variables expanded.
        self._connectionString = None
        # Maximum number of entries in the list of recent databases
        self.maxRecentEntries = 10
        # External editor, to be passed to the add condition dialog
        self._externalEditor = "emacs"  # set in readSettings
        # Prepare the GUI.
        self.setupUi(self)
        # --- Part of the initialization that require the GUI objects. ---
        # Window title
        self.setWindowTitle(self.appName)
        # Connect the models
        self.models = {
            "tree": CondDBStructureModel(),
            "nodes": CondDBNodesListModel(),
            "tags": CondDBTagsListModel(),
            "iovs": CondDBIoVModel(),
            "fields": CondDBPayloadFieldModel(),
        }
        setModelsIcons(self.icons)
        self.hierarchyTreeView.setModel(self.models["tree"])
        self.pathComboBox.setModel(self.models["nodes"])
        self.tagComboBox.setModel(self.models["tags"])
        self.iovView.setModel(self.models["iovs"])
        self.fieldsView.setModel(self.models["fields"])
        for m in [self.models[n] for n in ["tree", "nodes", "iovs", "fields"]]:
            self.openedDB.connect(m.connectDB)
        self.openedDB.connect(tagsGlobalCache.setDB)

        self.hierarchyTreeView.selectionModel().currentChanged.connect(
            self.selectedItem)

        # special settings for the tags model
        tagsmodel = self.models["tags"]
        self.changedPath.connect(tagsmodel.setPath)
        self.hideAutoCheckBox.stateChanged.connect(tagsmodel.setHideAutoTags)
        tagsmodel.setViewEnabled.connect(self.tagComboBox.setEnabled)
        tagsmodel.setViewEnabled.connect(self.hideAutoCheckBox.setEnabled)
        tagsmodel.setHideAutoTags(self.hideAutoCheckBox.checkState())
        # connection for the iovModel
        iovsmodel = self.models["iovs"]
        self.changedPathChannel.connect(iovsmodel.setPathChannel)
        self.sinceFilterWidget.validityKeyChange.connect(iovsmodel.setSince)
        self.untilFilterWidget.validityKeyChange.connect(iovsmodel.setUntil)
        self.tagComboBox.currentIndexChanged[str].connect(iovsmodel.setTag)
        iovsmodel.setViewEnabled.connect(self.iovView.setEnabled)
        self.iovView.setEnabled(False)
        hh = self.iovView.horizontalHeader()
        hh.setSectionResizeMode(QHeaderView.ResizeToContents)
        self.iovView.selectionModel().currentChanged.connect(
            iovsmodel.selectionChanged)
        iovsmodel.setCurrentIndex.connect(
            self.iovView.selectionModel().setCurrentIndex)
        self.iovUTCCheckBox.stateChanged.connect(iovsmodel.setShowUTC)
        iovsmodel.setShowUTC(self.iovUTCCheckBox.checkState())
        # Use a consistent DisplayFormat
        iovsmodel.setDisplayFormat(self.sinceFilterWidget.displayFormat())

        # connection for the fields model
        fieldsmodel = self.models["fields"]
        self.changedPath.connect(fieldsmodel.setPath)
        #fieldsmodel.setViewEnabled.connect(self.fieldsView.setEnabled)
        #self.fieldsView.setEnabled(False)
        fieldsmodel.setViewEnabled.connect(self.payloadGroupBox.setVisible)
        self.payloadGroupBox.setVisible(False)

        self.fieldsView.selectionModel().currentChanged.connect(
            fieldsmodel.selectionChanged)
        fieldsmodel.setCurrentIndex.connect(
            self.fieldsView.selectionModel().setCurrentIndex)

        # Filter panel
        # Default startup values for the IOV filter.
        self.sinceFilterWidget.setMaxEnabled(False)
        self.sinceFilterWidget.setDateTime(
            QDateTime.currentDateTime().addMonths(-1))
        #iovsmodel.setSince(self.sinceFilterWidget.toValidityKey())
        self.untilFilterWidget.setMaxChecked(True)
        #iovsmodel.setUntil(self.untilFilterWidget.toValidityKey())

        # When created, we have to ensure that everything is grayed out.
        self.databaseOpen.emit(False)
        self.menuEdit.setEnabled(False)
        self.menuAdvanced.setEnabled(False)

        # Triggers for the update of the central panel
        self.iovView.selectionModel().currentChanged.connect(self.showData)
        self.fieldsView.selectionModel().currentChanged.connect(self.showData)

        # Add the list of actions to show/hide the panels
        self.menuPanels.addAction(self.browsePanel.toggleViewAction())
        self.menuPanels.addAction(self.filterPanel.toggleViewAction())

        self.readSettings()

    ## Store settings into the configuration file
    def writeSettings(self):
        settings = QSettings()

        settings.beginGroup("MainWindow")
        settings.setValue("Size", self.size())
        settings.setValue("Pos", self.pos())
        settings.endGroup()

        settings.beginGroup("BrowsePanel")
        settings.setValue("Visible", self.browsePanel.isVisible())
        settings.setValue("Floating", self.browsePanel.isFloating())
        settings.setValue("Size", self.browsePanel.size())
        settings.setValue("Pos", self.browsePanel.pos())
        settings.endGroup()

        settings.beginGroup("FilterPanel")
        settings.setValue("Visible", self.filterPanel.isVisible())
        settings.setValue("Floating", self.filterPanel.isFloating())
        settings.setValue("Size", self.filterPanel.size())
        settings.setValue("Pos", self.filterPanel.pos())
        settings.endGroup()

        settings.beginGroup("DataView")
        settings.setValue("FixedWidthFont", self.dataView.isFixedWidthFont())
        settings.endGroup()

        settings.beginGroup("FindDialog")
        d = self.dataView.findDialog
        settings.setValue("Visible", d.isVisible())
        settings.setValue("Pos", d.pos())
        settings.setValue("Flags", d.getFindFlags())
        settings.setValue("WrappedSearch", d.getWrappedSearch())
        settings.endGroup()

        settings.setValue("IOVs/UTC", self.iovUTCCheckBox.isChecked())

        settings.beginWriteArray("Recent")
        recents = self.menuRecent.actions()
        i = 0
        for action in recents:
            settings.setArrayIndex(i)
            settings.setValue("ConnString", action.text())
            i += 1
        settings.endArray()

        settings.beginGroup("Misc")
        settings.setValue("ExternalEditor", self._externalEditor)
        settings.endGroup()

    ## Load settings from the configuration file
    def readSettings(self):
        settings = QSettings()

        settings.beginGroup("MainWindow")
        self.resize(settings.value("Size", QSize(965, 655)))
        self.move(settings.value("Pos", QPoint(0, 0)))
        settings.endGroup()

        settings.beginGroup("BrowsePanel")
        self.browsePanel.setVisible(settings.value("Visible", True, type=bool))
        self.browsePanel.setFloating(
            settings.value("Floating", False, type=bool))
        self.browsePanel.resize(settings.value("Size", QSize(250, 655)))
        self.browsePanel.move(settings.value("Pos", QPoint(0, 0)))
        settings.endGroup()

        settings.beginGroup("FilterPanel")
        self.filterPanel.setVisible(settings.value("Visible", True, type=bool))
        self.filterPanel.setFloating(
            settings.value("Floating", False, type=bool))
        self.filterPanel.resize(settings.value("Size", QSize(270, 655)))
        self.filterPanel.move(settings.value("Pos", QPoint(0, 0)))
        settings.endGroup()

        settings.beginGroup("DataView")
        self.dataView.setFixedWidthFont(
            settings.value("FixedWidthFont", False, type=bool))
        settings.endGroup()

        settings.beginGroup("FindDialog")
        d = self.dataView.findDialog
        d.setVisible(settings.value("Visible", False, type=bool))
        d.move(settings.value("Pos", QPoint(0, 0)))
        d.setFindFlags(settings.value("Flags", 0, type=int))
        d.setWrappedSearch(settings.value("WrappedSearch", True, type=bool))
        settings.endGroup()

        self.iovUTCCheckBox.setChecked(
            settings.value("IOVs/UTC", True, type=bool))

        size = settings.beginReadArray("Recent")
        for i in range(size):
            settings.setArrayIndex(i)
            conn = settings.value("ConnString", type=str)
            action = QAction(self)
            action.setText(conn)
            action.triggered.connect(self.openRecentDatabase)
            self.menuRecent.addAction(action)
        settings.endArray()

        settings.beginGroup("Misc")
        self._externalEditor = str(settings.value("ExternalEditor", "emacs"))
        settings.endGroup()

    ## Close Event handler
    def closeEvent(self, _event):
        self.writeSettings()

    ## Fills the menu of standard databases from the connString dictionary.
    #  @see getStandardConnectionStrings()
    def setDefaultDatabases(self, connStrings):
        self.menuStandard.clear()
        self.defaultDatabases = {}
        names = connStrings.keys()
        names.sort()
        for name in names:
            conn = connStrings[name]
            action = QAction(self)
            action.setText(name)
            action.setData(conn)
            action.setStatusTip(conn)
            action.triggered.connect(self.openStandardDatabase)
            self.menuStandard.addAction(action)
            self.defaultDatabases[name] = action
        self.menuStandard.setEnabled(not self.menuStandard.isEmpty())

    ## Slot called by the actions in the menu "Database->Standard".
    #  It can also be called passing the name of one of those databases.
    @pyqtSlot()
    def openStandardDatabase(self, name=None, readOnly=True):
        if name is None:
            sender = self.sender()
        else:
            if name in self.defaultDatabases:
                sender = self.defaultDatabases[name]
            else:
                QMessageBox.critical(
                    self, "Database nickname not known",
                    "The conditions database '%s' is not in "
                    "the list of known database." % name)
                return
        # Open the database using the connection string in the action
        self.openDatabase(str(sender.data()), readOnly=readOnly)

    ## Slot called by the actions in the menu "Database->Recent"
    def openRecentDatabase(self):
        sender = self.sender()
        self.openDatabase(str(sender.text()))

    ## Open the "create new database" dialog box
    def newDatabaseDialog(self):
        raise NotImplementedError()

    ## Open the "open database" dialog box
    def openDatabaseDialog(self):
        dd = OpenDatabaseDialog(self)
        if dd.exec_():
            connString = str(dd.connectionString())
            self.openDatabase(connString, dd.readOnly())

    ## Add a connection string to the list of the recent opened databases
    def _addToRecents(self, connString):
        # do nothing if there is no connection string
        if not connString:
            return
        # action to add to the menu
        action = None
        # check if the connection string is already in the list
        for oldAction in self.menuRecent.actions():
            if oldAction.text() == connString:
                # if it is found, remove it from the list and use it as action to add
                action = oldAction
                self.menuRecent.removeAction(action)
                break
        # if the action was not found in the menu, create a new one
        if action is None:
            action = QAction(self)
            action.setText(connString)
            action.triggered.connect(self.openRecentDatabase)
        # if the menu is not empty
        if self.menuRecent.actions():
            # add the action at the beginning of the list
            self.menuRecent.insertAction(self.menuRecent.actions()[0], action)
            # and remove the entries exceeding the maximum
            while len(self.menuRecent.actions()) > self.maxRecentEntries:
                self.menuRecent.removeAction(self.menuRecent.actions()[-1])
        else:
            # otherwise just add the action
            self.menuRecent.addAction(action)

    ## Open the database identified by a connection string or by a nickname.
    #  If name is an empty string (or None) the result is a disconnection from the
    #  current database.
    #
    #  @param connString connection string
    #  @param readOnly flag to select if the database has to be opened in read-only mode or in read/write mode
    def openDatabase(self, connString, readOnly=True):
        try:
            # Clean the central view to avoid that old data stays there
            self.dataView.clear()
            if connString:
                try:
                    self.db = openDB(connString, readOnly=readOnly)
                except Exception as x:
                    msg = ''.join(x.args)
                    if msg.startswith("Database not found:"):
                        QMessageBox.critical(self, "Cannot open database", msg)
                        return
                    raise
                title = "%s - %s" % (connString, self.appName)
                # Notify the widgets that the database is open
                self.databaseOpen.emit(True)
                # Notify the widgets if the database is open in read-only mode
                editable = not readOnly
                self.databaseOpenReadOnly.emit(bool(readOnly))
                self.menuEdit.setEnabled(editable)
                self.menuAdvanced.setEnabled(editable)
                if editable:
                    # The "Add Condition" and "Delete Node" entries should be
                    # active only when a node is selected (when opening a new
                    # db there is no selection)
                    self.actionAdd_condition.setEnabled(False)
                    self.actionDelete_node.setEnabled(False)
                    # ... same for the "new tag" and "delete tag"
                    self.actionNew_tag.setEnabled(False)
                    self.actionDelete_tag.setEnabled(False)
            else:
                self.db = None
                title = self.appName
                # Notify the widgets that there is no database open
                self.databaseOpen.emit(False)
                self.menuEdit.setEnabled(False)
                self.menuAdvanced.setEnabled(False)
            self.setWindowTitle(title)
            self._path = (None, None)
            # remember the used connection string
            self._connectionString = connString
            self._addToRecents(connString)
            # update the DB instance of the models
            self.openedDB.emit(self.db)
        except:
            self.exceptionDialog()

    ## Re-open the current database, changing the readOnly flag.
    #  If the database is already in the correct mode, nothing is done.
    def reopenDatabase(self, readOnly):
        path = self._path
        if readOnly != self.db.readOnly:
            self.openDatabase(self._connectionString, readOnly)
        if path:
            self._selectPath(path[0])

    ## Disconnect from the database.
    #  @see: openDatabase()
    def closeDatabase(self):
        self.openDatabase(None)

    ## Display the standard Qt information dialog box
    def aboutQt(self):
        QMessageBox.aboutQt(self)

    ## Display the application information dialog box
    def aboutDialog(self):
        app = QApplication.instance()
        message = '''<p><b>%s</b><br/>%s</p>
        <p>Browser for the LHCb COOL and Git condition database.</p>
        <p>This is build on top of the Python API to LHCb-COOL: CondDBUI,
        and the Python API to COOL: PyCool.</p>
        <p>The Graphical Library is PyQt %s, based on Qt %s</p>
        <p><i>Marco Clemencic, Nicolas Gilardi</i></p>''' \
        % (app.applicationName(), app.applicationVersion(), PYQT_VERSION_STR, qVersion())

        QMessageBox.about(self, app.objectName(), message)

    ## Slot to react to a selected item in the structure view
    @pyqtSlot('QModelIndex')
    def selectedItem(self, index):
        item = index.internalPointer()
        if self._path != (item.path, item.channel):
            try:
                i = self.models["nodes"].nodes.index(item.path)
                self._path = (item.path, item.channel)
                self.changedPath.emit(item.path)
                if item.leaf and not item.children:
                    self.changedPathChannel.emit(item.path, item.channel or 0)
                else:
                    self.changedPathChannel.emit('', 0)
                self.actionAdd_condition.setEnabled(not self.db.readOnly)
                # Nodes can be deleted only if the database is in r/w mode and
                # they are Folders or empty FolderSets
                self.actionDelete_node.setEnabled(
                    not self.db.readOnly and (item.leaf or not item.children))
                # Tags manipulation should be enabled only for FolderSets
                # and multi-version folders
                taggable = (not self.db.readOnly
                            and ((not item.leaf) or (not item.singleVersion)))
                self.actionNew_tag.setEnabled(taggable)
                self.actionDelete_tag.setEnabled(taggable)
            except ValueError:
                i = -1
            self.pathComboBox.setCurrentIndex(i)

    ## Slot to react to a selected entry in the path combo box
    @pyqtSlot(str)
    def selectedPath(self, path):
        path = str(path)
        if self._path != (path, None) and path:
            index = self.models["tree"].findPath(path)
            self.hierarchyTreeView.setCurrentIndex(index)
            self._path = (path, None)
            item = index.internalPointer()
            self.changedPath.emit(item.path)
            if item.leaf and not item.children:
                self.changedPathChannel.emit(path, 0)
            else:
                self.changedPathChannel.emit('', 0)
            # Nodes can be deleted only if they are Folders or empty FolderSets
            self.actionDelete_node.setEnabled(item.leaf or not item.children)

    ## Return the cool::FolderSet object currently selected in the structure tree.
    #  If the selected item is not a FolderSet, the parent is returned.
    #  If there is not database opened, returns None.
    def getSelectedFolderSet(self):
        if not self.db:
            return None
        # Get the FolderSet currently selected in the tree view ("/" if none)
        idx = self.hierarchyTreeView.currentIndex()
        while idx.isValid() and idx.internalPointer().leaf:
            idx = idx.internalPointer().parent.index
        if idx.isValid():
            currentFolderSet = idx.internalPointer().node
        else:
            currentFolderSet = self.models["tree"].root.node
        return currentFolderSet

    def testSlot(self):
        print "Test slot triggered"

    @property
    def icons(self):
        if self._icons is None:
            self._icons = {}
            style = QApplication.instance().style()
            for iconName, iconId in [
                ("folderset", QStyle.SP_DirIcon),
                ("folder", QStyle.SP_FileIcon),
                ("up", QStyle.SP_ArrowUp),
                ("down", QStyle.SP_ArrowDown),
            ]:
                self._icons[iconName] = style.standardIcon(iconId)
        return self._icons

    ## This function gets called if any selection changes and decides if there
    #  is something to show in the central widget.
    def showData(self):
        data = ""
        payload = self.models["iovs"].getPayload()
        if payload:
            field = self.models["fields"].getFieldName()
            # It may happen (it actually often does) that the function is
            # called for a change in the field before the IoVs are updated, so
            # it is better to check if the field exists in the payload.
            if field in payload:
                data = payload[field]
                if not isinstance(data, basestring):
                    data = str(data)
        self.dataView.setPlainText(data)

    ## Show a critical dialog with the latest exception traceback.
    #  @param source string representing the function that trapped the exception.
    def exceptionDialog(self, source=None):
        import traceback
        if source:
            msg = "Called from '%s':\n" % source
        else:
            msg = ""
        msg += traceback.format_exc()
        print msg
        QMessageBox.critical(self, "Exception in Python code", msg)

    ## Helper function useful to force a refresh of the models and the selection
    #  of node if specified.
    def _refreshModels(self, selectPath=None):
        # trigger a refresh of the caches in the models
        self.openedDB.emit(self.db)
        self._selectPath(selectPath)

    ## Helper function to select a path in both the combo box and the hierarchy
    #  view.
    def _selectPath(self, selectPath=None):
        if selectPath:
            #  select the specified path
            i = self.pathComboBox.findText(selectPath)
            self.pathComboBox.setCurrentIndex(i)
            self.selectedPath(selectPath)

    ## Add a new node to the database
    def newNodeDialog(self):
        fs = self.getSelectedFolderSet()
        fsname = str(fs.fullPath())
        if fsname != "/": fsname += "/"
        d = NewNodeDialog(self)
        folders = set()
        foldersets = set()
        for n in self.models["nodes"].nodes:
            if self.db.existsFolder(n):
                folders.add(n)
            else:
                foldersets.add(n)
        d.setNodes(folders, foldersets)
        d.setText(fsname)
        if d.exec_():
            if d.getType() == "FolderSet":
                storageType = "NODE"
                storageKeys = None  # ignored
                versionMode = None  # ignored
            else:
                storageType = "XML"  # actually it is not used
                storageKeys = {}
                for k, v in d.fields():
                    storageKeys[str(k)] = v
                versionMode = {
                    "MultiVersion": "MULTI",
                    "SingleVersion": "SIGLE"
                }[d.getType()]
            path = str(d.text())
            self.db.createNode(
                path=path,
                description=str(d.description()),
                storageType=storageType,
                versionMode=versionMode,
                storageKeys=storageKeys)
            # Now that the node has been created, we have to notify the models.
            # FIXME: this is the easiest solution to implement, but not optimal
            self._refreshModels(path)

    ## Ask for confirmation to delete the selected node and delete it if ok
    def deleteNode(self):
        path = str(self.pathComboBox.currentText())
        if path:
            message = """<p>Are you sure you want to delete the node?</p>
            <p><center><tt>%s</tt><center></p>
            <p><font color="red">The action cannot be undone.</font></p>""" % path
            answer = QMessageBox.question(self, "Delete node?", message,
                                          QMessageBox.Yes | QMessageBox.No,
                                          QMessageBox.No)
            if answer == QMessageBox.Yes:
                try:
                    self.db.deleteNode(path)
                except:
                    self.exceptionDialog()
                # Now that the node has been deleted, we have to notify the models.
                # FIXME: this is the easiest solution to implement, but not optimal
                self._refreshModels(parentpath(path))

    ## Helper to show a dialog about
    def _unimplemented(self):
        QMessageBox.information(
            self, "Unimplemented",
            "Sorry, the function you selected is not implemented yet.")

    ## Dump a snapshot of the current database to files
    def dumpToFiles(self):
        d = DumpToFilesDialog(self)
        if d.exec_():
            from CondDBUI.Admin import DumpToFiles
            monitor = ProgressDialog(self)
            monitor.setWindowModality(Qt.WindowModal)
            monitor.setWindowTitle("Dumping to files")
            monitor.setMinimumDuration(0)
            try:
                DumpToFiles(
                    database=self.db,
                    time=d.pointInTime.toValidityKey(),
                    tag=str(d.tag.currentText()),
                    srcs=['/'],
                    destroot=str(d.destDir.text()),
                    force=d.overwrite.isChecked(),
                    addext=False,
                    monitor=monitor)
                if monitor.wasCanceled():
                    monitor.reset()
                    QMessageBox.information(self, "Dump to files canceled",
                                            "The dump has been canceled.")
            except:
                monitor.reset()
                self.exceptionDialog()

    ## Create a slice of the current database to a database
    def createSlice(self):
        raise NotImplementedError()

    ## Add a new condition to the selected folder+channel
    def addCondition(self):
        d = AddConditionDialog(self, externalEditor=self._externalEditor)
        d.setShowUTC(self.iovUTCCheckBox.checkState())
        folder, channel = self._path
        d.setLocation(folder, channel)
        payload = self.models["iovs"].getPayload()
        if payload:
            d.buffer = payload
        if d.exec_():
            try:
                from Utils import BusyCursor
                _bc = BusyCursor()
                data = []
                for cond in d.conditionsStack:
                    data.append({
                        "since": cond.since,
                        "until": cond.until,
                        "channel": int(cond.channel),
                        "payload": cond.data
                    })
                folder = d.getFolder()
                self.db.storeXMLStringList(folder, data)
                self._refreshModels(folder)
            except:
                self.exceptionDialog()

    ## Create a new tag
    def newTag(self):
        path = self._path[0]
        d = NewTagDialog(self.db, path)
        if d.exec_():
            from Utils import BusyCursor
            _bc = BusyCursor()
            tag = str(d.tag.text())
            if self.db.existsFolder(path):
                self.db.recursiveTag(path, tag)
            else:
                tags = {}
                if path == "/":
                    p = ""
                else:
                    p = path
                for n in d.childTagsModel:
                    tags[p + "/" + n] = d.childTagsModel[n]
                self.db.moveTagOnNodes(path, tag, tags)
            self._refreshModels(path)

    ## Delete a tag
    def deleteTag(self):
        path = self._path[0]
        d = SelectTagDialog(path, self)
        d.tagLabel.setText("&Tag to delete")
        tag = d.run()
        if tag is not None:
            tag = str(tag)
            message = """<p>Are you sure you want to delete the tag</p>
            <p><center><tt>%s</tt><center></p>
            <p>from the node</p>
            <p><center><tt>%s</tt><center></p>
            <p><font color="red">The action cannot be undone.</font></p>""" % (
                tag, path)
            answer = QMessageBox.question(self, "Delete tag?", message,
                                          QMessageBox.Yes | QMessageBox.No,
                                          QMessageBox.No)
            if answer == QMessageBox.Yes:
                try:
                    self.db.deleteTag(path, tag)
                except:
                    self.exceptionDialog()
                # Now that the node has been deleted, we have to notify the models.
                # FIXME: this is the easiest solution to implement, but not optimal
                self._refreshModels(path)

    ## Display context menu for the tree view
    def showHierarchyContextMenu(self, position):
        index = self.hierarchyTreeView.indexAt(position)
        if index.isValid():
            menu = QMenu(self)
            item = index.internalPointer()
            if item.leaf:
                menu.addAction(self.actionAdd_condition)
            else:
                menu.addAction(self.actionNew_node)
            menu.addAction(self.actionNew_tag)
            menu.addSeparator()
            menu.addAction(self.actionCopy_path)
            menu.addSeparator()
            menu.addAction(self.actionDelete_node)
            menu.addAction(self.actionDelete_tag)
            menu.exec_(self.hierarchyTreeView.mapToGlobal(position))

    ## Copy the current selected path to the clipboard.
    def copyPathToClipboard(self):
        QApplication.clipboard().setText(self._path[0])

    ## Copy the current connection string to the clipboard.
    def copyConnStrToClipboard(self):
        QApplication.clipboard().setText(self._connectionString)
