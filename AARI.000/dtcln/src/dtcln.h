#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui 
{
  class Form;
}

class ISocketAdapter;

class DTCln : public QWidget 
{
  Q_OBJECT

public:
  explicit DTCln(QWidget *parent = nullptr);

public slots:
  void on_message(QString text);
  void on_send();

protected:
  Ui::Form *m_pForm;
  ISocketAdapter *m_pSock;
};

#endif // MAINWIDGET_H
