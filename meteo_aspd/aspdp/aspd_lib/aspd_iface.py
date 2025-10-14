'''
Created on 20 сент. 2019 г.

@author: wocson
'''

from aspd_lib.sysio import *

class IFace():

# Main constants colors
    MBGRCOLOR=BGBLUE        # Main background color
    MFGRCOLOR=GRAY          # Main foreground (self,text)color
    MFLBCOLOR=GRAY          # Main files bb color
    MPTHCOLOR=LCYAN         # Main paths color
    MSZVCOLOR=RED           # Main Size Files Billborard
    MDTFCOLOR=WHITE         # Main Date Color Billborard
    MTBTCOLOR=BLACK         # Main Title BillBoard Text
    MTBBCOLOR=BGGRAY        # Main Title BillBoard Background
    MTBNCOLOR=LYELLOW       # Main Title Column BillBoardList Name
    MTBDCOLOR=WHITE         # Main BillBoardList DIR
    MTETCOLOR=LRED          # Error Text
    MTEBCOLOR=BGBLUE        # Error Background

    MBGSCOLOR=BGBLUE        # Background color Select
    MFLSCOLOR=BIWHITE       # Foreground color Select
    MSZSCOLOR=BRED          # Size color Select

    TBCOLOR=BBCOLOR=FBCOLOR=PBCOLOR=SZCOLOR=DTCOLOR=TBTCOLOR=TBBCOLOR=''
    BNCOLOR=BDCOLOR=ETCOLOR=EBCOLOR=BGSCOLOR=FLSCOLOR=SZSCOLOR=''

    # Work common colors for screen outs
    color=dict()

    def __init__(self):
        self.color["TBCOLOR"]=self.MFGRCOLOR    # Text/Line  Billborard
        self.color["BBCOLOR"]=self.MBGRCOLOR    # BackGround Billborard

        self.color["FBCOLOR"]=self.MFLBCOLOR    # Files Billborard
        self.color["PBCOLOR"]=self.MPTHCOLOR    # Path Billborard
        self.color["SZCOLOR"]=self.MSZVCOLOR    # Size Files Billborard
        self.color["DTCOLOR"]=self.MDTFCOLOR    # Date Color Billborard

        self.color["TBTCOLOR"]=self.MTBTCOLOR   # Title BillBoard Text
        self.color["TBBCOLOR"]=self.MTBBCOLOR   # Title BillBoard Backgr

        self.color["BNCOLOR"]=self.MTBNCOLOR    # Title BillBoardList Name
        self.color["BDCOLOR"]=self.MTBDCOLOR    # BillBoardList DIR
        self.color["ETCOLOR"]=self.MTETCOLOR    # Error Text
        self.color["EBCOLOR"]=self.MTEBCOLOR    # Error Background

        self.color["BGSCOLOR"]=self.MBGSCOLOR   # BG BB Select
        self.color["FLSCOLOR"]=self.MFLSCOLOR   # File BB Select
        self.color["SZSCOLOR"]=self.MSZSCOLOR   # Size BB Select

        self.TBCOLOR=self.MFGRCOLOR
        self.BBCOLOR=self.MBGRCOLOR

        self.FBCOLOR=self.MFLBCOLOR
        self.PBCOLOR=self.MPTHCOLOR
        self.SZCOLOR=self.MSZVCOLOR
        self.DTCOLOR=self.MDTFCOLOR

        self.TBTCOLOR=self.MTBTCOLOR
        self.TBBCOLOR=self.MTBBCOLOR

        self.BNCOLOR=self.MTBNCOLOR
        self.BDCOLOR=self.MTBDCOLOR
        self.ETCOLOR=self.MTETCOLOR
        self.EBCOLOR=self.MTEBCOLOR

        self.BGSCOLOR=self.MBGSCOLOR
        self.FLSCOLOR=self.MFLSCOLOR
        self.SZSCOLOR=self.MSZSCOLOR

        self.setdefcolors()

    def getcolor(self, color):
        return self.color.get(color)

    def getbgcolor(self):
        return self.color['BBCOLOR']

    def setcolors(self, TXTCL=MFGRCOLOR, BGCL=MBGRCOLOR, FGCL=MFLBCOLOR, PTCL=MPTHCOLOR):
        ''' void setcolors FG BG FILE PATH'''
        self.color['TBCOLOR']=TXTCL
        self.color['BBCOLOR']=BGCL
        self.color['FBCOLOR']=FGCL
        self.color['PBCOLOR']=PTCL

    def setbackcolor(self, BBC=MBGRCOLOR):
        ''' void setbackcolor BBCOLOR '''
        self.color['BBCOLOR']=BBC

    def setbgcolor(self, BBC=MBGRCOLOR):
        ''' void setbackcolor BBCOLOR '''
        self.color['BBCOLOR']=BBC

    def settextcolor(self, TXC=MFGRCOLOR):
        ''' void settextcolor TBCOLOR '''
        self.color['TBCOLOR']=TXC

    def setcolor(self, TXC=MFGRCOLOR):
        ''' void settextcolor TBCOLOR '''
        self.color['TBCOLOR']=TXC

    def settitlebackcolor(self, BBC=MTBBCOLOR):
        self.color['TBBCOLOR']=BBC

    def settitletextcolor(self, TXTC=MTBTCOLOR):
        ''' void settextcolor TBCOLOR'''
        self.color['TBTCOLOR']=TXTC

    def setfilecolor(self, FBC=MFLBCOLOR):
        '''void setpathcolor FBCOLOR'''
        self.color['FBCOLOR']=FBC

    def setpathcolor(self, PBC=MPTHCOLOR):
        ''' void setfilecolor PBCOLOR '''
        self.color['PBCOLOR']=PBC

    def setsizecolor(self, SZC=MSZVCOLOR):
        ''' void setpathcolor SZCOLOR '''
        self.color['SZCOLOR']=SZC

    def setdatecolor(self, DTC=MDTFCOLOR):
        ''' void setpathcolor DTCOLOR '''
        self.color['DTCOLOR']=DTC

    def setdefcolors(self):
        ''' void setdefcolors void '''
        self.color['BBCOLOR']=self.MBGRCOLOR    # BackGround Billborard
        self.color['TBCOLOR']=self.MFGRCOLOR    # Text/Line  Billborard
        self.color['FBCOLOR']=self.MFLBCOLOR    # Files Billborard
        self.color['PBCOLOR']=self.MPTHCOLOR    # Path Billborard
        self.color['SZCOLOR']=self.MSZVCOLOR    # Size Files Billborard
        self.color['DTCOLOR']=self.MDTFCOLOR    # Date Color Billborard
        self.color['TBTCOLOR']=self.MTBTCOLOR   # Title BillBoard text
        self.color['TBBCOLOR']=self.MTBBCOLOR   # Title BillBoard backgr
        self.color['BNCOLOR']=self.MTBNCOLOR    # Title BillBoardList Name
        self.color['BDCOLOR']=self.MTBDCOLOR    # BillBoardList DIR
        self.color['ETCOLOR']=self.MTETCOLOR    # Error Text
        self.color['EBCOLOR']=self.MTEBCOLOR    # Error Background
        self.color['BGSCOLOR']=self.MBGSCOLOR   # BackGround Select
        self.color['FLSCOLOR']=self.MFLSCOLOR   # File BB Select
        self.color['SZSCOLOR']=self.MSZSCOLOR   # Size BB Select

cl=IFace()
