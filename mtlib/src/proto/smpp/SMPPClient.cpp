#include "mt/proto/smpp/SMPPClient.h"
#include "mt/proto/smpp/common/SMPPHelper.h"
#include "mt/proto/smpp/common/PduFactory.h"
#include "mt/proto/smpp/common/Utils.h"

namespace mt {

SMPPClient::SMPPClient() {

	MT_LOGINIT(SMPPClient)

	m_pduFactory = new PduFactory();

	inQ = new Queue<Data*> ;
	outQ = new Queue<Data*> ;
	userQ = new Queue<Data*> ;
	m_proc = new ClientProcessor();
	m_serviceType = "VMA";
	//	m_udhSmsId = 0;
	insp = NULL;
	m_mysql = NULL;
	m_startReqId = 1;
	m_stepReqId = 1;

}

bool SMPPClient::isActive() {
	bool ret = false;

	if (insp != NULL) {
		ret = insp->IsActive();
	}

	return ret;
}

void SMPPClient::close() {

	Data *data = new Data;
	data->msg = new Message();
	data->msg->type = UNBIND;
	Smpp::Unbind *un = new Smpp::Unbind();
	un->sequence_number(m_proc->GetSeqNum());
	data->hdr = un;
	pthread_mutex_init(&(data->unbindMtx), NULL);
	pthread_mutex_lock(&(data->unbindMtx));
	outQ->push(data);
	pthread_mutex_lock(&(data->unbindMtx));

}

SMPPClient::~SMPPClient() {
	delete inQ;
	delete outQ;
	delete userQ;
	delete m_proc;
}

unsigned int SMPPClient::GetReqId() {

	unsigned int tMsgIdsMaxId = 0;
	unsigned int tDeliverSmsCacheMaxId = 0;

	try {

		DbDataPtr dbData = m_mysql->exec(
				"SELECT req_id FROM tMsgIds ORDER by req_id DESC LIMIT 1");
		if (!dbData) {
			m_logger->debug("can't get message id from db. db is empty");
		} else {
			if (!dbData->isEmpty()) {
				tMsgIdsMaxId = dbData->getColumn(0).getUnsignedInt(0);
				m_logger->info("read max req_id from db tMsgIds %d",
						tMsgIdsMaxId);
				tMsgIdsMaxId++;
			}
		}

		dbData
				= m_mysql->exec(
						"SELECT req_id FROM tDeliverSmCache ORDER by req_id DESC LIMIT 1");
		if (!dbData) {
			m_logger->debug("can't get message id from db. db is empty");
		} else {
			if (!dbData->isEmpty()) {
				tDeliverSmsCacheMaxId = dbData->getColumn(0).getUnsignedInt(0);
				m_logger->info("read max req_id from db tDeliverSmCache %d",
						tDeliverSmsCacheMaxId);
				tDeliverSmsCacheMaxId++;
			}
		}
	} catch (const mt::Exception &e) {
		MT_LOGWARNING("data exception. " << e.displayText())
	}

	m_logger->info("Preseted req_id is %u", m_startReqId);

	if (tMsgIdsMaxId < tDeliverSmsCacheMaxId)
		tMsgIdsMaxId = tDeliverSmsCacheMaxId;

	if (tMsgIdsMaxId < m_startReqId)
		tMsgIdsMaxId = m_startReqId;

	if (m_startReqId % 2) { //odd
		if (!(tMsgIdsMaxId % 2)) {
			tMsgIdsMaxId++;
		}
	} else { //even
		if ((tMsgIdsMaxId % 2)) {
			tMsgIdsMaxId++;
		}
	}
	return tMsgIdsMaxId;

}

int SMPPClient::init(XmlConfigPtr cfg) {

	if (cfg) {
		m_serviceType = cfg->get("system_type");
		m_logger->debug("system type = %s", m_serviceType.c_str());

		m_systemId = cfg->get("system_id");
		m_connectionType = cfg->get("connection_type");
		MT_LOG_SETPREFIX(m_systemId + ":" + m_connectionType)
	} else {
		MT_LOGERROR("cfg is NULL");
		return -1;
	}

	m_codecToUcs2 = CodecFactory::getCodec("UTF-8", "UCS-2BE");
	m_codecTo8bit = CodecFactory::getCodec("UTF-8", "ASCII");
	m_codecTo7bit = CodecFactory::getCodec("UTF-8", "ASCII");

	if (!m_codecToUcs2 || !m_codecTo8bit || !m_codecTo7bit) {
		MT_LOGERROR("can't init codecs");
		return -1;
	}

	try {
		m_proc->setInQ(inQ);
		m_proc->setOutQ(outQ);
		m_proc->setUserQ(userQ);
		m_proc->setSystemId(m_systemId);
		m_proc->setConnectionType(m_connectionType);

		insp = new ClientInspector(cfg, m_proc);
		try {
			XmlConfigPtr dbCfg = cfg->getConfig(std::string("database"));
			if (dbCfg) {

				bool createDb = false;
				std::string host = dbCfg->get("host");
				std::string user = dbCfg->get("user");
				std::string pass = dbCfg->get("password");
				std::string db = dbCfg->get("database");
				if (db.length() == 0) {
					createDb = true;
					db = "msmpp_" + m_systemId;
				}

				m_mysql = new MysqlConnector(host, user, pass, db);

				if ((m_mysql->init(!createDb) < 0)) {
					m_logger->error("can't init database");
					return -1;
				}

				if (createDb) {
					try {

						try {
							m_mysql->execCmd("create database " + db);
							m_mysql->execCmd(
									"CREATE TABLE `tDeliverSmCache` (`req_id` int(11) NOT NULL default '0',"
										"`context` char(255) default NULL,"
										"`part_num` int(11) default NULL,"
										"PRIMARY KEY  (`req_id`)"
										") ENGINE=MyISAM DEFAULT CHARSET=latin1");

							m_mysql->execCmd(
									"CREATE TABLE `tMsgIds` (`msg_id` char(65) NOT NULL default '',`req_id` int(11) default NULL,PRIMARY KEY  (`msg_id`)) ENGINE=MyISAM DEFAULT CHARSET=latin1");
						} catch (const mt::DbExistDbException &e) {
							MT_LOGDEBUG("already have db");
						}

					} catch (...) {
						MT_LOGERROR("can't create database '" << db <<"'for smpp client")
						return -1;
					}
					m_mysql->execCmd("use " + db);

				}

				if (m_proc->InitDb(host, user, pass, db) < 0) {
					m_logger->error("can't init database");
					return -1;
				}

			} else {
				m_logger->error("can't find database config");
				return -1;
			}

		} catch (const mt::DbException& e) {
			MT_LOGERROR("can't init data base. " << e.displayText())

		}
		unsigned char ton = 0;
		unsigned char npi = 0;

		XmlConfigPtr bindCfg = cfg->getConfig(std::string("bind_params"));
		if (bindCfg) {
			std::string sTon = bindCfg->get("ton");
			std::string sNpi = bindCfg->get("npi");
			const char* str = sTon.c_str();
			ton = str[0] - 0x30;
			if (ton > 7) {
				m_logger->warn(
						"unknown value for bind_params ton %s. use default 0",
						sTon.c_str());
				ton = 0;
			}

			str = sNpi.c_str();
			npi = str[0] - 0x30;
			if (npi > 34) {
				m_logger->warn(
						"unknown value for bind_params npi %s. use default 0",
						sNpi.c_str());
				npi = 0;
			}
		}

		insp->SetTon(ton);
		insp->SetNpi(npi);

		unsigned int reqId = GetReqId();
		m_logger->info("statrt request id value is %u step for reqId is %u",
				reqId, m_stepReqId);
		m_proc->SetRequestId(reqId);
		m_proc->SetStepReqId(m_stepReqId);

		m_proc->start();
		insp->start();

	} catch (...) {
		m_logger->error("can't init SMPP Client. exception");
		return -1;
	}
	return 0;
}

//return pointer to msg or NULL if error occured; blocking function
Message* SMPPClient::recv() {
	Message* msg;
	Data* data = userQ->pop();
	if (data == NULL) {
		return NULL;
	}

	switch (data->hdr->command_id()) {

	case Smpp::CommandId::SubmitSmResp:

		msg = new Message();
		msg->id = data->id;
		msg->status = data->status;
		msg->type = SUBMIT_RESP;
		break;

	case Smpp::CommandId::DeliverSm:
		msg = data->msg;
		data->msg = NULL;
		msg->id = data->id;
		msg->status = 0;
		msg->type = DELIVERY_REPORT;
		m_logger->debug("send deliver sm to user. context is %s, id is %u",
				msg->context.c_str(), msg->id);

		break;

	case Smpp::CommandId::SubmitSm:
		msg = data->msg;
		data->msg = NULL;
		msg->id = data->id;

		break;

	default:
		msg = NULL;
		break;

	}

	delete data;

	m_logger->debug(
			"return msg: (%s), to (%s), from (%s) deliver(%.2x), context (%s), id %u",
			msg->body.c_str(), msg->to.c_str(), msg->from.c_str(),
			(uint8_t) msg->registeredDelivery, msg->context.c_str(),
			(uint32_t) msg->id);

	return msg;

}

int SMPPClient::send(Message* msg) {
	int reqId = -1;
	uint8_t esmClass = 0x00;

	Request* req = NULL;
	Data* data = NULL;
	CodecPtr codec;

	switch (msg->codingScheme) {
	case ASCII:
		codec = m_codecTo7bit;
		break;
	case UCS2:
		codec = m_codecToUcs2;
		break;
	default:
		m_logger->warn("coding data scheme not setted");
		return -1;
	}

	std::string msgBody;
	int len = codec->Encode(msg->body, msgBody);
	if (len < 0) {
		m_logger->warn("data encoding error");
		return -1;
	}

	try {
		reqId = m_proc->GetRequestId();
		req = new Request(reqId);
		if (msg->registeredDelivery) {
			req->setDeliverResp(true);
		}

		std::vector<MessagePart>* vect = Utils::segmentateSmsBody(
				(const uint8_t*) msgBody.data(), msgBody.length());

		if (vect == NULL) {
			delete req;
			return -1;
		}

		req->SetTotalParts(vect->size());

		for (unsigned int i = 0; i < vect->size(); i++) {

			MessagePart part = (*vect)[i];
			data = new Data;
			if (req->GetTotalParts() > 1) {
				data->udhMsgId = part.first[3];
				data->udhTotal = part.first[4];
				data->udhPartNum = part.first[5];
				esmClass = 0x40;
			}

			data->req = req;
			data->id = reqId;
			data->hdr = (Smpp::Header*) m_pduFactory->createSubmitSm(msg->to,
					msg->from, part, msg->codingScheme,
					(uint8_t) msg->registeredDelivery, esmClass);
			delete part.first;

			if (data->hdr != NULL) {
				MT_LOGDEBUG("send to processor SubmitSm from("<<msg->from<<"), to("<< msg->to<<")");
				MT_LOGTRACE("queue address: " << inQ <<", size: "<< inQ->size())
				inQ->push(data);
			} else {
				delete req;
				delete data;
				return -1;
			}
		}

		delete vect;

		if (req->needDeliverResp()) {
			char sql_buf[2048];
			snprintf(sql_buf, 2048,
					"insert into tDeliverSmCache values( %u, '%s', %u)",
					req->id, msg->context.c_str(), req->GetTotalParts());
			std::string sqlQuery = sql_buf;
			m_mysql->execCmd(sqlQuery);
		}

	} catch (const std::exception &e) {
		m_logger->error("can't send msg to %s, body %s (%s)", msg->to.c_str(),
				msg->body.c_str(), e.what());
		return -1;
	};
	return reqId;

}

}

