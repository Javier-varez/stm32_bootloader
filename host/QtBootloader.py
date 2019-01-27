import bootloader
import portManagement
import sys
import serial
import os
import time
from PyQt5.QtWidgets import *
from PyQt5.uic import *
from PyQt5 import QtCore
from elfHandler import ElfHandler

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.initUI()

    def initUI(self):
        if getattr(sys, 'frozen', False):
            application_path = os.path.dirname(sys.executable)
        elif __file__:
            application_path = os.path.dirname(__file__)
        mainwindowFile = os.path.join(application_path, 'res/mainwindow.ui')

        self.window = loadUi(mainwindowFile)
        self.port = ''
        self.loadPortList()

        self.window.loadBinButton.clicked.connect(self.loadBinary)
        self.window.ChooseButton.clicked.connect(self.chooseFile)
        self.window.PortList.itemSelectionChanged.connect(self.setCurrentPort)
        self.window.getInfoButton.clicked.connect(self.obtainInformation)
        self.window.refreshButton.clicked.connect(self.loadPortList)
        self.window.memoryList.itemSelectionChanged.connect(self.updateAddressAndLengthFields)

        self.loadThread = LoadThread()
        self.infoThread = InfoThread()
        self.infoThread.feedbackSignal.connect(self.setInformation)
        self.loadThread.progressSignal.connect(self.updateProgressBar)
        self.loadThread.feedbackSignal.connect(self.updateFeedbackLabel)

        self.window.show()

    def chooseFile(self):
        file, _ = QFileDialog.getOpenFileName(self, "Open Binary", "", "Bin (*.elf)")
        self.window.ExeTextField.setText(file)

    def setCurrentPort(self):
        items = self.window.PortList.selectedItems()
        for item in items:
            self.port = item.text()

    def loadPortList(self):
        self.port = ''
        ports = portManagement.getPortList()
        self.window.PortList.clear()
        for port in ports:
            item = QListWidgetItem(port)
            self.window.PortList.addItem(item)
            self.window.PortList.setCurrentItem(item)
            self.port = port

    def obtainInformation(self):
        if not self.infoThread.isRunning():
            self.infoThread.setPort(self.port)
            self.infoThread.start()

    def setInformation(self, maxDSize, memories):
        self.memories = memories
        self.updateMemoryList()
        self.setMaxDLLenField(maxDSize)

    def updateAddressAndLengthFields(self):
        row = self.window.memoryList.currentRow()
        if (row != -1) and (row < len(self.memories)):
            self.setAddressField(self.memories[row].getAddr())
            self.setLengthField(self.memories[row].getSize())
        else:
            self.setAddressField(None)
            self.setLengthField(None)

    def updateMemoryList(self):
        self.window.memoryList.clear()
        for mem in self.memories:
            item = QListWidgetItem(mem.getType())
            self.window.memoryList.addItem(item)

        #set the first element as selected
        self.window.memoryList.setCurrentRow(0)
        self.updateAddressAndLengthFields()


    def setAddressField(self, data):
        if data is not None:
            self.window.AddresTextField.setText('0x%08x' % data)
        else:
            self.window.AddresTextField.setText('')

    def setLengthField(self, data):
        if data is not None:
        	self.window.LengthTextField.setText('0x%08x' % data)
        else:
        	self.window.LengthTextField.setText('')

    def setMaxDLLenField(self, data):
        if (data is not None) and (data != 0):
	    self.window.MaxDLTextField.setText('0x%08x' % data)
        else:
            self.window.MaxDLTextField.setText('')

    def loadBinary(self):
        if not self.loadThread.isRunning():
            # Start job
            self.loadThread.setPort(self.port)
            self.loadThread.setFileName(self.window.ExeTextField.text())
            self.loadThread.start()

    def updateProgressBar(self, progress):
        self.window.loadProgressBar.setValue(progress)

    def updateFeedbackLabel(self, feedbackStr):
        self.window.feedbackLabel.setText(feedbackStr)

class InfoThread(QtCore.QThread):
    feedbackSignal = QtCore.pyqtSignal(int, list)

    def __init__(self):
        super(InfoThread, self).__init__()

    def setPort(self, port):
        self.port = port

    def run(self):
        self.feedbackSignal.emit(0, [])
        uart = portManagement.getUart(self.port)
        if uart == None:
            return
        maxDSize, memories = bootloader.getInfo(uart)
        if maxDSize is not None:
            self.feedbackSignal.emit(maxDSize, memories)
        uart.close()

class LoadThread(QtCore.QThread):
    progressSignal = QtCore.pyqtSignal(int)
    feedbackSignal = QtCore.pyqtSignal(str)

    internalProgressSignal = QtCore.pyqtSignal(int)

    def __init__(self):
        super(LoadThread, self).__init__()
        self.internalProgressSignal.connect(self.reportProgress)

    def setFileName(self, name):
        self.filename = name

    def setPort(self, port):
        self.port = port

    def reportProgress(self, sectionProgress):
        totalSize = 0
        sentSize = 0
        for i in range(len(self.sections)):
            totalSize += self.sections[i].getSize()
            if i < self.currentSection:
                sentSize += self.sections[i].getSize()
            elif i == self.currentSection:
                sentSize += (self.sections[i].getSize() * sectionProgress) / 100

        currentProgress = int((sentSize*100) / totalSize)
        self.progressSignal.emit(currentProgress)

    def findBootAddress(self):
        self.bootAddr = 0
        section = self.elfHandler.getSectionByName('.isr_vector')
        if section is not None:
            print "using isr_vector addr"
            self.bootAddr = section.getLMA()
        else:
            section = self.elfHandler.getSectionByName('.text')
            if section is not None:
                self.bootAddr = section.getLMA()
        print "Boot Addr = 0x%08x" % self.bootAddr

    def isMemoryUsed(self, memory):
        memStart = memory.getAddr()
        memEnd = memStart + memory.getSize()
        for section in self.sections:
            lma = section.getLMA()
            if (lma >= memStart) and (lma < memEnd):
                return True
        return False

    def initializeMemories(self):
        #uart must have larger timeout, since it is a long operation
        uart = portManagement.getUart(self.port, 10)
        _, availableMemories = bootloader.getInfo(uart)
        for mem in availableMemories:
            if self.isMemoryUsed(mem):
                print "Initializing %s" % mem.getType()
                self.feedbackSignal.emit("Initializing %s" % mem.getType())
                bootloader.initializeMemory(uart, availableMemories.index(mem))
                
    def run(self):
        self.progressSignal.emit(0)
        self.feedbackSignal.emit('')
        if not os.path.isfile(self.filename):
            self.feedbackSignal.emit("Error opening file: %s" % self.filename)
            return
        uart = portManagement.getUart(self.port)
        if uart == None:
            self.feedbackSignal.emit("Couldn't find port %s" % self.port)
            return
        self.elfHandler = ElfHandler(self.filename)
        self.sections = self.elfHandler.getLoadableSections()
        self.initializeMemories()
        self.findBootAddress()
        for i in range(len(self.sections)):
            self.feedbackSignal.emit("Loading section %s" % self.sections[i].getName())
            data = self.elfHandler.getSectionData(self.sections[i].getName())
            self.currentSection = i
            bootloader.loadData(uart, self.sections[i].getLMA(), self.sections[i].getSize(), data, self.internalProgressSignal)
        bootloader.boot(uart, self.bootAddr)
        self.progressSignal.emit(100)
        self.feedbackSignal.emit("Done!")
        uart.close()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainWindow = MainWindow()
    sys.exit(app.exec_())
