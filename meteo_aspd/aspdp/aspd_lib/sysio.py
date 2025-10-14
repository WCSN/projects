'''
Created on Jul 11, 2016

@author: wocson
'''

import glob
import os, locale, sys
import re, subprocess
import termios, tty

from aspd_lib.config import conf
import chardet

# from overloading import overloads, overloaded, overload
# Дополнительные свойства для текcта:
BOLD='\033[1m'              #  ${BOLD}      # жирный шрифт (интенсивный цвет)
DBOLD='\033[2m'             #  ${DBOLD}    # полу яркий цвет (тёмно-серый, независимо от цвета)
NBOLD='\033[22m'            #  ${NBOLD}    # установить нормальную интенсивность
UNDERLINE='\033[4m'         #  ${UNDERLINE}  # подчеркивание
NUNDERLINE='\033[4m'        #  ${NUNDERLINE}  # отменить подчеркивание
BLINK='\033[5m'             #  ${BLINK}    # мигающий
NBLINK='\033[5m'            #  ${NBLINK}    # отменить мигание
INVERSE='\033[7m'           #  ${INVERSE}    # реверсия (знаки приобретают цвет фона, а фон -- цвет знаков)
NINVERSE='\033[7m'          #  ${NINVERSE}    # отменить реверсию

# Reset
STDCL='\033[0m'             # все атрибуты по умолчанию
NORMAL=STDCL            

# Regular Colors
BLACK='\033[0;30m'        # Black
RED='\033[0;31m'          # Red
GREEN='\033[0;32m'        # Green
YELLOW='\033[0;33m'       # Yellow
BROWN='\033[0;33m'        # Yellow
BLUE='\033[0;34m'         # Blue
PURPLE='\033[0;35m'       # Purple
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'         # Cyan
GRAY='\033[0;37m'         # ${GRAY}      # серый цвет знаков
DGRAY='\033[1;30m'        # ${DGRAY}

# High Intensity
LBLACK='\033[0;90m'       # Black
LRED='\033[0;91m'         # Red
LGREEN='\033[0;92m'       # Green
LYELLOW='\033[0;93m'      # Yellow
LBLUE='\033[0;94m'        # Blue
LPURPLE='\033[0;95m'      # Purple
LMAGENTA='\033[0;95m'     # Magenta
LCYAN='\033[0;96m'        # Cyan
WHITE='\033[0;97m'        # White

# Bold
BBLACK='\033[1;30m'       # Black
BRED='\033[1;31m'         # Red
BGREEN='\033[1;32m'       # Green
BYELLOW='\033[1;33m'      # Yellow                      
BBLUE='\033[1;34m'        # Blue                        
BPURPLE='\033[1;35m'      # Purple                      
BMAGENTA='\033[1;35m'     # Magenta                     
BCYAN='\033[1;36m'        # Cyan
BWHITE='\033[1;37m'       # White

# Bold High Intensity
BIBLACK='\033[1;90m'      # Black
BIRED='\033[1;91m'        # Red
BIGREEN='\033[1;92m'      # Green
BIYELLOW='\033[1;93m'     # Yellow
BIBLUE='\033[1;94m'       # Blue
BIPURPLE='\033[1;95m'     # Purple
BIMAGENTA='\033[1;95m'    # Magenta
BICYAN='\033[1;96m'       # Cyan
BIWHITE='\033[1;97m'      # White

# Underline
UBLACK='\033[4;30m'       # Black
URED='\033[4;31m'         # Red
UGREEN='\033[4;32m'       # Green
UYELLOW='\033[4;33m'      # Yellow
UBLUE='\033[4;34m'        # Blue
UPURPLE='\033[4;35m'      # Purple
UMAGENTA='\033[4;35m'     # Magenta
UCYAN='\033[4;36m'        # Cyan
UWHITE='\033[4;37m'       # White

# Background
BGBLACK='\033[40m'       # Black
BGRED='\033[41m'         # Red
BGGREEN='\033[42m'       # Green
BGYELLOW='\033[43m'      # Yellow
BGBROWN='\033[43m'       # ${BGBROWN}
BGBLUE='\033[44m'        # Blue
BGPURPLE='\033[45m'      # Purple
BGMAGENTA='\033[45m'     # MAGENTA
BGCYAN='\033[46m'        # Cyan
BGWHITE='\033[47m'       # White
BGGRAY='\033[47m'        # ${BGGRAY}
BGDEF='\033[49m'         # ${BGDEF}

# High Intensity backgrounds
HIBGBLACK='\033[0;100m'   # Black
HIBGRED='\033[0;101m'     # Red
HIBGGREEN='\033[0;102m'   # Green
HIBGYELLOW='\033[0;103m'  # Yellow
HIBGBLUE='\033[0;104m'    # Blue
HIBGPURPLE='\033[0;105m'  # Purple
HIBGMAGENTA='\033[0;105m' # Magenta
HIBGCYAN='\033[0;106m'    # Cyan
HIBGWHITE='\033[0;107m'   # White

