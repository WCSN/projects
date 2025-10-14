/*
 * SMPPServer.cpp
 *
 *  Created on: 28.10.2009
 *      Author: hakamasy
 */

#include "mt/proto/smpp/SMPPServer.h"
#include "mt/proto/smpp/common/SMPPHelper.h"
#include "mt/proto/smpp/common/PduFactory.h"
#include "mt/proto/smpp/common/Utils.h"

#include <string>

#define MAXSMSLENGTH 140

namespace mt {
using namespace smpp;

SMPPServer::SMPPServer() {

	MT_LOGINIT(SMPPServer)

	smsId = 0;
	inQ = new Queue<Data*> ();
	outQ = new Queue<Data*> ();
	userQ = new Queue<Data*> ();
	proc = new ServerProcessor();//smpp::ServerProcessor::GetInstance();
	serviceType = "VMA";
	sndr = NULL;
	rcvr = NULL;
	m_pduFactory = new PduFactory();

}

int SMPPServer::init(XmlConfigPtr cfg) {

	m_codecToUcs2 = CodecFactory::getCodec("UTF-8", "UCS-2BE");
	m_codecTo8bit = CodecFactory::getCodec("UTF-8", "ASCII");
	m_codecTo7bit = CodecFactory::getCodec("UTF-8", "ASCII");

	if (!m_codecToUcs2 || !m_codecTo8bit || !m_codecTo7bit) {
		MT_LOGERROR("can't init codecs");
		return -1;
	}


	if (cfg) {
		serviceType = cfg->get("system_type");
	}

	proc->setInQ(inQ);
	proc->setOutQ(outQ);
	proc->setUserQ(userQ);

	servInsp = new smpp::ServerInspector(cfg);
	servInsp->SetProcessor(proc);
	if (servInsp->Init() < 0)
		return -1;

	try {
		proc->start();
		servInsp->start();
	}
	catch(const mt::ThreadException &e) {
		MT_LOGERROR("can't start all threads");
		return -1;
	}

	return 0;
}

Message* SMPPServer::recv() {

	Message* msg = NULL;
	Data* data = userQ->pop();
	if (data->msg != NULL) {
		msg = data->msg;
		data->msg = NULL;
		std::string tmp = msg->context;
		msg->context = data->systemId;
		msg->context.append(" " + tmp);
		if (data->id == 0) {
			msg->id = proc->GetRequestId();
		} else {
			msg->id = data->id;
		}
	}

	delete data;

	MT_LOGDEBUG("return msg: ("<<msg->body<<"), to ("<<msg->to << "), from ("<<msg->from<<") context ("<<msg->context<<"), id " <<msg->id)
	return msg;
}

int SMPPServer::send(Message* msg) {
	int reqId = -1;
	Request* req = NULL;
	Data* data = NULL;
	CodecPtr codec;
	uint8_t esmClass = 0x00;
	std::vector<std::string>* bodies;
	std::string sysId;
	int len;
	std::string msgBody;

	uint8_t esm_class = 0x00;

	switch (msg->type) {

	case DELIVERY_REPORT:
		m_logger->debug(
				"process delivery report. context is %s, from(%s) to(%s)",
				msg->context.c_str(), msg->from.c_str(), msg->to.c_str());

		bodies = ProcessDeliverContext(msg->context, &(sysId));
		if(msg->body.length() != 0){
			bodies = new std::vector<std::string>;
			bodies->push_back(msg->body);
			esm_class = 0x00;
		}else{
			esm_class = 0x04;
		}


		reqId = proc->GetRequestId();
		req = new Request(reqId);

		for (unsigned int i = 0; i < bodies->size(); i++) {
			data = new Data;
			data->fromUser = true;
			data->systemId = sysId;
			data->hdr = (Smpp::Header*) m_pduFactory->createDeliverSm(msg->to,
					msg->from, (*bodies)[i], esmClass);
			if (data->hdr != NULL) {
				data->req = req;
				data->id = reqId;
				inQ->push(data);
			} else {
				m_logger->warn("can't create smpp deliverSm PDU");
				delete data;
				return -1;
			}

		}
		break;

	case SMS:
		m_logger->debug("recv SMS from usersm from(%s), to(%s)",
				msg->from.c_str(), msg->to.c_str());

		switch (msg->codingScheme) {
		case ASCII:
			codec = m_codecTo7bit;
			break;
		case UCS2:
			codec = m_codecToUcs2;
			break;
		default:
			m_logger->warn("coding data scheme not set");
			return -1;
		}

		len = codec->Encode(msg->body, msgBody);
		if (len < 0) {
			m_logger->warn("can't encode msg body to (%s) from (%s)",
					msg->to.c_str(), msg->from.c_str());
			return -1;
		}

		try {
			reqId = proc->GetRequestId();
			req = new Request(reqId);

			std::vector<MessagePart>* vect = Utils::segmentateSmsBody((const uint8_t*)msgBody.data(),
					msgBody.length());
			if (vect == NULL) {
				delete req;
				return -1;
			}

			req->SetTotalParts(vect->size());
			req->parts = new std::vector<PartStatus>(vect->size(), PartStatus(
					0, false));

			for (unsigned int i = 0; i < vect->size(); i++) {

				MessagePart part = (*vect)[i];
				data = new Data;
				data->systemId = sysId;
				data->fromUser = true;
				if (req->GetTotalParts() > 1) {
					data->udhMsgId = part.first[3];
					data->udhTotal = part.first[4];
					data->udhPartNum = part.first[5];
					esmClass = 0x40;

				}
				data->req = req;
				data->id = reqId;
				data->hdr = (Smpp::Header*) m_pduFactory->createSubmitSm(
						msg->to, msg->from, part, msg->codingScheme,
						(uint8_t) msg->registeredDelivery, esmClass);
				delete part.first;

				if (data->hdr != NULL) {
					m_logger->debug(
							"send to processor submit sm from(%s), to(%s)",
							msg->from.c_str(), msg->to.c_str());
					inQ->push(data);
				} else {
					delete req;
					delete data;
					return -1;
				}
			}

			delete vect;
		}
		catch (std::exception e) {
			m_logger->error("can't send msg to %s, body %s (%s)",
					msg->to.c_str(), msg->body.c_str(), e.what());
			return -1;
		}
		;
		break;

	default:
		break;
	};

	return reqId;
}

std::vector<std::string>* SMPPServer::ProcessDeliverContext(
		std::string context, std::string *sysId) {
	char c_sysId[64];
	char c_msgId[64];

	const char* dataContext = context.c_str();
	unsigned int len = 0;
	std::vector<std::string> *msgIds = new std::vector<std::string>;
	sscanf(dataContext + len, "%s", c_sysId);
	len += strlen(c_sysId) + 1;
	sysId->assign(c_sysId);

	while (len < strlen(dataContext)) {

		sscanf((char*) (dataContext + len), "%s", c_msgId);
		std::string msgId = c_msgId;
		/*	std::string msgId = "id=";
		 msgId.append(c_msgId);*/
		msgIds->push_back(msgId);
		len += strlen(c_msgId) + 1;
		//		printf("\n\n\n\n len %u, strlen %u\n\n\n", len,  strlen(dataContext));
	}
	return msgIds;
}

std::vector<MessagePart>* SMPPServer::SegmentateMsg(const char* msg,
		unsigned int len) {

	std::vector<MessagePart>* vect = new std::vector<MessagePart>;
	char count = 1;
	unsigned int cur = 0, partLen;

	struct UdhHeader udh;
	size_t udhLen = sizeof(udh);
	udh.length = 0x05;
	udh.refNum = 0x00;
	udh.size = 0x03;
	udh.msgId = smsId++;
	udh.total = len / (MAXSMSLENGTH - 6) + 1;
	m_logger->debug("total len is %d, part count is %x\n", len, udh.total);

	do {
		MessagePart part;
		char* ptr = new char[MAXSMSLENGTH];
		udh.partNum = count++;
		memcpy(ptr, &udh, udhLen);

		if (cur + MAXSMSLENGTH - udhLen < len) {
			memcpy(ptr + udhLen, msg + cur, MAXSMSLENGTH - udhLen);
			partLen = MAXSMSLENGTH;
		} else {
			memcpy(ptr + udhLen, msg + cur, len - cur);
			partLen = len - cur + udhLen;
		}

		part.first = ptr;
		part.second = partLen;

		vect->push_back(part);
		cur += partLen - udhLen;

	} while (cur < len);

	return vect;
}


}
