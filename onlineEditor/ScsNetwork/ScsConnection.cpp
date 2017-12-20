#include "stdafx.h"
#include "ScsConnection.h"
#include "NetworkManager.h"
#include "Poco/URI.h"


ScsConnection::ScsConnection(ScsNetworkEvent*pEvent,NetworkManager* pNetworkManager)
{
	m_pEvent = pEvent;
	m_pNetworkManager=pNetworkManager;
	m_ws = nullptr;
	m_bStop = true;
}


ScsConnection::~ScsConnection()
{
	
}

void ScsConnection::connect(){
	
}

void ScsConnection::release(){
	if (m_bStop == true)
		return;
	m_pEvent = nullptr;
	m_bStop = true;
	try{
		m_thread.join(100);
		releaseResouce();
	}catch(Exception &ex){

	}
	
	delete this;
}

bool ScsConnection::open(const std::string url){
	if (m_bStop ==false)
		return false;
	m_lastPongTime = 0;
	m_lastRecvTime = 0;
	m_lastPingTime = 0;
	m_totalDelay = 0;
	Poco::URI uri(url);
	host = uri.getHost();
	port = uri.getPort();
	m_url =  uri.getPathAndQuery();
	m_cs = new HTTPClientSession(host,port);
	m_request = new HTTPRequest(HTTPRequest::HTTP_GET,m_url);
	m_response = new HTTPResponse();
	try{
		m_ws = new WebSocket(*m_cs, *m_request, *m_response);
		
	}
	catch (Exception&){
		//releaseResouce();
		//return false;
		m_ws = nullptr;
		if (m_pNetworkManager)
			m_pNetworkManager->onNetError("first not connect");
		//m_bStop = false;
		//m_pNetworkManager->start();
		return false;
	}
	m_bStop = false;
	m_thread.start(*this);
	return true;
}

void ScsConnection::reConnect(){
	if (m_bStop == false)
		return ;
	LogTrace("********* start reconnect\n");
	if (m_pEvent != nullptr)
		m_pEvent->on_scs_net_log("start reconnect");
	m_lastPongTime = 0;
	m_lastRecvTime = 0;
	m_lastPingTime = 0;

	m_totalDelay = 0;
	m_cs = new HTTPClientSession(host,port);
	m_request = new HTTPRequest(HTTPRequest::HTTP_GET,m_url);
	m_response = new HTTPResponse();
	//WebSocket::setExtInfo(m_url, m_cuId, m_userId, m_version);
	try{
		m_ws = new WebSocket(*m_cs, *m_request, *m_response);
	}
	catch (Exception&){
		releaseResouce();
		if (m_pNetworkManager)
			m_pNetworkManager->onNetError("reconnect find a error");
		return;
	}
	m_bStop = false;
	m_thread.start(*this);
}

bool ScsConnection::isConnect()
{
	if (m_ws == nullptr)
	{
		return false;
	}
	return true;
}

