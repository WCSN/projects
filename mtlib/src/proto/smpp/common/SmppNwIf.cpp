#include "mt/proto/smpp/common/SmppNwIf.h"
#include "mt/proto/smpp/pdu/smpp.hpp"
#include "mt/thread/AutoMutex.h"
#include "mt/proto/smpp/common/PduLogHelper.h"

#include "mt/logger/LoggerFactory.h"

namespace mt {

namespace smpp {

SmppNwIf::~SmppNwIf(){

	delete m_rcvBuf;
}

SmppNwIf::SmppNwIf() {

	MT_LOGINIT(SmppNwIf);
	m_rcvBuf = new RcvBuf();
	hexLogEnable = true;
	smppLogEnable = true;
	CDRlogger = LoggerFactory::instance()->createLogger("cdr_logger");
	active = false;
	deletInactive = false;
	hexLogger = LoggerFactory::instance()->createLogger("BinaryPDULogger");
	smppLogger = LoggerFactory::instance()->createLogger("SmppPDULogger");
	notifyInspector = true;

	m_maxRps = 0;
	m_sendTimeout = 0;

}

bool SmppNwIf::IfInactive() {
	AutoMutex a(deletInactiveMtx);
	return deletInactive;
}

void SmppNwIf::SetIfInactive(bool b) {
	AutoMutex a(deletInactiveMtx);
	deletInactive = b;
}

bool SmppNwIf::IsActive() {
	AutoMutex a(activeMtx);
	return active;
}

void SmppNwIf::Activate() {
	AutoMutex a(activeMtx);
	active = true;
}

void SmppNwIf::Deactivate() {
	AutoMutex a(activeMtx);
	active = false;
}

int SmppNwIf::SendPdu(TCPSocket* s, Smpp::Header* h) {

	if (s == NULL)
		s = sock;
	if (h == NULL)
		return -1;

	int bytesSent = -1;
	uint8_t* buf = NULL;
	unsigned int len;

	Smpp::BindReceiver* brv;
	Smpp::BindReceiverResp* brvr;
	Smpp::BindTransmitter* btm;
	Smpp::BindTransmitterResp* btmr;
	Smpp::SubmitSm* sm;
	Smpp::SubmitSmResp* smr;
	Smpp::DeliverSm* dm;
	Smpp::DeliverSmResp* dmr;
	Smpp::BindTransceiverResp* btr;
	Smpp::BindTransceiver* btv;
	Smpp::EnquireLink *enq;
	Smpp::EnquireLinkResp *enqr;
	Smpp::Unbind *unb;
	switch (h->command_id()) {
	case 0x00000001:

		brv = (Smpp::BindReceiver*) h;
		buf = (uint8_t*) brv->encode();
		len = (unsigned int) brv->command_length();

		break;

	case 0x80000001:

		brvr = (Smpp::BindReceiverResp*) h;
		buf = (uint8_t*) brvr->encode();
		len = (unsigned int) brvr->command_length();
		break;

	case 0x00000002:

		btm = (Smpp::BindTransmitter*) h;
		buf = (uint8_t*) btm->encode();
		len = (unsigned int) btm->command_length();

		break;

	case 0x80000002:

		btmr = (Smpp::BindTransmitterResp*) h;
		buf = (uint8_t*) btmr->encode();
		len = (unsigned int) btmr->command_length();
		break;

	case 0x00000004:

		sm = dynamic_cast<Smpp::SubmitSm*>(h);
		buf = (uint8_t*) sm->encode();
		len = (unsigned int) sm->command_length();
		LogSm(sm, false);

		break;

	case 0x80000004:

		smr = (Smpp::SubmitSmResp*) h;
		buf = (uint8_t*) smr->encode();
		len = (unsigned int) smr->command_length();
		LogSmResp(smr, false);

		break;

	case 0x00000005:

		dm = (Smpp::DeliverSm*) h;
		buf = (uint8_t*) dm->encode();
		len = (unsigned int) dm->command_length();

		break;

	case 0x80000005:

		dmr = (Smpp::DeliverSmResp*) h;
		buf = (uint8_t*) dmr->encode();
		len = (unsigned int) dmr->command_length();
		break;

	case 0x80000009:

		btr = (Smpp::BindTransceiverResp*) h;
		buf = (uint8_t*) btr->encode();
		len = (unsigned int) btr->command_length();

		break;

	case 0x00000009:

		btv = (Smpp::BindTransceiver*) h;
		buf = (uint8_t*) btv->encode();
		len = (unsigned int) btv->command_length();

		break;

	case 0x00000015:
		enq = (Smpp::EnquireLink*) h;
		buf = (uint8_t*) enq->encode();
		len = (unsigned int) enq->command_length();
		break;

	case 0x80000015:
		enqr = (Smpp::EnquireLinkResp*) h;
		buf = (uint8_t*) enqr->encode();
		len = (unsigned int) enqr->command_length();
		break;

	case Smpp::CommandId::Unbind:
		unb = (Smpp::Unbind*) h;
		buf = (uint8_t*)unb->encode();
		len = (unsigned int) unb->command_length();
		break;
	default:
		MT_LOGWARNING("has now wraper for sending comman_id (" << h->command_id() << ")")
		break;
	}

	if (buf != NULL) {
		bytesSent = s->Send((char*)buf, len);
		if (hexLogEnable) {
			LogPduHex(false, buf, len);
		}
		if (smppLogEnable && h != NULL) {
			LogPdu(h);
		}

	}

	return bytesSent;

}

Smpp::Header* SmppNwIf::RecvPdu(TCPSocket* s) {

	if (s == NULL)
		s = sock;

	if(m_rcvBuf == NULL)
		return NULL;

	Smpp::Header* hdr = NULL;
	int ret = 0;

	int recvedBytes = s->Recv((char*)m_rcvBuf->buf, 16);
	if (recvedBytes != 16) {
		MT_LOGERROR("can't recv smpp pdu header. recv only  "<<recvedBytes<<" bytes");
		return NULL;
	}

	unsigned int len = ntohl(m_rcvBuf->hdr.length);

	if(len > sizeof(RcvBuf)){
		MT_LOGERROR("can't recv smpp pdu header. length is too much: "<<len)
		return NULL;
	}

	do {

		if (len > 16) {
			ret = s->Recv((char*)(m_rcvBuf->buf + recvedBytes), len - recvedBytes);
			if (ret <= 0) {
				MT_LOGERROR("can't recv smpp pdu body");
				return NULL;
			}
			recvedBytes += ret;
		}
	} while ((unsigned) recvedBytes < len);

	if (hexLogEnable) {
		LogPduHex(true, m_rcvBuf->buf, len);
	}
	Smpp::BindReceiver *brv = NULL;
	Smpp::BindReceiverResp *brvr = NULL;
	Smpp::BindTransmitterResp *btmr = NULL;
	Smpp::BindTransmitter *btm = NULL;
	Smpp::BindTransceiver *btv = NULL;
	Smpp::SubmitSmResp *ssr = NULL;
	Smpp::SubmitSm *sm = NULL;
	Smpp::BindTransceiverResp *btvr = NULL;
	Smpp::DeliverSm *ds = NULL;
	Smpp::DeliverSmResp *dsr = NULL;
	Smpp::EnquireLink *enq = NULL;
	Smpp::EnquireLinkResp *enqr = NULL;
	try {
		switch (ntohl(m_rcvBuf->hdr.cmd)) {

		case 0x00000001:
			brv = new Smpp::BindReceiver();
			brv->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			hdr = (Smpp::Header*) brv;
			break;
		case 0x80000001:
			brvr = new Smpp::BindReceiverResp();
			hdr = (Smpp::Header*) brvr;
			brvr->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;

		case 0x00000002:
			btm = new Smpp::BindTransmitter();
			hdr = (Smpp::Header*) btm;
			btm->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;
		case 0x80000002:
			btmr = new Smpp::BindTransmitterResp();
			hdr = (Smpp::Header*) btmr;
			btmr->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;

		case 0x00000009:
			btv = new Smpp::BindTransceiver();
			hdr = (Smpp::Header*) btv;
			btv->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;
		case 0x80000009:
			btvr = new Smpp::BindTransceiverResp();
			hdr = (Smpp::Header*) btvr;
			btvr->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;
		case 0x00000004:
			sm = new Smpp::SubmitSm();
			hdr = (Smpp::Header*) sm;
			sm->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			LogSm(sm, true);
			break;
		case 0x80000004:
			ssr = new Smpp::SubmitSmResp();
			hdr = (Smpp::Header*) ssr;
			ssr->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			LogSmResp(ssr, true);
			break;

		case 0x00000005:
			ds = new Smpp::DeliverSm();
			hdr = (Smpp::Header*) ds;
			ds->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;
		case 0x80000005:
			dsr = new Smpp::DeliverSmResp();
			hdr = (Smpp::Header*) dsr;
			dsr->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;

		case 0x00000015:
			enq = new Smpp::EnquireLink();
			hdr = (Smpp::Header*) enq;
			enq->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;

		case 0x80000015:
			enqr = new Smpp::EnquireLinkResp();
			hdr = (Smpp::Header*) enqr;
			enqr->decode((const Smpp::Uint8*) m_rcvBuf->buf);
			break;

		default:
			MT_LOGWARNING("("<<m_systemId << ") RecvPdu: can't find wrapper for command: "<< ntohl(m_rcvBuf->hdr.cmd))
		}
	}
	catch (const std::exception &e) {
		MT_LOGWARNING("("<<m_systemId << ") exception in pdu parsing cmd_id: "<< ntohl(m_rcvBuf->hdr.cmd) <<". "<<e.what())
	}

	if (smppLogEnable && hdr != NULL) {
		LogPdu(hdr, true);
	}

	return hdr;

}

void SmppNwIf::LogPduHex(bool rcv, const uint8_t* const ptr, unsigned int len) {
	std::string logmsg = "";
	char buf[64];
	if (rcv) {
		logmsg = "RX:";
	} else {
		logmsg = "TX:";
	}

	for (unsigned int i = 0; i < len; i++) {
		if (!(i % 4))
			logmsg.append(" ");
		snprintf(buf, 64, "%.2x", (unsigned char) ptr[i]);
		logmsg.append(buf);
	}

	hexLogger->info("%s", logmsg.c_str());
}

void SmppNwIf::LogSm(Smpp::SubmitSm* sm, bool rcv) {

	std::string logStr, logmsg;
	char buf[1024];

	if (rcv) {
		logmsg = "RX:";
	} else {
		logmsg = "TX:";
	}

	Smpp::SmeAddress addr = sm->source_addr();
	std::string src = addr.address();

	addr = sm->destination_addr();
	std::string dst = addr.address();

	snprintf(
			buf,
			1024,
			"from %s, sn(%u), command_id(%u), status(%u), source_addr(%s), destination_addr(%s)",
			m_systemId.c_str(), sm->sequence_number(), sm->command_id(),
			sm->command_status(), src.c_str(), dst.c_str());

	logStr = buf;

	if (logStr.length()) {
		logmsg.append(logStr);
		CDRlogger->info("%s", logmsg.c_str());
	}
}

void SmppNwIf::LogSmResp(Smpp::SubmitSmResp* smr, bool rcv) {
	std::string logStr, logmsg;
	char buf[1024];

	if (rcv) {
		logmsg = "RX:";
	} else {
		logmsg = "TX:";
	}

	snprintf(buf, 1024, "from %s sn(%u), command_id(%u), status(%u)",
			m_systemId.c_str(), smr->sequence_number(), smr->command_id(),
			smr->command_status());

	logStr = buf;

	if (logStr.length()) {
		logmsg.append(logStr);
		CDRlogger->info("%s", logmsg.c_str());
	}
}

void SmppNwIf::LogPdu(Smpp::Header* h, bool rcv) {
	std::string logStr, logmsg;

	if (rcv) {
		logmsg = "RX:";
	} else {
		logmsg = "TX:";
	}

	if (!h->command_status()) {

		switch (h->command_id()) {

		case 0x80000002:
			logStr
					= PduLogHelper::LogBindTvResp(
							(Smpp::BindTransceiverResp*) h);
			break;
		case 0x00000002:
			logStr = PduLogHelper::LogBindTv((Smpp::BindTransceiver*) h);
			break;

		case 0x00000004:
			logStr = PduLogHelper::LogSubmitSm(dynamic_cast<Smpp::SubmitSm*>(h));
			break;
		case 0x80000004:
			logStr = PduLogHelper::LogSubmitSmResp((Smpp::SubmitSmResp*) h);
			break;
		case 0x00000005:
			logStr = PduLogHelper::LogDeliverSm((Smpp::DeliverSm*) h);
			break;
		case 0x80000005:
			logStr = PduLogHelper::LogDeliverSmResp((Smpp::DeliverSmResp*) h);
			break;

		case 0x00000006:
			logStr = PduLogHelper::LogUnbind((Smpp::Unbind*) h);
			break;

		case 0x00000015:
			logStr = PduLogHelper::LogEnqLink((Smpp::EnquireLink*) h);
			break;
		case 0x80000015:
			logStr = PduLogHelper::LogEnqLinkResp((Smpp::EnquireLinkResp*) h);
			break;

		}

	}


	if (logStr.length()) {
		logmsg.append(logStr);
		smppLogger->info("%s", logmsg.c_str());
	}
}

}

}

