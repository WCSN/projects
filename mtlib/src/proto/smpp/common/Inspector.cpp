/*
 * Inspector.cpp
 *
 *  Created on: 01.12.2009
 *      Author: hakamasy
 */
#include <netinet/in.h>

#include "mt/proto/smpp/common/Inspector.h"

namespace mt{

namespace smpp{

Inspector::~Inspector() {

	delete recvBuf;
}

Inspector::Inspector(Processor* p) :
	SmppNwIf() {
	ton = 0;
	npi = 0;
	port = 9932;
	m_systemId = "mobitek8";
	password = "jmagazin";
	systype = "VMA";
	recvBuf = new char[RECBBUFLEN];
	proc = p;
	sndr = NULL;
	rcvr = NULL;
}

void Inspector::SendUnbind(TCPSocket* sock) throw (std::exception) {

	Smpp::Unbind *un = new Smpp::Unbind();
	un->sequence_number(proc->GetSeqNum());

	int bytesSent = SendPdu(sock, (Smpp::Header*) un);
	if (bytesSent <= 0) {
		throw(TcpSocketException("network error"));
	}
}

int Inspector::RecvUnbindResp(TCPSocket* sock) throw (std::exception) {

	int status = -1;
	Smpp::Header* hdr = RecvPdu(sock);
	if (hdr != NULL)
		status = hdr->command_status();
	return status;
}

void Inspector::SendBind(TCPSocket* sock) throw (std::exception) {

	int bytesSent = 0;
	Smpp::Header* hdr = NULL;
	Smpp::BindReceiver rvpdu;
	Smpp::BindTransceiver tvpdu;
	Smpp::BindTransmitter pdu;

	switch (type) {
	case bindReciever:
		rvpdu.system_id(m_systemId.c_str());
		rvpdu.password(password.c_str());
		rvpdu.system_type(systype.c_str());
		rvpdu.interface_version(0x34);
		rvpdu.addr_npi(npi);
		rvpdu.addr_ton(ton);
		hdr = &rvpdu;
		break;

	case bindTranciever:
		tvpdu.system_id(m_systemId.c_str());
		tvpdu.password(password.c_str());
		tvpdu.system_type(systype.c_str());
		tvpdu.interface_version(0x34);
		tvpdu.addr_npi(npi);
		tvpdu.addr_ton(ton);
		hdr = &tvpdu;
		break;

	case bindTransmiter:
		pdu.system_id(m_systemId.c_str());
		pdu.password(password.c_str());
		pdu.system_type(systype.c_str());
		pdu.interface_version(0x34);
		pdu.addr_npi(npi);
		pdu.addr_ton(ton);
		hdr = &pdu;
		break;
	}

	if (hdr != NULL) {
		bytesSent = SendPdu(sock, hdr);
		if (bytesSent <= 0) {
			throw(TcpSocketException("network error"));
		}
	}

	m_logger->debug("Sending a bind");

}

void Inspector::SendBindResp(TCPSocket* sock, BindType type)
		throw (std::exception) {

	int bytesSent = 0;
	Smpp::Header* hdr = NULL;
	Smpp::BindReceiverResp* rvr = NULL;
	Smpp::BindTransceiverResp* tvr = NULL;
	Smpp::BindTransmitterResp* tmr = NULL;

	switch (type) {
	case bindReciever:
		rvr = new Smpp::BindReceiverResp();
		rvr->system_id("m.smpp_router");
		hdr = rvr;
		break;

	case bindTranciever:
		tvr = new Smpp::BindTransceiverResp();
		tvr->system_id("m.smpp_router");
		hdr = tvr;

		break;

	case bindTransmiter:
		tmr = new Smpp::BindTransmitterResp();
		tmr->system_id("m.smpp_router");
		hdr = tmr;

		break;
	}

	if (hdr != NULL) {
		bytesSent = SendPdu(sock, hdr);
		if (bytesSent <= 0) {
			throw(TcpSocketException("network error"));
		}
		delete hdr;
	}

	m_logger->debug("send bind ack");

}

BindParams Inspector::RecvBind(TCPSocket* sock) throw (std::exception) {
	std::string sType;
	BindType tp;
	Smpp::BindReceiver* brv;
	Smpp::BindTransceiver* btv;
	Smpp::BindTransmitter* btm;
	Smpp::Header* hdr = RecvPdu(sock);
	if (hdr == NULL) {
		m_logger->error("RecvPdu return error");
		throw NetworkException("recv return error");
	}

	switch (hdr->command_id()) {
	case Smpp::CommandId::BindReceiver:
		brv = (Smpp::BindReceiver*) hdr;
		//		system_id();
		m_systemId = brv->system_id();
		password = brv->password();
		type = bindTransmiter;
		addr = inet_ntoa(sock->remote.sin_addr);
		port = ntohs(sock->remote.sin_port);
		sType = "BindReceiver";
		tp = bindReciever;
		break;

	case Smpp::CommandId::BindTransceiver:
		btv = (Smpp::BindTransceiver*) hdr;
		m_systemId = btv->system_id();
		password = btv->password();
		type = bindTransmiter;
		addr = inet_ntoa(sock->remote.sin_addr);
		port = ntohs(sock->remote.sin_port);
		sType = "BindTransceiver";
		tp = bindTranciever;
		break;

	case Smpp::CommandId::BindTransmitter:
		btm = (Smpp::BindTransmitter*) hdr;
		m_systemId = btm->system_id();
		password = btm->password();
		type = bindTransmiter;
		addr = inet_ntoa(sock->remote.sin_addr);
		port = ntohs(sock->remote.sin_port);
		sType = "BindTransmitter";
		tp = bindTransmiter;
		break;
	default:
		m_logger->error("unknown data type recv");
		throw NetworkException("unknown data");
		break;

	}

	CDRlogger->info(
			"INBOUND CONNECTION from (%s:%u), login %s, pass %s, type %s",
			addr.c_str(), port, m_systemId.c_str(), password.c_str(), sType.c_str());

	BindParams params;
	params.sysId = m_systemId;
	params.type = tp;
	return params;

}

int Inspector::RecvBindResp(TCPSocket* sock) throw (std::exception) {
	std::string sType;
	Smpp::Header* hdr = RecvPdu(sock);
	if (hdr == NULL) {
		m_logger->error("RecvPdu return error");
		throw NetworkException("recv return error");
	}

	m_logger->trace("recv bind ack");

	switch (type) {
	case bindReciever:
		sType = "BindReceiver";
		break;
	case bindTranciever:
		sType = "BindTransceiver";
		break;
	case bindTransmiter:
		sType = "BindTransmitter";
		break;
	}

	switch (hdr->command_status()) {
	case Smpp::CommandStatus::ESME_ROK:
		CDRlogger->info(
				"OUTBOUND CONNECTION to (%s:%u) login %s, pass %s, type %s",
				addr.c_str(), port, m_systemId.c_str(), password.c_str(),
				sType.c_str());
		break;

	case Smpp::CommandStatus::ESME_RALYBND:
		m_logger->warn(
				"already binded to to (%s:%u) login %s, pass %s, type %s. unbind",
				addr.c_str(), port, m_systemId.c_str(), password.c_str(),
				sType.c_str());
		break;

	default:
		m_logger->warn("unknown status %u", hdr->command_status());
		break;
	}

	return hdr->command_status();

}

}
}

