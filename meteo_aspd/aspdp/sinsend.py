#!/bin/python3
'''
Created on 4 нояб. 2019 г.

@author: wocson
'''
import locale
import sys, os 

from aspd_lib.aspd_iface import cl
from aspd_lib.aspd_sys import *
from aspd_lib.config import conf, AspdConfig
from aspd_lib.help import mhelp, usage, version, unknown

version="0.241"
ARGC=len(sys.argv)

ASPDLOG=conf.getvalue('LOG');
SIGN_DELAY=conf.getvalue('SIGN_DELAY')
LDIRDELAY=conf.getvalue('LDIRDELAY')
LASTMT=conf.getvalue('LASTMT')
LASTFILESEND='' 
WSLZ=conf.getvalue('WIDESCREEN') 
RETCHS='' 
LDTTM=''

SIN0=conf.getvalue('SIN0')
SIN1=conf.getvalue('SIN1')
SIN2=conf.getvalue('SIN2')
SIN3=conf.getvalue('SIN3')
STATION=conf.getvalue('STATION')

def inform():
    cl.settextcolor(LYELLOW)    
    outt(' СИН-сроки АСПД ААНИИ:' , 'scr')
    outt(' 1. '+SIN0+' '+STATION+' 3. '+SIN2+' '+STATION, 'scr')
    outt(' 2. '+SIN1+' '+STATION+' 4. '+SIN3+' '+STATION, 'scr')
    cl.setdefcolors()
    outt("", 'scr')
    cl.setcolors(LGREEN)
    outt(" СИН, если необходимо, можно cформировать до текущего СИН-срока.", 'scr')
    cl.setdefcolors()
    outt(" СИН будет помещён в отложенную отправку.", 'scr')
    outt("", 'scr')
    outt("    '[3]Отл. СИН'    - Просмотр(удаление) отложенных СИН", 'scr')
    outt("    '[4]Контр. отпр' - Контроль автоматической отправки", 'scr')
    cl.setcolors(LYELLOW)
    outt("", 'scr')
    outt(" СИН будет отправлен автоматически в ближайший СИН-срок! Будьте внимательны.", 'scr')
    cl.setdefcolors()

def mshelp():
    infoline("Справка", 'scr')
    inform()
    outt("Меню:", 'scr')
    outt(" [1] - Отправка/подготовка СИН", 'scr')
    outt(" [2] - Отправка климатического сообщения", 'scr')
    outt(" [3] - Просмотр/Удаление отложенной отправки СИН", 'scr')
    outt(" [4] - Контроль отложенной отправки СИН", 'scr')
    outt(" [5] - Список файлов каталога отправки/приёма на АСПД", 'scr')
    outt(" [6] - Удалить последний отправленный СИН с сервера АСПД", 'scr')
    outt(" [7] - ФАП", 'scr')
    outt(" [s] - Переход в коммандную строку", 'scr')
    outt(" [h] - Эта справка", 'scr')
    outt(" [x] - Выход из программы отправки СИН", 'scr')

def msgnextsin():
    TIMESIN=conf.getvalue('TIMESIN')
    cl.setcolors(LGREEN)
    outt(TIMESIN, 'scr')
    cl.setdefcolors()
    
def mlhelp():
    msgnextsin()
    cl.setcolors(BLACK, BGGRAY)
    outt("Выход 'x' или 'Ctrl-C Выход из Shell: 'exit' Cправка: 'h' ", 'scr')
    cl.setdefcolors()

def tip():
    cl.setcolors(LGREEN)
    outt("[3]Отл.СИН-Проверить. [4]'Контроль'-Ход отправки. Отправка состоялась: [5]Запрос / [6]Удалить", 'scr')
    cl.setdefcolors()

