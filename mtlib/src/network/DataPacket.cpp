#include <mt/network/DataPacket.h>


namespace mt {
	DataPacket::DataPacket() : m_srcHost(0),
														 m_srcPort(0),
														 m_dstHost(0),
														 m_dstPort(0) {
	}

	DataPacket::DataPacket(const std::string& src_host, uint32_t src_port,
												 const std::string& dst_host, uint32_t dst_port,
												 const ByteBuffer& data) :  m_srcHost(StrUtils::stringToIp(src_host)),
																										m_srcPort(src_port),
																										m_dstHost(StrUtils::stringToIp(dst_host)),
																										m_dstPort(dst_port),
																										m_data(data) {
	}

	DataPacket::DataPacket(uint32_t src_host, uint32_t src_port,
												 uint32_t dst_host, uint32_t dst_port,
												 const ByteBuffer& data) :  m_srcHost(src_host),
																										m_srcPort(src_port),
																										m_dstHost(dst_host),
																										m_dstPort(dst_port),
																										m_data(data) {
	}

	DataPacket::~DataPacket() {
	}
}
