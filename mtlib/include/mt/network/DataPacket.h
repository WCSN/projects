#ifndef __NETWORK__DATA_PACKET__H
#define __NETWORK__DATA_PACKET__H

#include <mt/common/DynamicBuffer.h>
#include <mt/common/SmartPtr.h>
#include <mt/utils/StrUtils.h>

namespace mt {

	/*!
	 * @class DataPacket mt/network/DataPacket.h
	 * @brief Shell for data packets.
	 */

	class DataPacket {

	public:
		//@{
		/**
		 * Default constructor
		 */
		DataPacket();

		/**
		 * @param src_host Source host of the packet.
		 * @param src_port Source port of the packet.
		 * @param dst_host Destination host of the packet.
		 * @param dst_port Destination port of the packet.
		 * @param data Data of the packet.
		 */
		DataPacket(const std::string& src_host, uint32_t src_port,
							 const std::string& dst_host, uint32_t dst_port,
							 const ByteBuffer& data);

		/**
		 * @param src_host Source host of the packet.
		 * @param src_port Source port of the packet.
		 * @param dst_host Destination host of the packet.
		 * @param dst_port Destination port of the packet.
		 * @param data Data of the packet.
		 */
		DataPacket(uint32_t src_host, uint32_t src_port,
							 uint32_t dst_host, uint32_t dst_port,
							 const ByteBuffer& data);

		virtual ~DataPacket();
		//@}

		/// Return source host of the packet as std::string.
		std::string srcHostAsString() const;

		/// Return source host of the packet as uint32_t.
		uint32_t srcHost() const;

		/// Return source port of the packet.
		uint32_t srcPort() const;

		/// Return destination host of the packet as std::string.
		std::string dstHostAsString() const;

		/// Return destination host of the packet as uint32_t.
		uint32_t dstHost() const;

		/// Return destination port of the packet.
		uint32_t dstPort() const;

		const ByteBuffer& data() const;

	private:
		uint32_t m_srcHost;						/**< Source host of the packet. */
		uint32_t m_srcPort;						/**< Source port of the packet. */
		uint32_t m_dstHost;						/**< Destination host of the packet. */
		uint32_t m_dstPort;						/**< Destination port of the packet. */
		ByteBuffer m_data;	          /**< Packet's data. */
	};

	typedef SmartPtr<DataPacket> DataPacketPtr;

	inline std::string DataPacket::srcHostAsString() const {
	    return StrUtils::ipToString(m_srcHost);
	}

	inline uint32_t DataPacket::srcHost() const {
	    return m_srcHost;
	}

	inline uint32_t DataPacket::srcPort() const {
	    return m_srcPort;
	}

    inline std::string DataPacket::dstHostAsString() const {
        return StrUtils::ipToString(m_dstHost);
    }

    inline uint32_t DataPacket::dstHost() const {
        return m_dstHost;
    }

    inline uint32_t DataPacket::dstPort() const {
        return m_dstPort;
    }

		inline const ByteBuffer& DataPacket::data() const {
			return m_data;
		}

		typedef mt::SmartPtr<DataPacket> DataPacketPtr;
} // namespace mt

#endif /* __NETWORK__DATA_PACKET__H */
