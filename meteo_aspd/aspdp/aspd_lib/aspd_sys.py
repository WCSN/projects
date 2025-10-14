'''
    apsd exchange
'''
import time

from aspd_lib.aspd_dt import *
from aspd_lib.aspd_iface import cl
from aspd_lib.config import conf
from aspd_lib.help import VERSION, mhelp
from aspd_lib.sysio import *
from docutils.nodes import line
from humanize import naturalsize
import numpy as num

WORKTYPE=conf.getvalue('WORKTYPE')
WLLZ=int(conf.getvalue('WIDESCREEN'))
WSLZ=int(conf.getvalue('WSLZ'))

def wprint(text, colors, typeout='all'):
    ''' out test to log or/and screen '''
    fmt="{:{lwidth}s}{:s}"
    if typeout=="all" or typeout=="log":
        write2log(conf.getvalue('LOG'), colors+fmt.format(text[0], text[1], lwidth=WLLZ-strlen(text[1]))+STDCL)
    if typeout=="all" or typeout=="scr":
        print(colors+fmt.format(text[0], text[1], lwidth=WSLZ-strlen(text[1]))+STDCL)

def outt(outtext='', typeout='all'):
    ''' void outt('text',typeout) '''
    wprint((outtext, ''), cl.getcolor('TBCOLOR')+cl.getcolor('BBCOLOR'), typeout)

def infoline(outtext='', typeout='all'):
    ''' void infoline('text',typeout) '''
    DT=getdatetime()
    if WORKTYPE=='debug': DT="░D░"+DT
    wprint((outtext, DT), cl.getcolor('TBTCOLOR')+cl.getcolor('TBBCOLOR'), typeout)
    cl.setdefcolors()

def abook():
    '''
        abook() show a.book void abook void
    '''
    conf.getadrbook()
    i=n=0; ppz=3; adz=conf.XLENA; ptz=conf.XPATH;

    infoline("Адресная книга", 'scr')
    lz=WSLZ-(ppz+adz+ptz+5)
    topl="┌"+'─'*ppz+'┬'+'─'*adz+'┬'+'─'*ptz+'┬'+'─'*lz+'┐';
    outt(topl, 'scr')

    BBCOLOR=cl.getcolor('BBCOLOR')
    PPCL=LYELLOW+BBCOLOR; ADRCL=LYELLOW+BBCOLOR; PTCL=LYELLOW+BBCOLOR; CMTCL=LYELLOW+BBCOLOR; DEFCL=GRAY+BBCOLOR;
    fmtA='│'+PPCL+'{:{align}{num}s}'+DEFCL+'│'+ADRCL+'{:{adr}s}'+DEFCL+'│'+PTCL+'{:{path}s}'+DEFCL+'│'+CMTCL+'{:{rem}s}'+DEFCL+'│'
    lnif=fmtA.format("#", " АДРЕССАТ", " КАТАЛОГ АСПД/e-mail", " Примечание", align='^', num=ppz, adr=adz, path=ptz, rem=lz)
    outt(lnif, 'scr')
    # midl=topl.replace('┌', '├').replace('┬', '┼').replace('┐', '┤'); outt(midl,'scr')

    ADRCL=PTCL=CMTCL=WHITE+BBCOLOR; GRPCL=BICYAN+BBCOLOR;

    fmtA='│'+PPCL+'{:{align}{num}s}'+DEFCL+'│'+GRPCL+'{:{adr}s}'+DEFCL+'│'+PTCL+'{:{path}s}'+DEFCL+'│'+CMTCL+'{:{rem}s}'+DEFCL+'│'
    fmtG=fmtA.replace('│', '┼'); fmtG='├'+fmtG[1:(len(fmtG)-1)]+'┤'

    ADRCL=WHITE+BBCOLOR; PTCL=LCYAN+BBCOLOR; CMTCL=GRAY+BBCOLOR; GRPCL=BICYAN+BBCOLOR;
    fmtA='│'+PPCL+'{:{align}{num}s}'+DEFCL+'│'+ADRCL+'{:{adr}s}'+DEFCL+'│'+PTCL+'{:{path}s}'+DEFCL+'│'+CMTCL+'{:{rem}s}'+DEFCL+'│'

    for i in range(conf.COUNTGRADR):
        # group
        if conf.WABLNS[i][0]=='@':
            lnif=fmtG.format('─'*ppz, conf.WABLNS[i].replace('@', ' ')+ADRCL+' '+'─'*(adz-len(conf.WABLNS[i])-1), '─'*ptz, '─'*lz, align='^', num=ppz, adr=adz, path=ptz, rem=lz)
            outt(lnif, 'scr')
        # adress
        else:
            lnif=fmtA.format(str(n+1), conf.WADRESS[n], conf.WFTPPTH[n], conf.WCOMNTS[n][0:lz], align='^', num=ppz, adr=adz, path=ptz, rem=lz)
            outt(lnif, 'scr')
            n+=1
    dwnl=topl.replace('┌', '└').replace('┬', '┴').replace('┐', '┘')
    outt(dwnl, 'scr')
    status()

def updatesessionofday(FILEINDEX):
    '''
        updatesessionofday(file)
        update session of day
    '''
    NS=1
    DATE=getdatetime('%d%m')  # get local date
    if os.path.isfile(FILEINDEX):
        with open(FILEINDEX, "r") as file:
            for line in file:
                tmp=line.split(' ')
                SAVEDATE=tmp[0].split(':')[1]
                if DATE==SAVEDATE:
                    NS=int(tmp[1].split(':')[1])
                    NS+=1

    conf.setvalue('NUMSESSOFDAY', NS)
    record="Date:"+DATE+" N:"+str(NS)+" Sess:"+str(NS)
    write2dbf(FILEINDEX, record)

def msgsendnumber():
    '''
        msgsendnumber()
        show and update number of send
    '''
    updatesessionofday(conf.getvalue('INDEX_SND'))
    numsessofday=conf.getvalue('NUMSESSOFDAY')
    txt="Отправка на А эС Пэ Дээ номер "+str(numsessofday)
    txt2="Отправка. АСПД. "+conf.getvalue('FTPSERVER')+" № "+str(numsessofday)
    rita(txt); infoline(txt2);

def GenCmdSend():
    '''
        GenCmdSend()
        generate cmsd send for lftp
    '''
    i=0; PUTCMD=LSTCMD=''
    saveLFname=saveRFpath=""; NMM='';
    # ML = int((WLLZ - 42) / 2); cml = 0;

    if conf.nFLSND>0 :

        conf.FTPCMD=conf.FTPCMDOPEN

        for i in range(conf.nFLSND):
            # droped repeate adresses and files
            if not (saveLFname==conf.WRFNPTH[i] and saveRFpath==conf.WRFNPTH[i]):
                if conf.WRFNPTH[i].find('@')!=-1:
                    conf.nMAILSND+=1
                    conf.WMEMAILAD.append(conf.WRFNPTH[i])
                    conf.WMSUBJECT.append(conf.WSUBJCT[i])
                    conf.WMTXTMAIL.append(conf.WLETTXT[i])
                    conf.WMATTACHM.append(conf.WLFNPTH[i])
                else:
                    conf.nFTPSND+=1
                    NMM=getfnamefpath(conf.WLFNPTH[i])             # последнее слово в path - имя файла(?)
                    PUTCMD+="rm -f "+conf.WRFNPTH[i]+"/"+NMM+";"
                    PUTCMD+="put -c -O "+conf.WRFNPTH[i]+" "+conf.WLFNPTH[i]+";"
#                   PUTCMD+="chmod 666 "+WRFNPTH[i]+"/"+NMM+";"
                    saveLFname=conf.WLFNPTH[i]; saveRFpath=conf.WRFNPTH[i];

#        for ftppath in unique_everseen(conf.WRFNPTH):

        tmp=list()
        for apth in conf.WRFNPTH:
            if apth.find('@')==-1:
                tmp.append(apth)

        for ftppath in num.unique(tmp):
            LSTCMD+="echo "+ftppath+";rels "+ftppath+";"

        conf.FTPCMD+=PUTCMD+LSTCMD+conf.FTPCMDCLOSE
    else:
        outt("Нет файлов для отправки")

def getfsinfo(PATHDIRFL):
    '''
        get file system list
    '''
    shellcmd('rm '+conf.getvalue('FLLSLOG'), '/dev/null')
    filelist=lsdir(PATHDIRFL)
    conf.aFLS.clear()
    conf.aLSZ.clear()

    # ftp send
    if len(filelist)>0:
        for fpthfile in filelist :
            conf.aFLS.append(getfnamefpath(fpthfile))                  # get namefile from fullpath
            conf.aLSZ.append(os.path.getsize(PATHDIRFL+'/'+fpthfile))
        shellcmd('ls -l "'+PATHDIRFL+'"', conf.getvalue('FLLSLOG'), 'w')

    # e-mail attachments
    if len(conf.PTFATCH)>0:
        for pthatfile in conf.PTFATCH:
            conf.aFLS.append(getfnamefpath(pthatfile))                 # get namefile from fullpath
            if pthatfile.find('/')!=-1: 
                conf.aLSZ.append(os.path.getsize(pthatfile))
            else: 
                conf.aLSZ.append(os.path.getsize(PATHDIRFL+'/'+pthatfile))

def getfnszdt(STR, FNAMESIZE, FSIZESIZE=10):
    '''
        get file name and file size
    '''
    FLMODE='z'; FNREAL=TXTFNML=TXTFNMS=TXTFSZ=FLDATE=FUSER=''; FSZBYTE=0

    if STR.find("total")==-1 and STR.find("b'")==-1 and len(STR)>0:

        linearr=STR.split(' ')
        buff=list(); tmplist=list()
        endless=''; i=0

        for ll in linearr:
            if strlen(ll)!=0:
                tmplist.append(clean_escape(ll).rstrip())

        for i in range(len(tmplist)):
            if strlen(tmplist[i])!=0:
                if i<=7:
                    buff.append(clean_escape(tmplist[i]).rstrip())
                elif i>7:
                    endless+=tmplist[i]+' '

        FLMODE=buff[0]
        if i>=8:
            buff.append(clean_escape(endless).rstrip())

            # fnamesize
            FNREAL=buff[8]
            TXTFNML=TXTFNMS=FNREAL
            hs=FNAMESIZE[0]//2
            hl=FNAMESIZE[1]//2

            if FNAMESIZE[0]<=strlen(FNREAL):
                TXTFNMS=FNREAL[:hs]+"<=>"+FNREAL[-hs:]

            if FNAMESIZE[1]<=strlen(FNREAL):
                TXTFNML=FNREAL[:hl]+"<=>"+FNREAL[-hl:]

            # sizesiize
            FSZBYTE=buff[4]
            if FSIZESIZE<=strlen(FSZBYTE):
                TXTFSZ=str(naturalsize(FSZBYTE))
            else:
                TXTFSZ=FSZBYTE
            # user
            FUSER=buff[2]
            # date
            FLDATE=buff[5]+' '+buff[6]+' '+buff[7]
        else:
            if FLMODE.find('/')!=-1:
                for i in range(len(tmplist)):
                    endless+=tmplist[i]+' '

                FNREAL=TXTFNML=TXTFNMS=clean_escape(endless).rstrip();
                TXTFSZ=""; FSZBYTE=0; FLDATE="";
            else:
                return None
    else:
        return None

    return {'FM':FLMODE, 'FR':FNREAL, 'TN':(TXTFNMS, TXTFNML), 'FZ':FSZBYTE, 'TZ':TXTFSZ, 'FD':FLDATE, 'FU':FUSER}

def ErrorParce(LN):
    '''
        out error if file or directory not permission
    '''
    buff=LN.split(":")
    Path=num.unique(conf.WFTPPTH)
    last=len(buff)
    tmp=(buff[2]+buff[last-1]).strip().replace('\n', '')
    for pt in Path:
        if tmp.find(pt)!=-1: return [pt, tmp]
    return ['NoPth', 'NoName']

