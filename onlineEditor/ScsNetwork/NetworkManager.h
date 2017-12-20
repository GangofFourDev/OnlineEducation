#ifndef NETWORKMANAGER_H__
#define NETWORKMANAGER_H__

/**
  *    ��������࣬������һ���̣߳�������������֮��
  *
  */
#include <vector>
#include "ZybNetwork.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/Timestamp.h"
#include "Poco/ThreadPool.h"
#include "Poco/Net/NameValueCollection.h"

using namespace Poco;
using namespace Poco::Net;
using namespace std;
class ScsConnection;
class HttpJob;

class NetworkManager :public Poco::Runnable,public IScsNetwork
{
public:
	NetworkManager(ScsNetworkEvent*pEvent);
	~NetworkManager();

	static NetworkManager * m_instance;
	static NetworkManager * GetInstance(ScsNetworkEvent*pEvent);

	virtual bool open(const std::string url);
	virtual void closeScsConnection();
	virtual void connect();
	virtual void release();
	virtual void sendTextMessage(const char* message);
	virtual void reConnect();
	virtual bool isConnect();

	virtual  void httpGet(std::string url,HttpHandler*handler,int tryTotal = 1,int timeOut = 5);
	virtual  void httpPost(std::string url,std::string paras,HttpHandler*handler,int tryTotal = 1,int timeOut = 5);
	virtual  void httpUpload(std::string url,std::string strFileName,HttpHandler*handler,std::string formFileName="file",
		int tryTotal = 1,int timeOut = 600);
	virtual void setCookie(std::string strCookie);

public:
	//������ִ���
	void onNetError(char* reason);
	//�����ѻָ������������߳̽�������״̬
	void onNetResume();
	//�̹߳�������
	virtual void run();

private:
	bool m_bStop;
	ScsNetworkEvent*m_pEvent;
	ScsConnection* m_pScsConnection;
	Thread m_thread;
	//�����������ӵĴ���
	int m_tryCount;
	Event m_Event;
	bool m_bConnected;
	//����������
	bool m_bRetrying;
	Timestamp timeHeart;
	
	//http thread pool
	ThreadPool pool;

	/****http��ر���******/
	vector<HttpJob*> m_httpReqList;
	Mutex m_httpMutex;
	NameValueCollection m_Cookie;

	Mutex m_Mutex;
	//�û��ص��˳����ӣ�������
	bool m_bUserStopConnection;
};

#endif // NETWORKMANAGER_H__