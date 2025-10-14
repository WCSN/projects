'''
Created on 10 нояб. 2019 г.

@author: wocson
'''

from multiprocessing import Process
import time

from aspd_lib.aspd_sys import infoline, rita
from aspd_lib.config import conf
from aspd_lib.sysio import *

from aspd_lib.winlib import Window, ftpw

MScr=None
MDL=0

def MMenu():
#    w=Window('', 1, 1, conf.TC, 1, BLACK, BGGRAY).show()
    conf.setvalue('WSLZ', conf.TC)
    infoline("Интерактивный режим")

def MainScreen():
    shellcmd('clear')
    global MScr; global MDL

    MMenu()

    LfPan=Window('ASPD', 1, 2, int(conf.TC/2), int((2*conf.TL)/3), LCYAN)
    LfPan.addItem('Ожидание информации...')
    LfPan.show()

    RtPan=Window('Local', int(conf.TC/2)+1, 2, int(conf.TC/2), int((2*conf.TL)/3), LCYAN)
    RtPan.show()
    
    MDL=int(((conf.TL)/3)/2)
    logw=Window('Log', 1, int((2*conf.TL)/3)+3, conf.TC, MDL+5, BLACK, BGGRAY)
    RtPan.show(); logw.show()
    
    info=Window('', 1, int((2*conf.TL)/3)+MDL+9, conf.TC, 2, LCYAN, BGBLUE)
    info.show()
    cmdl=Window('> ', 1, int((2*conf.TL)/3)+MDL+11, conf.TC, 1, GRAY, BGBLACK)
    cmdl.show()
    keyl=Window('1)СИН 2)Климат 3)Отмена 4)Контроль 5)Запрос 6)Удалить 7)ФАП S)hell H)elp E)xit', 1, int((2*conf.TL)/3)+MDL+12, conf.TC, 1, BLACK, BGGRAY)
    keyl.show()
    MScr={'LfPan':LfPan, 'RtPan':RtPan, 'log':logw, 'info':info, 'cmdl':cmdl, 'keyl': keyl}
    
    Process(target=listlocal()).start()
    Process(target=listaspd()).start()

def closeinter():
    gotoxy(0, conf.TL)
    
def lsloc(path, title, msg):    
    filelist=lsdir(path);

    if len(filelist)==0:
        MScr['log'].addItem(msg)
        rita(msg)
        MScr['log'].show()
    else:
        MScr['RtPan'].addItem(title+':', BIWHITE);
        MScr['RtPan'].addItem(path, LYELLOW);
        for fname in filelist:
            MScr['RtPan'].addItem(fname, LCYAN);
        MScr['RtPan'].show()
    
def listlocal():
    LDIR_ARCH=conf.getvalue('LDIR_ARCH')
    ARCHDIR=conf.getvalue('ARCHDIR')
    ARCHMETEO=LDIR_ARCH+'/Meteo/'+ARCHDIR
    ARCHSEND=LDIR_ARCH+'/Send/'+ARCHDIR
    ARCHRECV=LDIR_ARCH+'/Recv/'+ARCHDIR
    LDIRDELAY=conf.getvalue('LDIRDELAY')
    
    lsloc(ARCHMETEO, 'Метео', 'Не было метео сообщений');
    lsloc(ARCHSEND, 'Отправленные', 'Не было отправки сообщений');
    lsloc(ARCHRECV, 'Принятые', 'Не было приёма сообщений');
    lsloc(LDIRDELAY, 'Отложенные', 'Нет отложенных сообщений');

def lsftp(ftp, path):
    return ftp.ftpls(path)

def addfilename(ftp, path):
    MScr['LfPan'].addItem('[ftp://'+ftp.ftpconn.host+path+']:', BIYELLOW)
    MScr['LfPan'].addItem('..', LCYAN)
    filename=[]
    filename=lsftp(ftp, path)
    if len(filename)>0:
        for fl in filename:
            MScr['LfPan'].addItem(fl, LCYAN)
        MScr['LfPan'].show()
    else:
        MScr['log'].addItem('Файлов нет')

    MScr['LfPan'].show()
    MScr['log'].show

def listaspd():
    MScr['LfPan'].clean()
    ftp=ftpw(conf.getvalue('FTPSERVER'), conf.getvalue('FTPPORT'))
    #ftp=ftpw('193.227.233.4', 21, 'vos', '2017vOsC47b90')
    addfilename(ftp, '/aspd/')
    addfilename(ftp, '/rae/vos/')
    ftp.ftpclose()

def onediraspd(path):
    MScr['LfPan'].clean()
    ftp=ftpw(conf.getvalue('FTPSERVER'), conf.getvalue('FTPPORT'))
    #ftp=ftpw('193.227.233.4', 21, 'vos', '2017vOsC47b90')
    addfilename(ftp, path)
    ftp.ftpclose()

def inter():
    MainScreen()
    WRK=True
    while WRK:
        chs=inputxy(3, int((2*conf.TL)/3)+MDL+11)
        cmds=chs.split(' ')
        MScr['cmdl'].clean()
        argc=len(cmds)
        
        for case in switch(cmds[0]):
            if case('list', 'll', 'l'):
                MScr['RtPan'].clean()
                Process(target=listlocal()).start()
                break
            if case('rlist', 'rl', 'i'):
                MScr['LfPan'].clean()
                if argc==1:
                    Process(target=listaspd()).start()
                else:
                    Process(target=onediraspd(cmds[1])).start()                    
                break
            if case('clean', 'c'):            
                for win in MScr:
                    MScr[win].clean()
                break
            if case('exit', 'e'):            
                WRK=False
                shellcmd('clear')
                print('Done')
            if case('bye', 'b'):       
                WRK=False
            else:
                pass
    
