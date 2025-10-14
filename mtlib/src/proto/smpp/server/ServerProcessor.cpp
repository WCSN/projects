#include "mt/proto/smpp/server/ServerProcessor.h"

namespace mt {
namespace smpp {

//ServerProcessor* ServerProcessor::instance = NULL;

Connection::~Connection() {
	if (reciver)
		delete reciver;
	if (sender)
		delete sender;
}

Connection::Connection(BindType t, Queue<Data*> *q) {
	reciver = NULL;
	sender = NULL;
	type = t;
	in = q;
	out = new Queue<Data*> ();
}

std::string ServerProcessor::GetCurrentClientsInfo() {

	std::string ret;
	AutoMutex a(clientsMtx);
	std::map<std::string, ClientConnection*>::iterator it = clients.begin();

	while (it != clients.end()) {
		ClientConnection* clnt = it->second;
		if (clnt->rv != NULL) {
			ret.append(std::string("system id: ") + it->first + std::string(
					"\ttype: Receiver\n"));
		}
		if (clnt->tx != NULL) {
			ret.append(std::string("system id: ") + it->first + std::string(
					"\ttype: Transceiver\n"));
		}
		if (clnt->tm != NULL) {
			ret.append(std::string("system id: ") + it->first + std::string(
					"\ttype: Transmitter\n"));
		}
		it++;
	}

	return ret;

}

void ServerProcessor::SendData(Data* data) {

	//	const char* dataContext = data->context.c_str();

	Queue<Data*>* q = NULL;
	ClientConnection* clntConn = GetClient(data->systemId);
	if (clntConn == NULL) {
		m_logger->warn("has no client with such system id %s",
				data->systemId.c_str());
		return;
	}

	if (clntConn->rv != NULL) {
		q = clntConn->rv->out;
	} else if (clntConn->tx != NULL) {
		q = clntConn->tx->out;
	} else {
		m_logger->warn("can't send deliver sm. has no appropriate connection");
		return;
	}
	q->push(data);

}

void ServerProcessor::runLoop() {
	int res;
	unsigned long sn;
	std::map<unsigned long, Request*>::iterator it;

	Data* data = inQ->pop();
	if (data == NULL) {
		usleep(1);
		return;
	}

	switch (data->hdr->command_id()) {
	case Smpp::CommandId::DeliverSm:

		m_logger->debug("Recv Deliver sm pdu");
		sn = GetSeqNum();
		data->hdr->sequence_number(sn);
		m_reqMap[sn] = data->req;
		data->req = NULL;
		SendData(data);
		m_logger->trace("Processor send Deliver sm pdu");
		break;
	case Smpp::CommandId::SubmitSm:

		m_logger->trace("Processor recv Submit sm pdu");
		if (!GetMessage(data)) {
			res = 0;
			if (data->udhTotal) {
				res = ProcessMP(data);
			}
			if (!res) {
				m_logger->debug("Processor send Submit sm msg to user");
				userQ->push(data);
			}
		} else {
			MT_LOGTRACE("can't process pdu");
			delete data;
		}
		break;

	default:
		MT_LOGWARNING("can't find such command_id: "<<data->hdr->command_id())
		break;
	}

	return;
}




int ServerProcessor::GetMessage(Data* data) {
	Smpp::SubmitSm* pdu = (Smpp::SubmitSm*) data->hdr;

	Message* msg = new Message();
	Smpp::SmeAddress smeSrcAddr = pdu->source_addr();
	Smpp::SmeAddress smeDstAddr = pdu->destination_addr();
	CodecPtr cdc;
	msg->type = SMS;
	msg->from = smeSrcAddr.address();
	msg->to = smeDstAddr.address();

	Smpp::ShortMessage sm = pdu->short_message();

	msg->registeredDelivery = pdu->registered_delivery();
	msg->codingScheme = smppHelper->getCodingScheme(pdu);
	smppHelper->getMessage(pdu, msg->body);

	if (msg->registeredDelivery) {
		msg->context = data->msgId + " ";
	}

	data->msg = msg;

	return 0;
}

void* ServerProcessor::ConnectionsMonitor(void* p) {
	ServerProcessor* proc = (ServerProcessor*) p;
	LoggerPtr logger = LoggerFactory::instance()->createLogger(
			"Active Connections");
	while (1) {
		sleep(3);
		std::string body = proc->GetCurrentClientsInfo();
		if (body.length() == 0) {
			body = "no clients\n";
		}
		logger->info("\n-------------------\n%s-------------------",
				body.c_str());
	}

	return NULL;

}

}
}

