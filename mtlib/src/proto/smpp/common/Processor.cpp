#include "mt/proto/smpp/common/Processor.h"

namespace mt {

namespace smpp {

Processor::Processor() {
	MT_LOGINIT(Processor)

	inQ = NULL;
	outQ = NULL;
	userQ = NULL;
	smppHelper = new SMPPHelper();

	smppHelper->init();

	seqNum = 1;
	requiestId = 1;
	stepReqId = 1;
	hexLogger = LoggerFactory::instance()->createLogger("BinaryPdu");
	CDRlogger = LoggerFactory::instance()->createLogger("cdr_logger");
	mysql = NULL;

}

int Processor::ProcessMP(Data* data) {

	MT_LOGTRACE("Process multipart dst(" << data->msg->to << "), from ("<<data->msg->from<<")")

	bool isAllPartsRcv = true;
	Request* req = GetRequestByMsisdn(data);

	(*(req->parts))[data->udhPartNum].second = true;

	if (req->msg == NULL) {
		req->msg = data->msg;
	} else {
		req->msg->body.append(data->msg->body);
		req->msg->context.append(data->msg->context);
		delete data->msg;
	}
	data->msg = NULL;

	for (unsigned int i = 1; i < req->parts->size(); i++) {
		if (!(*(req->parts))[i].second) {
			isAllPartsRcv = false;
		}
	}

	if (isAllPartsRcv) {
		data->msg = req->msg;
		RemoveRequestByMsisdn((char*) req->msg->to.c_str());
		delete req;
	} else {
		return 1;
	}

	return 0;
}

void Processor::RemoveRequestByMsisdn(const std::string &to) {
	m_mpMap.erase(to);
}





Request* Processor::GetRequestBySN(uint32_t i) {

	Request *req = NULL;
	std::map<uint32_t, Request*>::iterator it = m_reqMap.find(i);

	if (it != m_reqMap.end()) {
		req = it->second;
		m_reqMap.erase(it);
	}

	return req;
}

void Processor::setRequestBySN(uint32_t sn, Request* req ){
	m_reqMap[sn] = req;
}


Request* Processor::GetRequestByMsisdn(Data *data) {

	std::map<std::string, Request*>::iterator it = m_mpMap.find(data->msg->to);
	if (it != m_mpMap.end()) {
		return it->second;
	}

	Request *req = new Request();
	req->parts = new std::vector<PartStatus>(data->udhTotal + 1, PartStatus(0,
			false));
	m_mpMap[data->msg->to] = req;

	return req;
}

}
}
