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
	//重新连接
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
	//最后接收到服务器数据包的时间
	unsigned int m_lastRecvTime;
	//最近发出ping包的时间
	unsigned int m_lastPingTime;
	//最近收到pong包的时间
	unsigned int m_lastPongTime;
	//ping包平均响应时间
	unsigned int m_avgDelay;
	unsigned int m_totalDelay;
	unsigned int m_pingCount;

	NetworkManager * m_pNetworkManager;
	//锁
	FastMutex m_lock;
	queue<char*> m_sendBufList;
	bool m_bHasAction;
	std::string host;
	unsigned short port;
};

#endif // SCSCONNECTION_H__