def sending(TYPELETTER=''):
    msgll('meteo'); 
    msgll('delay');

    for case in switch(TYPELETTER):
        if case('avos', 'AVOS'):
            RETCHS=MChdelay() #==> $RETCHS
            for case in switch(RETCHS):
                if case(1, 3):
                    pass
                    break
                if case(0, 2):
                    if not tmcheckavos('avos'):
                        answ=youchoice("Cоздать СИН-сообщение?", cl.getcolor('TBCOLOR'))
                        if answ:
                            msglt('avos')
                        elif not answ:
                            outt('Отмена', 'scr')
                    else: 
                        msglt('avos')
                    break
            break
        if case('climat', 'CLIMAT'):
            if not tmcheckavos('climat'):
                pass
            else:
                msglt('climat')
            break
        if case('awr', 'AWR'):
            msglt('grp', 'ФАП', 'awr')
            break
        else: 
            pass
  
    if os.path.isfile(LASTMT): 
        LASTFILESEND=readfirstline(LASTMT)

def checkdelay(okeys=''):
    ret=False
    if os.path.isfile(SIGN_DELAY):
        msg=readfirstline(SIGN_DELAY)
        cl.setcolors(LRED)
        outt("Есть отложенный СИН! "+msg)
        if okeys=="OUTKEY": tip()
        cl.setdefcolors()
        ret=True
    return ret

def MChdelay():
    RETCHS=0
    if checkdelay():
        outt('Проверьте введённые данные:', 'scr')
        print(readfirstline(LDIRDELAY+'/avos.txt'))
        WRK=True
        while WRK:
            outt('1 - Переключиться на контроль отправки СИН', 'scr')
            outt('2 - Удалить отложенный и создать новый СИН', 'scr')
            outt('3 - Вернуться в Меню', 'scr')
            chm=int(input("?==> "))
            for case in switch(chm):
                if case(1):
                    delaysendcontrol()
                    RETCHS=1 
                    WRK=False
                    break;
                if case(2):
                    delaycancel() 
                    RETCHS=2
                    WRK=False
                    break
                if case(3):
                    RETCHS=3
                    WRK=False 
                    break
                else:
                    outt("Неизвестный ответ", 'scr');
    
    return RETCHS

def MainMenu():
    tmenu=''
    key=list()
    word=list()

    TMH=int(getdatetimeUTC('%H'))
    TMM=int(getdatetimeUTC('%M'))
    settmsin(TMH, TMM)
    TIMESIN=conf.getvalue('TIMESIN')
    
    checkdelay("OUTKEY")  
    mlhelp()
   
    BDCL=DGRAY+BGCYAN 
    
    if TIMESIN.find('Текущий')!=-1 and os.path.isfile(LASTMT): BDCL=LYELLOW+BGRED
    
    key.append('1'); word.append('СИН'); key.append('2'); word.append('Климат'); key.append('3'); word.append('Отл.СИН'); 
    key.append('4'); word.append('Контр.отпр'); key.append('5'); word.append('Запрос'); key.append('6'); word.append(BDCL+'Удалить!'); 
    key.append('7'); word.append('ФАП'); key.append('S'); word.append('hell');   key.append('H'); word.append('Справка'); 
    key.append('X'); word.append('Выход'); 
    
    count=len(key)
    KTCL=BWHITE; KBCL=BGBLACK; TNCL=BLACK; BNCL=BGCYAN;
    
    fmt=KTCL+KBCL+"{:1s}"+TNCL+BNCL+"{:3s}"
    for i in range(count):
        tmenu+=fmt.format(key[i], word[i])    

    dt=2
    if WORKTYPE=="debug": dt=5
    spacefil="░"+version+'░'+"░"*(conf.getvalue('WSLZ')-strlen(tmenu+version+getdatetime())-dt)
    #Show menu
    infoline(tmenu+spacefil, 'scr')

def delaycancel():
    msgll('delay')

    if os.path.isfile(SIGN_DELAY):
        outt("Содержимое отложенного СИН:")
        print(readfirstline(LDIRDELAY+'/avos.txt'))
        
        if youchoice("Удалить отложенный СИН?"):
            msg=readfirstline(SIGN_DELAY)
            outt("СИН. "+msg+". Удаление!")
            rmdelay()
        else:
            return;
    else:
        cl.setcolors(LYELLOW)
        outt("Отправка не была назначена.")
      
