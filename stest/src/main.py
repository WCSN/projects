#!/bin/python
'''
Created on 12 РЅРѕСЏР±. 2020 Рі.

@author: vnoskov
'''
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QCheckBox
from PyQt5.QtGui import QColor
from PyQt5.QtCore import Qt, QRect, QProcess

from multiprocessing import Process

import sys, os
from ui_stest import Ui_MainWindow
from tedit import TEdit

import datetime

class STest( QMainWindow, Ui_MainWindow ):

    prbarvol = 0; step_prbarvol = 0; nTs = 0
    NameTest = []; Cmd = []; Options = []; ChBox = []
    SelectAll = False
    logfile = 'stest.log'
    repodir = '.'

    def __init__( self ):
        super().__init__()
        self.setupUi( self )
        self.addstrtolog( "Loaded system test ..." )
        self.StartButton.clicked.connect( self.Start )
        self.StartButton.setDisabled( True )
        self.CloseButton.clicked.connect( self.CloseApp )
        self.SelectAllButton.clicked.connect( self.selectChbxAll )

        self.tedit = TEdit( self )
        self.TestCmdButton.clicked.connect( self.teditCmd )

        if self.tedit.checkfilecmd():
            self.getcmd()

    def teditCmd( self ):
        self.tedit.open_file()
        self.tedit.show()

    def selectChbxAll( self ):
        for nT in range( self.nTs ):
            if self.SelectAll:
                self.ChBox[nT].setChecked( False )
                self.SelectAllButton.setText( "Select All" )
                self.lcdTestCount.display( 0 )
            else:
                self.ChBox[nT].setChecked( True )
                self.SelectAllButton.setText( "UnSelect All" )

        if self.SelectAll: self.SelectAll = False
        else: self.SelectAll = True

    def setstat_chBox( self ):
        ntest = 0
        if self.nTs > 0:
            self.test_enable = True

            for nT in range( self.nTs ):
                if self.ChBox[nT].isChecked():
                    ntest = ntest + 1
            if ntest > 0:
                self.lcdTestCount.display( ntest )
                self.StartButton.setDisabled( False )
            else:
                self.StartButton.setDisabled( True )
        else:
            self.test_enable = False

    def addstrtolog( self, strlog = "", lcolor = "black" ):
        logtext = datetime.datetime.today().strftime( "%Y%m%d-%H:%M:%S.%f" ) + " " + strlog
        self.LogWidget.addItem( logtext )

        with open( self.logfile, 'a' ) as file:
            file.write( logtext + '\n' )
        file.close()

        items = self.LogWidget.findItems( logtext, Qt.MatchExactly )
        if len( items ) > 0:
            for item in items:
                item.setForeground ( QColor( lcolor ) )

        self.LogWidget.scrollToBottom()
        self.LogWidget.repaint()

    def exec_test( self, type_test, cmd, options ):
        # run test
        self.addstrtolog( "Start: " + type_test, "blue" )
        self.addstrtolog( "CMD:" + cmd + ' ' + options )
        self.prbarvol = self.prbarvol + self.step_prbarvol
        self.addstrtolog( "Wait ended test(s)..." )
        self.repaint()
        self.process_exec( cmd, options )
        self.progressBar.setProperty( "value", self.prbarvol )
        self.addstrtolog( 'Finished: ' + type_test, "blue" )

    def process_exec( self, cmd = 'ls', options = "" ):
        args = []; output = ''

        for opt in options.split( ' ' ):
            args.append( opt )

        process = QProcess()
        # process.setWorkingDirectory( self.repodir )
        process.start( cmd, args )

        if process.waitForStarted( 5000 ):
            if process.waitForFinished( 30000 ) and process.exitCode() == 0:
                output = str( process.readAllStandardOutput(), "utf-8" )
                for line in output.splitlines( True ):
                    self.addstrtolog( line.rstrip() )
        else:
            self.addstrtolog( "Process not start...", 'red' )

    def Run_Test( self ):
        ntest = 0

        for nT in range( self.nTs ):
            if self.ChBox[nT].isChecked():
                self.addstrtolog( "Test: " + self.NameTest[nT] + " enabled", "blue" )
                ntest = ntest + 1
            else:
                if self.chBoxLogverb.isChecked():
                    self.addstrtolog( "Test: " + self.NameTest[nT] + " disabled", "gray" )
        self.repaint()

        if ntest > 0:
            self.step_prbarvol = 100 / ntest
            ntest = 0

            for nT in range( self.nTs ):
                if self.ChBox[nT].isChecked():
                    ntest = ntest + 1
                    self.exec_test( self.NameTest[nT], self.Cmd[nT], self.Options[nT] )
                    self.lcdTestCount.display( self.nTs - ntest )

            self.addstrtolog( "Finished all test(s)...", "blue" )

    def chbxOff( self ):
        for nT in range( self.nTs ):
            if self.ChBox[nT].isChecked():
                self.ChBox[nT].setChecked( False )
        self.test_enable = False

        self.prbarvol = 0
        self.lcdTestCount.display( 0 )
        self.repaint()

    def SetStateElem( self, state ):
        self.grBoxTest.setDisabled( state )
        self.SelectAllButton.setDisabled( state )
        if state: self.StartButton.setDisabled( True )
        else:
            self.StartButton.setDisabled( state )
            self.SelectAllButton.setText( "Select All" )
        self.TestCmdButton.setDisabled( state )
        self.CloseButton.setDisabled( state )
        self.chBoxLogverb.setDisabled( state )

    def Start( self ):
        if self.test_enable:
            self.progressBar.setProperty( "value", 0 )
            self.addstrtolog( "Start all enabled tests" )
            self.SetStateElem( True )

            self.Run_Test()

            self.SetStateElem( False )
            self.chbxOff()
        else:
            self.addstrtolog( "Test(s) not enabled..." )

    def getcmd( self ):
        self.NameTest.clear()
        self.Cmd.clear()
        self.Options.clear()

        for chbx in self.grBoxTest.findChildren( QtWidgets.QCheckBox ):
            chbx.deleteLater()

        self.ChBox.clear()

        if os.path.isfile( self.tedit.filecmd ):
            line = ""; ich = 20; self.nTs = 0;
            for line in open( self.tedit.filecmd ):
                line = line.strip()
                lnz = len( line )
                if lnz == 0: continue
                if not line[0] in ( '#', '\n' ):
                    tmp = line.split( ';' )

                    self.ChBox.append( QCheckBox( self.grBoxTest ) )
                    self.ChBox[self.nTs].setGeometry( QRect( 2, ich, 80, 18 ) )
                    self.ChBox[self.nTs].setText( tmp[0].strip() )
                    self.ChBox[self.nTs].stateChanged.connect( self.setstat_chBox )
                    self.ChBox[self.nTs].setObjectName( tmp[0].strip() )
                    self.ChBox[self.nTs].show()

                    self.NameTest.append( tmp[0].strip() )
                    self.Cmd.append( tmp[1].strip() )
                    self.Options.append( tmp[2].strip() )

                    self.nTs = self.nTs + 1
                    ich = ich + 20
        else:

            self.addstrtolog( "File list command " + self.tedit.filecmd + " not find!", "red" )

        self.repaint()

    def CloseApp( self ):
        self.addstrtolog( "Close and stop all test" )
        self.close()


def main():

    app = QApplication( sys.argv )
    win = STest()
    win.show()
    sys.exit( app.exec() )


if __name__ == '__main__':
    main()
