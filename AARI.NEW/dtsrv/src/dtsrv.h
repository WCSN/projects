#ifndef DTSRV_H
#define DTSRV_H
#include <QObject>
#include <QList>

class QTcpServer;
class QTcpSocket;
class ISocketAdapter;

class DTSrv : public QObject 
{
  Q_OBJECT

public:
  explicit DTSrv(int nPort, QObject *parent = nullptr);

public slots:
  void on_newConnection();
  void on_disconnected();
  void on_message(QString);

protected:
  QTcpServer* m_ptcpDTSrv;
  QList<ISocketAdapter*> m_clients;
};

#endif // SERVER_H
