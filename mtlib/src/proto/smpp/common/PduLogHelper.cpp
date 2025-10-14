/*
 * PduLogHelper.cpp
 *
 *  Created on: 11.02.2010
 *      Author: hakamasy
 */

#include "mt/proto/smpp/common/PduLogHelper.h"
#include "mt/proto/smpp/common/SMPPHelper.h"

namespace mt{

namespace smpp{

std::string PduLogHelper::LogUnbind(Smpp::Unbind* unb){

	char buf[10000] = "";

	std::string msg;

	sprintf(
			buf,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u)",
				unb->command_length(), unb->command_id(), unb->command_status(),
				unb->sequence_number()

	);

	if (strlen(buf)) {
		msg = buf;
	}
	return msg;

}



std::string PduLogHelper::LogEnqLink(Smpp::EnquireLink* enq){

	char buf[10000] = "";

	std::string msg;

	sprintf(
			buf,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u)",
				enq->command_length(), enq->command_id(), enq->command_status(),
				enq->sequence_number()

	);

	if (strlen(buf)) {
		msg = buf;
	}
	return msg;

}



std::string PduLogHelper::LogEnqLinkResp(Smpp::EnquireLinkResp* enqr){


	char buf[10000] = "";

	std::string msg;

	sprintf(
			buf,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u)",
				enqr->command_length(), enqr->command_id(), enqr->command_status(),
				enqr->sequence_number()

	);

	if (strlen(buf)) {
		msg = buf;
	}
	return msg;

}


std::string PduLogHelper::LogSubmitSm(Smpp::SubmitSm* sm) {

	if(sm == NULL){
		return std::string("dynami cast submit sm error occured");
	}

	char buf[10000] = "";
	std::string msg;

	std::string stype;
	stype = sm->service_type();

	unsigned char src_ton, src_npi;
	Smpp::SmeAddress addr = sm->source_addr();
	src_ton = addr.ton();
	src_npi = addr.npi();
	std::string src = addr.address();

	unsigned char dst_ton, dst_npi;
	addr = sm->destination_addr();
	dst_ton = addr.ton();
	dst_npi = addr.npi();
	std::string dst = addr.address();

	unsigned char esm_class, proto_id, prior_f, reg_deliv, rep_if_pres,
			data_cod, sm_default_msg_id, sm_len;
	esm_class = sm->esm_class();
	proto_id = sm->protocol_id();
	prior_f = sm->priority_flag();
	reg_deliv = sm->registered_delivery();
	rep_if_pres = sm->replace_if_present_flag();
	data_cod = sm->data_coding();
	sm_default_msg_id = sm->sm_default_msg_id();
	sm_len = sm->sm_length();
//	std::string smsg = SMPPHelper::GetMessage(sm);

	snprintf(buf, 10000, "SMPP_HEADER:"
		"common_length(%u), "
		"common_id(%u), "
		"common_status(%u), "
		"seq_number(%u) MANDATORY_PARAMS: "
		"service_type('%s')),"
		"source_addr_ton(%u)"
		"source_addr_npi(%u)"
		"source_addr_src(%s)"
		"destination_addr_ton(%u)"
		"destination_addr_npi(%u)"
		"destination_addr_src(%s)"
		"esm_class(%u)"
		"protocol_id(%u)"
		"priority_flag(%u)"
		"registered_delivery(%u)"
		"replace_if_present_flag(%u)"
		"data_coding(%u)"
		"sm_default_msg_id(%u)"
		"sm_length(%u)"
//		"sm(%s)"

//TODO add output for msg body
			, sm->command_length(), sm->command_id(), sm->command_status(),
			sm->sequence_number(), stype.c_str(), src_ton, src_npi,
			src.c_str(), dst_ton, dst_npi, dst.c_str(), esm_class, proto_id,
			prior_f, reg_deliv, rep_if_pres, data_cod, sm_default_msg_id,
			sm_len

	);

	if (strlen(buf))
		msg = buf;

	return msg;
}



std::string PduLogHelper::LogDeliverSm(Smpp::DeliverSm* dm) {

	char buf[1024] = "";
	std::string msg;

	std::string stype;
	stype = dm->service_type();

	unsigned char src_ton, src_npi;
	Smpp::SmeAddress addr = dm->source_addr();
	src_ton = addr.ton();
	src_npi = addr.npi();
	std::string src = addr.address();

	unsigned char dst_ton, dst_npi;
	addr = dm->destination_addr();
	dst_ton = addr.ton();
	dst_npi = addr.npi();
	std::string dst = addr.address();

	unsigned char esm_class, proto_id, prior_f, reg_deliv, rep_if_pres,
			data_cod, sm_default_msg_id, sm_len;
	esm_class = dm->esm_class();
	proto_id = dm->protocol_id();
	prior_f = dm->priority_flag();
	reg_deliv = dm->registered_delivery();
	rep_if_pres = dm->replace_if_present_flag();
	data_cod = dm->data_coding();
	sm_default_msg_id = dm->sm_default_msg_id();
	sm_len = dm->sm_length();
//	std::string smsg = SMPPHelper::GetMessage(dm);

	snprintf(buf, 1024, "SMPP_HEADER:"
		"common_length(%u), "
		"common_id(%u), "
		"common_status(%u), "
		"seq_number(%u) MANDATORY_PARAMS: "
		"service_type('%s')),"
		"source_addr_ton(%u)"
		"source_addr_npi(%u)"
		"source_addr_src(%s)"
		"destination_addr_ton(%u)"
		"destination_addr_npi(%u)"
		"destination_addr_src(%s)"
		"esm_class(%u)"
		"protocol_id(%u)"
		"priority_flag(%u)"
		"registered_delivery(%u)"
		"replace_if_present_flag(%u)"
		"data_coding(%u)"
		"sm_default_msg_id(%u)"
		"sm_length(%u)"
//		"dm(%s)"

			, dm->command_length(), dm->command_id(), dm->command_status(),
			dm->sequence_number(), stype.c_str(), src_ton, src_npi,
			src.c_str(), dst_ton, dst_npi, dst.c_str(), esm_class, proto_id,
			prior_f, reg_deliv, rep_if_pres, data_cod, sm_default_msg_id,
			sm_len

	);

	return std::string(buf);
}


