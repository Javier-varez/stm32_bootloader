import subprocess
import sys
import shlex
import struct

class Section:
    def __init__(self):
        self.flags = []

    def setVMA(self, vma):
        self.vma = vma

    def setLMA(self, lma):
        self.lma = lma

    def setSize(self, size):
        self.size = size

    def setName(self, name):
        self.name = name

    def setFileOffset(self, fileOffset):
        self.fileOffset = fileOffset
        
    def addFlag(self, flag):
        self.flags.append(flag)

    def getFlags(self):
        return self.flags

    def getVMA(self):
        return self.vma

    def getLMA(self):
        return self.lma

    def getSize(self):
        return self.size

    def getName(self):
        return self.name

    def getFileOffset(self):
        return self.fileOffset

    def describe(self):
        print "Section name: %s" % self.name
        print "\tLMA: 0x%08x" % self.lma
        print "\tVMA: 0x%08x" % self.vma
        print "\tSize: 0x%08x" % self.size
        print "\tFile Offset: 0x%08x" % self.fileOffset
        print "\tAttributes:"
        for attr in self.flags:
            print "\t\t%s" % attr


class HeaderParser:
    def __init__(self, data):
        self.sections = []

        foundHeader = False
        sectionLine = True
        currentSection = None
        for line in data.splitlines():
            tokens = shlex.split(line)
            try:
                if tokens[0] == "Idx":
                    foundHeader = True
                    continue
            except:
                continue

            if not foundHeader:
                continue

            if sectionLine:
                currentSection = Section()
                currentSection.setName(tokens[1])
                currentSection.setSize(int(tokens[2], 16))
                currentSection.setVMA(int(tokens[3], 16))
                currentSection.setLMA(int(tokens[4], 16))
                currentSection.setFileOffset(int(tokens[5], 16))
                sectionLine = False
            else:
                for attr in tokens:
                    currentSection.addFlag(attr.replace(',', ''))
                self.sections.append(currentSection)
                sectionLine = True

    def getSections(self):
        return self.sections

class ElfHandler:
    def __init__(self, filepath, CC='arm-none-eabi-', Path=''):
        self.CC = CC
        self.Path = Path
        self.filepath = filepath
        proc = subprocess.Popen([self.Path + self.CC + 'objdump', '-h', filepath], stdout=subprocess.PIPE)
        stdoutdata, _ = proc.communicate()
        parser = HeaderParser(stdoutdata)
        self.sections = parser.getSections()

    def getLoadableSections(self):
        loadableSections = []
        for section in self.sections:
            if 'LOAD' in section.getFlags():
                loadableSections.append(section)
        return loadableSections

    def getSectionData(self, name):
        for section in self.sections:
            if name is section.getName():
                with open(self.filepath, "rb") as elfFile:
                    elfFile.seek(section.getFileOffset())
                    return elfFile.read(section.getSize())
        return None

if __name__ == "__main__":
    elfHandler = ElfHandler(sys.argv[1])
    for section in elfHandler.getLoadableSections():
        section.describe()
        data = elfHandler.getSectionData(section.getName())
        print "%r" % data 
