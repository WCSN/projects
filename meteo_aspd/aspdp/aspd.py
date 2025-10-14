#!/bin/python
'''
Created on 18 сент. 2019 г.

@author: wocson
'''
import getopt
import locale
import sys

from aspd_lib.aspd_iface import cl
from aspd_lib.aspd_sys import *
from aspd_lib.config import conf, AspdConfig
from aspd_lib.help import mhelp, usage, version, unknown
from aspd_lib.inter import *
from aspd_lib.sysio import *
import blessings

SILENT = True


def main():
    ARGC = len( sys.argv )

    if ARGC == 1:
        usage();
        return 0;
    else:
        for case in switch( sys.argv[1] ):
            if case( 'help', 'h' ):
                if ARGC == 3: mhelp( sys.argv[2] )
                else: mhelp()
                break
            if case( 'rmdl' ):
                rmdelay()
                break
            if case( 'llist', 'll', 'l' ):
                if ARGC == 3: msgll( sys.argv[2] )
                else: msgll()
                break
            if case( 'rita', 'rt', 't' ):
                if ARGC == 3: rita( sys.argv[2] )
                else: mhelp( sys.argv[1] )
                break
            if case( 'ver', 'v' ):
                version()
                break
            if case( 'stat', 'st', 'u' ):
                status()
                break
            if case( 'setperm', 'sp', 'p' ):
                if ARGC == 3:setallperm( sys.argv[2] )
                else:setallperm()
                break
            if case( 'getcfg', 'gf', 'g' ):
                tmcheckavos( 'avos', True )
                if ARGC == 3: print( conf.getvalue( sys.argv[2] ), end = '' );
                else: mhelp( sys.argv[1] )
                break
            if case( 'send', 'sn', 's' ):
                if not locked( conf.getvalue( 'LOCKED_SEND' ) ):
                    signsend = conf.getvalue( 'SIGN_SEND' )
                    signdelay = conf.getvalue( 'SIGN_DELAY' )

                    if sys.argv[ARGC - 1].find( '--size=' ) != -1:
                        conf.setvalue( 'ARCHSIZE', sys.argv[ARGC - 1].split( '=' )[1] )
                        ARGC -= 1

                    if ARGC == 2:
                        if os.path.isfile( signsend ):
                            shellcmd( 'rm ' + signsend, '/dev/null' )
                            msgsend()
                            shellcmd( 'sudo killall aspdtail', '/dev/null' )
                        if os.path.isfile( signdelay ):
                            if tmcheckavos( 'avos', 'SILENT' ):
                                shellcmd( 'rm ' + signdelay, '/dev/null' )
                                delayfile = conf.getvalue( 'LDIRDELAY' ) + "/avos.txt"
                                if os.path.isfile( delayfile ):
                                    shellcmd( 'mv ' + delayfile + ' ' + conf.getvalue( 'LDIRFORSEND' ), '/dev/null' )
                                    msgsend()
                                    shellcmd( 'sudo killall aspdtail' )
                    elif ARGC == 3:
                        msgsend( sys.argv[2] )
                    elif ARGC == 4:
                        msgsend( sys.argv[2], sys.argv[3] )
                    else:
                        if conf.getvalue( 'WORKTYPE' ) == 'debug': outt( "Нет сигнала на отправку в " + conf.getvalue( "INFODIR" ) )
                break
            if case( 'recv', 'rc', 'r' ):
                if not locked( conf.getvalue( 'LOCKED_RECV' ) ):
                    if ARGC == 4: msgrecv( sys.argv[2], sys.argv[3] )
                    elif ARGC == 3: msgrecv( sys.argv[2] )
                    else: msgrecv()
                break
            if case( 'email', 'em', 'm' ):
                if not locked( conf.getvalue( 'LOCKED_MAIL' ) ):
                    ADRS = ""
                    if ARGC == 3:
                        ADRS = sys.argv[2]
                    else:
                        ADRS = psecho( "E-mail: " )
                        if not ADRS.find( "@" ) > -1:
                            outt( "Адрес задан неверно. Отмена" );  # CLOSE();
                            mhelp( sys.argv[1] )
                            return 1

                    SUBJ = ""; TXTM = ""; ATCH = "";
                    try:
                        opts, args = getopt.getopt( sys.argv[3:], "s:t:a:f:", ["subj=", "text=", "attach=", "file="] )
                    except getopt.GetoptError as err:
                        print( err )
                        usage()
                        sys.exit( 2 )

                    for opt, content in opts:
                        if opt in ( "-s", "--subj" ):
                            SUBJ = content
                        elif opt in ( "-t", "--text" ):
                            TXTM = content
                        elif opt in ( "-a", "--attach" ):
                            ATCH = content
                        elif opt in ( "-f", "--file" ):
                            f = open( content )
                            TXTM += "\n\n" + f.read()
                            f.close()
                        else:
                            assert False, "Задана недопустимая опция:" + opt
                    msgmail( ADRS, SUBJ, TXTM, ATCH )
                break
            if case( 'rlist', 'rl', 'i' ):
                if ARGC == 3:
                    msgrl( sys.argv[2] );
                else:
                    msgrl();
                break
            if case( 'del', 'dl', 'd' ):
                if ARGC == 3:
                    msgdel( sys.argv[2] );
                break
            if case( 'abook', 'ab', 'a' ):
                abook()
                break
            if case( 'letter', 'lt', 'e' ):
                if ARGC > 2:
                    if sys.argv[2] == 'avos':
                        if not tmcheckavos( sys.argv[2] ):
                            answ = youchoice( "Cоздать СИН-сообщение?", cl.getcolor( 'TBCOLOR' ) )
                            if answ:
                                msglt( 'avos' )
                            elif not answ:
                                outt( 'Отмена', 'scr' )
                        else:
                            msglt( 'avos' )
                        break
                    elif sys.argv[2] == 'climat':
                        if not tmcheckavos( sys.argv[2] ):
                            answ = youchoice( "Cоздать сообщение типа CLIMAT?" )
                            if answ: msglt( 'climat' )
                            elif not answ: outt( 'Отмена', 'scr' )
                        else:
                            msglt( 'climat' )
                        break
                    elif sys.argv[2] == 'grp':
                        if ARGC == 3:
                            msglt( sys.argv[2] )
                        elif ARGC == 4:
                            msglt( sys.argv[2], sys.argv[3] )
                        elif ARGC == 5:
                            msglt( sys.argv[2], sys.argv[3], sys.argv[4] )
                        break
                    else:
                        msglt( sys.argv[2] )
                else:
                    msglt()
                break
            if case( 'inter', 'in' ):
                inter()
                break
            else:
                unknown( sys.argv[1] )
                break
    return 0  # main


if __name__ == '__main__':
    locale.setlocale( locale.LC_ALL, "ru_RU.UTF-8" )
    try:
        if WORKTYPE == 'debug':
            # shellcmd('./testaspd.sh vos_att nostart')
            pass
        if WORKTYPE == 'debug':
            # shellcmd('touch ./store02/aspd_debug/infobox/send')
            pass
        sys.exit( main() )
    except KeyboardInterrupt:
        sys.exit( 0 )
    finally:
        CLOSE()