std::string PduLogHelper::LogDeliverSmResp(Smpp::DeliverSmResp* dmr) {
	char buf[1024] = "";

	std::string message_id = dmr->message_id();

	snprintf(buf, 1024, "SMPP_HEADER:"
		"common_length(%u), "
		"common_id(%u), "
		"common_status(%u), "
		"seq_number(%u) MANDATORY_PARAMS: message_id('%s'))",
			dmr->command_length(), dmr->command_id(), dmr->command_status(),
			dmr->sequence_number(), message_id.c_str());

	return std::string(buf);

}


std::string PduLogHelper::LogSubmitSmResp(Smpp::SubmitSmResp* smr) {
	char buf[1024] = "";

	std::string msg;
	std::string message_id = smr->message_id();

	snprintf(buf, 1024, "SMPP_HEADER:"
		"common_length(%u), "
		"common_id(%u), "
		"common_status(%u), "
		"seq_number(%u) MANDATORY_PARAMS: message_id('%s'))",
			smr->command_length(), smr->command_id(), smr->command_status(),
			smr->sequence_number(), message_id.c_str());

	return std::string(buf);

}

std::string PduLogHelper::LogBindTvResp(Smpp::BindTransceiverResp* btr) {

	char buf[1024] = "";

	std::string sysId = btr->system_id();
	snprintf(
			buf, 1024,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u) MANDATORY_PARAMS: system_id('%s')) OPTIONAL_PARAMS: sc_interface_ver()",
			btr->command_length(), btr->command_id(), btr->command_status(),
			btr->sequence_number(), sysId.c_str()

	);

	return std::string(buf);
}

std::string PduLogHelper::LogBindTv(Smpp::BindTransceiver* btv) {
	char buf[10000] = "";

	std::string msg;

	std::string sysId = btv->system_id();
	std::string pass = btv->password();
	std::string sysType = btv->system_type();

	unsigned char ton, npi, ifV;
	npi = btv->addr_npi();
	ton = btv->addr_ton();
	ifV = btv->interface_version();


	sprintf(
			buf,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u) MANDATORY_PARAMS: system_id('%s')), "
				"password('%s'),"
				"ton(%u),"
				"npi(%u),"
				"interface_version(%u) OPTIONAL_PARAMS system_type('%s')",
			btv->command_length(), btv->command_id(), btv->command_status(),
			btv->sequence_number(), sysId.c_str(), pass.c_str(),
			ton, npi, ifV, sysType.c_str()

	);

	if (strlen(buf)) {
		msg = buf;
	}

	return msg;
}


std::string PduLogHelper::LogBindTmResp(Smpp::BindTransmitterResp* btr) {

	char buf[10000] = "";

	std::string msg;

	std::string sysId = btr->system_id();
	snprintf(
			buf, 10000,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u) MANDATORY_PARAMS: system_id('%s')) OPTIONAL_PARAMS: sc_interface_ver()",
			btr->command_length(), btr->command_id(), btr->command_status(),
			btr->sequence_number(), sysId.c_str()

	);

	if (strlen(buf)) {
		msg = buf;
	}
	return msg;
}

std::string PduLogHelper::LogBindTm(Smpp::BindTransmitter* btv) {
	char buf[1024] = "";

	std::string sysId = btv->system_id();
	std::string pass = btv->password();
	std::string sysType = btv->system_type();

	unsigned char ton, npi, ifV;
	npi = btv->addr_npi();
	ton = btv->addr_ton();
	ifV = btv->interface_version();


	snprintf(
			buf, 1024,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u) MANDATORY_PARAMS: system_id('%s')), "
				"password('%s'),"
				"ton(%u),"
				"npi(%u),"
				"interface_version(%u) OPTIONAL_PARAMS system_type('%s')",
			btv->command_length(), btv->command_id(), btv->command_status(),
			btv->sequence_number(), sysId.c_str(), pass.c_str(),
			ton, npi, ifV, sysType.c_str()

	);

	return std::string(buf);
}



std::string PduLogHelper::LogHdr(Smpp::Header* hdr) {

	char buf[10000] = "";

	std::string msg;

	snprintf(
			buf, 10000,
			"SMPP_HEADER:"
				"common_length(%u), "
				"common_id(%u), "
				"common_status(%u), "
				"seq_number(%u)",
				hdr->command_length(), hdr->command_id(), hdr->command_status(),
				hdr->sequence_number()
	);

	if (strlen(buf)) {
		msg = buf;
	}
	return msg;
}
}
}
