'''
Created on 14 сент. 2019 г.

@author: wocson
'''

from datetime import timedelta, datetime
import os
import subprocess

from blessings import Terminal

class AspdConfig():

    permdr="u-r-w-x,g-r-w-x,o-r-w-x"
    perm="u+r+w+X,g+r+w+X,o-r-w-x"
    permfile="u+r+w-x,g+r+w-x,o-r-w-x"

    permAllrw="u+r+w-x,g+r+w-x,o+r+w-x"
    permAllrwx="u+r+w+x,g+r+w+x,o+r+w+x"
    permAllrwX="u+r+w+X,g+r+w+X,o+r+w+X"
    
    permGOr="u+r+w-x,g+r-w-x,o+r-w-x"
    permGOrx="u+r+w+x,g+r-w+x,o+r-w+x"
    permGOrX="u+r+w+X,g+r-w+X,o+r-w+X"

    permOr="u+r+w+x,g+r+w+x,o+r-w-x"
    permOrx="u+r+w+x,g+r+w+x,o+r-w+x"
    permOrX="u+r+w+X,g+r+w+X,o+r-w+X"

    owner="root:aspd"
    
    WLFNPTH=[]; WRFNPTH=[]; WLETTXT=[]; WSUBJCT=[]
    WNMGROUP=[]; WABLNS=[]; WADRESS=[]; WFTPPTH=[]; WCOMNTS=[]; WADRGROUP=dict();
    PTFATCH=[]; NMFATCH=[];
    WMEMAILAD=[]; WMSUBJECT=[]; WMTXTMAIL=[]; WMATTACHM=[]
    aFLS=[]; aLSZ=[]

    SIGNATURE01=SIGNATURE02=TIMESIN=FTPCMD=SCANS=MANAGE=BINDIR=UNKNOWNDIR=TEMPLATES=''
    INFODIR=ASPDWORKDIR=ASPDLOGDIR=TMPASPD=DBDIR=EDITOR=TMPFILE=SIGN=''
    NULL=nenv=''
    RITASERV=True

    nFLSND=nMAILSND=nFTPSND=XLENA=XPATH=COUNTADRESS=COUNTGRADR=0
    nGRP=-1
    Err=False
    WORKTYPE=''
    TC=TL=0
    config=dict()
    ABOOKINIT=False
    AWRLET=False
    
    def __init__(self, worktype='debug'):
        '''
            Constructor
        '''
        self.RITASERV=True
        self.NULL=open('/dev/null', 'a') 
        self.nenv={'TERM': 'xterm', **os.environ}    

        self.config['WORKTYPE']=worktype
        self.WORKTYPE=worktype
            
        self.TC=self.getTermWidth()
        self.TL=self.getTermHeight()
        if self.TC==None: 
            self.TC=100
        if self.TL==None: 
            self.TL=65

        if self.WORKTYPE=='debug': 
            pathconf='./etc/aspd/aspd_debug.conf'
        else: 
            pathconf='/usr/local/etc/aspd/aspd.conf'

        self.config['CONFIG']=pathconf

        if os.path.isfile(pathconf): 
            self.readconf(pathconf)
        else: 
            print("Конфигурационный файл не найден")
        
        if self.getvalue('RITAVOICE')=="OFF":
            self.RITASERV=False
        
        self.config['WSLZ']=self.TC
        self.config['LOCKED_SEND']="/tmp/locked_aspd_send"
        self.config['LOCKED_RECV']="/tmp/locked_aspd_recv"
        self.config['LOCKED_MAIL']="/tmp/locked_aspd_mail"        
        INFODIR=self.config['INFODIR']
        self.INFODIR=INFODIR
        self.config['SIGN_SEND']=INFODIR+'/send'
        self.config['SIGN_DELAY']=INFODIR+'/delay'
        ASPDWORKDIR=self.config['ASPDWORKDIR']
        self.ASPDWORKDIR=ASPDWORKDIR
        self.config['TEMPLATES']=ASPDWORKDIR+"/infobox/Templates"    # Templates
        self.config['LDIRFORSEND']=ASPDWORKDIR+"/outbox"            # каталог отправки первичных файлов
        self.config['LDIRRECV']=ASPDWORKDIR+"/Recv"                # каталог приёма
        self.config['LDIRSEND']=ASPDWORKDIR+"/Send"                # каталог отправки
        self.config['LDIRDELAY']=ASPDWORKDIR+"/Delay"    
        self.config['LDIRFORSEND']=ASPDWORKDIR+'/outbox'
        DBTYPE=""

        if self.WORKTYPE=="debug" : DBTYPE="_debug"
        DBDIR=self.getvalue('DBDIR')

        self.DBDIR=DBDIR
        self.config['INDEX_AVS']=DBDIR+"/db"+DBTYPE+"/index.avs"     # index avos
        self.config['INDEX_CLM']=DBDIR+"/db"+DBTYPE+"/index.clm"     # index clim
        self.config['INDEX_SVC']=DBDIR+"/db"+DBTYPE+"/index.svc"     # index svc
        self.config['INDEX_VOS']=DBDIR+"/db"+DBTYPE+"/index.vos"     # index vos
        self.config['INDEX_VRS']=DBDIR+"/db"+DBTYPE+"/index.vrs"     # index vrs
        self.config['INDEX_SND']=DBDIR+"/db"+DBTYPE+"/index.snd"     # index send
        self.config['INDEX_RCV']=DBDIR+"/db"+DBTYPE+"/index.rcv"     # index recv
        self.config['INDEX_DEL']=DBDIR+"/db"+DBTYPE+"/index.del"     # index del
        self.config['INDEX_EML']=DBDIR+"/db"+DBTYPE+"/index.eml"     # index email  
        
        # LOGinfo files
        ASPDLOGDIR=self.getvalue('ASPDLOGDIR')
        self.ASPDLOGDIR=ASPDLOGDIR
        self.config['LOG']=ASPDLOGDIR+'/aspd.log'
        self.logcheck()
        self.config['FLLSLOG']=ASPDLOGDIR+"/fsls.log"
        self.config['FTPSLOG']=ASPDLOGDIR+"/ftps.log"
        self.config['FTPRLOG']=ASPDLOGDIR+"/ftpr.log"
        self.config['FTPLLOG']=ASPDLOGDIR+"/ftpl.log"
        self.config['MAILLOG']=ASPDLOGDIR+"/mail.log"
        self.config['STATSAVE']=ASPDLOGDIR+"/stat.sav"
                
        # TMP files
        TMPASPD=self.getvalue('TMPASPD')
        self.TMPASPD=TMPASPD
        self.config['TMP']="/tmp/aspd.tmp"
        self.TMPFILE=self.config['TMPFILE']=TMPASPD+"/tmpfl.tmp"
        self.config['LASTMT']=TMPASPD+"/lastmt.tmp"
        
        # Variables
        self.config['NUMSESSOFDAY']=0
        self.config['nFLSND']=0

        # Work directory check and create if not have
        
        self.config['SINTMIN']=self.getvalue('SIN0').split("-")[0].split(":")[1]
        self.config['SINTMAX']=self.getvalue('SIN0').split("-")[1].split(":")[1].split(' ')[0]
        
        self.FTPCMDOPEN="open "+self.getvalue('FTPSERVER')+";user "+self.getvalue('FTPUSER')+" "+self.getvalue('FTPPASSWD')+";"
        self.FTPCMDCLOSE="bye;"
        
        self.config['LDIR_ARCH']=ASPDWORKDIR+"/L_Archive/RAE"+self.getvalue('NUMEXP')+'/'+self.getdatetime('%Y-%m') # local date
        self.config['RDIR_ARCH']=ASPDWORKDIR+"/R_Archive/RAE"+self.getvalue('NUMEXP')+'/'+self.getdatetime('%Y-%m') # local date
        self.config['ARCHDIR']=self.getdatetime('%m%d') # local date

        self.MANAGE=self.INFODIR+'/manage'
        self.SCANS=self.INFODIR+'/scans'
        self.TEMPLATES=self.INFODIR+'/Templates'
        self.EDITOR=self.getvalue('EDITOR')

        self.CrWorkDir()
        
    def getTermHeight(self):
        return Terminal().height

    def getTermWidth(self):
        return Terminal().width
    
    def CrWorkDir(self)->None:
        if not os.path.isdir(self.ASPDWORKDIR):
            self.shellcmd('mkdir -p '+self.ASPDWORKDIR)
            self.permArchDir()
                
            self.shellcmd('mkdir -p '+self.ASPDWORKDIR+'/Send')
            self.shellcmd('mkdir -p '+self.ASPDWORKDIR+'/Recv')
            self.shellcmd('mkdir -p '+self.ASPDWORKDIR+'/Delay')
            self.shellcmd('mkdir -p '+self.ASPDWORKDIR+'/outbox')
            self.permArchDir()
        
        if not os.path.isdir(self.INFODIR):
            self.shellcmd('mkdir -p '+self.INFODIR)
            self.shellcmd('mkdir -p '+self.ASPDLOGDIR)
            self.shellcmd('mkdir -p '+self.getvalue('BINDIR'))
            self.shellcmd('mkdir -p '+self.getvalue('UNKNOWNDIR'))
            self.shellcmd('mkdir -p '+self.MANAGE)
            self.shellcmd('mkdir -p '+self.SCANS)
            self.shellcmd('mkdir -p '+self.TEMPLATES)
            self.shellcmd('mkdir -p '+self.TMPASPD)
            self.permInfoDir()
    
    def permArchDir(self, opt='Rf'):
        self.shellcmd('sudo chmod -f '+self.perm+' '+self.ASPDWORKDIR)
        self.shellcmd('sudo chmod -'+opt+' '+self.perm+' '+self.ASPDWORKDIR)
        self.shellcmd('sudo chown -'+opt+' '+self.owner+' '+self.ASPDWORKDIR)

    def permInfoDir(self, opt='Rf'):
        # aspd files dir
        self.shellcmd('sudo chmod -'+opt+' '+self.perm+' '+self.INFODIR)
        self.shellcmd('sudo chmod -f  '+self.permOrX+' '+self.INFODIR)
        self.shellcmd('sudo chmod -'+opt+' '+self.permAllrw+' '+self.ASPDLOGDIR)
        self.shellcmd('sudo chmod -f  '+self.permAllrwX+' '+self.ASPDLOGDIR)
        self.shellcmd('sudo chmod -'+opt+' '+self.permOrx+' '+self.getvalue('BINDIR'))
        self.shellcmd('sudo chmod -f  '+self.permOrX+' '+self.getvalue('BINDIR'))
        self.shellcmd('sudo chmod -'+opt+' '+self.permAllrw+' '+self.getvalue('UNKNOWNDIR'))
        self.shellcmd('sudo chmod -f  '+self.permAllrwX+' '+self.getvalue('UNKNOWNDIR'))
        self.shellcmd('sudo chmod -'+opt+' '+self.permOrx+' '+self.MANAGE)
        self.shellcmd('sudo chmod -f  '+self.permOrX+' '+self.MANAGE)
        self.shellcmd('sudo chmod -'+opt+' '+self.perm+' '+self.SCANS)
        self.shellcmd('sudo chmod -f  '+self.perm+' '+self.SCANS)
        self.shellcmd('sudo chmod -f  '+self.permGOrX+' '+self.TEMPLATES)
        self.shellcmd('sudo chmod -f  '+self.permGOr+' '+self.TEMPLATES+'/*')
        self.shellcmd('sudo chmod -'+opt+' '+self.permAllrw+' '+self.TMPASPD)
        self.shellcmd('sudo chmod -f  '+self.permAllrwX+' '+self.TMPASPD)
        self.shellcmd('sudo chown -'+opt+' '+self.owner+' '+self.INFODIR)
        # aspd db dir
        self.shellcmd('sudo chmod -'+opt+' '+self.perm+' '+self.getvalue('DBDIR'))
        self.shellcmd('sudo chmod -f  '+self.perm+' '+self.getvalue('DBDIR'))
        self.shellcmd('sudo chown -'+opt+' '+self.owner+' '+self.getvalue('DBDIR'))
        
    def getadrbook(self):
        '''
        getadrbook geterate internal a.book for send void setadrbook void
        '''
        n=0; i=0; adrline=""; tmp=''; lnz=0; 
        if self.ABOOKINIT: return
    
        abookdb=self.getvalue("ADRBOOK")
        adringrp=[]
        
        if os.path.isfile(abookdb): 
            adrline=""
            for adrline in open(abookdb):
                adrline=adrline.strip()
                lnz=len(adrline)
                if lnz==0: continue
                if not adrline[0] in ('#', '\n'):
                    if adrline[0]=='[':
                        self.nGRP+=1
                        self.WNMGROUP.append(adrline.replace('[', '').replace(']', '').replace('\n', ''))
                        lnz=len(self.WNMGROUP[self.nGRP]);
                        if len(adringrp)>0: 
                            self.WADRGROUP[self.WNMGROUP[self.nGRP-1]]=adringrp.copy()
                            adringrp.clear()
                        self.WABLNS.append('@'+self.WNMGROUP[self.nGRP])
                    else:
                        tmp=adrline.split(';')
                        self.WADRESS.append(tmp[0])
                        lnz=len(self.WADRESS[n]);
                        if lnz>self.XLENA: self.XLENA=lnz;
                        adringrp.append(self.WADRESS[n])     # Add adress in group
                        self.WFTPPTH.append(tmp[1])
                        lnz=len(self.WFTPPTH[n]); 
                        if lnz>self.XPATH: self.XPATH=lnz
                        if len(tmp)>2:
                            tmp[2]=tmp[2].strip()
                            if tmp[2].find('#')==-1: self.WCOMNTS.append(tmp[2])
                        else: self.WCOMNTS.append('')
                        self.WABLNS.append(adrline)
                        n+=1
                    i+=1
            self.WADRGROUP[self.WNMGROUP[self.nGRP]]=adringrp.copy()
        else: 
            print("Файл адресной книги не найден")
            
        self.COUNTADRESS=n
        self.COUNTGRADR=i
        self.ABOOKINIT=True

    def crfsetpm(self, file="", prm=perm):
        ''' create file and set permissions '''
        if file=="": return
        if not os.path.isfile(file): self.shellcmd("sudo touch "+file)
        self.shellcmd("sudo chmod "+prm+" "+file)
        self.shellcmd("sudo chown "+self.owner+" "+file)

    def crdsetpm(self, pdir='', prm=''):
        ''' create directories and set permissions '''
        if pdir=='': return
        if prm=='': prm=self.perm
        if not os.path.isdir(pdir): self.shellcmd("sudo mkdir -p "+pdir)
        self.shellcmd("sudo chmod "+prm+" "+pdir)
        self.shellcmd("sudo chown "+self.owner+" "+pdir)
      
    def logcheck(self):    
        aspdlogdir=self.getvalue('ASPDLOGDIR')
        if not os.path.isdir(aspdlogdir): 
            self.crdsetpm(aspdlogdir)
        aspd_log=self.getvalue('LOG')
        if not os.path.isfile(aspd_log): 
            self.crfsetpm(aspd_log, self.permAllrwX)
                
    def getvalue(self, confprm=''):
        return self.config.get(confprm, None)
        
    def setvalue(self, namevalue, value):
        self.config[namevalue]=value

    def readconf(self, conffile):
        line=""; ln=''
        for line in open(conffile):
            if line[0]!='#' and line.find("=")!=-1 :
                line=line.replace('\t', ' ')
                lnp=line.split("=")
                ln=lnp[1].split('#')
                ln=ln[0].replace('"', '')
                ln=ln.rstrip()                
                
                if ln[0]=='$':
                    lnrp=ln.replace('$', '')
                    lnrp=lnrp.split('/')[0]
                    ln2p=self.config.get(lnrp, '0')
                    ln=ln.replace('$'+lnrp, ln2p)
                
                self.config[lnp[0]]=ln

    def timeshift(self, datenow, tmshift='0'):   # tmshift = '-1d', '+5h', '-7d'
        left=1; uptm=1; val=sm=''
        # lst = list(tmshift)
        for sm in tmshift:
            if sm=='-': left=-1
            elif sm=='h': uptm=60
            elif sm=='d': uptm=24*60
            elif sm.isdigit(): val+=sm
        
        val=int(val)*left*uptm 
        return datenow+timedelta(minutes=val)

    def getdatetime(self, fmttime='', tmshift='0'):
        if fmttime=='': fmttime=self.getvalue('DTSTAMP')
        date=datetime.now()
        if tmshift!='0': date=self.timeshift(date, tmshift)
        tm="{:{fmt}}".format(date, fmt=fmttime)
        return tm
        
    def shellcmd(self, cmd, repfile='/dev/null', mode='w', shell='/bin/bash'):
        if mode=='>': mode='w'
        elif mode=='>>': mode='a'
        if not repfile=='':
            file=open(repfile, mode) 
            subprocess.Popen(cmd, shell=True, executable=shell, stdout=file, stderr=file)
            file.close()

    def __del__(self):
        pass

conf=AspdConfig("debug")
#conf=AspdConfig("work")

