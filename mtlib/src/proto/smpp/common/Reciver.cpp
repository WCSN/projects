/*
 * Reciver.cpp
 *
 *  Created on: 05.11.2009
 *      Author: hakamasy
 */

#include "mt/proto/smpp/common/Reciver.h"
#include "mt/proto/smpp/common/Inspector.h"

#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/thread/AutoMutex.h"

namespace mt {
namespace smpp {

Reciver::~Reciver() {

	MT_LOGDEBUG(m_systemId << ": reciever destructor!!!")
	stop();
}

Reciver::Reciver(Queue<Data*>* q, Inspector* i) {
	MT_LOGINIT(Reciver)
	sock = NULL;
	m_queue = q;
	inspector = i;
	m_systemId = inspector->getSystemId();
}


void Reciver::stop() {
	MT_LOGDEBUG(m_systemId << ": stop")
	notifyInspector = false;
	Deactivate();
	sock->Close();
	InfiniteThread::stop();

}

void Reciver::SetSocket(TCPSocket* s) {
	sock = s;
	Activate();
}

std::string Reciver::SendSubmitResp(Smpp::SubmitSm* pdu) {

	time_t t;
	t = time(&t);
	char buf[255];
	Smpp::SubmitSmResp resp;
	resp.sequence_number(pdu->sequence_number(), false);
	snprintf(buf, 255, "m.smpp_%u%u", (unsigned int) t, pdu->sequence_number());
	resp.message_id(buf);
	SendPdu((Smpp::Header*) &resp);
	return std::string(buf);

}

int Reciver::SendDeliverResp(Smpp::DeliverSm* pdu) {

	char buf[255];
	Smpp::DeliverSmResp resp;
	resp.sequence_number(pdu->sequence_number(), false);
	snprintf(buf, 255, "m.smpp_%u", pdu->sequence_number());
	resp.message_id(buf);
	return SendPdu((Smpp::Header*) &resp);
}

int Reciver::SendEnqLinkResp(Smpp::EnquireLink* pdu) {

	Smpp::EnquireLinkResp resp;
	resp.sequence_number(pdu->sequence_number(), false);
	return SendPdu(sock, (Smpp::Header*) &resp);

}


void Reciver::runLoop() {

	Data *data;
	bool needProcessing = false;
	if (IsActive()) {
		Smpp::Header *hdr = RecvPdu();

		if (hdr != NULL) {
			data = new Data(hdr, false);
			data->systemId = m_systemId;

			switch (hdr->command_id()) {

			case 0x00000004:
				data->msgId = SendSubmitResp(((Smpp::SubmitSm*) hdr));
				needProcessing = true;
				break;
			case 0x80000004:
				needProcessing = true;
				break;

			case 0x00000005:
				needProcessing = true;
				SendDeliverResp(((Smpp::DeliverSm*) hdr));
				break;
			case 0x80000005:
				break;

			case 0x00000015:
				SendEnqLinkResp((Smpp::EnquireLink*) hdr);
				break;
			case 0x80000015:
				break;
			default:
				MT_LOGERROR("unknown cmd (" << hdr->command_id() << ")")
				Deactivate();
			}

			if (needProcessing) {
				MT_LOGTRACE(m_systemId << ": send to processor")
				m_queue->push(data);
			} else {
				delete data;
			}

		} else {
			MT_LOGERROR(m_systemId << ": recvPdu return error")
			Deactivate();

		}
	} else {

		MT_LOGWARNING(m_systemId << ": reciever is inactive. break")
		if( notifyInspector )
			inspector->NetworkError(this);
	}

}

}
}

