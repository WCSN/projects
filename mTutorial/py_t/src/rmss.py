#!/bin/python3
'''
Created on Jul 11, 2016

@author: wocson
'''
import sys
from wsys.sysio import *
import time
import os
import math
from math import floor

def usage():
    print ("rmss.py 0.0.4 - remove extra spaces and tabs at the ends of lines in text files.\nUsage:\n\trmss source_file [target_file]\n")


class Statistics():
    spaces = 0
    tabs = 0
    lines = 0
    corline = 0

    def __init__(self):
        pass

    def __del__(self):
        print("__________________________")
        print("Lines:")
        print("  all      :", self.lines)
        print("  correct  :", self.corline)
        print("Removed:")
        print("  spaces   :", self.spaces)
        print("  tabs     :", self.tabs)

def isbinaryfile(namefile):

    with open(namefile, "rb") as binary_file:

        i = 0
        binary_file.seek(0)  # Go to beginning
        byte = binary_file.read(1)

        while byte != b'':
            if byte == b'\x00':
                print('This is binary file...\n Do not clean :(')
                binary_file.close()
                return True

            i = i + 1
            binary_file.seek(i)  # Go to next byte
            byte = binary_file.read(1)

    return False

def rmss(srcefile = '', targfile = ''):
    try:
        stat = Statistics()

        print("Cleaning file: ", srcefile)
        print(" -Get data")
        with open(srcefile) as fn:
            content = fn.readlines()

        fn.close()
        line = ''
        outcontent = []

        print(" -Prepare data")
        for line in content:
            stat.lines = stat.lines + 1
            tmpstr = line[::-1]

            corln = False

            for sym in tmpstr:
                if sym == ' ':
                    stat.spaces = stat.spaces + 1
                    corln = True
                elif sym == '\t':
                    stat.tabs = stat.tabs + 1
                    corln = True
                elif sym == '\n':
                    continue
                else:
                    break

            if corln:
                stat.corline = stat.corline + 1

            line = line.rstrip()
            outcontent.append(line)

        print(" -Save data to:", targfile)


        dest = open(targfile, 'w')
        for item in outcontent:
            dest.write("%s\n" % item)
        dest.close()

        print ('Ok. Done')

    except OSError as err:
        print("{0}".format(err))
        return 1
    except:
        print("Unexpected error:", sys.exc_info()[0])
        raise

def main():

    if len(sys.argv) >= 3:
        srcefile = sys.argv[1]
        destfile = sys.argv[2]
    elif len(sys.argv) == 2:
        srcefile = sys.argv[1]
        destfile = sys.argv[1]
    else:
        usage()
        return 1

    if not isbinaryfile(srcefile):
        rmss(srcefile, destfile)

if __name__ == '__main__':
    main()

