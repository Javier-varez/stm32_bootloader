import serial
import binascii
import sys
import struct
import math
import time
import checksum
from PyQt5 import QtCore

commands = {
        'setBlock' : b'\x00',
        'getInfo'  : b'\x01',
        'boot'     : b'\x02'
    }

badChecksumError = b'\x05'

errors = {
        b'\x00' : 'No error',
        b'\x01' : 'Block Overflow',
        b'\x02' : 'Memory limit exceeded',
        b'\x03' : 'Start Pointer is invalid',
        b'\x04' : 'Max. Download Size Exceeded',
        b'\x05' : 'Bad Checksum'
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

def printError(error):
    print "Received error: %s" % errors[error]

def getInfo(uart):
    if uart is None:
        return None, None, None
    uart.write(commands['getInfo'])
    if not getAck(uart):
        print "Error getting information. Nack"
        return None, None, None
    memStart = parseInt(uart.read(4))
    memSpace = parseInt(uart.read(4))
    maxDownloadSize = parseInt(uart.read(4))
    return memStart, memSpace, maxDownloadSize

def setBlock(uart, addr, size, data):
    uart.write(commands['setBlock'])
    uart.write(intToBytes(addr))
    uart.write(intToBytes(size))
    if not getAck(uart):
        error = uart.read(1)
        printError(error)
        return error
    for i in range(size):
        time.sleep(0.0001)
        uart.write(data[i])
    C0, C1 = checksum.calculateFletcher(data)    
    uart.write(intToByte(C0))
    uart.write(intToByte(C1))
    if not getAck(uart):
        error = uart.read(1)
        printError(error)
        return error
    return b'\x00'

def loadData(uart, lma, length, data, signal=None):
    if length == 0:
        return
    memStart, memSpace, maxDSize = getInfo(uart)
    
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

def boot(uart):
    if uart is None:
        return
    uart.write(commands['boot'])
    if not getAck(uart):
        print "Didn't get Ack!!"
