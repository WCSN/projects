#!/bin/ruby

require 'Qt4'
require './editor.rb'

# Ghbdtn!!!! Привет!!!!
 
class StartQT4 < Qt::MainWindow
 
  slots 'file_dialog()', 'file_save()'
 
  def initialize parent=nil
    super
    @ui = Ui_Notepad.new
    @ui.setupUi self
    Qt::Object.connect(@ui.b_open, SIGNAL('clicked()'), self, SLOT('file_dialog()'))
    Qt::Object.connect(@ui.b_save, SIGNAL('clicked()'), self, SLOT('file_save()'))
  end
 
  def file_dialog
    f = Qt::FileDialog
    if @filename = f.getOpenFileName
      text = File.new(@filename).read
      @ui.editor_window.setText text
    end
  end
 
  def file_save
    if @filename
      f = File.new  @filename, 'w' 
      f.puts @ui.editor_window.toPlainText
      f.close
    end
  end
end
 
if $0 == __FILE__
    app = Qt::Application.new(ARGV)
    myapp = StartQT4.new
    myapp.show
    app.exec
end

