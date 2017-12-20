#ifndef SCSCONNECTION_H__
#define SCSCONNECTION_H__

#include "ZybNetwork.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include <map>
#include <queue>
#include <vector>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

class NetworkManager;

class ScsConnection :public Poco::Runnable
{
public:
	ScsConnection(ScsNetworkEvent*pEvent,NetworkManager* pNetworkManager);
	~ScsConnection();
	
public:
	virtual bool open(const std::string url);

	virtual void connect();
	virtual void release();
	//��������
	void reConnect();
	bool isConnect();
	virtual void sendTextMessage(const char* message);
	virtual void run();
	
private:
	void releaseResouce();
	bool sendData();

public:
	/*std::string m_cuId;
	std::string m_userId;
	std::string m_version;*/
	std::string m_url;

	HTTPClientSession* m_cs;
	HTTPRequest *m_request;
	HTTPResponse *m_response;
	WebSocket *m_ws;

	bool m_bStop;
	Thread m_thread;
	ScsNetworkEvent*m_pEvent;
	//�����յ����������ݰ���ʱ��
	unsigned int m_lastRecvTime;
	//�������ping����ʱ��
	unsigned int m_lastPingTime;
	//����յ�pong����ʱ��
	unsigned int m_lastPongTime;
	//ping��ƽ����Ӧʱ��
	unsigned int m_avgDelay;
	unsigned int m_totalDelay;
	unsigned int m_pingCount;

	NetworkManager * m_pNetworkManager;
	//��
	FastMutex m_lock;
	queue<char*> m_sendBufList;
	bool m_bHasAction;
	std::string host;
	unsigned short port;
};

#endif // SCSCONNECTION_H__