REGSS='\033*m'

## SWITCH
class switch(object):

    def __init__(self, value):
        self.value=value  # значение, которое будем искать
        self.fall=False   # для пустых case блоков

    def __iter__(self):     # для использования в цикле for
        """ Возвращает один раз метод match и завершается """
        yield self.match
        raise StopIteration

    def match(self, *args):
        """ Указывает, нужно ли заходить в тестовый вариант """
        if self.fall or not args:
            # пустой список аргументов означает последний блок case
            # fall означает, что ранее сработало условие и нужно заходить 
            #   в каждый case до первого break
            return True
        elif self.value in args:
            self.fall=True
            return True
        return False
## SWITCH END

def clean_escape(srcstr):
    ansi_escape=re.compile(r'(?:\x1B[@-_]|[\x80-\x9F])[0-?]*[ -/]*[@-~]')
    return ansi_escape.sub('', srcstr)
    
def strlen(string):
    return len(clean_escape(string))

def replacepos(srcstr, insstr='', pos=0, typereplace="OVER"): # num position on symbol first 0

    if typereplace=="OVER": sign=1
    else: sign=-1
    pos-=1
    shift=sign*strlen(insstr)
    return srcstr[:pos]+insstr+srcstr[pos+shift:]

def replacenum(srcstr, substr, replstr, num=0): # num номер по счёту для замены
    buff=srcstr.split(substr)
    if (strlen(substr)-1)<=num:
        return srcstr
    return substr.join(buff[:num])+replstr+substr.join(buff[num:])

def replaceone(srcstr, substr, replstr, pos=0): # num position on symbol first 0
    return srcstr[:pos]+srcstr[pos:].replace(substr, replstr, 1)
    #=======================================================================================================
    # s  = 'test test test'
    # result s[:5]+s[5:].replace('test','TEST',1)
    # #'test TEST test'
    #=======================================================================================================

def cprint(param):
    print (param, end='')

def getposxy():
    buf=''
    stdin=sys.stdin.fileno()
    tattr=termios.tcgetattr(stdin)

    try:
        tty.setcbreak(stdin, termios.TCSANOW)
        sys.stdout.write("\x1b[6n")
        sys.stdout.flush()

        while True:
            buf+=sys.stdin.read(1)
            if buf[-1]=="R":
                break

    finally:
        termios.tcsetattr(stdin, termios.TCSANOW, tattr)

    # reading the actual values, but what if a keystroke appears while reading
    # from stdin? As dirty work around, getpos() returns if this fails: None
    try:
        matches=re.match(r"^\x1b\[(\d*);(\d*)R", buf)
        groups=matches.groups()
    except AttributeError:
        return None

    return (int(groups[1]), int(groups[0]))

def gotoxy(x, y):
    sys.stdout.write('\033['+str(y)+';'+str(x)+'f')

def outtextxy(txt, x=0, y=0):
    gotoxy(x, y); 
    print(txt, end='')

def inputxy(x, y, string=''):
    gotoxy(x, y); print('   ', end='')
    gotoxy(x, y); inp=input(string)
    return inp

def opt():
    argc=len(sys.argv)
    print ('ARGC      :', argc)

    if argc==1:
        return 1

    print ('ARGV      :', sys.argv[1:])

    words=(sys.argv[1:])[0].split(':')
    print (words)

def lsdir(dpath='/', mask='*'):
    return glob.glob1(dpath, mask)

def sumdirsize(pathdir):
    from pathlib import Path
    # size = sh.du("-s", ".") 
    size=sum([f.stat().st_size for f in Path(pathdir).glob("**/*")])
    return size

def readfile(filename=''):
    if os.path.isfile(filename):
        try:
            with open(filename) as flr:
                fc=flr.read()
                return fc
        except IOError:
            return "Файл "+filename+" не прочитан"
    else:
        return "Файл "+filename+" не прочитан"

def readfirstline(filename=''):
    if os.path.isfile(filename):
        try:
            file=open(filename) 
            fl=file.readline()
            file.close()
            return fl
        except IOError:
            return "Файл "+filename+" не прочитан"
    else:
        return "Файл "+filename+" не найден"
    
def getfnamefpath(fullpath):
    return os.path.split(fullpath)[1]

def getfpathfpath(fullpath):
    return os.path.split(fullpath)[0]

def StrUTF8(bstr): #bytes
    locale.setlocale(locale.LC_ALL, "ru_RU.UTF-8")
    if len(bstr)==0: return str(bstr).replace("b'", '').replace("'", '')
    enc=chardet.detect(bstr).get('encoding')
    if enc!=None:
        if enc.lower()=='ascii':
            cnvstr=str(bstr)
        elif enc.lower()=='windows-1253':
            cnvstr=bstr.decode('cp866')
        elif enc.lower()=='utf-8':
            cnvstr=bstr.decode('utf-8')
        elif enc.lower()!='utf-8':
            cnvstr=bstr.decode(enc)
        else:
            cnvstr=str(bstr)

    return cnvstr.replace("b'", '').replace("'", '')
    
