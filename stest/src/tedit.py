'''
Created on 23 нояб. 2020 г.

@author: wocson
'''
import os, datetime

from PyQt5.QtWidgets import QMainWindow, QLabel, QFrame, QFileDialog
from ui_tedit import Ui_TEditWindow


class VLine( QFrame ):

    # a simple VLine, like the one you get from designer
    def __init__( self ):
        super( VLine, self ).__init__()
        self.setFrameShape( self.VLine | self.Sunken )


class TEdit( QMainWindow, Ui_TEditWindow ):

    filecmd = "testcmd.conf"

    def __init__( self , mainwindow, **kwargs ):
        super().__init__( mainwindow, **kwargs )
        self.setupUi( self )
        self.mainw = mainwindow

        # self.OpenButton.clicked.connect(self.open_file)
        self.SaveButton.clicked.connect( self.save_file )
        self.CloseButton.clicked.connect( self.closeEdit )
        self.FileButton.clicked.connect( self.open_dialog_box )

        self.lbl1 = QLabel( "Cmd: " )
        self.lbl1.setStyleSheet( 'border: 0; color:  blue;' )
        self.lbl2 = QLabel( "Data : " )
        self.lbl2.setStyleSheet( 'border: 0; color:  blue;' )

        self.statusBar.reformat()
        # self.statusBar.setStyleSheet('border: 1; background-color: #FFF8DC;')
        # self.statusBar.setStyleSheet("QStatusBar::item {border: none;}")

        self.statusBar.addPermanentWidget( VLine() )  # <---
        self.statusBar.addPermanentWidget( self.lbl1 )
        self.statusBar.addPermanentWidget( VLine() )  # <---
        self.statusBar.addPermanentWidget( self.lbl2 )
        self.statusBar.addPermanentWidget( VLine() )  # <---

        self.lbl1.setText( "Cmd: 15" )
        self.lbl2.setText( "Date: " + datetime.datetime.today().strftime( "%Y.%m.%d %H:%M" ) )

    def open_dialog_box( self ):
        self.filecmd = QFileDialog.getOpenFileName()[0]
        self.open_file()

    def checkfilecmd( self ):
        try:
            if not os.path.isfile( self.filecmd ):
                self.create_file()
                return True
            else:
                return True
        except Exception:
            return False

    def open_file( self ):
        try:
            with open( self.filecmd ) as f:
                self.textEdit.setPlainText( f.read() )
                self.statusBar.showMessage( 'Файл комманд ' + self.filecmd + ' открыт' )
        except Exception:
            self.statusBar.showMessage( 'Файл комманд не найден' )

    def save_file( self ):
        try:
            with open( self.filecmd, 'w' ) as f:
                f.write( self.textEdit.toPlainText() )
        except Exception:
            self.statusBar.showMessage( 'Файл комманд не найден' )

    def create_file( self ):
        with open( self.file, 'w' ) as f:
            f.write( self.textEdit.toPlainText() )
        self.stausBar.showMessage( 'Файл комманд создан. Введите комманды.' )

    def closeEdit( self ):
        self.mainw.getcmd()
        self.close()
