#!/usr/local/bin/python
#  -*- coding: utf-8 -*-
'''
Created on Dec 7, 2011

Create new and checked upload new data to database

@author: wocson
'''

import MySQLdb as mDB
#import string
import sys
import wcpack.wctools as wt
import wcpack.azsncfg as cfg

sql = ''
file_sql = '../sql/create_db0.sql'

try:
    wt.Message( 'Connect to host: ' + cfg.Host + '. Database: ' + cfg.Db )
    db = wt.DBOperation( cfg.Db )
    db.OpenDB()
    wt.Message( 'DataBase ' + cfg.Db + ' exists. Close access.' )
    wt.Message( 'Success. End.' )

except mDB.connections.DatabaseError, e:
    wt.Message( e.args[0], e.args[1], 'Create Database ' + cfg.Db + ' now...' )

    if ( e.args[0] == 1049 ): # db not find
        db = wt.DBOperation( '' )
        db.OpenDB()
        i = 0

# Create main tables

        sqlsrcs = wt.getSQLsrc( file_sql )
        wt.Message( 'OK.', 'Get SQL command from ', sqlsrcs )
        sqlcmds = wt.getSQLcmds( sqlsrcs );
        wt.Message( 'Execute SQL command...', sqlsrcs )

        for sqlcmd in sqlcmds:
            wt.Message( '\nCommand:\n', sqlcmd, '\nDataBase out:' )
            db.Execute( sqlcmd )
            wt.DataOut( db.Fetchall() )

# Create regions tables
        db.CommitDB()
        wt.Message( 'Create regions tables...' )

        db.Execute( 'SELECT `NAME_DTFO` FROM `RUSSIAN`' )
        data = db.Fetchall()

        for x in data:
        #Message(x)
            # SELECT * FROM `CTFO` WHERE 1
            db.Execute( 'SELECT `NAME_DTAS` FROM `' + x[0] + '` WHERE 1' )
            data2 = db.Fetchall()
            for y in data2:
                sql = """ 
                CREATE TABLE IF NOT EXISTS `""" + y[0] + """` (`id` INT(11) NOT NULL AUTO_INCREMENT,
                `NAME_AZS` CHAR(128) NOT NULL, 
                `A80` CHAR(7) NOT NULL, 
                `A92` CHAR(7) NOT NULL,
                `A95` CHAR(7) NOT NULL,
                `A98` CHAR(7) NOT NULL,
                `DT0` CHAR(7) NOT NULL,
                `DT1` CHAR(7) NOT NULL,
                `Note` CHAR(255) NOT NULL,
                `Author` CHAR(64) NOT NULL,
                `EMAIL` CHAR(128) NOT NULL,
                PRIMARY KEY(`id`));
                """
                db.Execute( sql )
                wt.Message( 'Create ' + y[0], ' OK.' )
                db.CommitDB()

                sql = """
                INSERT INTO `""" + y[0] + """` SET 
                `NAME_AZS`='ЛукОйл',`A80`='24.50',`A92`='27.89',`A95`='29.70',
                `A98`='33.70',`DT0`='29.60',`DT1`='31.60',
                `Note`='Сволочи вороватые, а присадки мочат дрыг за пару лет... а карбовые просто с первой заправки Луком.. херовый луц.',
                `Author`='Васиссуалий Пупкинсон',
                `EMAIL`='vasspu@nahnah.zu';
                """
                db.Execute( sql )

                sql = """
                INSERT INTO `""" + y[0] + """` SET 
                `NAME_AZS`='ТНК',`A80`='24.50',`A92`='27.89',`A95`='29.70',
                `A98`='33.70',`DT0`='29.60',`DT1`='31.60',
                `Note`='Отличные гамбургеры... а вот бензин гавно :(',
                `Author`='Васиссуалий Пупкинсон',
                `EMAIL`='vasspu@nahnah.zu';
                """
                db.Execute( sql )

        db.CommitDB()
        db.CloseDB()
        wt.Message( 'Success. End.' )
    else:
        sys.exit( e.args[0] )
