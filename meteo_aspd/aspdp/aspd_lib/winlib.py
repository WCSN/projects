'''
Created on 10 нояб. 2019 г.

@author: wocson
'''
from ftplib import FTP

from aspd_lib.aspd_iface import cl
from aspd_lib.aspd_sys import outt
from aspd_lib.config import conf
from aspd_lib.sysio import *

class ftpw():
    
    ftpconn=None
    
    def __init__(self, host=None, port=None, user=None, passwd=None):
        if host!=None:
            if port==None: 
                port=conf.getvalue('FTPPORT')
                if port==None: port=21
            if user==None: 
                user=conf.getvalue('FTPUSER')
                if user==None: user='anonymouse'
            if passwd==None: 
                passwd=conf.getvalue('FTPPASSWD')
                if passwd==None: passwd='bill@gates.net'
            self.ftpopen(host, port, user, passwd)
    
    def ftpopen(self, HOST, PORT, USER, PASSWD):
        self.ftpconn=FTP()
        self.ftpconn.connect(HOST, PORT)
        self.ftpconn.login(USER, PASSWD)
    
    def ftpclose(self):
        self.ftpconn.quit()   
    
    def ftpls(self, path):
        data=[]; flist=[]
        self.ftpconn.cwd(path) 
        self.ftpconn.dir(data.append)
        
        for line in data:
            fl=[]
            for ll in line.split(' '):
                if ll!='': 
                    fl.append(ll)
            flist.append(fl[8]+' '+fl[2]+' '+fl[4]+' '+fl[5]+':'+fl[6]+':'+fl[7]);
        return flist

    def download(self, filename):
        try:
            self.ftpconn.retrbinary("RETR "+filename , open(filename, 'wb').write)
        except:
            return "Error"
    
    def upload(self, filename):
        ext=os.path.splitext(filename)[1]
        if ext in (".txt", ".htm", ".html"):
            self.ftpconn.storlines("STOR "+filename, open(filename))
        else:
            self.ftpconn.storbinary("STOR "+filename, open(filename, "rb"), 1024)

class Window():
    
    WTitle=''; WX=WY=WW=WH=0; WTcolor=WBcolor=''
    contentwin=None
    
    def __init__(self, Title='', X=0, Y=0, W=0, H=0, wtcolor=cl.TBCOLOR, wbcolor=cl.BBCOLOR):
        self.WTitle=Title; self.WX=X; self.WY=Y; self.WW=W; self.WH=H
        self.WTcolor=wtcolor; self.WBcolor=wbcolor
        self.contentwin=dict()
        
    def getWinattr(self):
        return (self.WTitle, self.WX, self.WY, self.WW, self.WH, self.WTcolor, self.WBcolor)
    
    def show(self):
        cl.setcolors(self.WTcolor, self.WBcolor)
        if self.WH>=3:
            topl='┌'+'─'*(self.WW-2)+'┐'; 
            self.wdraw(topl, self.WX, self.WY)
            wincr=('│{:{wfild}s}│')
            sh=1;cr=2; sp=' '
        else:
            wincr=('{:{wfild}s}')
            sh=0; cr=0; sp=''

        cl.setcolors(self.WTcolor, self.WBcolor)
        for y in range(self.WY+sh, self.WY+self.WH):
            self.wdraw(wincr.format('', wfild=self.WW-cr), self.WX, y)
            
        if self.WTitle!='':
            if self.WH>=3: cl.setcolors(cl.TBTCOLOR, cl.TBBCOLOR)
            else: cl.setcolors(self.WTcolor, self.WBcolor)
            self.wdraw(sp+self.WTitle+sp, self.WX+sh, self.WY)        

        cl.setcolors(self.WTcolor, self.WBcolor)
        if self.WH>=3:           
            dwnl=topl.replace('┌', '└').replace('┐', '┘')
            self.wdraw(dwnl, self.WX, y+1)
        
        items=len(self.contentwin)
        itemfmt=('{:{wfild}s}')
        for i in range(0, items):
            lnitem=len(self.contentwin[i][0])
            cl.setcolors(self.contentwin[i][3], self.contentwin[i][4])
            self.wdraw(itemfmt.format(self.contentwin[i][0], wfild=lnitem), self.contentwin[i][1], self.contentwin[i][2])
    
    def wdraw(self, item, x, y):
        gotoxy(x, y); 
        print(cl.getcolor('TBCOLOR')+cl.getcolor('BBCOLOR')+item+STDCL, end='')

    def addItem(self, Item=None, fgcolor=0, bgcolor=0):
        if Item!=None:
            if fgcolor==0: fgcolor=self.WTcolor
            if bgcolor==0: bgcolor=self.WBcolor
            y=len(self.contentwin)
            self.contentwin[y]=(Item, self.WX+1, self.WY+y+1, fgcolor, bgcolor)

    def clean(self):
        self.contentwin=None
        self.contentwin=dict()
        self.show()
        
