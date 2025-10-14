/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <qt_windows.h>
#include <QtGui>

// ======================================================================
class WinAPI : public QWidget {
protected:
    virtual bool winEvent(MSG* pmsg, long* result)
    {
        QString str1 = "Windows Version = " 
                       + QString::number(QSysInfo::WindowsVersion);
        QString str2 = "Windows Message";

        switch(pmsg->message) {
        case WM_RBUTTONDOWN:
            ::MessageBox(winId(), 
                         (const WCHAR*)str1.utf16(), 
                         (const WCHAR*)str2.utf16(),
                         MB_OK | MB_ICONEXCLAMATION
                        );
            break;
        default:
            ;
        }
        return QWidget::winEvent(pmsg, result);
    }

    virtual void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);

        HDC        hdc    = painter.paintEngine()->getDC();
        HBRUSH     hBrush = ::CreateSolidBrush(RGB(255, 0, 255));
        HPEN       hPen   = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 128));
        QString    str    = "GDI Drawing";
        TEXTMETRIC tm;     

        ::GetTextMetrics(hdc, &tm);
        ::SelectObject(hdc, hBrush);
        ::SelectObject(hdc, hPen);
        ::Ellipse(hdc, 0, 0, width(), height());
        ::TextOut(hdc, 
                  width() / 2 - (tm.tmAveCharWidth * str.length()) / 2, 
                  (height() - tm.tmHeight) / 2, 
                  (const WCHAR*)str.utf16(), 
                  str.length()
                 );
        ::DeleteObject(hBrush);
        ::DeleteObject(hPen);
        painter.paintEngine()->releaseDC(hdc);
    }

public:
    WinAPI(QWidget* pwgt = 0) : QWidget(pwgt) 
    {
    }
};

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    WinAPI       winAPI;

    winAPI.resize(280, 200);
    winAPI.show();

    return app.exec();
}
