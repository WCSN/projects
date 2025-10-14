#include "mt/proto/smpp/client/ClientProcessor.h"

namespace mt {
namespace smpp {

ClientProcessor::ClientProcessor() :
	Processor() {

	MT_LOGINIT(ClientProcessor)
}

int ClientProcessor::InitDb(std::string h, std::string u, std::string p,
		std::string db) {

	mysql = new MysqlConnector(h, u, p, db);

	if (mysql->init() < 0) {
		m_logger->error("can't init database");
		return -1;
	}

	return 0;
}

int ClientProcessor::GetDeliverMessage(Data* data) {
	Smpp::DeliverSm* pdu = (Smpp::DeliverSm*) data->hdr;

	Message* msg = new Message();

	msg->type = DELIVERY_REPORT;
	msg->from = pdu->source_addr().address();
	msg->to = pdu->destination_addr().address();
	msg->registeredDelivery = pdu->registered_delivery();
	msg->codingScheme = smppHelper->getCodingScheme(pdu);
	smppHelper->getMessage(pdu, msg->body);

	data->msg = msg;
	data->esm_class(pdu->esm_class());

	return 0;
}

void ClientProcessor::runLoop() {
	try {
		std::string dstAddr, srcAddr;
		Smpp::SubmitSm *sm;
		Smpp::DeliverSm *deliverSm;
		Request *req;
		unsigned long sn;

		MT_LOGTRACE("try get from queue address: " << inQ <<", size: "<< inQ->size())
		Data* data = inQ->pop();
		MT_LOGTRACE("get from queue address: " << inQ <<", size: "<< inQ->size())

		if (data == NULL) {
			usleep(1);
			return;
		}

		switch (data->hdr->command_id()) {
		case Smpp::CommandId::DeliverSm:
			deliverSm = (Smpp::DeliverSm*) data->hdr;

			srcAddr = deliverSm->source_addr().address();
			dstAddr = deliverSm->destination_addr().address();
			MT_LOGTRACE("recv DeliverSm pdu src(" << srcAddr << ") dst(" << dstAddr << ")")
			if (!GetDeliverMessage(data)) {
				if (data->isNormalMessage()) {

					MT_LOGTRACE("Processor send normal message sm msg to user");
					userQ->push(data);

				} else {
					if (!ProcessDeliverSm(data)) {
						MT_LOGDEBUG("Processor send Delivery report to user.");
						userQ->push(data);
					} else {
						MT_LOGTRACE("not all delivery report recieved");
					}
				}
			} else {
				MT_LOGWARNING("can't process pdu");
				delete data;
			}
			break;
		case Smpp::CommandId::SubmitSm:
			sm = (Smpp::SubmitSm*) data->hdr;
			srcAddr = sm->source_addr().address();
			dstAddr = sm->destination_addr().address();
			MT_LOGTRACE("recv Submit sm pdu src(" << srcAddr <<") dst("<< dstAddr<<")")

			sn = GetSeqNum();
			data->hdr->sequence_number(sn);
			setRequestBySN(sn, data->req);
			data->req = NULL;
			outQ->push(data);
			MT_LOGTRACE("put SubmitSm in out queue. size is: "<<outQ->size()<<" address: "<<outQ)
			break;

		case Smpp::CommandId::SubmitSmResp:
			sn = data->hdr->sequence_number();
			req = GetRequestBySN(sn);
			if (req != NULL) {
				if (req->needDeliverResp())
					insertMsgId(req->id, data->hdr);

				if (!req->HasResp(data->hdr->command_status())) {

					data->status = req->GetStatus();
					data->id = req->id;

					MT_LOGTRACE("send submit sm resp to user");
					userQ->push(data);
					delete req;
					req = NULL;
				}
			} else {
				MT_LOGWARNING("can't find request for syq_no "<<sn)
			}

			break;
		default:
			break;
		}

	} catch (...) {
		MT_LOGERROR("catch unknown exception in main cycle")
	}
	return;
}

void ClientProcessor::insertMsgId(unsigned int reqId, Smpp::Header* hdr) {

	char sql_buf[1024];
	std::string msgId;

	if (!hdr->command_status()) {

		Smpp::SubmitSmResp *sm = (Smpp::SubmitSmResp*) hdr;

		msgId = sm->message_id();
		snprintf(sql_buf, 1024, "insert into tMsgIds values('%s', '%u')",
				msgId.c_str(), reqId);
		std::string sqlQuery = sql_buf;

		try {
			mysql->execCmd(sqlQuery);
		} catch (const std::exception &e) {
			MT_LOGERROR("Database exception: "<<e.what())
		}

	} else {
		MT_LOGERROR("SubmitSmStatus not ok. no processing.")
	}

}

unsigned int ClientProcessor::ProcessDeliverSm(Data* data) {

	unsigned int msgPartRecieved = 0, ret = 0;
	char sql_query[2048], c_msgId[256];
	std::string sqlQuery;
	try {
		if (data->msg->body.find("id:") != std::string::npos)
			sscanf(data->msg->body.c_str(), "id:%s", c_msgId);
		else
			sscanf(data->msg->body.c_str(), "%s", c_msgId);

		snprintf(
				sql_query,
				2048,
				"SELECT tDeliverSmCache.context, tDeliverSmCache.part_num, tDeliverSmCache.req_id FROM tMsgIds, tDeliverSmCache where "
					"tMsgIds.msg_id = '%s' and tDeliverSmCache.req_id = tMsgIds.req_id",
				c_msgId);

		DbDataPtr dbData = mysql->exec(std::string(sql_query));
		if (!dbData) {
			MT_LOGWARNING("can't get message id from db. db is empty. "<<std::string(sql_query))
			ret = 1;
		} else {
			if (!dbData->isEmpty()) {
				data->msg->context = dbData->getColumn(0).getString(0);
				msgPartRecieved = dbData->getColumn(1).getUnsignedInt(0);
				data->id = dbData->getColumn(2).getUnsignedInt(0);

				msgPartRecieved--;
				snprintf(
						sql_query,
						2048,
						"UPDATE tDeliverSmCache SET part_num=%u where req_id = %u",
						msgPartRecieved, data->id);
				mysql->execCmd(std::string(sql_query));

				MT_LOGDEBUG("req id = " << data->id<<" sent parts was "<<msgPartRecieved)

				if (!msgPartRecieved) {

					snprintf(sql_query, 2048,
							"DELETE from tDeliverSmCache where req_id = %u",
							data->id);
					mysql->execCmd(std::string(sql_query));

					snprintf(sql_query, 2048,
							"DELETE from tMsgIds where req_id = %u", data->id);
					mysql->execCmd(std::string(sql_query));

				} else {
					ret = 1;
				}

			} else {
				MT_LOGWARNING("row is emty. " << std::string(sql_query))
				ret = 1;
			}

		}

	} catch (const std::exception &e) {
		MT_LOGERROR("Database exception: "<<e.what())
	}

	return ret;
}

}
}
