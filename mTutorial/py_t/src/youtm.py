#!/bin/python3
'''
Created on Jul 11, 2016

@author: wocson
'''

import sys
from wsys.sysio import *
import time
import os

argc = len(sys.argv)
os.system('clear')

colortext(LCYAN)
gotoxy(1,1); print ('hours= ')
gotoxy(1,2); print ('  min= ')
gotoxy(1,3); print ('  sec= ')
gotoxy(1,5); print ('sec= ')

if argc >= 2:
    str0 = sys.argv[1]
    words = str0.split(':')
    words.reverse()

    s = int(words[0])
    m = int(words[1])
    if len(words)==3:
        h = int(words[2])
    else:
        h = 0
    gotoxy(8,1); print (h)
    gotoxy(8,2); print (m)
    gotoxy(8,3); print (s)

else:
    colortext(LCYAN)
    h = inputxy(8,1)
    m = inputxy(8,2)
    s = inputxy(8,3)

ch='y'

while ch=='y':

    gotoxy(6,5); print (h * 60 * 60 + m * 60 + s)
    if argc >= 2:
        break
    gotoxy(1,7); ch = input('Continue?(y/n):')
    gotoxy(1,7);     print ('                 ', end='')

gotoxy(1,7); print ('done')
