#ifndef CLIENTPROCESSOR_H_
#define CLIENTPROCESSOR_H_

#include "mt/proto/smpp/common/Processor.h"

namespace mt {
namespace smpp {
class ClientProcessor: public Processor {

protected:

public:

	void runLoop();

	int GetDeliverMessage(Data* data);

	void insertMsgId(unsigned int reqId, Smpp::Header* sm);

	virtual ~ClientProcessor() {
	}

	int InitDb(std::string h, std::string u, std::string p, std::string db);
	unsigned int ProcessDeliverSm(Data* data);

	ClientProcessor();

};

}
}

#endif /* CLIENTPROCESSOR_H_ */
