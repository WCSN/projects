#!/usr/local/bin/python
#  -*- coding: utf-8 -*-
'''
Created on Dec 20, 2011

@author: wocson
'''

import MySQLdb as mDB
import sys

import wcpack.wctools as wt
import wcpack.azsncfg as cfg

file_sql = cfg.PATH_SQL + 'add_data_db0.sql'

try:

    wt.Message( 'Connect to host: ' + cfg.Host + '. Database: ' + cfg.Db )
    db = wt.DBOperation( cfg.Db )
    wt.Message( 'OK.' )
    db.OpenDB()

    print 'Все федеральные округа:'
    data = db.ShowTable( 'RUSSIAN' )

    db.CloseDB()
    wt.Message( 'OK.' )
    wt.Message( 'Success. End.' )
    sys.exit( 0 )

except mDB.connections.DatabaseError, e:
    wt.Message( e.args[0], e.args[1] )
    sys.exit( e.args[0] )
