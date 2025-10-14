#include <pthread.h>

#include "mt/proto/smpp/common/Sender.h"
#include "mt/proto/smpp/common/Inspector.h"
#include "mt/proto/smpp/common/Processor.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
namespace mt {

namespace smpp {

Sender::~Sender() {

	stop();
}

void Sender::Send(Smpp::Header* h) {
	Data* data = new Data();
	data->hdr = h;
	queue->push(data);
}

Sender::Sender(Queue<Data*>* q, Inspector* i) {
	MT_LOGINIT(Sender);
	queue = q;
	inspector = i;
	sock = NULL;
	m_systemId = inspector->getSystemId();
	needEnqLink = false;
	time_t t;
	t = time(&t);
	lastActivity = t;
	m_sockFd = socket(AF_INET, SOCK_DGRAM, 0);


}

void Sender::stop() {

	notifyInspector = false;
	Deactivate();
	Data* data = new Data();
	data->terminate = true;
	queue->push(data);
	InfiniteThread::stop();

}

void Sender::runLoop() {

	//	Processor *proc = (Processor*) sndr->inspector->GetProcessor();

	int bytesSent;
	time_t t;
	t = time(&t);

	//	lastActivity = t;

	if (IsActive()) {

		Data* data = queue->pop();

		if (data == NULL) {
			return;
		}

		if (data->terminate) {
			MT_LOGWARNING("Sender is terminating. break");
			Deactivate();
			delete data;
			return;
		}

		struct timeval tmVal;
		tmVal.tv_sec = m_sendTimeout / 1000000L;
		tmVal.tv_usec = m_sendTimeout % 1000000L;

		select(0, NULL, NULL, NULL, &tmVal);

		if (needEnqLink) {
			if ((data->msg != NULL) && (data->msg->type == ENQ_LINK)) {
				if (t < lastActivity + 10) {
					delete data;
					return;
				}
				MT_LOGDEBUG("send enq link")
			}
		}

		lastActivity = t;
		bytesSent = SendPdu(sock, data->hdr);
		if (bytesSent < 0) {
			MT_LOGWARNING("send return error");
			queue->push(data);
			Deactivate();
			return;
		}

		if (data->msg != NULL && data->msg->type == UNBIND) {
			pthread_mutex_unlock(&(data->unbindMtx));
			Deactivate();
		}

		delete data;

	} else {
		MT_LOGWARNING("Sender is inactive. break");
		if (notifyInspector)
			inspector->NetworkError(this);
	}

	return;
}

}
}
