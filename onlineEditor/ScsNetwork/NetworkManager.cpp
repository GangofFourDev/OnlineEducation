#include "stdafx.h"
#include "NetworkManager.h"
#include "ScsConnection.h"
#include <iostream>
#include <ostream>
#include <string.h>
#include "HttpJob.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/Handler.h"
#include "Poco/JSON/Template.h"
#include "Poco/JSON/Array.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Timestamp.h"
#include "ScsConnection.h"
#include "HttpJob.h"
using namespace Poco::Net;
using namespace Poco;

IScsNetwork*GetScsNetwork(ScsNetworkEvent*pEvent){
	return NetworkManager::GetInstance(pEvent);
}

NetworkManager*NetworkManager::m_instance = nullptr;
NetworkManager*NetworkManager::GetInstance(ScsNetworkEvent*pEvent){
	if (m_instance == nullptr){
		m_instance = new NetworkManager(pEvent);
	}
	return m_instance;
}

NetworkManager::NetworkManager(ScsNetworkEvent*pEvent):
	pool(3,4)
	, m_pScsConnection(NULL)
{
	m_pEvent = pEvent;
	m_Event.reset();
	m_bConnected = true;
	m_bRetrying = false;
	m_bUserStopConnection = false;
	pool.setStackSize(1);
}


NetworkManager::~NetworkManager()
{
}

void NetworkManager::onNetError(char* reason){
	Mutex::ScopedLock audoLock(m_Mutex);
	m_Event.set();
	m_bConnected = false;	
	if (m_pEvent&&m_bRetrying == false){
		m_pEvent->on_scs_disconnect();
		char strBuf[1024] = { 0 };
		sprintf(strBuf, "net connection has a error,reason:%s", reason);
		m_pEvent->on_scs_net_log(strBuf);
		m_pEvent->on_scs_net_quality(0);
		OutputDebugStringA("onNetError1\n");
	}
	m_bRetrying = true;
	m_Event.set();
	OutputDebugStringA("onNetError2\n");
}

//网络已恢复正常，重连线程进入休眠状态
void NetworkManager::onNetResume(){
	Mutex::ScopedLock audoLock(m_Mutex);
	m_Event.reset();
	if (m_pEvent)
		m_pEvent->on_scs_net_log("net connection has resume");
	m_bConnected = true;
	m_bRetrying = false;
	m_tryCount = 0;
}

void NetworkManager::httpGet(std::string url,HttpHandler*pHandler,int tryTotal,int timeOut){
	Mutex::ScopedLock autoLock(m_httpMutex);
	m_httpReqList.push_back(new HttpJob(url,pHandler,m_Cookie,tryTotal,timeOut));
}

void NetworkManager::httpPost(std::string url,std::string paras,HttpHandler*pHandler,int tryTotal,int timeOut){
	Mutex::ScopedLock autoLock(m_httpMutex);
	m_httpReqList.push_back(new HttpJob(url,paras,pHandler,m_Cookie,tryTotal,timeOut));
}

void NetworkManager::httpUpload(std::string url,std::string strFileName,HttpHandler*pHandler,std::string formFileName,
	int tryTotal,int timeOut ){
		Mutex::ScopedLock autoLock(m_httpMutex);
	m_httpReqList.push_back(new HttpJob(url,strFileName,pHandler,m_Cookie,formFileName,tryTotal,timeOut));
}

void NetworkManager::setCookie(std::string strCookie){
	m_Cookie.set("zybuss",strCookie);
}


void NetworkManager::run(){
	timeHeart = Timestamp();
	while(m_bStop ==false)
	{
		Thread::sleep(100);

		if (m_bConnected == false){
			Mutex::ScopedLock audoLock(m_Mutex);
			if (timeHeart.elapsed() > 3 * 1000 * 1000){
				if (m_pScsConnection&&m_bUserStopConnection == false)
					m_pScsConnection->reConnect();
				timeHeart = Timestamp();
				m_tryCount++;
			}
		}

		{
			Mutex::ScopedLock autoLock(m_httpMutex);
			//遍历http job 链表
			vector<HttpJob*>::iterator item = m_httpReqList.begin();
			while(item!=m_httpReqList.end()){
				HttpJob* job =*item;
				if(job!=nullptr&&job->needTry()==true){
					if(pool.used()<4)
						pool.start(*job);
				}else{
					//删除这个job
					if(job!=nullptr&&job->getState()>1){
						//if(job->getResult().size()>0)
						LogTrace("job has finish,state:%d\n",job->getState());
						item = m_httpReqList.erase(item++);
						delete job;
						continue;
					}
				}
				item++;
			}
		}//mutex scope
	}
	m_bRetrying = false;
}


void NetworkManager::connect(){
	if (m_pScsConnection == nullptr)
	{
		m_pScsConnection = new ScsConnection(m_pEvent, this);
	}
	m_bStop = false;
	m_thread.start(*this);
	m_Event.reset();
}

void NetworkManager::release(){
	m_bStop = true;

	m_Event.set();
	try{
		m_thread.join();
		pool.stopAll();
	}catch(Exception& e){

	}
	if(m_pScsConnection!=nullptr&&m_bUserStopConnection==false){
		m_pScsConnection->release();
		m_pScsConnection=nullptr;
	}
}

void NetworkManager::closeScsConnection(){
	Mutex::ScopedLock audoLock(m_Mutex);
	m_bUserStopConnection = true;
	if(m_pScsConnection!=nullptr){
		m_pScsConnection->release();
		m_pScsConnection=nullptr;
	}
	m_pScsConnection=nullptr;
}
void NetworkManager::sendTextMessage(const char* message){
	if(m_pScsConnection)
		m_pScsConnection->sendTextMessage(message);
}

bool NetworkManager::open(const std::string url){
	if (m_pScsConnection == nullptr)
	{
		m_pScsConnection = new ScsConnection(m_pEvent, this);
	}

	m_bUserStopConnection = false;
	return m_pScsConnection->open(url);
	
}

void NetworkManager::reConnect()
{
	m_bUserStopConnection = false;
	m_pScsConnection->reConnect();
}

bool NetworkManager::isConnect()
{
	return m_pScsConnection->isConnect();
}