def delaysendcontrol():
    msgll('delay')

    if os.path.isfile(SIGN_DELAY):
        if not youchoice("Перейти на контроль отправки?"): return
        msg="Контроль отправки."    
        infoline(msg, 'log')
        msg=readfirstline(SIGN_DELAY)
        infoline("Отложенный СИН: "+msg)
        cl.setcolors(LYELLOW)
        outt("После отправки СИН произойдёт возврат в Меню", 'scr')
        infoline("Ожидайте сообщений об отправке...", 'scr')
        shellcmd('clear')
        shellcmd('aspdtail -f -n 11 '+ASPDLOG)
    else:
        shellcmd('aspdtail -n 3 '+ASPDLOG)
        cl.setcolors(LYELLOW)
        outt('Отправка состоялась или не была назначена', 'scr')
        outt('[5]Запрос - Каталог АСПД', 'scr')
        cl.setdefcolors()
    
def listing(view="NO"):
    if view!="NO": shellcmd('clear')
    infoline("Каталоги отправки/приёма") 
    msgll('meteo') 
    msgll('delay') 
    cl.setcolors(LGREEN)
    outt("Запрос каталогов сервера АСПД может занять некоторое время.", 'scr')
    outt("Подождите...", 'scr')
    cl.setdefcolors()
    msgrl('/aspd')
    
def close():
    txt="Программа отправки метео-данных завершена."
    outt(txt); rita(txt)

def deletelast():
    if os.path.isfile(LASTMT):
        LASTFILESEND=readfirstline(LASTMT)
        if LASTFILESEND:
            cl.setcolors(LYELLOW, BGRED)
            outt("Удаление "+LASTFILESEND+" !!!")
            cl.setdefcolors()
            msgdel("/aspd/"+LASTFILESEND)
        if os.path.isfile(LASTMT):
            shellcmd('sudo rm '+LASTMT)
    else:
        cl.setcolors(LYELLOW)
        outt("Удалять нечего.", 'scr') 
        cl.setdefcolors()
        listing("NO")

def main():
    txt='Oтправкa метео данных. Пользователь: '+os.environ.get('LOGNAME')
    infoline(txt); rita(txt)
    
    if os.environ.get('SSH_TTY')!=None: infoline('CONNECTION='+os.environ.get('SSH_CONNECTION')+' TTY='+os.environ.get('SSH_TTY'))
    inform()
    MENU=True

    WRK=True
    while WRK:       
        if MENU: MainMenu()
        
        MENU=True
        chs=input('?=>')
        
        for case in switch(chs):
            if case('h', 'H'):
                mshelp()
                break
            if case('1'):
                shellcmd('clear')
                sending('avos')
                break
            if case('2'):
                shellcmd('clear')
                sending('climat')
                break
            if case('3'):
                rita('Отмена отложенного СИН')
                shellcmd('clear')
                delaycancel()
                break
            if case('4'):
                rita('Контроль отправки')
                shellcmd('clear')
                delaysendcontrol()
                break
            if case('5'):
                rita('Запрос каталогов')    
                shellcmd('clear')
                listing()
                break
            if case('6'):
                rita('Удаление последней отправки')
                if LASTMT:
                    deletelast()
                else:
                    MENU=False
                break
            if case('7'):
                rita('Отправка ФАП')
                shellcmd('clear')
                sending('awr')
                break
            if case('s', 'S'):
                rita('Переход в шелл')
                mlhelp()
                shellcmd('zsh') #, '/dev/stdout', '>', 'ORDER', '/bin/zsh')
                break
            if case('x', 'q', 'X', 'Q', 'exit', 'EXIT'):
                rita("Метео отправка завершена")
                WRK=False
                if os.path.isfile(LASTMT):
                    shellcmd('sudo rm -f "'+LASTMT+'"')
                break
            else:
                MENU=False
                break
                
if __name__=='__main__':
    locale.setlocale(locale.LC_ALL, "ru_RU.UTF-8")
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        sys.exit(0)
    finally:
        CLOSE()
        
