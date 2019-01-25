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

        self.loadThread = LoadThread()
        self.infoThread = InfoThread()

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
            self.infoThread.feedbackSignal.connect(self.setInformation)
            self.infoThread.start()

    def setInformation(self, memStart, memSpace, maxDSize):
        self.setAddressField(memStart)
        self.setLengthField(memSpace)
        self.setMaxDLLenField(maxDSize)


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
        if data is not None:
	    self.window.MaxDLTextField.setText('0x%08x' % data)
        else:
            self.window.MaxDLTextField.setText('')

    def loadBinary(self):
        if not self.loadThread.isRunning():
            # Start job
            self.loadThread.setPort(self.port)
            self.loadThread.setFileName(self.window.ExeTextField.text())
            self.loadThread.progressSignal.connect(self.updateProgressBar)
            self.loadThread.feedbackSignal.connect(self.updateFeedbackLabel)
            self.loadThread.start()

    def updateProgressBar(self, progress):
        self.window.loadProgressBar.setValue(progress)

    def updateFeedbackLabel(self, feedbackStr):
        self.window.feedbackLabel.setText(feedbackStr)

class InfoThread(QtCore.QThread):
    feedbackSignal = QtCore.pyqtSignal(int, int, int)

    def __init__(self):
        super(InfoThread, self).__init__()

    def setPort(self, port):
        self.port = port

    def run(self):
        self.feedbackSignal.emit(None, None, None)
        uart = portManagement.getUart(self.port)
        if uart == None:
            return
        memStart, memSpace, maxDSize = bootloader.getInfo(uart)
        uart.close()
        self.feedbackSignal.emit(memStart, memSpace, maxDSize)

class LoadThread(QtCore.QThread):
    progressSignal = QtCore.pyqtSignal(int)
    feedbackSignal = QtCore.pyqtSignal(str)

    def __init__(self):
        super(LoadThread, self).__init__()

    def setFileName(self, name):
        self.filename = name

    def setPort(self, port):
        self.port = port

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
        elfHandler = ElfHandler(self.filename)
        sections = elfHandler.getLoadableSections()
        for i in range(len(sections)):
            self.feedbackSignal.emit("Loading section %s" % sections[i].getName())
            self.progressSignal.emit(100*i/len(sections))
            data = elfHandler.getSectionData(sections[i].getName())
            bootloader.loadData(uart, sections[i].getLMA(), sections[i].getSize(), data)
        bootloader.boot(uart)
        self.progressSignal.emit(100)
        self.feedbackSignal.emit("Done!")
        uart.close()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainWindow = MainWindow()
    sys.exit(app.exec_())
