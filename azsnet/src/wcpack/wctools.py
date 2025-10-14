#!/usr/local/bin/python
# -*- coding: utf-8 -*-
'''
Created on Dec 12, 2011

@author: wocson
'''

import MySQLdb as mDB;
from datetime import datetime;

import wcpack.azsncfg as cfg;

NONE = -1; INFO = 0; WARN = 1; ERROR = 2; DEBUG = 3;

InfoLevel = cfg.Infolevel

def timestamp():
        t = datetime.now();
        return t.strftime( "%Y%m%d %H:%M:%S" );

def dblevel( dlevel ):
    debinfo = ' [INFO]:', ' [WARN]:', ' [ERROR]:', ' [DEBUG]:'
    if dlevel > -1:
        return timestamp() + debinfo[dlevel];
    else:
        return '';

def Message( mess0 = '', mess1 = '', mess2 = '', mess3 = '', mess4 = '' ):
    if cfg.Infolevel != NONE:
        print dblevel( InfoLevel ), mess0, mess1, mess2, mess3, mess4
    return 0;

def getSQLsrc( filesrc_cmd ):
    fl = open( filesrc_cmd, "r" );
    all_lines = fl.readlines();
    fl.close();
    return all_lines;


def getSQLcmds( sqlsrcs ):

    sqlcmds = [];
    sql = '';

    for line in sqlsrcs:
        if line.find( "--" ) > -1: pass
        else:
            if line.find( '\n' ) > -1:
                line = line.replace( '\n', ' ' )
                sql += line
                if sql.find( ';' ) > -1:
                    sqlcmds.append( sql );
                    sql = '';
    return sqlcmds;

#def DataOut( data ):
#    for x in data:
#        #Message(x)
#        i = 0
#        while i < len( x ):
#            print x[i],
#            i += 1
#        print ''

def DataOut( data ):
    for x in data:
        print ( ': '.join( x ) )



tl_corn = u'┌'
tr_corn = u'┐'
tline = u'─'
vline = u'│'
msl_line = u'├'
msr_line = u'┤'
bl_corn = u'└'
br_corn = u'┘'

def ttline( lensf ):
    return tl_corn + tline.join( tline * ( x ) for x in lensf ) + tr_corn

def mstline( lensf ):
    return msl_line + tline.join( tline * ( x ) for x in lensf ) + msr_line

def mline( lensf, tcols ):
    rm = vline + vline.join( str( x ) + ' ' * ( lensf[i] - len( x.decode( 'utf-8' ) ) ) for i, x in enumerate( tcols ) ) + vline
    return  rm

def btline( lensf ):
    return bl_corn + tline.join( tline * ( x ) for x in lensf ) + br_corn

def conv2list( tpls ):
    lst = []
    lsts = []
    for tpl in tpls:
        for x in tpl:
            lst.append( str( x ) )
        lsts.append( lst )
        lst = []
    return lsts

class DBOperation( object ):
    def __init__( self, Dbname ):
        self.Db = Dbname;

    def OpenDB( self ):
        self.m_db = mDB.connect( cfg.Host, cfg.User, cfg.Passwd, self.Db, cfg.Port, use_unicode = 1, charset = 'utf8' );
        self.m_db.set_character_set( 'utf8' );
        self.m_cursor = self.m_db.cursor();
        self.m_cursor.execute( 'SET NAMES utf8' );
        self.m_cursor.execute( 'SET CHARACTER SET utf8' );
        self.m_cursor.execute( 'SET character_set_connection=utf8' );

    def Execute( self, sqlcmd ):
        return self.m_cursor.execute( sqlcmd );

    def CommitDB( self ):
        self.m_db.commit();

    def CloseDB( self ):
        self.m_db.close();

    def Fetchall( self ):
        return self.m_cursor.fetchall();

    def ShowTable( self, nametable ):

        # Get data from table
        #
        #    Note!!! If use unicode utf-8 an len('RUS') return 3 but len('РУС') return 6 !!!  
        #    use len('РУС'.decode('utf-8')) return 3 !!!
        #

        self.Execute( 'SELECT * FROM ' + nametable + ' WHERE 1' )
        data = self.Fetchall()
        # Convert to list
        dt = conv2list( data )
        lens = [0] * len( dt[0] ) # list of max lens for columns

        for row in dt: # get rows data
            for num, col in enumerate( row ):
                cLen = len( col.decode( 'utf-8' ) )
                if lens[num] < cLen:
                    lens[num] = cLen

        # Get headers table
        self.Execute( 'DESCRIBE ' + nametable )
        heads = self.Fetchall()

        # Convert to list   
        head = conv2list( heads )
        header = []

        # Correct max lens if headers columns great col data
        for num, col in enumerate( head ):
            header.append( col[0] )
            cLen = len( col[0].decode( 'utf-8' ) )
            if lens[num] < cLen:
                lens[num] = cLen

        print ttline( lens )          # Draw top line table
        print mline( lens, header )    # Draw midle line header table
        print mstline( lens )         # Draw midle separate line table

        for row in dt:
            print mline( lens, row )   # Draw midle data line table
        print btline( lens )

        return data
