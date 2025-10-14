'''
Created on 19 сент. 2019 г.

@author: wocson
'''
################################################################
# #
# # HELP
# #
# # usage out screen help
# #
# # void usage typehelp
# #

import sys
from aspd_lib.config import conf
from aspd_lib.sysio import *

VERSION="1.185"
Requared="sudo mc mcedit lftp mutt rar 7z zip"
Last="rita correct"

_HLPCMD=[];_HLPTXT=[]; _HLPNOTE=[]

_HLPCMD.append("send|se|s [/lpath/file] [rpath] [--size=]")
_HLPTXT.append("Oтправка сообщений для АСПД\n\t--size - размер части архива")
_HLPCMD.append("recv|re|r [/rpath/file] [lpath]")
_HLPTXT.append("Приём сообщений от АСПД.")
_HLPCMD.append("del|dl|d [/rpath/rfile]")
_HLPTXT.append("Удаление файлов из каталогов сервера АСПД. \n "+LYELLOW+"Внимание:"+BIRED+" Осторожно! Не удалите чужие файлы!"+STDCL)
_HLPCMD.append("email|em|m e-mail [-s [subj]] [-t [text]] [-a [attach]] [-f [filemail(txt)]]")
_HLPTXT.append("\n\tОтправка e-mail.")
_HLPCMD.append("letter|lt|e [type letter]"+STDCL+" или "+WHITE+"grp [ГРУППА] [тип письма]")
_HLPTXT.append("Создание письма.\n\t"+STDCL+"тип письма:"+WHITE+" avos climat awr vos svc vrs\n\t"+STDCL+"Выберите тип и адрес. При выборе"+CYAN+" grp"+STDCL+" без указания типа формируется письмо типа "+CYAN+"vos"+STDCL)
_HLPCMD.append("setperm|sp|p [-v]")
_HLPTXT.append("Установка прав на рабочие каталоги.\n\t"+WHITE+"-v"+STDCL+" - подробно")
_HLPCMD.append("rlist|rl|i [rpath]")
_HLPTXT.append("Проверка состояния каталогов удалённого сервера.\n\
\tEсли [rpath] не задан выдаётся содержимое каталогов, определённых в конфигурационном файле"+CYAN+" aspd.conf "+STDCL)
_HLPCMD.append("llist|ll|l [/lpath|meteo|send|recv|delay]")
_HLPTXT.append("Cписок файлов по локальным каталогам отправки/приёма\n\
\tЕсли [/lpath/[file]] не задан выдаётся содержимое каталогов хранения, определённых в конфигурационном файле"+CYAN+" aspd.conf"+STDCL+"\n\
\tили\n\
\t - meteo - метео отправки\n\
\t - delay - отложенные СИН\n\
\t - send  - vos,svc,vrs отправки\n\
\t - recv  - принятые файлы\n\
\tКаталоги задаются в"+CYAN+" aspd.conf"+STDCL)
_HLPCMD.append("rita|rt|t [текст]")
_HLPTXT.append("Голосовое сообщение.")
_HLPCMD.append("abook|ab|a")
_HLPTXT.append("Вывод адресов отправки. Определяется в"+CYAN+" abook.d"+STDCL)
_HLPCMD.append("stat|st|u")
_HLPTXT.append("Вывод статистики")
_HLPCMD.append("ver|v")
_HLPTXT.append("Вывод версии")
_HLPCMD.append("rmdl")
_HLPTXT.append("Удалить отложенные отправки")
_HLPCMD.append("getcfg|gf|g")
_HLPTXT.append("Получить значение конфигурационного праметра для сторонней программы")
_HLPCMD.append("help|h [command]")
_HLPTXT.append("Эта справка")

_HLPNOTE.append(LYELLOW+" Файлы:"+WHITE+" aspd.py"+STDCL+"       - главный рабочий файл. Вызывается "+CYAN+"aspd"+STDCL)
_HLPNOTE.append(WHITE+"\taspd.conf"+STDCL+"     - конфигурационный файл")
_HLPNOTE.append(WHITE+"\taspd_sys.py"+STDCL+"   - основные функции управления")
_HLPNOTE.append(WHITE+"\taspd_iface.py"+STDCL+" - интерфейсные функции")
_HLPNOTE.append(WHITE+"\taspd_dt.py"+STDCL+"    - функции времени")
_HLPNOTE.append(WHITE+"\tconfig.py"+STDCL+"     - обработчик конфигурационного файла")
_HLPNOTE.append(WHITE+"\thelp.py"+STDCL+"       - функции справки")
_HLPNOTE.append(WHITE+"\tinter.py"+STDCL+"      - модуль интерактивного режима")
_HLPNOTE.append(WHITE+"\tsysio.py"+STDCL+"      - функции ввода-выбода")
_HLPNOTE.append(WHITE+"\tsysut.py"+STDCL+"      - функции ввода-выбода TCP/IP")
_HLPNOTE.append(WHITE+"\twinlib.py"+STDCL+"     - интерфейсные функции интерактивного режима")

