#include "dtsrv.h"
#include "serversocketadapter.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

DTSrv::DTSrv(int nPort, QObject *parent) :
	QObject(parent),
	m_ptcpDTSrv(new QTcpServer(this)) 
{
  connect(m_ptcpDTSrv, SIGNAL(newConnection()), SLOT(on_newConnection()));

  if (false == m_ptcpDTSrv->listen(QHostAddress::Any, nPort)) {
    m_ptcpDTSrv->close();
    throw m_ptcpDTSrv->errorString();
  } 
}

void DTSrv::on_newConnection() 
{
  QTextStream(stdout) << "new connection" << endl;

  QTcpSocket* pclientSock = m_ptcpDTSrv->nextPendingConnection();
  ISocketAdapter *pSockHandle = new ServerSocketAdapter(pclientSock);

  m_clients.push_back(pSockHandle);

  pSockHandle->sendString("connect");

  connect(pSockHandle, SIGNAL(disconnected()), SLOT(on_disconnected()));
  connect(pSockHandle, SIGNAL(message(QString)), SLOT(on_message(QString)));
}

void DTSrv::on_disconnected() 
{
  QTextStream(stdout) << "client disconnected" << endl;
  ISocketAdapter* client = static_cast<ServerSocketAdapter*>(sender());
  m_clients.removeOne(client);
  delete client;
}

void DTSrv::on_message(QString msg) 
{
  foreach (ISocketAdapter *sock, m_clients)
    sock->sendString(msg);
}