def converter(filePath):
    locale.setlocale(locale.LC_ALL, "ru_RU.UTF-8")
    with open(filePath, "rb") as F:
        text=F.read()
        enc=chardet.detect(text).get("encoding")
        if enc and enc.lower()!="utf-8":
            try:
                text=text.decode(enc)
                text=text.encode("utf-8")
                with open(filePath, "wb") as f:
                    f.write(text)
                print ("%s сконвертирован."%filePath)
            except:
                print ("Ошибка в имени файла: название содержит русские символы.")
            else :
                print ("Файл %s находится в кодировке %s и не требует конвертирования."%(filePath, enc))

def isvar(chvar=''):
    if chvar in globals():
        return True
    elif chvar in locals():
        return True
    return False

#===================================================================================================================================
# Проверка существования ключа в словаре способ #1
# 
# if key in dictname:
# # key exists in dictname
# 
# 
# Проверка существования переменной в локальной видимости
# 
# if 'myVar' in locals():
# # myVar exists
# 
# 
# Проверка существования переменной в глобальной видимости
# 
# if 'myVar' in globals():
# # myVar exists
# 
# 
# Проверка существования переменной используя исключения
# 
# try:
# myVar
# except NameError:
# myVar = None
# 
# 
# Проверка существования ключа в словаре способ #2
# 
# try:
# dictname['key']
# except KeyError:
# dictname['key'] = None
# 
# 
# Проверка существования индекса в списке
# 
# try:
# dictname['1']
# except IndexError:
# dictname['1'] = None
# 
# 
# Проверка наличия метода, свойства в объекте
# 
# if hasattr(obj, 'attr_name'):
# # obj.attr_name exists
#===================================================================================================================================

def psecho(prompt, tcolor=GRAY, bcolor=BGBLUE):
    locale.setlocale(locale.LC_ALL, "ru_RU.UTF-8")
    sys.stdout.write(tcolor+bcolor+prompt+STDCL)
    return StrUTF8(bytes(input(), 'utf-8')).strip()
    
def youchoice(prompt, tcolor=GRAY, bcolor=BGBLUE):
    choice=True
    while True:
        sys.stdout.write(tcolor+bcolor+prompt+"(y/n): "+STDCL)
        chs=input().strip(); sys.stdout.write(STDCL)
        if chs in ('y', 'yes', 'Y', 'YES'): choice=True; break
        if chs in ('n', 'no', 'N', 'NO'): choice=False; break

    return choice

def isdirnotempty(dirpath="./"):
    if os.path.isdir(dirpath):
        dir_contents=[x for x in os.listdir(dirpath) if not x.startswith(dirpath)]
        if len(dir_contents)>0:
            return True
        else:
            return False
    return False

def setpermfile(filename):
    subprocess.Popen('sudo chmod -f '+conf.permfile+' '+filename, executable='/bin/bash', stdout=conf.NULL, stderr=conf.NULL, shell=True, env=conf.nenv)
    
def shellcmd(cmd, repfile='/dev/stdout', mode='>', pipetype='ORDER', sh='/bin/bash'):
    '''
        shellcmd(cmd, filelog, mode, typepipe='ORDER') - command store in pipeline
        shellcmd(cmd, filelog, mode, typepipe='ASYNC') - command run now
    '''
    ret=-333
    if mode=='>': mode='w'
    elif mode=='>>': mode='a'
    
    file=open(repfile, mode)
    if file!=None: 
        if pipetype=='ORDER':
            ret=subprocess.Popen(cmd, executable=sh, stdout=file, stderr=file, shell=True, env=conf.nenv).wait()
        else:
            ret=subprocess.Popen(cmd, executable=sh, stdout=file, stderr=file, shell=True, env=conf.nenv)
        if repfile.find('/dev/')==-1:
            setpermfile(repfile)
        file.close()
    return ret

def write2log(filename, outtext=''):
    with open(filename, 'a') as file:
        file.write(outtext+'\n')
    file.close()
    setpermfile(filename)

def write2dbf(filename, outtext=''):
    with open(filename, 'w') as file:
        file.write(outtext)
    file.close()
    setpermfile(filename)

def write2file_str(filename: str, outtext: str=''):
    with open(filename, 'a') as file:
        file.write(outtext)
#        file.write('\r\n')
    file.close()
    setpermfile(filename)

def write2file_list(filename: str, outlist: list=list()):
    with open(filename, 'a') as file:
        for outline in outlist:
            file.write(outline)
#        file.write('\r\n')
    file.close()
    setpermfile(filename)