_HLPNOTE.append(LYELLOW+" Внимание:"+STDCL+" 1. Если параметры комманд не указаны, то приём/отправка/выполнение")
_HLPNOTE.append("\tидут по параметрам определённым в"+CYAN+" aspd.conf"+STDCL)
_HLPNOTE.append("\t2. root: Установить в"+CYAN+" sudoers (sudo)"+STDCL+":\n"+CYAN+"\tCmnd_Alias CMD=/usr/bin/aspd,/usr/local/bin/mcedit,/usr/local/bin/mcview,\ \n\t/bin/chmod,/usr/bin/touch,/bin/mkdir,/bin/cp,/bin/mv,/bin/rm,/usr/sbin/chown,service,squid\n\t%aspd ALL=NOPASSWD: CMD"+STDCL)
_HLPNOTE.append("\t3. Требуются: "+CYAN+Requared+STDCL)

HLPCMD=tuple(_HLPCMD);    del _HLPCMD; 
HLPTXT=tuple(_HLPTXT);    del _HLPTXT;
HLPNOTE=tuple(_HLPNOTE);  del _HLPNOTE;

def unknown(typehelp=''):
    TYPEHELP=typehelp
    if TYPEHELP!='':
        print(" Команда "+WHITE+TYPEHELP+STDCL+" не известна.")
        print(" Справка:"+WHITE+" aspd help [command]"+STDCL)

def mhelp(typehlp='help'):
    TXT=""; ih=0;
  
    if typehlp in ('send', 'se', 's'):
        ih=0; TXT="Ex.:\taspd send\n\taspd s ./src/fileto.send /rae/bel --size=2M\
\n Ex.:\t--size=7M | =512k | =777G"
    elif typehlp in ('recv', 're', 'r'):
        ih=1; TXT="Ex.:\taspd recv\n\taspd r  /rae/vos/file4.recv ./src/"
    elif typehlp in ('del', 'dl', 'd'):
        ih=2; TXT="Ex.:\taspd del /rae/vos/fileto.del"
    elif typehlp in ('email', 'em', 'm'):
        ih=3;
        TXT='Ex.:\n 1. aspd email vostok.station@yandex.ru -a "./test_vos" -t "Текст письма" -s "Заголовок письма"\n\
 2. aspd email vostok.station@yandex.ru --attach="./test_vos.txt --text="Текст письма" --subj="Заголовок письма"'
    elif typehlp in ('letter', 'lt', 'e'):
        ih=4; TXT="Ex.:\taspd letter\n\taspd lt vos\n\taspd lt avos"
    elif typehlp in ('setperm', 'sp', 'p'):
        ih=5; TXT="Ex.:\taspd setperm\n\taspd sp -v"
    elif typehlp in ('rlist', 'rl', 'i'):
        ih=6; TXT="Ex.:\taspd rlist\n\taspd rl -a\n\taspd rl /rae/vos"
    elif typehlp in ('llist', 'll', 'l'):
        ih=7; TXT="Ex.:\taspd llist\n\taspd ll ./src"
    elif typehlp in ('rita', 'rt', 't'):
        ih=8;
        TXT='Ex.:\taspd rita "Привет мир"'
    elif typehlp in ('abook', 'ab', 'a'):
        ih=9; TXT="Ex.:\taspd abook\n\taspd ab\n\taspd a"
    elif typehlp in ('stat', 'st', 'u'):
        ih=10; TXT="Ex.:\taspd status"
    elif typehlp in ('ver', 'v'):
        ih=11; TXT="Ex.:\taspd ver"
    elif typehlp in ('rmdl'):
        ih=12; TXT="Ex.:\taspd rmdl"
    elif typehlp in ('getcfg', 'gf', 'g'):
        ih=13; TXT='Ex.:\taspd getcfg "NAME_CFG_PAR"'
    elif typehlp in ('help', 'h'):
        usage()
        ih=14; TXT="Ex.:\taspd help rita"
    else:
        unknown(typehlp); return;
        
    print(WHITE+" aspd {0:s} - {1:50s}".format(HLPCMD[ih]+STDCL, HLPTXT[ih]))
    print('{:s}'.format(" "+TXT))

def usage():
   
    print ("\n"+WHITE+" aspd"+STDCL+" Отправка и приём сообщений АСПД")
    print (" Использование:"+WHITE+" aspd [комманда] [параметр]"+STDCL)
    print (" Справка:"+WHITE+" aspd help [комманда]"+STDCL)
    print ("\n Комманды:")
  
    i=0; line=""
    for line in HLPCMD:
        line=WHITE+line+STDCL
        print(" {0:<30s} - {1:40s}".format(line, HLPTXT[i]))
        i+=1
  
    for line in HLPNOTE:
        print(line)
    print('')    
    version()
 
def version():
    print('{:s}{:{align}{width}}{:s}'.format(WHITE+BGBLUE, 'asdp v'+VERSION+' (c) wocson (python '+sys.version.split()[0]+')', STDCL, align='>', width=conf.getvalue('WSLZ')))
