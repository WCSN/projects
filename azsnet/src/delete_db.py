#/!usr/local/bin/python
# -*- coding: utf-8 -*-
'''
Created on 11.12.2011

@author: wocson
'''
import sys
import MySQLdb as mDB
import wcpack.wctools as wt
import wcpack.azsncfg as cfg

try:
    wt.Message( 'Connect to host: ' + cfg.Host + '. Database: ' + cfg.Db )
    db = wt.DBOperation( cfg.Db )
    wt.Message( 'OK.' )
    db.OpenDB()
    wt.Message( 'Delete database ' + cfg.Db, db.Execute( 'DROP DATABASE ' + cfg.Db ), 'Sussed' )
    db.CommitDB()
    db.CloseDB()
    wt.Message( 'End.' )
    sys.exit( 0 )

except mDB.connections.DatabaseError, e:
    wt.Message( e.args[0], e.args[1] )

