#include "mt/proto/smpp/client/ClientInspector.h"
#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/thread/AutoMutex.h"
namespace mt {
namespace smpp {

void ClientInspector::NetworkError(SmppNwIf*) {

	AutoMutex a(terminateMtx);
	MT_LOGERROR("Network Error sndr and reciver terminated")

	if (sndr) {
		delete sndr;
		sndr = NULL;
	}
	if (rcvr) {
		delete rcvr;
		rcvr = NULL;
	}

	Deactivate();
}

ClientInspector::ClientInspector(XmlConfigPtr cfg, Processor* p) :
	Inspector(p) {

	MT_LOGINIT(ClientInspector);

	m_connectionType = cfg->get("connection_type");
	m_systemId = cfg->get("system_id");

	MT_LOG_SETPREFIX(m_systemId + ":" + m_connectionType)

	if (!m_connectionType.compare("TM")) {
		type = bindTransmiter;

	} else if (!m_connectionType.compare("TV")) {
		type = bindTranciever;

	} else if (!m_connectionType.compare("RV")) {
		type = bindReciever;
	} else {
		MT_LOGERROR("unknown client type ("<<m_connectionType <<"). set default type tranciever");
		type = bindTranciever;
	}

	addr = cfg->get("host");
	std::string portStr = cfg->get("port");
	port = atoi(portStr.c_str());
	password = cfg->get("password");
	systype = cfg->get("system_type");

	XmlConfigPtr loadCfg = cfg->getConfig("load");
	if(loadCfg){
		std::string maxLoad = loadCfg->get("max_rps");
		if(maxLoad.length()){
			m_maxRps = atoi(maxLoad.c_str());
		}
	}

	if(!m_maxRps){
		m_maxRps = 100;
	}
	recvBuf = new char[RECBBUFLEN];
}

void ClientInspector::runLoop() {

	TCPSocket *sock = NULL;

	bool lastBindResp = false;
	try {

		if (!IsActive()) {
			sock = new TCPSocket();
			sock->Connect((char*) addr.c_str(), port);

			if (lastBindResp) {
				SendUnbind(sock);
				RecvUnbindResp(sock);
				lastBindResp = false;
				throw std::exception();
			}

			SendBind(sock);
			int status = RecvBindResp(sock);
			if (status != 0) {
				if (status == 5) {
					lastBindResp = true;
				}
				throw std::exception();
			}

			MT_LOGDEBUG("create sender for client(system_id: "<<m_systemId<<")");
			sndr = new Sender(proc->GetOutQ(), this);
			sndr = sndr;
			sndr->SendEnqLink(true);
			sndr->SetSocket(sock);
			sndr->setMaxRps(m_maxRps);
			sndr->setSystemId(m_systemId);
			sndr->setConnectionType(m_connectionType);
			sndr->start();

			MT_LOGDEBUG("create reciever for client(system_id: "<<m_systemId<<")");
			rcvr = new Reciver(proc->GetInQ(), this);
			rcvr = rcvr;
			rcvr->setSystemId(m_systemId);
			rcvr->SetSocket(sock);
			rcvr->start();

			Activate();
			MT_LOGDEBUG("initialization passed successfully(system_id: "<<m_systemId<<")");

		} else {
			Data *data = new Data();
			data->msg = new Message();
			data->msg->type = ENQ_LINK;
			Smpp::EnquireLink *enq = new Smpp::EnquireLink();
			enq->sequence_number(proc->GetSeqNum());
			data->hdr = enq;
			proc->GetOutQ()->push(data);

		}
	}
	catch (const std::exception &e) {
		MT_LOGWARNING("can't init connection to to addr: "<<addr<<", port: "<< port);
		if (sock != NULL) {
			delete sock;
			sock = NULL;
		}
		if (sndr != NULL) {
			delete sndr;
			sndr = NULL;

		}
		if (rcvr != NULL) {
			delete rcvr;
			rcvr = NULL;
		}

		//lastBindResp = true;
	};
	sleep(1);

}

}
}
