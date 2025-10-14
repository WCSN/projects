#!/usr/local/bin/python
#  -*- coding: utf8 -*-
'''
Created on 11.12.2011

@author: wocson
'''

import MySQLdb as mDB
import sys

import wcpack.wctools as wt 
import wcpack.azsncfg as cfg

file_sql = '/home/wocson/projects/azsnet/sql/getdata_db0.sql'

#===================================================================
try:
    wt.Message('Connect to host: ' + cfg.Host + '. Database: ' + cfg.Db)
    db = wt.DBOperation(cfg.Db)
    wt.Message('OK.')
    db.OpenDB()
    wt.Message('OK.','Get SQL command from '+file_sql)
    sqlsrcs = wt.getSQLsrc(file_sql)   
    wt.Message('Execute SQL command...',sqlsrcs)
    
    sqlcmds = wt.getSQLcmds(sqlsrcs);
    
    for sqlcmd in sqlcmds:
        wt.Message('\nCommand:\n',sqlcmd)
        db.Execute(sqlcmd)
        wt.DataOut(db.Fetchall());
        
    print 'Все федеральные округа:'
    data = db.ShowTable('RUSSIAN')

    
    for nametable in data:
        print nametable[0],' Федеральный округ:',nametable[1]
        dat = db.ShowTable(nametable[2])
        for dt in dat:
            db.ShowTable(dt[2])
            
    db.CloseDB()
    wt.Message('OK.')
    wt.Message('Success. End.')
    sys.exit(0)
    
except mDB.connections.DatabaseError, e:
    wt.Message(e.args[0], e.args[1])
    sys.exit(e.args[0])
