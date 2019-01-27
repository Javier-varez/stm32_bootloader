import serial
import binascii
import sys
import struct
import math
import time
import checksum
from PyQt5 import QtCore

class Memory:
    def __init__(self, baseAddr, size, flashType):
        typeList = ['RAM', 'FLASH']
        self.baseAddr = baseAddr
        self.size = size
        self.flashType = typeList[flashType]

    def getAddr(self):
        return self.baseAddr

    def getSize(self):
        return self.size

    def getType(self):
        return self.flashType

commands = {
        'setBlock'             : b'\x00',
        'getInfo'              : b'\x01',
        'boot'                 : b'\x02',
        'initializeMemory'     : b'\x03'
    }

badChecksumError = b'\x05'

setBlockErrors = {
        b'\x00' : 'No error',
        b'\x01' : 'Block Overflow',
        b'\x02' : 'Memory limit exceeded',
        b'\x03' : 'Start Pointer is invalid',
        b'\x04' : 'Max. Download Size Exceeded',
        b'\x05' : 'Bad Checksum',
        b'\x06' : 'Unknown Memory'
    }

bootErrors = {
        b'\x00' : 'Invalid Boot Address'
    }

def getAck(uart):
    ackByte = uart.read(1)
    return ackByte == b'\x5A'

def parseInt(data):
    byteArray = bytearray(data)
    integerVal = 0
    for i in range(4):
        integerVal += byteArray[i] << (i*8)
    return integerVal

def intToBytes(data):
    return struct.pack('<i', data)

def byteToInt(data):
    return struct.unpack('<B', data)

def intToByte(data):
    return struct.pack('<B', data)

def printError(command, error):
    if command == b'\x00':
        print "Received error: %s" % setBlockErrors[error]
    elif command == b'\x02':
        print "Received error: %s" % bootErrors[error]

def getInfo(uart):
    if uart is None:
        return None, []
    uart.write(commands['getInfo'])
    if not getAck(uart):
        print "Error getting information. Nack"
        return None, []
    maxDownloadSize = parseInt(uart.read(4))
    numMemorySections = parseInt(uart.read(4))
    memList = []
    for i in range(numMemorySections):
        baseAddr = parseInt(uart.read(4))
        size = parseInt(uart.read(4))
        memType = parseInt(uart.read(4))
        mem = Memory(baseAddr, size, memType)
        memList.append(mem)
    return maxDownloadSize, memList

def setBlock(uart, addr, size, data):
    uart.write(commands['setBlock'])
    uart.write(intToBytes(addr))
    uart.write(intToBytes(size))
    if not getAck(uart):
        error = uart.read(1)
        printError(b'\x00', error)
        return error
    for i in range(size):
        time.sleep(0.0001)
        uart.write(data[i])
    C0, C1 = checksum.calculateFletcher(data)    
    uart.write(intToByte(C0))
    uart.write(intToByte(C1))
    if not getAck(uart):
        error = uart.read(1)
        printError(b'\x00', error)
        return error
    return b'\x00'

def loadData(uart, lma, length, data, signal=None):
    if length == 0:
        return
    maxDSize, _ = getInfo(uart)
    
    nChunks = int(math.ceil(float(length) / maxDSize))
    for i in range(nChunks):

        #Obtain current chunk
        Chunk = []
        if i is not nChunks-1:
            Chunk = data[i*maxDSize : ((i+1)*maxDSize)]
        else:
            Chunk = data[i*maxDSize:]

        # Send block
        error = badChecksumError
        nMaxRetries = 5
        nRetries = 0
        while (error == badChecksumError) and (nRetries < nMaxRetries):
            error = setBlock(uart, lma+i*maxDSize, len(Chunk), Chunk)
            nRetries = nRetries + 1

        # Report progress if needed
        if signal is not None:
            if i != nChunks-1:
                progress = ((i+1) * maxDSize * 100) / length
            else:
                progress = 100
            signal.emit(progress)

def boot(uart, bootAddr):
    if uart is None:
        return
    uart.write(commands['boot'])
    time.sleep(0.001)
    uart.write(intToBytes(bootAddr))
    if not getAck(uart):
        print "Didn't get Ack!!"
        error = uart.read(1)
        printError(b'\x02', error)

def initializeMemory(uart, index):
    if uart is None:
        return
    uart.write(commands['initializeMemory'])
    time.sleep(0.001)
    uart.write(intToBytes(index))
    if not getAck(uart):
        print "Didn't Ack"
    #wait until completion
    if not getAck(uart):
        print "Didn't get second Ack"