def fslist(FSLOG, title='Список', FSTYPE='FLSFS'):
    '''
        show file list for parh
    '''
    if not os.path.isfile(FSLOG):
        return False
    
    infoline(title)
    usz=9; szsz=12; dtsz=14;
    ASIZE=len(conf.aFLS)

    lz=WSLZ-40; lgz=WLLZ-40
    BBCOLOR=cl.getcolor('BBCOLOR'); BGSCOLOR=cl.getcolor('BGSCOLOR'); FLSCOLOR=cl.getcolor('FLSCOLOR');
    PPCL=LYELLOW+BBCOLOR; USCL=LYELLOW+BBCOLOR; SZCL=LYELLOW+BBCOLOR; DTCL=LYELLOW+BBCOLOR; DFCL=GRAY+BBCOLOR;
    fmtN=DFCL+'│'+PPCL+'{:{align}{name}s}'+DFCL+'│'+USCL+'{:{user}s}'+DFCL+'│'+SZCL+'{:{size}s}'+DFCL+'│'+DTCL+'{:{date}s}'+DFCL+'│'

    LLIST=SLIST=DFCL
    topl="┌"+"─"*lz+"┬"+"─"*usz+"┬"+"─"*szsz+"┬"+"─"*dtsz+"┐"+STDCL+'\n';
    SLIST+=topl;
    topll="┌"+"─"*lgz+"┬"+"─"*usz+"┬"+"─"*szsz+"┬"+"─"*dtsz+"┐"+STDCL+'\n';
    LLIST+=topll;
    SLIST+=fmtN.format(" Имя", " Польз.", " Размер", " Дата", align='<', name=lz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'
    LLIST+=fmtN.format(" Имя", " Польз.", " Размер", " Дата", align='<', name=lgz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'
    midl=topl.replace('┌', '├').replace('┬', '┼').replace('┐', '┤')
    midll=topll.replace('┌', '├').replace('┬', '┼').replace('┐', '┤')

    SLIST+=DFCL+midl
    LLIST+=DFCL+midll

    if FSTYPE=='FLSFS':
        for atch in conf.PTFATCH:
            if os.path.isfile(atch):
                shellcmd('ls -l "'+atch+'"', FSLOG, 'a')
            else:
                outt("Файл вложение: "+getfnamefpath(atch)+" не найден")

    SUMSIZE=FCOUNT=0
    ERROR=['/###', '/###']

    with open(FSLOG, "rb") as File:
        ffilear=File.read().split(b'\n')
        for farr in ffilear:
            LN=StrUTF8(farr)
            if len(LN)<=0:continue
            if (LN.find("Ошибка") or LN.find("Error"))!=-1:
                ERROR=ErrorParce(LN);
                continue
            fl=getfnszdt(LN, (lz-4, lgz-4), 10)
            if fl!=None:
                FSIZE=fl['TZ']; FMODE=fl['FM']; FREAL=fl['FR']
                FSZBYTE=int(fl['FZ']);
                cl.setdefcolors()

                if FMODE.find('/')==-1:
                    FCOUNT+=1
                    # file dir show
                    if FMODE[0]=='d':
                        FSIZE="-DIR"
                        cl.setfilecolor(BIWHITE);  cl.setsizecolor(BIWHITE);
                    elif FMODE[0]=='l':
                        FSIZE="-LNK"
                        cl.setfilecolor(BICYAN); cl.setsizecolor(BICYAN);
                    else:
                        FSIZE=fl['TZ']
                        SUMSIZE+=FSZBYTE
                        cl.setdefcolors()

                    if ASIZE>0:
                        for i in range(ASIZE):
                            if fl['FR']==conf.aFLS[i] and FSTYPE=='FTPFS':
                                cl.setbackcolor(BGSCOLOR); cl.settextcolor(GRAY); cl.setdatecolor(WHITE);
                                cl.setsizecolor(BRED); cl.setfilecolor(FLSCOLOR);
                                break;
                            elif FSTYPE=='DELAY':
                                cl.setbackcolor(BGRED); cl.settextcolor(LYELLOW); cl.setdatecolor(WHITE);
                                cl.setsizecolor(LYELLOW); cl.setfilecolor(LYELLOW);
                                break;

                    FBCOLOR=cl.getcolor('FBCOLOR'); SZCOLOR=cl.getcolor('SZCOLOR'); DTCOLOR=cl.getcolor('DTCOLOR');
                    NMCL=FBCOLOR+BBCOLOR; USCL=LYELLOW+BBCOLOR; SZCL=SZCOLOR+BBCOLOR; DTCL=DTCOLOR+BBCOLOR;
                    fmtL=DFCL+'│'+NMCL+'{:{align}{name}s}'+DFCL+'│'+USCL+'{:>{user}s}'+DFCL+'│'+SZCL+'{:>{size}s}'+DFCL+'│'+DTCL+'{:>{date}s}'+DFCL+'│'
                    SLIST+=fmtL.format(fl['TN'][0], fl['FU'], FSIZE, fl['FD'], align='<', name=lz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'
                    LLIST+=fmtL.format(fl['TN'][1], fl['FU'], FSIZE, fl['FD'], align='<', name=lgz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'

                else:
                    # Path show
                    SZCOLOR=cl.getcolor('SZCOLOR'); DTCOLOR=cl.getcolor('DTCOLOR');
                    NMCL=BIYELLOW+BBCOLOR; USCL=LYELLOW+BBCOLOR; SZCL=WHITE+BBCOLOR; DTCL=DTCOLOR+BBCOLOR;
                    fmtL=DFCL+'│'+NMCL+'{:{align}{name}s}'+DFCL+'│'+USCL+'{:>{user}s}'+DFCL+'│'+SZCL+'{:>{size}s}'+DFCL+'│'+DTCL+'{:>{date}s}'+DFCL+'│'
                    SLIST+=fmtL.format('['+FREAL.replace('\n', '')+']:', "", "-ADR", fl['FD'], align='<', name=lz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'
                    LLIST+=fmtL.format('['+FREAL.replace('\n', '')+']:', "", "-ADR", fl['FD'], align='<', name=lgz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'
                    if LN.find(ERROR[0])!=-1:
                        ERC=LYELLOW+BGRED
                        fmtEr=DFCL+'│'+ERC+'{:<{err}s}'+DFCL+'│'+STDCL+'\n';
                        SLIST+=fmtEr.format(ERROR[1], err=WSLZ-2)
                        LLIST+=fmtEr.format(ERROR[1], err=WLLZ-2)

    cl.setdefcolors()
    SLIST+=DFCL+midl.replace('┼', '┴', 1)
    LLIST+=DFCL+midll.replace('┼', '┴', 1)
    SUMSIZE=naturalsize(SUMSIZE)
    dt=getdatetime('%d %b %Y') # local date
    NMCL=LYELLOW+BBCOLOR;
    formatL=DFCL+'│'+NMCL+'{:{align}{name}s}'+DFCL+' '+USCL+'{:>{user}s}'+DFCL+'│'+SZCL+'{:>{size}s}'+DFCL+'│'+DTCL+'{:>{date}s}'+DFCL+'│'
    SLIST+=formatL.format(" Файлов: "+str(FCOUNT), "", SUMSIZE, dt, align='<', name=lz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'
    LLIST+=formatL.format(" Файлов: "+str(FCOUNT), "", SUMSIZE, dt, align='<', name=lgz, user=usz, size=szsz, date=dtsz)+STDCL+'\n'
    SLIST+=DFCL+'└'+'─'*lz+'─'+'─'*usz+'┴'+'─'*szsz+'┴'+'─'*dtsz+'┘'
    LLIST+=DFCL+'└'+'─'*lgz+'─'+'─'*usz+'┴'+'─'*szsz+'┴'+'─'*dtsz+'┘'
    outt(SLIST, 'scr')
    outt(LLIST, 'log')
    cl.setdefcolors()
    return True

def emailsend():
    '''
        send email
    '''
    conf.getadrbook()
    ret=False

    # Check mail have
    im=len(conf.WMEMAILAD);
    if im==0 : return 0

    txt="Отправка e-mail"
    infoline(txt); # rita(txt);
    DT=getdatetime()

    titl=len('Subject: ');
    lz=WSLZ-2; tmst=len(DT); lza=lz-(titl+tmst); lzi=lz-tmst;
    lgz=WLLZ-2; tmst=len(DT); lgza=lgz-(titl+tmst); lgzi=lgz-tmst;

    outt("┌"+"─"*lz+"┐", 'scr');
    outt("┌"+"─"*lgz+"┐", 'log')

    lnintol="├"+"─"*lz+"┤"
    lnintoll="├"+"─"*lgz+"┤"
    BBCOLOR=cl.color['BBCOLOR']; TBCOLOR=cl.color['TBCOLOR']; DEFC=cl.color['TBCOLOR']+cl.color['BBCOLOR'];
    TIMEC=TBCOLOR+BBCOLOR; ERORC=LYELLOW+BGRED; ADRSC=LGREEN+BBCOLOR;
    SUBJC=WHITE+BBCOLOR; ATCHC=LCYAN+BBCOLOR; LNTXC=GRAY+BBCOLOR

    fmtAd='│ E-mail: '+ADRSC+'{:{align}{work}s}'+TIMEC+'{:>{timest}s}'+DEFC+'│'+STDCL
    fmtSj='│Subject: '+SUBJC+'{:{align}{work}s}'+TIMEC+'{:>{timest}s}'+DEFC+'│'+STDCL
    fmtAt='│ Attach: '+ATCHC+'{:{align}{work}s}'+TIMEC+'{:>{timest}s}'+DEFC+'│'+STDCL
    fmtLi='│'+LNTXC+'{:{align}{work}s}'+TIMEC+'{:>{timest}s}'+DEFC+'│'+STDCL
    fmtEr='│'+ERORC+'  Error: '+ERORC+'{:{align}{work}s}'+TIMEC+'{:>{timest}s}'+DEFC+'│'+STDCL

    CCLIST=""

    for i in range(im):
        # адрес
        outt(fmtAd.format(conf.WMEMAILAD[i], getdatetime(), align='<', work=lza, timest=tmst), 'scr')
        outt(fmtAd.format(conf.WMEMAILAD[i], getdatetime(), align='<', work=lgza, timest=tmst), 'log')
        # сабжа
        if len(conf.WMSUBJECT[i])<=0: SUBJ=conf.getvalue('STATION')+' Station'
        else: SUBJ=conf.WMSUBJECT[i]
        outt(fmtSj.format(SUBJ, getdatetime(), align='<', work=lza, timest=tmst), 'scr')
        outt(fmtSj.format(SUBJ, getdatetime(), align='<', work=lgza, timest=tmst), 'log')

        # Обработка аттачей
        ATTACH=ATTACHLIST=''
        for fname in conf.WMATTACHM[i].split(';'):
            if len(fname)==0: continue
            fname=fname.replace('\\', '')
            if os.path.isfile(fname):
                fname=getfnamefpath(fname)
                ATTACHLIST+=fname+'\r\n'
                outt(fmtAt.format(fname, getdatetime(), align='<', work=lza, timest=tmst), 'scr')
                outt(fmtAt.format(fname, getdatetime(), align='<', work=lgza, timest=tmst), 'log')
            else:
                outt(fmtEr.format(fname+" не найден", getdatetime(), align='<', work=lza, timest=tmst), 'scr')
                outt(fmtEr.format(fname+" не найден", getdatetime(), align='<', work=lgza, timest=tmst), 'log')

        ATTACH+=conf.WMATTACHM[i].replace(';', ' ')
        # Письмо текст
        if i>0:  CCLIST+=conf.WMEMAILAD[i]+', '
        
    TEXTLETTER="Здравствуйте.\n\n"

    if len(conf.WMTXTMAIL[i])==0: TEXTLETTER+='Примите сообщение от станции '+conf.getvalue('STATION')+'.\n'
    else: TEXTLETTER+=conf.WMTXTMAIL[i]+"\n"

    if conf.AWRLET:
        TEXTLETTER+='\r\n==================================================\r\n'
        TEXTLETTER+=readfile(ATTACH)
        TEXTLETTER+='\r\n==================================================\r\n'

    if len(conf.WMATTACHM[i])==0:
        TEXTLETTER+=readfile("/usr/share/wlib/.signature")
    else:
        TEXTLETTER+="\nСмотрите вложениия:\n"+ATTACHLIST+"\n"
        TEXTLETTER+=readfile("/usr/share/wlib/.signature")

    cmdlt='echo -e "'+TEXTLETTER+'" | sudo mutt -s "'+SUBJ+'" -b "'+conf.getvalue('BCCMAIL')+'"'+' -c "'+CCLIST+'" '  

    if len(conf.WMATTACHM[i])!=0:
        cmdlt+=' -a "'+ATTACH+'" '
    
    cmdlt+=conf.WMEMAILAD[0]
        
    shellcmd(cmdlt, conf.getvalue('MAILLOG'), '>', 'ASYNC')

    time.sleep(1)
    outt(fmtLi.format("Почтовый агент запущен в фоне.", getdatetime(), align='<', work=lzi, timest=tmst), 'scr')
    outt(fmtLi.format("Почтовый агент запущен в фоне.", getdatetime(), align='<', work=lgzi, timest=tmst), 'log')

    updatesessionofday(conf.getvalue('INDEX_EML'))
    ret=True

    if i<im-1: outt(lnintol)

    outt("└"+"─"*lz+"┘", 'scr');
    outt("└"+"─"*lgz+"┘", 'log');
    #nMAILSND=0;
    return ret

def maxsend():
    '''
        out messages if get max send iterations
    '''
    cl.settextcolor(LRED)
    outt("Макс. кол-во ("+str(conf.getvalue('MAXRPT'))+") попыток. Проверьте соединение")
    cl.setdefcolors()
    rita("Максимальное количество попыток. Проверьте соединение. Отправка отменена.")

def fseqsize(TITLE, FSLOG):
    '''
        check send file size
    '''
    if not os.path.isfile(FSLOG): return False

    ret=False;
    count=len(conf.aFLS)

    if count>0:
        infoline("Размер. "+TITLE)
        lz=0; szsz=14; stsz=3;
        cl.setcolors(BLACK, BGGRAY, BLUE, BLACK)
        lz=WSLZ-(szsz*2+stsz+5)
        lgz=WLLZ-(szsz*2+stsz+5)

        BBCOLOR=cl.getcolor('BBCOLOR');
        PPCL=LYELLOW+BBCOLOR; ADRCL=LYELLOW+BBCOLOR; PTCL=LYELLOW+BBCOLOR; CMTCL=LYELLOW+BBCOLOR; DFCL=BLACK+BBCOLOR;
        LLIST=SLIST=DFCL
        topl=DFCL+'┌'+'─'*lz+'┬'+'─'*szsz+'┬'+'─'*szsz+'┬'+'─'*stsz+'┐'+STDCL+'\n';
        topll=DFCL+'┌'+'─'*lgz+'┬'+'─'*szsz+'┬'+'─'*szsz+'┬'+'─'*stsz+'┐'+STDCL+'\n';
        SLIST+=topl
        LLIST+=topll
        fmtNm=DFCL+'│'+PPCL+'{:{align}{name}s}'+DFCL+'│'+ADRCL+'{:{lsize}s}'+DFCL+'│'+PTCL+'{:{rsize}s}'+DFCL+'│'+CMTCL+'{:{status}s}'+DFCL+'│'+STDCL+'\n'
        SLIST+=fmtNm.format(" Имя", " Локальный", " FTP АСПД", " S", align='<', name=lz, lsize=szsz, rsize=szsz, status=stsz)
        LLIST+=fmtNm.format(" Имя", " Локальный", " FTP АСПД", " S", align='<', name=lgz, lsize=szsz, rsize=szsz, status=stsz)
        midl=topl.replace('┌', '├').replace('┬', '┼').replace('┐', '┤')
        midll=topll.replace('┌', '├').replace('┬', '┼').replace('┐', '┤')
        SLIST+=DFCL+midl
        LLIST+=DFCL+midll

        PPCL=BLUE+BBCOLOR; ADRCL=RED+BBCOLOR; PTCL=RED+BBCOLOR; CMTCL=GREEN+BBCOLOR; DEFCL=BLACK+BBCOLOR;
        fmtFl=DFCL+'│'+PPCL+'{:{align}{name}s}'+DFCL+'│'+ADRCL+'{:>{lsize}s}'+DEFCL+'│'+PTCL+'{:>{rsize}s}'+DFCL+'│'+CMTCL+'{:>{status}s}'+DFCL+'│'+STDCL+'\n'

        ERROR=['/###', '/###']

        with open(FSLOG, "rb") as File:
            ffilear=File.read().split(b'\n')
            for farr in ffilear:
                LN=StrUTF8(farr)

                if len(LN)<=0:continue
                if (LN.find("Ошибка") or LN.find("Error"))!=-1:
                    ERROR=ErrorParce(LN);
                    continue

                fl=getfnszdt(LN, (lz-4, lgz-4), 10)
                if fl!=None:
                    FNAMES=str(fl['TN'][0]);
                    FNAMEL=str(fl['TN'][1]);
                    FNREAL=fl['FR'];

                    if FNAMES.find('/')==-1 :
                        for afls, alsz in zip(conf.aFLS, conf.aLSZ):
                            if FNREAL==afls:
                                FSIZE=fl['TZ'];
                                PPCL=BLUE+BBCOLOR; ADRCL=RED+BBCOLOR; PTCL=RED+BBCOLOR; CMTCL=GREEN+BBCOLOR; DEFCL=BLACK+BBCOLOR;
                                fmtFl=DFCL+'│'+PPCL+'{:{align}{name}s}'+DFCL+'│'+ADRCL+'{:>{lsize}s}'+DFCL+'│'+PTCL+'{:>{rsize}s}'+DFCL+'│'+CMTCL+'{:>{status}s}'+DFCL+'│'+STDCL+'\n'
                                if int(FSIZE)==alsz and int(FSIZE)>0 and alsz>0:
                                    SLIST+=fmtFl.format(FNAMES, FSIZE, str(alsz), GREEN+BBCOLOR+"OK ", align='<', name=lz, lsize=szsz, rsize=szsz, status=stsz)
                                    LLIST+=fmtFl.format(FNAMEL, FSIZE, str(alsz), GREEN+BBCOLOR+"OK ", align='<', name=lgz, lsize=szsz, rsize=szsz, status=stsz)
                                    ret=True
                                    break
                                else:
                                    SLIST+=fmtFl.format(FNAMES, FSIZE, str(alsz), LRED+BBCOLOR+"BAD", align='<', name=lz, lsize=szsz, rsize=szsz, status=stsz)
                                    LLIST+=fmtFl.format(FNAMEL, FSIZE, str(alsz), LRED+BBCOLOR+"BAD", align='<', name=lgz, lsize=szsz, rsize=szsz, status=stsz)

                    # if echo "$ErrLN" | grep "$FNAME" > /dev/null; then ErrorMsg; fi
                    else:
                    # Path show
                        PPCL=LYELLOW+BBCOLOR; ADRCL=WHITE+BBCOLOR; PTCL=RED+BBCOLOR; CMTCL=GREEN+BBCOLOR; DEFCL=BLACK+BBCOLOR;
                        fmtFl=DFCL+'│'+PPCL+'{:{align}{name}s}'+DEFCL+'│'+ADRCL+'{:>{lsize}s}'+DEFCL+'│'+PTCL+'{:>{rsize}s}'+DEFCL+'│'+CMTCL+'{:>{status}s}'+DEFCL+'│'+STDCL+'\n'
                        SLIST+=fmtFl.format('['+FNAMES+']:', "-ADR", "", "", align='<', name=lz, lsize=szsz, rsize=szsz, status=stsz)
                        LLIST+=fmtFl.format('['+FNAMEL+']:', "-ADR", "", "", align='<', name=lgz, lsize=szsz, rsize=szsz, status=stsz)
                        if LN.find(ERROR[0])!=-1:
                            ERC=LYELLOW+BGRED
                            fmtEr=DFCL+'│'+ERC+'{:<{err}s}'+DFCL+'│'+STDCL+'\n';
                            SLIST+=fmtEr.format(ERROR[1], err=WSLZ-2)
                            LLIST+=fmtEr.format(ERROR[1], err=WLLZ-2)
        cl.setdefcolors
        SLIST+=DFCL+'└'+'─'*lz+'┴'+'─'*szsz+'┴'+'─'*szsz+'┴'+'─'*stsz+'┘'
        LLIST+=DFCL+'└'+'─'*lgz+'┴'+'─'*szsz+'┴'+'─'*szsz+'┴'+'─'*stsz+'┘'
        outt(SLIST, 'scr')
        outt(LLIST, 'log')
    else:
        ret=False

    return ret

def ftpsend(LDIR):
    '''
        send to ftp
    '''
    MAXRPT=int(conf.getvalue('MAXRPT'))
    FTPSLOG=conf.getvalue('FTPSLOG')

    if conf.nFTPSND>0:
        infoline("Отправка на ASPD "+conf.getvalue('FTPSERVER'))
        getfsinfo(LDIR)

        for i in range(MAXRPT):
            shellcmd('lftp -c "'+conf.FTPCMD+'"', FTPSLOG, ">")

            if fseqsize("Отправка. АСПД "+conf.getvalue('FTPSERVER'), FTPSLOG):
                infoline('Файлы для АСПД отправлены #'+str(i+1)+'('+str(MAXRPT)+')')
                rita("Файлы для А эС Пэ Дэ отпрaвлены. Размеры файлов совпадают.")
                fslist(FTPSLOG, "Отправка на АСПД "+conf.getvalue('FTPSERVER'), 'FTPFS')
                break;
            else:
                cl.settextcolor(RED)
                outt('Повторная отправка файла #'+str(i+1)+'('+str(MAXRPT)+')')
                cl.setdefcolors()
                rita('Повторная отправка. Попытка номер '+str(i+1))
        if i>=MAXRPT: maxsend()

def ftpsendonce(FILESEND):
    '''
        send to ftp one file
    '''
    if os.path.isfile(FILESEND):
        MAXRPT=int(conf.getvalue('MAXRPT'))
        FTPSLOG=conf.getvalue('FTPSLOG')

        infoline("Отправка ftp. "+conf.getvalue('FTPSERVER'))
        lpath=getfpathfpath(FILESEND)

        if lpath=='': lpath='./'
        outt("Файл: "+FILESEND)
        getfsinfo(lpath+'/')

        for i in range(MAXRPT):
            infoline('Попытка #'+str(i+1)+'('+str(MAXRPT)+')')
            shellcmd('lftp -c "'+conf.FTPCMD+'"', FTPSLOG, '>')

            if fseqsize("Отправка "+FILESEND, FTPSLOG):
                infoline('Успешная отправка #'+str(i+1)+'('+str(MAXRPT)+')')
                rita("Отправлены файлы для А эС Пэ Дэ. Размеры файлов совпадают.")
                fslist(FTPSLOG, 'Отправка. АСПД '+FILESEND, 'FTPFS')
                break;
            else:
                cl.setdefcolors()
                outt("Повторная попытка #"+str(i+1)+'('+str(MAXRPT)+')')
                rita("Повторная потытка номер "+str(i+1)+" из "+str(MAXRPT))
        if i>=MAXRPT-1: maxsend()
    else:
        outt("Файл: "+FILESEND+"не найден")

def fmtcheck(FILECHECK):
    '''
        check format templates
    '''
    if os.path.isfile(FILECHECK):
        fl=open(FILECHECK, 'r')
        firstline=fl.read();
        fl.close()
        if len(firstline)>0:
            F_SIGNATURE01=firstline.split(' ')[0]
            F_SIGNATURE02=firstline.split(' ')[1]

            if F_SIGNATURE01=="#!AWR": return True
            elif conf.SIGNATURE01==F_SIGNATURE01 and conf.SIGNATURE02==F_SIGNATURE02:
                return True
            else:
                NAMEUNKNOWN=getdatetime('%m%d-%H%M')+'-noformat-'+getfnamefpath(FILECHECK) # local date
                shellcmd('mv -f "'+FILECHECK+'" "'+conf.getvalue('UNKNOWNDIR')+'/'+NAMEUNKNOWN+'"')

                cl.settextcolor(LMAGENTA)
                outt("Файл "+FILECHECK+". Ошибка формата.")
                outt("Файл перемещён в "+conf.getvalue('UNKNOWNDIR')+" с именем:")
                cl.settextcolor(LGREEN)
                outt(NAMEUNKNOWN)
                cl.setdefcolors
                rita("Ошибка формата. Проверьте сигнатуру файла. Файл перемещён в каталог инфобокс.")
                return False
    else:
        return False

def sizecheck(FILECHECK=''):
    '''
        check size templates
    '''
    FSIZE=os.path.getsize(FILECHECK)

    if  FSIZE>=int(conf.getvalue('MINFSIZE')):
        return True
    else:
        NAMEUNKNOWN=getdatetime('%m%d-%H%M')+'-nosize-'+getfnamefpath(FILECHECK) # local date
        shellcmd('mv -f "'+FILECHECK+'" "'+conf.getvalue('UNKNOWNDIR')+'/'+NAMEUNKNOWN+'"')

        cl.settextcolor(LMAGENTA)
        outt("Файл "+FILECHECK)
        outt("имеет слишком малый размер: "+str(FSIZE)+" Проверьте содержимое файла.")
        outt("Файл перемещён в $UNKNOWNDIR с именем:")
        cl.settextcolor(LRED)
        outt(NAMEUNKNOWN)
        cl.setdefcolors
        rita("Файл имеет малый размер. Проверьте файл. Отмена отправки. Файл перемещён в каталог инфобокс.")
        return False

def msgsendtimeavos():
    '''
        messages if time not SIN-time
    '''
    cl.settextcolor(LRED);
    txt="Отправка СИН осуществляется до СИН-срока."
    outt(txt); rita(txt);
    cl.setdefcolors()
    cl.settextcolor(LYELLOW)
    outt(conf.TIMESIN)
    cl.settextcolor(LGREEN)
    txt="Сохранённый СИН будет отправлен автоматически на следующем СИН-сроке"
    outt(txt); rita("Отложенная отправка");
    cl.setdefcolors()

def msgsendtimeclim():
    '''
        messages if time not CLIMAT-time
    '''
    outt("")
    cl.settextcolor(LRED);
    txt="Соообщение тип CLIMAT отправляется поздно по времени."
    outt(txt); rita(txt);
    cl.setdefcolors()

def settmsin(TMH:int, TMM:int):
    '''
        set TIME for varible SIN-time
    '''
    TMHM=int(TMH)*60+int(TMM)
    SINTMAX=int(conf.getvalue('SINTMAX'))

    INT0000=0
    INT0600=6*60;     INT0020=SINTMAX;         # 00:20-06:00
    INT1200=12*60;    INT0620=6*60+SINTMAX;    # 06:20-12:00
    INT1800=18*60;    INT1220=12*60+SINTMAX;   # 12:20-18:00
    INT2400=24*60;    INT1820=18*60+SINTMAX;   # 18:20-00:00

    if TMHM>INT0020 and TMHM<INT0600:   conf.TIMESIN="Следующий СИН-срок: "+conf.getvalue('SIN1')
    elif TMHM>INT0620 and TMHM<INT1200: conf.TIMESIN="Следующий СИН-срок: "+conf.getvalue('SIN2')
    elif TMHM>INT1220 and TMHM<INT1800: conf.TIMESIN="Следующий СИН-срок: "+conf.getvalue('SIN3')
    elif TMHM>INT1820 and TMHM<INT2400: conf.TIMESIN="Следующий СИН-срок: "+conf.getvalue('SIN0')

    elif TMHM>=INT0000 and TMHM<=INT0020: conf.TIMESIN="Текущий СИН-срок: "+conf.getvalue('SIN1')
    elif TMHM>=INT0600 and TMHM<=INT0620: conf.TIMESIN="Текущий СИН-срок: "+conf.getvalue('SIN2')
    elif TMHM>=INT1200 and TMHM<=INT1220: conf.TIMESIN="Текущий СИН-срок: "+conf.getvalue('SIN3')
    elif TMHM>=INT1800 and TMHM<=INT1820: conf.TIMESIN="Текущий СИН-срок: "+conf.getvalue('SIN0')

    conf.TIMESIN+=' '+conf.getvalue('STATION')
    conf.setvalue('TIMESIN', conf.TIMESIN)

def tmcheckavos(TYPEMSG, silent=False):
    '''
        check SIN-time
    '''
    txt=''; TMH=TMM=TMDM=0

    if TYPEMSG=="avos":
        FILECHECK=conf.getvalue('LDIRFORSEND')+'/'+TYPEMSG+'.txt'
        # for avos!!! UTC!
        TMH=int(getdatetimeUTC('%H'))
        TMM=int(getdatetimeUTC('%M'))
        SINTMIN=int(conf.getvalue('SINTMIN'))
        SINTMAX=int(conf.getvalue('SINTMAX'))
        #TMH=18; TMM=10;

        # 00:00-00:20 06:00-06:20 12:00-12:20 18:00-18:20 UTC
        if (TMH==0 or TMH==6 or TMH==12 or TMH==18) and (TMM>=SINTMIN and TMM<=SINTMAX):
            settmsin(TMH, TMM)
            return True
        else:
            settmsin(TMH, TMM)
            if not silent :
                if os.path.isfile(FILECHECK):
                    shellcmd('sudo mv -f "'+FILECHECK+'" "'+conf.getvalue('LDIRDELAY')+'/"')
                    write2dbf(conf.getvalue('SIGN_DELAY'), "cоздано: "+getdatetime("%Y.%m.%d %H:%M:%S"))
                msgsendtimeavos()
            return False

    elif TYPEMSG=="climat":
        FILECHECK=conf.getvalue('LDIRFORSEND')+'/avos'+TYPEMSG+'.txt'
        TMDM=int(getdatetimeUTC('%d'))  # # ... too UTC min's daymonth
        # TMDM=0;
        if TMDM<=3 :
            return True
        else:
            if not silent:
                msgsendtimeclim()
                cl.settextcolor(LGREEN)
                txt="Отправить сообщение типа CLIMAT?(y/n):"
                while True:
                    ch=psecho(txt)
                    if ch in ('y', 'Y'):
                        return True
                        break
                    elif ch in ('n', 'N'):
                        if os.path.isfile('"'+conf.getvalue('LDIRFORSEND')+'/avosclimat.txt"'):
                            shellcmd('sudo rm "'+conf.getvalue('LDIRFORSEND')+'/avosclimat.txt"')
                            outt("Cообщение типа CLIMAT удалено.")
                        return False
                        break
                    else:
                        pass

            else:
                return False

    cl.setdefcolors

def AddFnamePath(LPFNAME='', RMPTHNAME='', SUBJECT='', TXTMAIL=''):
    '''
        add file name for send ftp and e-mail
    '''
    LPFNAME=LPFNAME.strip().replace(' ', '\ ')
    conf.WLFNPTH.append(LPFNAME)
    conf.WRFNPTH.append(RMPTHNAME)
    conf.WSUBJCT.append(SUBJECT)
    conf.WLETTXT.append(TXTMAIL)
    conf.nFLSND+=1

def checkdbfile(DBFILE):
    '''
        che daba file and sen new if not have
    '''
    if not os.path.isfile(DBFILE):
        DATE=getdatetime('%d%m')
        record="Date:"+DATE+" N:0 Sess:0"
        write2dbf(DBFILE, record)

def getNS(DBFILE):
    '''
        get number session
    '''
    with open(DBFILE, "r") as file:
        for line in file:
            tmp=line.split(' ')
            NS=tmp[1].split(':')[1]
    return int(NS)

def setsign(DBFILE=''):
    '''
        generate sing-send number
    '''
    conf.SIGN=''
    DATE=SAVEDATE=TMH=''; NS=1

    checkdbfile(DBFILE)

    if DBFILE!=' ':
        DATE=getdatetime('%d%m')
        if DBFILE==conf.getvalue('INDEX_AVS'):
        # for avos send time
            TMH=getdatetimeUTC('%H')

            if   TMH=='23' or TMH=='00': TMH='00'
            elif TMH=='05' or TMH=='06': TMH='06'
            elif TMH=='11' or TMH=='12': TMH='12'
            elif TMH=='17' or TMH=='18': TMH='18'
            else: TMH='ZZ'

            SIGN=getdatetime('%d')+TMH
            NS=getNS(DBFILE); NS+=1

        elif DBFILE==conf.getvalue('INDEX_CLM'):
            TMD=getdatetimeUTC('%d')
            if int(TMD)>3: SIGN='ZZ35'
            else: SIGN=getdatetime('%m', '-30d')+'35'
            NS=getNS(DBFILE); NS+=1
        else:
            with open(DBFILE, "r") as file:
                for line in file:
                    tmp=line.split(' ')
                    SAVEDATE=tmp[0].split(':')[1]
            if DATE==SAVEDATE:
                NS=getNS(DBFILE); NS+=1
            if NS<10 : SIGN=DATE+"0"+str(NS)
            else: SIGN=DATE+str(NS)
        record="Date:"+DATE+" N:"+str(NS)+" Sess:"+SIGN
        write2dbf(DBFILE, record)
    else:
        cl.setcolors(LRED, cl.getcolor('BBCOLOR'))
        outt("ERROR: setsign need set requare parameter!!!")
        cl.setdefcolors
    return SIGN

def cparchfiles(DIRSAVE, NAMESENDMSG, TYPELETTER):
    '''
        copy send files to archives directories
    '''
    LDIR_ARCH=conf.getvalue('LDIR_ARCH')
    RDIR_ARCH=conf.getvalue('RDIR_ARCH')
    ARCHDIR=conf.getvalue('ARCHDIR')
    LDIR=LDIR_ARCH+'/'+DIRSAVE+'/'+ARCHDIR
    RDIR=RDIR_ARCH+'/'+DIRSAVE+'/'+ARCHDIR

    if not os.path.isdir(LDIR): conf.crdsetpm(LDIR)
    if not os.path.isdir(RDIR): conf.crdsetpm(RDIR)

    shellcmd('sudo chmod -Rf '+conf.perm+' "'+LDIR+'"', "/dev/null", '>')
    shellcmd('sudo chown -Rf '+conf.owner+' "'+LDIR+'"', "/dev/null", '>')
    shellcmd('sudo chmod -Rf '+conf.perm+' "'+RDIR+'"', "/dev/null", '>')
    shellcmd('sudo chown -Rf '+conf.owner+' "'+RDIR+'"', "/dev/null", '>')

    shellcmd('cp -f "'+NAMESENDMSG+'" "'+LDIR+'"')
    shellcmd('cp -f "'+NAMESENDMSG+'" "'+RDIR+'"')

    if DIRSAVE=="OutBox":
        LDIRSND=LDIR_ARCH+'/Send/'+ARCHDIR
        RDIRSND=RDIR_ARCH+'/Send/'+ARCHDIR

        if not os.path.isdir(LDIRSND): conf.crdsetpm(LDIRSND)
        if not os.path.isdir(RDIRSND): conf.crdsetpm(RDIRSND)

        shellcmd('sudo chmod -Rf '+conf.perm+' "'+LDIRSND+'"', "/dev/null", '>')
        shellcmd('sudo chown -Rf '+conf.owner+' "'+LDIRSND+'"', "/dev/null", '>')
        shellcmd('sudo chmod -Rf '+conf.perm+' "'+RDIRSND+'"', "/dev/null", '>')
        shellcmd('sudo chown -Rf '+conf.owner+' "'+RDIRSND+'"', "/dev/null", '>')

        shellcmd('cp -f "'+NAMESENDMSG+'" "'+LDIRSND+'"')
        shellcmd('cp -f "'+NAMESENDMSG+'" "'+RDIRSND+'"')

    if os.path.isfile(TYPELETTER):
        shellcmd('rm "'+TYPELETTER+'"')

def make_METEO(TYPEMSG, SIGNATURE, DBFILE):
    '''
        make(prepare) METEO sends
    '''
    EMAIL=TXTMAIL=''
    LDIRFORSEND=conf.getvalue('LDIRFORSEND')

    if TYPEMSG=='avos':
        filesrc=LDIRFORSEND+'/'+TYPEMSG+'.txt'
    elif TYPEMSG=='climat':
        filesrc=conf.getvalue('LDIRFORSEND')+'/avos'+TYPEMSG+'.txt'

    if os.path.isfile(filesrc):
        conf.SIGNATURE01=SIGNATURE; conf.SIGNATURE02=conf.getvalue('METEOSIGNATURE')

        if fmtcheck(filesrc) and sizecheck(filesrc) and tmcheckavos(TYPEMSG):
            SIGN=setsign(DBFILE)
            filesend="avos"+SIGN+".txt";
            write2dbf(conf.getvalue('LASTMT'), filesend)
            ldirsend=conf.getvalue('LDIRSEND')
            filesend=ldirsend+'/'+filesend
            shellcmd('cp "'+filesrc+'" "'+filesend+'"')
            cparchfiles('Meteo', filesend, filesrc)
            # for ASPD /aspd
            AddFnamePath(filesend, conf.getvalue('FTPSTDPATHSEND'))
            # AddFnamePath(filesend, "/rae/vos")

            # for PROGRESS avos climat and emailing
            if TYPEMSG=='climat':
                AddFnamePath(filesend, "/rae/vos")
                AddFnamePath(filesend, "/rae/prg")
                if conf.getvalue('WORKTYPE')=='debug': EMAIL="admin_vostok@aari.ru"
                else: EMAIL="meteo@progress.aari.ru"
                TXTMAIL="Примите файл климата станция "+conf.getvalue('STATION')+" от "+getdatetimeUTC('%B %Y')
                AddFnamePath(filesend, EMAIL, 'Климат', TXTMAIL)

def setperm(file):
    '''
        set permissions for archives and work directories
    '''

    shellcmd('sudo chmod '+conf.perm+' "'+file+'"')
    shellcmd('sudo chown '+conf.owner+' "'+file+'"')

def SVC_TITLE(lttype=''):
    '''
        put SVC-title for letters
    '''
    tsvc=''
    if lttype=='SVC': tsvc='СЖЦ '
    # MSK date +3h from UTC! Time ASPD!
    return tsvc+'ВОСТОК '+getdatetimeUTC('%d/%m %H%M', '+3h')+'=\r\n'

def stdvrstxt(LETTERNAME, NAMEARCH):
    '''
    stdvrstxt genegate std subletter
    void stdvrstxt letterout namevrs SIGNVRS
    '''

    write2file_str(LETTERNAME, SVC_TITLE())
    LETTER="АСПД НС=\r\n\r\n"
    LETTER+="ПРОШУ ПОДТВЕРДИТЬ ПРИЁМ "+NAMEARCH.upper()+" FTP ИЛИ EMAIL\r\n"
    LETTER+="\r\nСОДЕРЖИТ:\r\n"

    LIST=lsdir(conf.getvalue('LDIRFORSEND'), '*')
    for lsn in LIST: LETTER+=lsn+'r\n'
    LETTER+="\r\nУВАЖЕНИЕМ=\r\n"
    LETTER+="$NSIGN ВОСТОК ШРМ-\r\n"
    write2file_str(LETTERNAME, LETTER)

def prepname(atname=''):
    '''
        get file name
    '''
    LDIRSEND=conf.getvalue('LDIRSEND')
    newatname=LDIRSEND+'/'+atname
    LDIRFORSEND=conf.getvalue('LDIRFORSEND')
    srcfile=LDIRFORSEND+'/'+atname
    if os.path.isfile(srcfile):
        shellcmd('mv "'+srcfile+'" "'+newatname+'"')
    return newatname

def parceletter(TYPELT, LETNAME, NAMESEND, NSIGN, ARCH_PARTS='', NAMEARCH=''):
    ''' # Letter compiling
        TAGs:
            Letter tag:
                only for e-mail adress
                E-MAIL: tag for string email tags
                TMAIL: text for e-mail place to letter
            
            SUBJ:   for email and tlg adress
            ATTACH: attaches send for all adress and ASPD and e-mail
                    if tag plase e-mail string attachs will be sen only for this e-mail
                    if tag place in body mail send for all        
            CHEEF:  add cheef sign
            GRECV: or GCONF:  add string confirmed of recv
            SHRM:   add ship radio master sign
            [DATE] [WEEK] [YEAR] [MONTH] - add it for value
    '''
    conf.getadrbook()

    strlet=''
    ADRESS=[]; ADRESSNW=[]; LBODY=[]; LSIGN=[]
    SUBJ=TXTMAIL=""; ATCH=''; i=0
    conf.PTFATCH.clear();  conf.NMFATCH.clear()
    LDIRFORSEND=conf.getvalue('LDIRFORSEND')
    LDIRSEND=conf.getvalue('LDIRSEND')
    NATCH=list(); PATCH=list()
    F_SIGNATURE=readfirstline(LETNAME).split(" ")
    SHRM=CHEEFLINE=GCONF=False

    month=getmonth()
    with open(LETNAME, "r") as file:
        for strlet in file:
            strlet=strlet.replace('\n', '')
            if strlet.find('!AWR')!=-1:
                conf.AWRLET=True

            if len(strlet)>0:
                if strlet[0]!='#':
                    strlet=strlet.split('#')[0]
                    strlet=strlet.replace('[YEAR]', getdatetime('%Y'))
                    strlet=strlet.replace('[WEEK]', getdatetime('%W'))
                    
                    if strlet.find('[MONTH-1]')!=-1:
                        month=getmonth('-1m')
                        strlet=strlet.replace('[MONTH-1]', month.upper()) 
                    if strlet.find('[MONTH]')!=-1:
                        strlet=strlet.replace('[MONTH]', month.upper()) 
                        
                    strlet=strlet.replace('[DATE]', getdatetime(conf.getvalue('DTSTAMP')))                                                

                    if strlet.find('CHEEF:')!=-1: CHEEFLINE=True; continue
                    if strlet.find('GRECV:')!=-1 or strlet.find('GCONF:')!=-1: GCONF=True; continue
                    if strlet.find('SHRM:')!=-1:  SHRM=True; continue
                    if strlet.find("ATTACH:")!=-1 and strlet[0:7]=='ATTACH:':
                        atname=strlet.split(':')[1].rstrip()
                        tmp=atname.split('/')
                        conf.NMFATCH.append(tmp[len(tmp)-1])
                        if atname.find('/')!=-1:
                            if os.path.isfile(atname):
                                conf.PTFATCH.append(atname)
                            else:
                                cl.settextcolor(RED)
                                outt('Файл: '+atname+' не найден')
                                cl.setdefcolors()
                        else:
                            atname_path=prepname(atname)
                            if os.path.isfile(atname_path):
                                conf.PTFATCH.append(atname_path)
                            else:
                                cl.settextcolor(RED)
                                outt('Файл: '+atname+' не найден')
                                outt('Проверьте файл и повторите его оправку!')
                                cl.setdefcolors()

                    elif strlet[0]=='@':
                        i=0; slet=strlet.replace('@', '', 1)
                        if slet.find("E-MAIL:")!=-1:
                            e_adr=slet.split(';')
                            email=e_adr[0].split(":")[1]
                            for adrc in e_adr:
                                if adrc.find("SUBJ:")!=-1:
                                    SUBJ=adrc.split(":")[1]
                                    continue
                                elif adrc.find("TMAIL:")!=-1:
                                    TXTMAIL="Cообщение от станции "+conf.getvalue('STATION')+'\r\n'
                                    TXTMAIL+=adrc.split(":")[1]+'\r\n'
                                    continue
#                                elif adrc.find("ATTACH:")!=-1:
#                                    tmp=adrc.split(':')[1].split(',')
#                                    for atname in tmp:
#                                        PATCH.append(atname)
#                                        tmp=atname.split('/')
#                                       NATCH.append(tmp[len(tmp)-1])
#                                    continue

                            ADRESS.append(email+' / '+SUBJ+'=\r\n')

                            if len(conf.PTFATCH)>0:
                                TXTMAIL+='\r\nФайлы вложения:\r\n'
                                for atname in conf.NMFATCH: TXTMAIL+=atname+'\r\n'
                                for atname in NATCH: TXTMAIL+=atname+'\r\n'
                                for atname in conf.PTFATCH:
                                    if atname.find('/')==-1:
                                        atname=prepname(atname)
                                    ATCH+=atname+';'
                                for atname in PATCH:
                                    if atname.find('/')==-1:
                                        atname=prepname(atname)
                                    ATCH+=atname+';'
                            # All files for vrs-letter need to attaches
                            if TYPELT=='VRS':
                                for atname in ARCH_PARTS:
                                    if atname.find('/')==-1:
                                        atname=prepname(atname)
                                    ATCH+=atname+';'
                            if NAMESEND.find('/')==-1:
                                NAMESEND=LDIRFORSEND+'/'+NAMESEND
                            ATCH+=NAMESEND+';'
                            AddFnamePath(ATCH, email, SUBJ, TXTMAIL)
                        else:
                            TMP=slet.split(";")
                            strl=TMP[0].strip()
                            if len(TMP)>1:
                                SUBJ=TMP[1].strip().split(':')[1]

                            for wadress in conf.WADRESS:
                                if strl==wadress:
                                    ADRESS.append(wadress.upper()+'=\r\n')
                                    if TYPELT!='VRS':
                                        AddFnamePath(NAMESEND, conf.WFTPPTH[i], SUBJ);
                                    j=0
                                    for ptfatch in conf.PTFATCH:
                                        if TYPELT!='VRS':
                                            AddFnamePath(ptfatch, conf.WFTPPTH[i], SUBJ);
                                        j+=1
                                    break
                                i+=1
                            else:
                                ADRESSNW.append(strl+'=\r\n')
                                
                    elif strlet.find('=')!=-1:
                        LSIGN.append(strlet.upper()+'\r\n')
                        continue
                    elif strlet[0]=='!':
                        LBODY.append(strlet.replace('!', '', 1)+'\r\n')
                    else:
                        LBODY.append(strlet.upper()+'\r\n')

    # done with
    # Create letter
    write2file_str(NAMESEND, SVC_TITLE(TYPELT))
    if not F_SIGNATURE[0]=="#!AWR":
        if len(ADRESS)==0:
            txt="Не указаны адреса отправки"
            rita(txt)
            cl.setcolors(LYELLOW, BGRED)
            outt(txt)
            cl.setdefcolors()
            return False
        else:
            write2file_list(NAMESEND, ADRESS)
            write2file_list(NAMESEND, ADRESSNW)
            
    write2file_str(NAMESEND, '\r\n')

    if TYPELT=='VRS':
        shellcmd('sudo rm '+LETNAME)
        if len(conf.aFLS)>0:
            LBODY.append("\r\n"+NAMEARCH+" СОДЕРЖИТ:\r\n")
            for afls, alsz in zip(conf.aFLS, conf.aLSZ):
                LBODY.append(' '+afls+' ('+str(alsz)+' байт)\r\n')
        GCONF=True

    if len(conf.NMFATCH)>0:
        LBODY.append("\r\nФайлы вложения:\r\n")
        i=1
        for atname in conf.NMFATCH:
            LBODY.append(' '+str(i)+". "+atname+'\r\n'); i+=1

    if GCONF: LBODY.append("\r\nПРОШУ ПОДТВЕРДИТЬ ПРИЁМ "+NAMEARCH+" FTP ИЛИ EMAIL СПС\r\n")

    write2file_list(NAMESEND, LBODY)
    write2file_str(NAMESEND, '\r\n')

    LSIGN.insert(0, 'УВАЖЕНИЕМ=\r\n')
    if CHEEFLINE: 
        LSIGN.append(NSIGN+' '+conf.getvalue('STATION')+' '+conf.getvalue('CHEEF')+"-\r\n")
    else: SHRM=True
    
    if SHRM: 
        LSIGN.append(NSIGN+' '+conf.getvalue('STATION')+' ШРМ-\r\n')

    write2file_list(NAMESEND, LSIGN)
    return True

def make_VOS_SVC(FMASK='', SIGNATURE='', DBFILE=''):
    '''
        make(prepare) VOS_SVC-letter sends
    '''
    conf.SIGNATURE01=SIGNATURE; conf.SIGNATURE02='RADIO'
    
    if FMASK=='svc': LTTYPE='SVC'
    elif FMASK=='vos': LTTYPE='VOS'

    LDIRFORSEND=conf.getvalue('LDIRFORSEND'); MASK=FMASK+'*.txt'
    # FILES_SEND=fnmatch.filter(os.listdir(dirpath), "vos*.txt")
    FILES_SEND=lsdir(LDIRFORSEND, MASK)
    if len(FILES_SEND)>0:
        for FILESRC in FILES_SEND:
            FILESRC=LDIRFORSEND+'/'+FILESRC
            if fmtcheck(FILESRC) and sizecheck(FILESRC):
                SIGN=setsign(DBFILE);
                LDIRSEND=conf.getvalue('LDIRSEND')
                FILESEND=LDIRSEND+'/'+FMASK+SIGN+".txt"
                if parceletter(LTTYPE, FILESRC, FILESEND, SIGN):
                    setperm(FILESEND)
                    cparchfiles("OutBox", FILESEND, FILESRC)
                else:
                    shellcmd('sudo rm '+FILESEND)
                    outt('Письмо удалено. Отправка отменена')
                    return False
            else:
                sendcancel("Отмена VOS/SVC отправки")
                sendcancel("Файл "+FILESRC+" не найден")
                return False
    return True

def messagePrepVRS(SIGN, VRSSNM, NAMEARCHLT):
    '''
        make(prepare) VRS-letter sends
    '''
    ARCHTYPE=conf.getvalue('ARCHTYPE')
    outt("Подписной # "+SIGN+" Письмо: "+VRSSNM+" Формат архива: "+ARCHTYPE+".")
    outt("Имя файла: "+NAMEARCHLT)

def archivesvrs(VRSADD):
    '''
        copy VRS-files to archives
    '''
    ARCHDIR=conf.getvalue('ARCHDIR')
    LDIR=conf.getvalue('LDIR_ARCH')+'/OutBox/'+ARCHDIR
    RDIR=conf.getvalue('RDIR_ARCH')+'/OutBox/'+ARCHDIR

    # Copy VRS-letter to OutBox
    if not os.path.isdir(LDIR): conf.crdsetpm(LDIR)
    if not os.path.isdir(RDIR): shellcmd('mkdir -p "'+RDIR+'"')

    shellcmd('cp "'+VRSADD+'" "'+LDIR+'/"')
    shellcmd('cp "'+VRSADD+'" "'+RDIR+'/"')

    # Copy all to Archive OutBox
    LDIRSND=conf.getvalue('LDIR_ARCH')+'/Send/'+ARCHDIR
    RDIRSND=conf.getvalue('RDIR_ARCH')+'/Send/'+ARCHDIR
    LDIRFORSEND=conf.getvalue('LDIRFORSEND')
    LDIRSEND=conf.getvalue('LDIRSEND')
    ARCHTYPE=conf.getvalue('ARCHTYPE')

    if not os.path.isdir(LDIRSND): conf.crdsetpm(LDIRSND)
    if not os.path.isdir(RDIRSND): conf.crdsetpm(RDIRSND)

    if isdirnotempty(LDIRFORSEND):
        shellcmd('cp -R "'+LDIRFORSEND+'"/* "'+LDIR+'"/')
        shellcmd('cp -R "'+LDIRFORSEND+'"/* "'+RDIR+'"/')
        shellcmd('rm -R "'+LDIRFORSEND+'"/*')

    shellcmd('cp "'+LDIRSEND+'"/*'+ARCHTYPE+'* "'+LDIRSND+'"/')
    shellcmd('cp "'+LDIRSEND+'"/*'+ARCHTYPE+'* "'+RDIRSND+'"/')
    shellcmd('cp "'+VRSADD+'" "'+LDIRSND+'/"')
    shellcmd('cp "'+VRSADD+'" "'+RDIRSND+'/"')

def sendcancel(msg):
    '''
        Cancel sending
    '''
    cl.settextcolor(LYELLOW); cl.setbackcolor(BGRED)
    outt(msg)
    cl.setdefcolors()

def sfq(numparts=0, type='7z'):
    '''
        return string nums archive parts
    '''
    SFQ=""
    if numparts<10: 
        if type=='7z': SFQ="00"+str(numparts)
        elif type=='zip': SFQ="0"+str(numparts-1)
    elif numparts>=10: 
        if type=='7z': SFQ="0"+str(numparts)
        elif type=='zip': SFQ=str(numparts-1)
    elif numparts>=100: 
        if type=='7z': SFQ=str(numparts)
        elif type=='zip': SFQ=str(numparts-1)
    return SFQ

def make_VRS():
    '''
        make(prepare) VRS sends
    '''
    LDIRFORSEND=conf.getvalue("LDIRFORSEND")
    ARCH_PARTS=[]
    
    if isdirnotempty(LDIRFORSEND):

        LETTERSRC=conf.getvalue('LDIRFORSEND')+"/vrs.txt"
        LETTERTMP=conf.getvalue('TMPASPD')+"/vrs.txt"
        conf.SIGNATURE01='#!VRS'; conf.SIGNATURE02='RADIO';

        FILEPACKSIZE=int(conf.getvalue('FILEPACKSIZE'))
        ARCHSIZE=str(conf.getvalue('ARCHSIZE'))
        ARCHTYPE=conf.getvalue('ARCHTYPE') # !!!
        LDIRSEND=conf.getvalue('LDIRSEND')
        FTPSTDPATHSEND=conf.getvalue('FTPSTDPATHSEND')

        # Calculate sign for send
        SIGN=setsign(conf.getvalue('INDEX_VRS'))
        VRSSNM="vrs"+SIGN+".txt"
        LETTERINFO=LDIRSEND+'/'+VRSSNM
        NAMEARCH="vrs"+SIGN+"."+ARCHTYPE

        if not os.path.isfile(LETTERSRC):
            outt('Сопроводительный файл отсутствует.')
            rita('Сопроводительный файл отсутствует.')
#            WL1=True
#            while WL1:
#                for case in switch(psecho('Отправить стандартный файл?(y/n/a):')):
#                    if case('y', 'N'):
#                        outt('Отправляется стандартный сопроводительный файл')
#                        stdvrstxt(LETTERSRC, NAMEARCH)
#                        WL1=False
#                        break
#                    if case('n', 'N', 'a', 'A', 'e', 'E'):
#                        outt("Отмена отправки")
#                        return
#                    else:
#                        pass

        if not fmtcheck(LETTERSRC):
            sendcancel("Отмена VRS-отправки.")
            return

        shellcmd('mv -f "'+LETTERSRC+'" "'+LETTERTMP+'"')
        getfsinfo(LDIRFORSEND)
        fslist(conf.getvalue('FLLSLOG'), 'Файлы для VRS отправки:')

        # Get size of files in directory before compressed
        if sumdirsize(LDIRFORSEND)>=FILEPACKSIZE:
            conf.setvalue('BIGSIZE', True)
            rita("Внимание! Большой размер архива. Несколько частей. Проверьте его отправку!")
            cl.settextcolor(LRED)
            outt('Многотомный архив! Проверьте отправку!')
            szbefore=sumdirsize(LDIRFORSEND)
            cl.settextcolor(LGREEN)
            outt('Размер архивной части: '+ARCHSIZE)
            cl.setdefcolors()

            if ARCHTYPE=='rar':
                shellcmd('rar a -ep -m5 -v'+ARCHSIZE+' "'+LDIRSEND+'/'+NAMEARCH+'" "'+LDIRFORSEND+'"/*', '/dev/null')
                ARCH_PARTS=lsdir(LDIRSEND, 'vrs'+SIGN+'.part*')
                NAMEARCHLT='vrs'+SIGN+'.part1-'+str(len(ARCH_PARTS))+'.'+ARCHTYPE
            elif ARCHTYPE=='zip':
                shellcmd('zip -9 -r -s '+ARCHSIZE+' "'+LDIRSEND+'/'+NAMEARCH+'" "'+LDIRFORSEND+'"/*', '/dev/null')
                ARCH_PARTS=lsdir(LDIRSEND, 'vrs'+SIGN+'.z*')
                NAMEARCHLT='vrs'+SIGN+'.zip-z'+sfq(len(ARCH_PARTS), ARCHTYPE)
            else:
                shellcmd('7z a -v'+ARCHSIZE+' "'+LDIRSEND+'/'+NAMEARCH+'" "'+LDIRFORSEND+'"/*', '/dev/null')
                ARCH_PARTS=lsdir(LDIRSEND, 'vrs'+SIGN+'.'+ARCHTYPE+'*')
                NAMEARCHLT="vrs"+SIGN+"."+ARCHTYPE+".001-"+sfq(len(ARCH_PARTS), ARCHTYPE)

            messagePrepVRS(SIGN, VRSSNM, NAMEARCHLT)
            shellcmd('mv -f "'+LETTERTMP+'" "'+LETTERSRC+'"')
            parceletter('VRS', LETTERSRC, LETTERINFO, SIGN, ARCH_PARTS, NAMEARCHLT) # Генерация cопроводительного письма
            shellcmd('rm -f "'+LETTERSRC+'"')
            shellcmd('mv -f "'+LETTERINFO+'" "'+LDIRSEND+'/"')
            LETTERINFO=LDIRSEND+'/'+VRSSNM
            AddFnamePath(LETTERINFO, FTPSTDPATHSEND)
            for arch_part in ARCH_PARTS:
                AddFnamePath(LDIRSEND+'/'+arch_part, FTPSTDPATHSEND)
        else:
            szbefore=sumdirsize(LDIRFORSEND)
            if ARCHTYPE=='rar':
                shellcmd('rar a -ep -m5 "'+LDIRSEND+'/'+NAMEARCH+LDIRFORSEND+'"/*', '/dev/null')
            elif ARCHTYPE=='zip':
                shellcmd('zip -9 -r "'+LDIRSEND+'/'+NAMEARCH+'" "'+LDIRFORSEND+'"/*', '/dev/null')
            else:
                shellcmd('7z a "'+LDIRSEND+'/'+NAMEARCH+'" "'+LDIRFORSEND+'"/*', '/dev/null')
            messagePrepVRS(SIGN, VRSSNM, NAMEARCH)
            ARCH_PARTS=lsdir(LDIRSEND, NAMEARCH)
            shellcmd('mv -f "'+LETTERTMP+'" "'+LETTERSRC+'"')
            parceletter('VRS', LETTERSRC, LETTERINFO, SIGN, ARCH_PARTS, NAMEARCH) # Генерация cопроводительного письма
            shellcmd('rm -f "'+LETTERSRC+'"')
            shellcmd('mv -f "'+LETTERINFO+'" "'+LDIRSEND+'/"')
            LETTERINFO=LDIRSEND+'/'+VRSSNM
            AddFnamePath(LETTERINFO, FTPSTDPATHSEND)
            AddFnamePath(LDIRSEND+'/'+NAMEARCH, FTPSTDPATHSEND)

        # get size after compressed
        szpacksend=sumdirsize(LDIRSEND)
        outt('Объём файлов: '+str(naturalsize(szbefore))+'. Объём отправки: '+str(naturalsize(szpacksend)))
        cl.setdefcolors()
        archivesvrs(LETTERINFO)

def msgsend(FLSENDPT='', RMPATH=conf.getvalue('FTPSTDPATHSEND')):
    '''
        send mrssages to ftp ASPD and to e-mail
    '''
    if FLSENDPT=='':
        if not os.path.isfile(FLSENDPT):
            LDIRFORSEND=conf.getvalue('LDIRFORSEND')
            if isdirnotempty(LDIRFORSEND):

                # Prepare files
                make_METEO("avos", '}|sm`', conf.getvalue('INDEX_AVS'))
                make_METEO("climat", '}|klb', conf.getvalue('INDEX_CLM'))
                make_VOS_SVC("vos", "#!VOS", conf.getvalue('INDEX_VOS'))
                make_VOS_SVC("svc", "#!SVC", conf.getvalue('INDEX_SVC'))
                make_VRS()

                LDIRSEND=conf.getvalue('LDIRSEND')

                if isdirnotempty(LDIRSEND):
                    if conf.getvalue('SQUID')=="OFF" and conf.getvalue('BIGSIZE')=="ON":
                        cl.settextcolor(LGREEN); outt('Squid stop...'); cl.setdefcolors();
                        shellcmd('sudo squid -k interrupt')

                    msgsendnumber()
                    GenCmdSend()
                    getfsinfo(LDIRSEND)
                    fslist(conf.getvalue('FLLSLOG'), "Файлы на отправку:")
                    emailsend()
                    ftpsend(LDIRSEND)
                    shellcmd('rm "'+LDIRSEND+'"/*')
                    infoline("Отправка завершена.")
                else:
                    infoline('Файлов для отправки в '+LDIRSEND+' нет')
                    rita("Файлов для отправки нет.")

                if conf.getvalue('SQUID')=="OFF" and conf.getvalue('BIGSIZE')=="ON":
                    cl.settextcolor(LGREEN); outt("Squid start..."); cl.setdefcolors()
                    shellcmd('sudo service squid start')
                    conf.setvalue('BIGSIZE', 'OFF')
            else:
                infoline("Файлов в каталоге: "+LDIRFORSEND+" нет")
                infoline("Отмена отправки")
                rita("Отмена. Файлов для отправки нет.")
    else:
        msgsendnumber()

        AddFnamePath(FLSENDPT, RMPATH)
        GenCmdSend()

        if os.path.isfile(FLSENDPT):
            ftpsendonce(FLSENDPT)
        else:
            cl.settextcolor(LMAGENTA)
            outt("Файл "+FLSENDPT+" не найден")
            cl.setdefcolors()
            rita("Файл для отправки не найден")

    status()

def chckhavefile(FNAME, LOGFL, RMPATH=''):
    FTPCMD=conf.FTPCMDOPEN
    FTPSTDPATHRECV=conf.getvalue('FTPSTDPATHRECV')

    if RMPATH=='': FTPCMD+='cd '+FTPSTDPATHRECV+';rels;'
    else: FTPCMD+='cd '+RMPATH+';rels;'

    FTPCMD+=conf.FTPCMDCLOSE
    shellcmd('lftp -c "'+FTPCMD+'"', LOGFL, '>')

    if os.path.isfile(LOGFL):
        with open(LOGFL, "r") as file:
            for LN in file:
                if LN.find(FNAME)!=-1:
                    return True;
                elif LN.find('550')!=-1:
                    return False;

    return False

def ftprecvonce(RMFILE, LPATH=''):
    ret=False
    LDIRRECV=conf.getvalue('LDIRRECV')

    rfile=getfnamefpath(RMFILE) # namefile
    rpath=getfpathfpath(RMFILE) # fullpath

    FTPRLOG=conf.getvalue('FTPRLOG')
    MAXRPT=conf.getvalue('MAXRPT')

    if chckhavefile(rfile, FTPRLOG, rpath):
        for i in range(int(MAXRPT)):
            outt("Запрос #"+str(i+1)+'('+MAXRPT+')')
            FTPCMD=conf.FTPCMDOPEN

            if LPATH!='': lpath=LPATH
            else: lpath=LDIRRECV+'/'+conf.getvalue('ARCHDIR')

            FTPCMD+="rels "+rpath+";get -O "+lpath+" -c "+RMFILE+";"
            FTPCMD+=conf.FTPCMDCLOSE

            shellcmd('lftp -c "'+FTPCMD+'"', FTPRLOG, '>')

            getfsinfo(lpath)

            if fseqsize("Приём "+RMFILE, FTPRLOG):
                outt('Файл '+RMFILE+' принят: #'+str(i+1)+'('+MAXRPT+')')
                rita('Приняты файлы от А эС Пэ Дэ. Размеры файлов совпадают.')
                ret=True
                break
            else:
                cl.settextcolor(LMAGENTA)
                outt('Файл '+RMFILE+' не принят. Повтор #'+str(i+1)+'('+MAXRPT+')')
                rita('Файл не принят. Повтор. '+str(i+1)+' из '+MAXRPT)
                cl.setdefcolors()

        if i>=int(MAXRPT):
            i-=1
            outt('Макс. кол-во '+str(i+1)+'('+MAXRPT+') попыток приёма. Проверьте соединение')
            rita('Максимальное количество попыток приёма. Проверьте соединение.')
        else:
            getfsinfo(lpath)
            fslist(FTPRLOG, 'Принятые', 'FTPFS')
    else:
        cl.settextcolor(LMAGENTA)
        outt('Файла '+RMFILE+' на сервере не найдено')
        cl.setdefcolors()

    return ret

def ftpdel(RPATHFN, TYPE=''):
    '''ftp deleter
        void ftpdel /rpath/fname
    '''
    rfile=getfnamefpath(RPATHFN)
    rpath=getfpathfpath(RPATHFN)

    if rfile=='': rfile='*'

    FTPCMD=conf.FTPCMDOPEN
    FTPCMD+='cd '+rpath+';echo '+rpath+' # LIST;rels;'
    FTPCMD+='echo '+rpath+' # DELETE;mrm '+rfile+';rels;'
    FTPCMD+=conf.FTPCMDCLOSE

    outt("Удаление файлов на FTP АСПД...")
    if TYPE!='SILENT':
        shellcmd('lftp -c "'+FTPCMD+'"', conf.getvalue('FTPLLOG'), '>')
        fslist(conf.getvalue('FTPLLOG'), "Содержимое: "+conf.getvalue('FTPSERVER')+' '+rpath, 'FTPFS')
    else:
        shellcmd('lftp -c "'+FTPCMD+'"', conf.getvalue('FTPLLOG'), '>', 'ASYNC')

def ftprecv(LDIR='', TEXT=''):
    '''
        ftprecv ftp receiver
        void ftprecv localdir text
    '''
    ret=False
    i=0; conf.FTPCMD=""; FRECV="*";
    FTPSTDPATHRECV=conf.getvalue('FTPSTDPATHRECV')
    FTPRLOG=conf.getvalue('FTPRLOG')
    MAXRPT=int(conf.getvalue('MAXRPT'))

    conf.FTPCMD=conf.FTPCMDOPEN
    conf.FTPCMD+='cd '+FTPSTDPATHRECV+';echo '+FTPSTDPATHRECV+';rels;mget -O \\"'+LDIR+'\\" -c "'+FRECV+'";'
    conf.FTPCMD+=conf.FTPCMDCLOSE

    for i in range(MAXRPT):
        shellcmd('lftp -c "'+conf.FTPCMD+'"', conf.getvalue('FTPRLOG'), '>')

        if isdirnotempty(LDIR):
            getfsinfo(LDIR+'/')
            TM=getdatetime("%H %M") # local date
            rita("Приняты файлы от А эС Пэ Дэ. Время "+TM)
            fslist(FTPRLOG, TEXT, 'FTPFS')

            if fseqsize("Приём от АСПД", FTPRLOG):
                txt="Проверьте каталог приёма."
                infoline(txt+"#"+str(i+1)+'('+str(MAXRPT)+')'); rita(txt);
                rita("Размер совпадает.")
                ret=True
                break;
            else:
                cl.settextcolor(RED)
                outt("Повторный приём #"+str(i+1)+'('+str(MAXRPT)+')')
                cl.setdefcolors()
                rita("Размер файла не совпадает. Повторный приём.")
        else:
            break;

    if i>=MAXRPT:
        infoline('Макс. кол-во '+str(i)+'('+str(MAXRPT)+') попыток приёма. Проверьте соединение')
        rita("Максимальное количество попыток приёма.")
        ret=False

    return ret

def msgrecv(RMPATHFL='', LPATH=''):
    '''
        Recv messages from ftp
    '''
    LDIRRECV=conf.getvalue('LDIRRECV')
    ARCHDIR=conf.getvalue('ARCHDIR')
    LDIR_ARCH=conf.getvalue('LDIR_ARCH')
    RDIR_ARCH=conf.getvalue('RDIR_ARCH')

    if RMPATHFL=='':
        infoline("Приём сообщений с сервера АСПД. "+conf.getvalue('FTPSERVER'))
        ret=ftprecv(conf.getvalue('LDIRRECV'), conf.getvalue('FTPSTDPATHRECV'))

        if isdirnotempty(LDIRRECV):
            outt("Принятые сообщения сохранены в /Recv/"+ARCHDIR)
            infoline("Очистка каталога "+conf.getvalue('FTPSTDPATHRECV'))
            ftpdel(conf.getvalue('FTPSTDPATHRECV')+"/", 'SILENT')

            if not os.path.isdir(LDIR_ARCH+'/Recv/'+ARCHDIR): conf.crdsetpm(LDIR_ARCH+'/Recv/'+ARCHDIR)
            if not os.path.isdir(RDIR_ARCH+'/Recv/'+ARCHDIR): conf.crdsetpm(RDIR_ARCH+'/Recv/'+ARCHDIR)

            shellcmd('cp "'+LDIRRECV+'"/* "'+LDIR_ARCH+'/Recv/'+ARCHDIR+'"/')
            shellcmd('cp "'+LDIRRECV+'"/* "'+RDIR_ARCH+'/Recv/'+ARCHDIR+'"/')
            shellcmd('rm "'+LDIRRECV+'"/*')
            if ret: updatesessionofday(conf.getvalue("INDEX_RCV"))
            status()
        else:
            cl.settextcolor(GRAY)
            outt("Сообщений от АСПД не принято.")
    else:
        outt("Запрос "+RMPATHFL+" от АСПД")
        ret=ftprecvonce(RMPATHFL, LPATH)
        if ret: updatesessionofday(conf.getvalue('INDEX_RCV'))
        status()

def msgll(LPATH=''):
    ''' get list files in local archives directories '''
    if LPATH!='': VIEW=LPATH
    else: VIEW='all'

    FLLSLOG=conf.getvalue('FLLSLOG')
    LDIR_ARCH=conf.getvalue('LDIR_ARCH')
    ARCHDIR=conf.getvalue('ARCHDIR')
    ARCHMETEO=LDIR_ARCH+'/Meteo/'+ARCHDIR
    ARCHSEND=LDIR_ARCH+'/Send/'+ARCHDIR
    ARCHRECV=LDIR_ARCH+'/Recv/'+ARCHDIR
    LDIRDELAY=conf.getvalue('LDIRDELAY')

    if LPATH.find("/")==-1:

        if VIEW=='all': infoline("Каталоги отправки / приёма")

        if VIEW in ('all', 'meteo'):
            txt="Метео"
            if os.path.isdir(ARCHMETEO):
                getfsinfo(ARCHMETEO)
                if not fslist(FLLSLOG, txt):
                    outt(txt+": Не было отправки СИН")
            else:
                outt(txt+": Не было отправки СИН")

        if VIEW in ('all', 'delay'):
            txt="Отложенный СИН"
            if os.path.isfile(LDIRDELAY+"/avos.txt"):
                getfsinfo(LDIRDELAY)
                if not fslist(FLLSLOG, txt, "DELAY"):
                    outt(txt+": Нет отложенных СИН")
            else:
                outt(txt+": Нет отложенных СИН")

        if VIEW in ('all', 'send'):
            txt="Отправленные"
            if os.path.isdir(ARCHSEND):
                getfsinfo(ARCHSEND)
                if not fslist(FLLSLOG, txt):
                    outt(txt+": Не было отправки")
            else:
                outt(txt+": Не было отправки")

        if VIEW in ('all', 'recv'):
            txt="Принятые"
            if os.path.isdir(ARCHRECV):
                getfsinfo(ARCHRECV)
                if not fslist(FLLSLOG, txt):
                    outt(txt+": Не было приёма")
            else:
                outt(txt+": Не было приёма")
    else:
        getfsinfo(LPATH)
        fslist(FLLSLOG, LPATH)

    if VIEW=='all': status()

def rmdelay():
    '''
        delete delay sends
    '''
    infoline("Удаление отложенного СИН")
    txt="";
    delaysign=conf.getvalue("INFODIR")+"/delay"
    delaysendfile=conf.getvalue("LDIRDELAY")+"/avos.txt"

    if os.path.isfile(delaysign):
        shellcmd('rm -v '+delaysign, '/dev/null', '>')
        txt+="Отложенный СИН отменён. "
    else:
        txt+="Нет отложенной отправки и "

    if os.path.isfile(delaysendfile):
        shellcmd('rm -v '+delaysendfile, '/dev/null', '>')
        txt+="Файл СИН удалён."
    else:
        txt+="файла СИН."

    outt(txt);
    status();

def msgrl(FTPPATH='', FTPLS=''):
    '''
        get list from remote(ftp) list
    '''
    FTPCMD=conf.FTPCMDOPEN
    FTPSTDPATHSEND=conf.getvalue("FTPSTDPATHSEND")
    FTPSTDPATHRECV=conf.getvalue("FTPSTDPATHRECV")

    if FTPPATH=='':
        FTPCMD+="cd "+FTPSTDPATHSEND+";echo "+FTPSTDPATHSEND+";rels;cd "+FTPSTDPATHRECV+";echo "+FTPSTDPATHRECV+";rels;"
    else:
        FTPCMD+="cd "+FTPPATH+";echo "+FTPPATH+";rels;"

    FTPCMD+=conf.FTPCMDCLOSE
    infoline("Просмотр каталогов сервера АСПД "+conf.getvalue('FTPSERVER'))
    FTPLLOG=conf.getvalue('FTPLLOG')
    shellcmd('lftp -c "'+FTPCMD+'"', FTPLLOG, '>')
    fslist(FTPLLOG, "Сервер "+conf.getvalue('FTPSERVER'), FTPLS)

    status()

def msgdel(RMPATH=''):
    '''
        delete file(s) from remore(ftp)
    '''
    outt("Удаление "+RMPATH)
    if RMPATH=='':
        ret=ftpdel(conf.getvalue('FTPSTDPATHRECV'))
    else:
        ret=ftpdel(RMPATH)

    if ret: updatesessionofday(conf.getvalue('INDEX_DEL'))
    status()

def rita(outtext=''):
    '''
        R)ечевой I)нформатор T)актической A)бстановки
    '''
    import socket

    if conf.WORKTYPE=='debug': return
    if not conf.RITASERV: return

    try:
        IP=conf.getvalue('RITASERVER')
        PORT=int(conf.getvalue('RITAPORT'))

        sock=socket.socket()
        
        sock.connect((IP, PORT))
        sock.settimeout(5)

        mixvol=conf.getvalue('RITAVOLUME')
        senddata="mixer vol "+mixvol+":"+mixvol+";"
        senddata+='echo "'+outtext+'" | RHVoice-test -p elena ;'

        dt=getdatetime("%k")
        if dt>=conf.getvalue('TMAF'):
            mixvol=conf.getvalue('RITAVOLAFT')
            senddata+="mixer vol "+mixvol+":"+mixvol+" ;"
            
        sock.send(bytes(senddata, 'utf-8'))
        sock.close()

    except OSError as msg:
        conf.RITASERV=False
        cl.settextcolor(RED)
        outt('Речевой сервер не найден')
        cl.setdefcolors()
        sock.close()
        return
        
def setallperm(opt=''):
    '''
        set all permissions
    '''
    if opt=='' or opt=="-v":
        if opt=='-v': opt=opt[1]
        opt="Rf"+opt
        infoline('Установка прав (opt: -'+opt+')')
        conf.permArchDir(opt)
        conf.permInfoDir(opt)
        outt("Установка прав завершена")
    else:
        cl.settextcolor(LRED)
        outt("Неверный параметр "+str(opt)+" для комманды setperm")
        mhelp('setperm')
        outt("Смотрите: man chown / man chmod")
        cl.setdefcolors

    status()

def locked(TYPELOCKED=''):
    '''
        check locked state
    '''
    ret=False
    if os.path.isfile(TYPELOCKED):
        ret=True
    else:
        shellcmd('touch "'+TYPELOCKED+'"')
    return ret

def inputmeteodata(SIGNATURE):
    '''
        input meteo-data
    '''
    TBCOLOR=cl.getcolor('TBCOLOR'); BBCOLOR=cl.getcolor('BBCOLOR')
    VCL=LGREEN+BBCOLOR; MCL=LGREEN+BBCOLOR; SZC=RED+BBCOLOR; DFC=TBCOLOR+BBCOLOR

    cl.settextcolor(LGREEN)
    lnm='─'*WSLZ
    outt(lnm, "scr")
    outt("Введите данные. Ввод завершить символом '='. Сохранить [Enter]", "scr")

    MINFSIZE=int(conf.getvalue('MINFSIZE'))

    WL_L1=True
    while WL_L1:
        outt(lnm, "scr"); cl.setdefcolors()
        scale=''
        for i in range(9): scale+=' '*9+str(i+1)
        scale=replacepos(scale, VCL+str(MINFSIZE)+DFC, MINFSIZE)
        outt(scale+" "+' '*5+MCL+"x10b", 'scr')
        scale=''
        for i in range(20): scale+='─'*4+'┼'
        outt(replacepos(scale, MCL+'▼'+DFC, MINFSIZE).strip(), 'scr')
        #cpos=getposxy()
        TLG=SIGNATURE+' '
        TLG+=psecho(TLG, LCYAN, BGBLACK);
        outt(replacepos(lnm, MCL+'▲'+DFC, MINFSIZE), "scr")
        att=""; fsz=len(TLG);
        if TLG[fsz-1]!='=': TLG=TLG+'='; att=" (добавленo '=')"; fsz+=1
        outt("TLG: "+TLG, 'log')

        txt1="Размер файла: "; t_1=len(txt1);
        f_z=len(str(fsz));
        txt2=' b'+att; t_2=WSLZ-t_1-f_z;

        outt(("{:{t1}s}"+SZC+"{:{fz}s}"+DFC+"{:<{t2}s}").format(txt1, str(fsz), txt2, t1=t_1, fz=f_z , t2=t_2))

        if fsz<=MINFSIZE:
            cl.settextcolor(LYELLOW); cl.setbackcolor(BGRED)
            outt("Размер сообщения меньше минимального. Повторите ввод.")
            cl.setdefcolors()
            continue
        WL_L2=True
        while WL_L2:
            for case in switch(psecho("Данные введены верно?(y/n/a):")):
                if case('y', 'N'):
                    outt("Данные для отправки:")
                    outt(lnm, 'scr'); outt(TLG); outt(lnm, 'scr');
                    WL_L1=WL_L2=False
                    break
                if case('n', 'N'):
                    WL_L2=False
                    break
                if case('a', 'A', 'e', 'E'):
                    outt("Отмена отправки")
                    TLG=''
                    WL_L1=WL_L2=False
                    break
                else:
                    pass

    return TLG

def genawr():
    '''
        get templates and gen awr(фап)
    '''
    AWRLT=''
    with open(conf.getvalue("TEMPLATES")+'/lt_fap.templ', "r") as file:
        for awrline in file:
            if len(awrline)>1:
                if awrline.find('UTC:')!=-1:
                    locale.setlocale(locale.LC_ALL, "en_US.UTF-8")
                    AWRLT+='!Actwx '+conf.getvalue('STATIONLT')+' SKIWAY '+getdatetimeUTC('%Y-%b-%d %H:%M').upper()+' UTC\n'
                    locale.setlocale(locale.LC_ALL, "ru_RU.UTF-8")
                elif awrline.find('@')!=-1 or awrline.find('!')!=-1:
                    AWRLT+=awrline
                else:
                    AWRLT+=awrline
            else:
                AWRLT+='\n'
    return AWRLT

def msglt(TYPELETTER='', GRPMSEND='', SECONDTYPE='vos'):
    '''
        create letter
    '''
    NMGRP=[]; adr=[]; GRP=False;
    conf.getadrbook()

    cl.setcolors(GRAY, BGBLUE)
    DT=getdatetime("%H %M")
    txt="Создание сообщения"; infoline(txt); rita(txt+". Время "+DT);

    # Выбор типа сообщения
    if TYPELETTER=='':
        outt(" avos - метео сводка        climat - климатическая метеосводка")
        outt("  vos - телеграмма             svc - служебная телеграмма")
        outt("  vrs - архивная телеграмма    grp - отправка на группу")
        outt("  awr - авиа погода            e(xit) отмена")
        str1="Выберите тип сообщения:"
        str2="(avos|climat|awr|vos|svc|vrs|grp|e(xit)):"
        rita(str1)

        while True:
            outt(str1)
            lttype=psecho(str2)

            if lttype=="e": outt("Отмена"); return;
            elif lttype in ("grp", "vos", "svc", "vrs", "avos", "climat", "awr"):
                break;
            else:
                outt("Неизвестный тип сообщения "+lttype)
                outt("Отмена")
                mhelp('letter')
                return
    else:
        lttype=TYPELETTER
        if lttype=="e": outt("Отмена"); return;
        elif lttype in ("grp", "vos", "svc", "vrs", "avos", "climat"):
            pass
        elif lttype in ("awr"):
            lttype="grp"
            GRPMSEND='ФАП'
            SECONDTYPE='awr'
        else:
            outt("Неизвестный тип сообщения "+lttype)
            outt("Отмена")
            mhelp ('letter')
            return

#   # choice adresses
    if lttype=='grp':
        GRP=True
        if GRPMSEND=='':
            lttype="vos"
            abook()
            txt="Групповая рассылка"; outt(txt); rita(txt);
            rita("Введите имена групп, через пробел.")
            tstr="Введите имена групп (имя1 имя2 ... e(xit)):"
            while True:
                ngroups=psecho(tstr).upper()
                if ngroups in ('e', 'E'): outt("Отмена."); return;
                else:
                    NMGRP=ngroups.split(' ')
                    break;
        else:
            NMGRP.append(GRPMSEND)
            if SECONDTYPE!='':
                lttype=SECONDTYPE

    if lttype=="avos":
        txt="Отправка СИН сообщения"
        infoline(txt); rita(txt)
    elif lttype=="climat":
        txt="Отправка сообщения о климате"
        infoline(txt); rita(txt)
    elif lttype=='awr':
        txt="Отправка ФАП сообщения"
        infoline(txt); rita(txt)
    else:
        if not GRP:
            abook()
            outt("Тип сообщения "+lttype)
            rita("Введите порядковые номера адресов, через пробел.")
            tstr="Выберите адреса (1 2 3... e(xit)):"
            while True:
                adress=psecho(tstr)
                if adress in ('e', 'E'): outt("Отмена."); return;
                else: adr=adress.split(' ')
                break;

    METEOSIGNATURE=conf.getvalue('METEOSIGNATURE')

    # add info to letter
    if lttype=="avos":
        FNAME="avos.txt"
        TMPFILE=conf.TMPASPD+'/'+FNAME
        SIGNATURE='}|sm` '+conf.getvalue('METEOSIGNATURE')
    elif lttype=="climat":
        FNAME="avosclimat.txt"
        TMPFILE=conf.TMPASPD+'/'+FNAME
        SIGNATURE='}|klb '+METEOSIGNATURE+' 111'
    elif lttype in ('vos', 'svc', 'vrs', 'awr', 'grp'):
        if lttype=='grp': lttype='vos'
        if lttype=="awr":
            FNAME="vos.txt"
            TMPFILE=conf.TMPASPD+'/'+FNAME
            write2log(TMPFILE, '#!AWR RADIO !')
            write2log(TMPFILE, "# !!! ====================================================== !!!")
            write2log(TMPFILE, "# !!! Удалите/Добавьте адреса!!!\n# !!! или отредактируйте адресную книгу в секции ФАП !!!")
        else:
            FNAME=lttype+'.txt'
            TMPFILE=conf.TMPASPD+'/'+FNAME
            write2log(TMPFILE, '#!'+lttype.upper()+' RADIO !')
            hlp=readfile(conf.getvalue("TEMPLATES")+'/lt_hlp.templ')
            write2log(TMPFILE, hlp)

        conf.crfsetpm(TMPFILE)
        
        vadr=list()
        if GRP :
            for NM in NMGRP:
                for vadr in conf.WADRGROUP[NM]:
                    write2log(TMPFILE, "@"+vadr)
        else:
            for i in range(conf.COUNTADRESS):
                for j in range(len(adr)):
                    if i+1==int(adr[j]):
                        write2log(TMPFILE, '@'+conf.WADRESS[i])
                        outt("Отправка для "+conf.WADRESS[i])
                        break

        if lttype=="awr":
            write2log(TMPFILE, "# !!! ====================================================== !!!")
            write2log(TMPFILE, genawr())
        else:
            write2log(TMPFILE, "")
            write2log(TMPFILE, "#ТЕКСТ")
            write2log(TMPFILE, "")
            write2log(TMPFILE, "#CHEEF:")
    else:
        outt("Неизвестный тип сообщения "+lttype+". Отмена")
        mhelp('letter')
        return

    # Завершение формирования письма
    WL_L1=True
    while WL_L1:
        rita("Редактирование сообщения")
        if lttype=="avos" or lttype=="climat":
            TLG=inputmeteodata(SIGNATURE)
            if TLG=='': break
            write2file_str(TMPFILE, TLG)
            conf.crfsetpm(TMPFILE)
            rita("Ввод завершён")
        else:
            shellcmd('export LANG=ru_RU.UTF-8 && export LC_ALL=ru_RU.UTF-8; export TERM=xterm; '+conf.EDITOR+' "'+TMPFILE+'"')
            rita("Редактирование завершено")

        WL_L2=True
        while WL_L2:
            for case in switch(psecho("Отправить сообщение?(y/n/редактировать(e)):")):
                if case('y', 'N'):
                    shellcmd('mv "'+TMPFILE+'" "'+conf.getvalue('LDIRFORSEND')+'"')
                    txt="Отправка сообщения."
                    outt(txt); rita(txt);
                    msgsend()
                    WL_L1=WL_L2=False
                    break
                if case('n', 'N'):
                    txt="Отправка отменена."
                    outt(txt); rita(txt);

                    WL_L3=True
                    while WL_L3:
                        for case in switch(psecho("Сохранить сообщение?(y/n):")):
                            if case('n', 'N'):
                                shellcmd('rm "'+TMPFILE+'"')
                                txt="Сообщение удалено."
                                outt(txt); rita(txt);
                                WL_L3=False
                                break
                            if case('y', 'Y'):
                                DT=getdatetime("%m%d-%H%M")     # local date
                                UNKNOWNDIR=conf.getvalue('UNKNOWNDIR')
                                shellcmd('mv "'+TMPFILE+'" "'+UNKNOWNDIR+'/'+DT+'-'+FNAME+'"')
                                txt="Сообщение сохраняется в каталоге для неотправленных"
                                outt(txt); rita(txt);
                                outt(UNKNOWNDIR+'/'+DT+'-'+FNAME)
                                WL_L3=False
                                break
                            else:
                                pass

                    WL_L1=WL_L2=False
                    break
                if case('e', 'E'):
                    WL_L2=False
                    break
                else:
                    pass

    if os.path.isfile(TMPFILE):
        shellcmd('rm "'+TMPFILE+'"')

def msgmail(EMAIL='', SUBJ='', TXTM='', ATCH=''):
    '''
        E-mail Send
        void msgmail e-mail subj text attach
    '''
    TMPFILE=conf.getvalue('TMPFILE')
    if EMAIL=='':
        rita("Укажите адрес.")
        EMAIL=psecho("E-mail: ")

    if SUBJ=='':
        rita("Задайте заголовок.")
        SUBJ=psecho("Заголовок: ")

    if ATCH=='':
        if TXTM=='':
            rita("Наберите сообщение.")
            shellcmd('export TERM=xterm; mcedit "'+TMPFILE+'"')
            TXTM=readfile(TMPFILE)
            outt("TMAIL:")
            print (TXTM)
            shellcmd('sudo rm "'+TMPFILE+'"')

    AddFnamePath(ATCH, EMAIL, SUBJ, TXTM)
    GenCmdSend()
    ret=emailsend()
    if ret: updatesessionofday(conf.getvalue('INDEX_SND'))
    status()

def getzerostat(dbname):
    '''
        get zero stat if file not have actual date
    '''
    if dbname==conf.getvalue('INDEX_AVS'):
        return"0000"
    elif dbname in (conf.getvalue('INDEX_SND'), conf.getvalue('INDEX_RCV'),
                    conf.getvalue('INDEX_DEL'), conf.getvalue('INDEX_EML')):
        return '0'
    else:
        return getdatetime('%d%m')+'00' # local date

def getsign(dbname):
    '''
        get current sing
    '''
    line=SAVEDATE=''
    DATE=getdatetime('%d%m') # local date
    if os.path.isfile(dbname):
        with open(dbname, "r") as file:
            for line in file:
                tmp=line.split(' ')
                SAVEDATE=tmp[0].split(':')[1]
                if DATE==SAVEDATE:
                    return tmp[2].split(':')[1]
                else:
                    return getzerostat(dbname)
    else:
        return getzerostat(dbname)

def status(tout='all'):
    '''
        show send statistics
    '''
    LINE='';

    NP=cl.getcolor('TBTCOLOR')+cl.getcolor('TBBCOLOR'); VP=cl.getcolor('SZCOLOR')+cl.getcolor('TBBCOLOR');
    SC=BLUE+cl.getcolor('TBBCOLOR'); RC=RED+cl.getcolor('TBBCOLOR'); DC=BLACK+cl.getcolor('TBBCOLOR');
    MC=GREEN+cl.getcolor('TBBCOLOR');

    CSIGNAVS=getsign(conf.getvalue('INDEX_AVS')).replace('\n', ''); CSIGNVOS=getsign(conf.getvalue('INDEX_VOS')).replace('\n', '');
    CSIGNSVC=getsign(conf.getvalue('INDEX_SVC')).replace('\n', ''); CSIGNVRS=getsign(conf.getvalue('INDEX_VRS')).replace('\n', '');
    CSIGNSND=getsign(conf.getvalue('INDEX_SND')).replace('\n', ''); CSIGNRCV=getsign(conf.getvalue('INDEX_RCV')).replace('\n', '');
    CSIGNDEL=getsign(conf.getvalue('INDEX_DEL')).replace('\n', ''); CSIGNEML=getsign(conf.getvalue('INDEX_EML')).replace('\n', '');
    CSIGNLST=readfirstline(conf.getvalue('STATSAVE')).replace('\n', '');

    LINE+=NP+'S'+SC; LINE+="{:2s}".format(CSIGNSND);
    LINE+=NP+'R'+RC; LINE+="{:2s}".format(CSIGNRCV);
    LINE+=NP+'D'+DC; LINE+="{:2s}".format(CSIGNDEL);
    LINE+=NP+'M'+MC; LINE+="{:2s}".format(CSIGNEML);

    LINE+=NP+'░AVS'+VP; LINE+="{:4s}".format(CSIGNAVS);
    LINE+=NP+' VOS'+VP; LINE+="{:6s}".format(CSIGNVOS);
    LINE+=NP+' SVC'+VP; LINE+="{:6s}".format(CSIGNSVC);
    LINE+=NP+' VRS'+VP; LINE+="{:6s}".format(CSIGNVRS); LINE+=NP;

    TIMEVAL=timer()
    TIMESHW=TIMEVAL+'/'+CSIGNLST
    TIMELAPS=VP+TIMESHW+NP

    shft=76; lns=len(TIMESHW)
    if WORKTYPE=="debug": shft=78;
    else: TIMELAPS+="░"

    lz=WSLZ-(shft+lns);
    lgz=WLLZ-(shft+lns);

    LLINE=LINE

    LINE+="░"+VERSION
    LINE+="░"*lz+TIMELAPS;
    LLINE+="░"+VERSION
    LLINE+="░"*lgz; LLINE+=TIMELAPS;

    if tout=='all' or tout=='scr':
        infoline(LINE.replace('\n', ''), 'scr')
    if tout=='all' or tout=='log':
        infoline(LLINE.replace('\n', ''), 'log')

    write2dbf(conf.getvalue('STATSAVE'), TIMEVAL)

def CLOSE():
    '''
        finaly
    '''
    LOCKED_SEND=conf.getvalue('LOCKED_SEND')
    LOCKED_RECV=conf.getvalue('LOCKED_RECV')
    LOCKED_MAIL=conf.getvalue('LOCKED_MAIL')
    TMP=conf.getvalue('TMP')
    TMPFILE=conf.getvalue('TMPFILE')
    LASTMT=conf.getvalue('LASTMT')

    if os.path.isfile(LOCKED_SEND):
        shellcmd('sudo rm -f "'+LOCKED_SEND+'"', "/dev/null")
    if os.path.isfile(LOCKED_RECV):
        shellcmd('sudo rm -f "'+LOCKED_RECV+'"')
    if os.path.isfile(LOCKED_MAIL):
        shellcmd('sudo rm -f "'+LOCKED_MAIL+'"')
    if os.path.isfile(TMP):
        shellcmd('sudo rm -f "'+TMP+'"')
    if os.path.isfile(TMP):
        shellcmd('sudo rm -f "'+TMP+'"')
    if os.path.isfile(TMPFILE):
        shellcmd('sudo rm -f "'+TMPFILE+'"')
