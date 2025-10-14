#!/usr/local/bin/python
# -*- coding: utf-8 -*-
'''
Created on Dec 12, 2011

@author: wocson
'''

import socket

NONE = -1; INFO = 0; WARN = 1; ERROR = 2; DEBUG = 3;

PATH_SQL = '/home/wocson/projects/azsnet/sql/'
PATH_CFG = ''
PATH_LIB = ''

Infolevel = NONE #DEBUG

host = socket.gethostname()

if host == 'wfid78-172.akbspb.ru':
    DBHost = 'localhost'
elif host == 'wcsn.wocson.ru':
    DBHost = 'gw-wcsn'

Host = DBHost
User = 'root'
Passwd = '_13woc11son68'
Db = 'AZSNET'
Port = 3306
REGNum = '78'

