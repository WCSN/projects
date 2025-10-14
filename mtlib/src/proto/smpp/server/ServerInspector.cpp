/*
 * RvServerInspector.cpp
 *
 *  Created on: 01.12.2009
 *      Author: hakamasy
 */

#include "mt/proto/smpp/server/ServerInspector.h"
#include "mt/proto/smpp/server/ServerProcessor.h"

namespace mt {
namespace smpp {

void ServerInspector::NetworkError(SmppNwIf* smppIf) {

	AutoMutex a(mtx);
	m_logger->trace("ServerInspector::NetworkError() was called by %s",
			smppIf->GetSystemId().c_str());
	ServerProcessor *srvProc = dynamic_cast<ServerProcessor*> (proc);
	ClientConnection* clntConn = srvProc->GetClient(smppIf->GetSystemId());
	Connection* conn = NULL;
	if (clntConn != NULL) {
		switch (smppIf->GetBindType()) {
		case bindReciever:
			conn = clntConn->rv;
			clntConn->rv = NULL;

			break;

		case bindTransmiter:
			conn = clntConn->tm;
			clntConn->tm = NULL;

			break;

		case bindTranciever:
			conn = clntConn->tx;
			clntConn->tx = NULL;
			break;
		}
	} else {
		m_logger->warn("can't find client with system id %s for disconnecting",
				smppIf->GetSystemId().c_str());
		return;
	}


	if ((clntConn->rv == NULL) && (clntConn->tm == NULL) && (clntConn->tx
			== NULL)) {
		m_logger->info("Client with sys id %s disconnected(%u, %u, %u)",
				smppIf->GetSystemId().c_str(), clntConn->rv, clntConn->tm,
				clntConn->tx);
		srvProc->UnRegisterClient(smppIf->GetSystemId());
	} else {
		m_logger->trace(
				"Client with sys id %s already have active connections",
				smppIf->GetSystemId().c_str());
	}

	if (conn != NULL) {
		delete conn;
	}

}

ServerInspector::ServerInspector(XmlConfigPtr cfg) :
	Inspector() {
	MT_LOGINIT(ServerInspector);
	//m_logger = LoggerFactory::getInstance()->getLogger("ServerInspector");
	addr = cfg->get("host");

	std::string portStr = cfg->get("port");
	port = atoi(portStr.c_str());
	serverSock = NULL;
	systype = "VMA";
	type = bindTransmiter;
	//	recvBuf = new char[RECBBUFLEN];

}

int ServerInspector::Init() {

	m_logger->debug("try bind to address %s, port %u", addr.c_str(), port);

	try {
		serverSock = new TCPSocket();
		serverSock->Listen((char*) addr.c_str(), port);
	}
	catch (std::exception e) {
		m_logger->warn("can't listen %s %u", addr.c_str(), port);
		return -1;
	};

	return 0;
}

/*
 void ServerInspector::Run() {

 pthread_t tid = 0;

 if (0 != pthread_create(&tid, NULL, Run, this)) {
 printf("can't start recv thread for RunOut command");
 }
 }
 */

bool ServerInspector::CheckClient(BindParams params) {

	bool ret = true;

	ServerProcessor* servProc = dynamic_cast<ServerProcessor*> (proc);
	ClientConnection* conn = servProc->GetClient(params.sysId);

	if (conn != NULL) {

		switch (params.type) {
		case bindReciever:
			if ((conn->rv != NULL) || (conn->tx != NULL))
				ret = false;
			break;
		case bindTranciever:
			if ((conn->tm != NULL) || (conn->tx != NULL) || (conn->rv != NULL))
				ret = false;
			break;
		case bindTransmiter:
			if ((conn->tx != NULL) || (conn->tm != NULL))
				ret = false;
			break;
		}
	}
	return ret;
}

void ServerInspector::runLoop() {

	TCPSocket * sock = NULL;
	ServerProcessor* servProc = dynamic_cast<ServerProcessor*> (proc);

	try {
		if (!IsActive()) {

			sock = serverSock->Accept();
			MT_LOGTRACE("try accept new connection");

			if (sock != NULL) {
				BindParams params = RecvBind(sock);
				SendBindResp(sock, params.type);

				ClientConnection* clntConn = servProc->GetClient(params.sysId);
				if (clntConn == NULL) {
					clntConn = new ClientConnection();
				}

				if (!CheckClient(params)) {
					MT_LOGWARNING("client with system id %s and such type already connected " << params.sysId.c_str());
					delete sock;
					return;
				}

				Connection* conn = new Connection(params.type,
						servProc->GetInQ());
				clntConn->SetConnection(params.type, conn);

				Sender* sndr = new Sender(conn->out, this);
				conn->sender = sndr;
				sndr->SetBindType(params.type);
				sndr->setSystemId(params.sysId);
				sndr->SetSocket(sock);
				sndr->start();

				Reciver *rcvr = new Reciver(conn->in, this);
				conn->reciver = rcvr;
				rcvr->SetBindType(params.type);
				rcvr->SetSocket(sock);
				rcvr->setSystemId(params.sysId);
				rcvr->start();

				servProc->RegisterClient(params.sysId, clntConn);
				m_logger->trace(
						"initialization for client %s passed successfully",
						params.sysId.c_str());
			} else {
				m_logger->warn("can't accept new connection");
			}

		}
	}
	catch (std::exception e) {
		m_logger->warn("can't init connection from addr %s, port %d",
				addr.c_str(), port);
		if (sock != NULL) {
			m_logger->trace("delete sock");
			delete sock;
		}
		sleep(1);
	};
}

}
}
