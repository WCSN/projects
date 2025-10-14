#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_released();

    // Print the document
    void doPrint();
    void doPrintPreview();
    void printPreview(QPrinter *printer);

private:
    Ui::MainWindow *ui;

    void createPdf();
};
#endif // MAINWINDOW_H
