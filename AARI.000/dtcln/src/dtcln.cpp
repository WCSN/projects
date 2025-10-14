#include "dtcln.h"
#include "ui_dtcln.h"
#include "clientsocketadapter.h"

DTCln::DTCln(QWidget *parent) :
	QWidget(parent), 
	m_pForm(new Ui::Form()), 
	m_pSock(new ClientSocketAdapter(this)) 
{
  m_pForm->setupUi(this);
  connect(m_pSock, SIGNAL(message(QString)), SLOT(on_message(QString)));
  connect(m_pForm->send, SIGNAL(clicked()), SLOT(on_send()));
}

void DTCln::on_message(QString text) 
{
  m_pForm->messages->setHtml(m_pForm->messages->toHtml() + text + "<br>");
}

void DTCln::on_send() 
{
  m_pSock->sendString(m_pForm->message->text());
  m_pForm->message->clear();
}
