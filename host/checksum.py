import struct

def calculateFletcher(data):
    C0 = 0
    C1 = 0
    for byte in data:
        bValue = struct.unpack('<B', byte)
        C0 = (C0 + bValue[0]) & 0xFF
        C1 = (C1 + C0) & 0xFF
    return C0, C1