void ScsConnection::releaseResouce(){
	if (m_ws != nullptr){
		m_ws->close();
		delete m_ws;
		m_ws = nullptr;
	}
	if (m_request != nullptr){
		delete m_request;
		m_request = nullptr;
	}
	if (m_cs != nullptr){
		delete m_cs;
		m_cs = nullptr;
	}
	if (m_response != nullptr){
		delete m_response;
		m_response = nullptr;
	}
	m_ws = nullptr;
}
void ScsConnection::run(){
	char* reason = "";
	char errorMsg[512] = {0};
	char buf[1024 * 10];
	int flags = 0;
	Poco::Timespan writeSpan(500 * 1000);
	Poco::Timespan errorSpan(10 * 1000);

	Poco::Timespan spanTimeOut(10 * 1000);
	Socket::SocketList readList;
	Socket::SocketList writeList;
	Socket::SocketList errorList;

	Timestamp timeHeart = Timestamp();
	Timestamp timeTimeout;
	try{
		if (m_ws!=nullptr&&m_ws->poll(writeSpan, Socket::SELECT_WRITE))
		{
			if (m_pNetworkManager)
				m_pNetworkManager->onNetResume();
			if (m_pEvent)
				m_pEvent->on_scs_connect();
		}
	}
	catch (Exception &ex){
		m_ws = nullptr;
		LogTrace("exception:%s\n",ex.message().data());
		reason = "can't write";
		goto OnError;
	}
	m_pingCount = 0;
	m_lastRecvTime = GetTickCount();
	
	while (m_bStop != true){
		if (m_ws == nullptr){
			reason = "m_ws is null";
			goto OnError;
		}
		
		try{
			m_bHasAction = false;
			{
				FastMutex::ScopedLock autoLock(m_lock);
				if (m_ws != nullptr&&m_ws->poll(writeSpan, Socket::SELECT_WRITE))
				if (sendData() == false){
					reason = "send data find a error";
					goto OnError;
				}
				
			}
			readList.push_back(*m_ws);
			errorList.push_back(*m_ws);
			if (m_ws!=nullptr&&Socket::select(readList, writeList, errorList, spanTimeOut))
			{
				if (m_bStop == true)
					break;
				if (readList.size() > 0){
					int n = 0;
					memset(buf, 0, 1024 * 10);
					if ((n = m_ws->receiveFrame(buf, 1024 * 10, flags)) > 0){
						m_bHasAction = true;
						if (n > 1){
							m_lastRecvTime = GetTickCount();
							if (n > 1024 * 10)
								break;
							buf[n] = '\0';
							if (m_pEvent != nullptr)
								m_pEvent->on_scs_data((const char*)buf);
							//::OutputDebugStringA(buf);
						}
						else{
							m_lastRecvTime = m_lastPongTime = ::GetTickCount();
							m_totalDelay += (m_lastPongTime - m_lastPingTime);
							if (m_pingCount > 0 && m_pingCount % 4 == 0){
								m_avgDelay = m_totalDelay / m_pingCount;
								m_pingCount = m_totalDelay = 0;
								char strBuf[64];
								sprintf(strBuf, "avg delay:%d\n", m_avgDelay);
						       //::OutputDebugStringA(strBuf);
								int level = 1;
								if (m_avgDelay <= 50)
									level = 4;
								else if (m_avgDelay <= 100)
									level = 3;
								else if (m_avgDelay < 200)
									level = 2;
								else
									level = 1;
								if (m_pEvent!=nullptr)
									m_pEvent->on_scs_net_quality(level);
							}
						}
					}
					
				}
				else{
					//::OutputDebugStringA("not read data \n");
				}
				//end read

				if (errorList.size() > 0){
					reason = "connection find a error";
					goto OnError;
				}
				
			}
	
		}catch (NetException&e){
			if(e.message().empty()==false){
				sprintf(errorMsg,"connection find a exceptin 1,%s\n",e.message().data());
				reason = errorMsg;
			}
			
			goto OnError;
		}
		
		if (timeHeart.elapsed() > 3 * 1000 * 1000)
		{
			//if (GetTickCount() - m_lastRecvTime > 15 * 1000)
			if (GetTickCount() - m_lastRecvTime > 3 * 60 * 1000)
			{
				reason = "connection time out";
				goto OnError;
			}
			

			if (m_ws != nullptr){
				try{
					//m_ws->ping();
					m_bHasAction = true;
				}
				catch (NetException&e){
					reason = "connection find a exceptin 2";
					goto OnError;
				}
				m_pingCount++;
				m_lastPingTime = ::GetTickCount();
			}
			timeHeart = Timestamp();
		}
		if (m_bHasAction == false)
			Thread::sleep(60);
		else
			Thread::sleep(10);
	}

OnError:
	m_bStop = true;
	releaseResouce();
	::OutputDebugStringA("************************on net error2 \n");
	if (m_pNetworkManager)
		m_pNetworkManager->onNetError(reason);
}
bool ScsConnection::sendData(){
	if (m_ws != nullptr){
		try
		{
			
			if(m_sendBufList.size() > 0)
			{
				m_bHasAction = true;
				char * strSend = m_sendBufList.front();
				if (m_ws!= nullptr)
				{
					int size = strlen(strSend);
					m_ws->sendFrame(strSend,size);
					m_sendBufList.pop();
					delete strSend;
				}
			}
			else{
				//::OutputDebugStringA("not data to send \n");
			}
		}
		catch (NetException&){
			if (m_pEvent)
				m_pEvent->on_scs_net_log("send data error");
			return false;
		}
	}
	
	return true;
}
void ScsConnection::sendTextMessage(const char* message){
	if (m_ws == nullptr)
		return;
	
	char *utfStr = new char[strlen(message)+1];
	strcpy(utfStr, message);
	//sprintf(utfStr, "%s\r\n", strCmd);
	FastMutex::ScopedLock autoLock(m_lock);
	m_sendBufList.push(utfStr);
	
}



