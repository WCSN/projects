=begin
** Form generated from reading ui file 'editor.ui'
**
** Created: ?? ??? 31 15:40:37 2015
**      by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
=end

class Ui_Notepad
    attr_reader :centralwidget
    attr_reader :b_open
    attr_reader :b_close
    attr_reader :editor_window
    attr_reader :b_save

    def setupUi(notepad)
    if notepad.objectName.nil?
        notepad.objectName = "notepad"
    end
    notepad.resize(800, 600)
    @centralwidget = Qt::Widget.new(notepad)
    @centralwidget.objectName = "centralwidget"
    @b_open = Qt::PushButton.new(@centralwidget)
    @b_open.objectName = "b_open"
    @b_open.geometry = Qt::Rect.new(0, 0, 81, 25)
    @b_close = Qt::PushButton.new(@centralwidget)
    @b_close.objectName = "b_close"
    @b_close.geometry = Qt::Rect.new(720, 0, 81, 25)
    @editor_window = Qt::TextEdit.new(@centralwidget)
    @editor_window.objectName = "editor_window"
    @editor_window.geometry = Qt::Rect.new(0, 25, 801, 576)
    @font = Qt::Font.new
    @font.family = "Liberation Mono"
    @editor_window.font = @font
    @b_save = Qt::PushButton.new(@centralwidget)
    @b_save.objectName = "b_save"
    @b_save.geometry = Qt::Rect.new(80, 0, 81, 25)
    notepad.centralWidget = @centralwidget

    retranslateUi(notepad)
    Qt::Object.connect(@b_close, SIGNAL('clicked()'), notepad, SLOT('close()'))

    Qt::MetaObject.connectSlotsByName(notepad)
    end # setupUi

    def setup_ui(notepad)
        setupUi(notepad)
    end

    def retranslateUi(notepad)
    notepad.windowTitle = Qt::Application.translate("notepad", "MainWindow", nil, Qt::Application::UnicodeUTF8)
    @b_open.text = Qt::Application.translate("notepad", "Open File", nil, Qt::Application::UnicodeUTF8)
    @b_close.text = Qt::Application.translate("notepad", "Close", nil, Qt::Application::UnicodeUTF8)
    @b_save.text = Qt::Application.translate("notepad", "Save File", nil, Qt::Application::UnicodeUTF8)
    end # retranslateUi

    def retranslate_ui(notepad)
        retranslateUi(notepad)
    end

end

module Ui
    class Notepad < Ui_Notepad
    end
end  # module Ui

