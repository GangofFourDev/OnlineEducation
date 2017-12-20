#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <ostream>
#include <string.h>
#include "HttpJob.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/Handler.h"
#include "Poco/JSON/Template.h"
#include "Poco/JSON/Array.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/FileStream.h"
#include "Poco/Net/FilePartSource.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using Poco::Dynamic::Array;
using Poco::DynamicStruct;
HttpJob::HttpJob(std::string url,HttpHandler*handler,NameValueCollection cookie,
	int tryTotal,int timeOut)
{
	m_url_ = url;
	m_Cookie = cookie;
	m_type = 0;
	m_tryCount = 0;
	m_tryTotal = tryTotal;
	m_timeOut = timeOut;
	m_state = 0;
	m_pHandler = handler;
}

HttpJob::HttpJob(std::string url,std::string paras,HttpHandler*handler,NameValueCollection cookie,
	int tryTotal,int timeOut)
{
	m_url_ = url;
	m_paras = paras;
	m_Cookie = cookie;
	m_type = 1;
	m_tryCount = 0;
	m_tryTotal = tryTotal;
	m_timeOut = timeOut;
	m_state = 0;
	m_pHandler = handler;
}

HttpJob::HttpJob(std::string url,std::string strFileName,HttpHandler*handler,NameValueCollection cookie,std::string formFileName,
	int tryTotal,int timeOut ){
		m_url_ = url;
		m_strFileName = strFileName;
		m_strFormName = formFileName;
		m_Cookie = cookie;
		m_type = 2;
		m_tryCount = 0;
		m_tryTotal = tryTotal;
		m_timeOut = timeOut;
		m_state = 0;
		m_pHandler = handler;
}

HttpJob::~HttpJob(void)
{
	if(m_pHandler)
		delete m_pHandler;
}

void HttpJob::runGet(){
	
	try{
		URI uri(m_url_);
		HTTPClientSession s(uri.getHost(), uri.getPort());
		s.setTimeout(Poco::Timespan(m_timeOut,0));
		HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery());
		if(m_Cookie.size()>0)
			request.setCookies(m_Cookie);
		std::ostringstream ostr;
		m_tryCount++;
		s.sendRequest(request);
		HTTPResponse response;
		std::istream& rs = s.receiveResponse(response);
		{
			int len = response.getContentLength();
			int status = response.getStatus();
			std::string contentType = response.getContentType();
			{
				Poco::Mutex::ScopedLock autoLock(m_Mutex);
				LogTrace("content len:%d,status:%d,content type: %s \n",
					len,status,contentType.data());
				if(status!=200){
					m_state = 4;
					return;
				}
			}
			
			if(m_pHandler)
			{
				if(m_pHandler->on_http_success(m_url_,rs,len,contentType)!=0){
					m_Mutex.lock();
					m_state = 4;
					m_Mutex.unlock();
				}
				return;
			}

			{
				Poco::Mutex::ScopedLock autoLock(m_Mutex);
				m_state = 2;
			}
		}

	}catch(NetException& e){
		{
			Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 3;
		}

		return;
	}
	catch(TimeoutException&e){
		LogTrace("request timeout,%s \n",m_url_.data());
		{
			Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 4;
		}
		return;
	}catch(Exception&e){
		{
			Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 3;
		}
		return;
	}
}
// post 上传
void HttpJob::runUpload(){
	URI uri(m_url_);
	HTTPClientSession s(uri.getHost(), uri.getPort());
	//s.setTimeout(Poco::Timespan(m_timeOut,0));
	s.setKeepAlive(true);
	HTTPRequest request(HTTPRequest::HTTP_POST, uri.getPathAndQuery(),HTTPMessage::HTTP_1_1);
	if(m_Cookie.size()>0)
		request.setCookies(m_Cookie);
	try{
		m_tryCount++;
		Poco::Net::HTMLForm form;
		form.setEncoding(HTMLForm::ENCODING_MULTIPART);
		form.addPart(m_strFormName, new FilePartSource(m_strFileName));
		form.prepareSubmit(request);
		form.write(s.sendRequest(request));        

		Poco::Net::HTTPResponse response;
		std::istream &rs = s.receiveResponse(response);
		std::ostringstream ostr;
		//StreamCopier::copyStream(rs, ostr);	
		{
			int len = response.getContentLength();
			int status = response.getStatus();
			std::string contentType = response.getContentType();
			Poco::Mutex::ScopedLock autoLock(m_Mutex);
			LogTrace("content len:%d,status:%d,content type: %s \n",
				len,status,contentType.data());
			if(m_pHandler)
			{
				if(m_pHandler->on_http_success(m_url_,rs,len,contentType)!=0)
					m_state = 4;
				return;
			}
			m_state = 2;
		}

	}catch(NetException& e){
		{
			//Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 3;
		}

		return;
	}
	catch(Poco::TimeoutException&e){
		{
			m_state = 4;
			LogTrace("request timeout,state:%d,url %s \n",m_state,m_url_.data());
		}
		return;
	}catch(Exception&e){
		{
			Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 3;
		}
		return;
	}
}
void HttpJob::runPost(){
	URI uri(m_url_);
	//参数为空，直接返回
	if(m_paras.empty() == true)
	{
		m_state = 3;
		return ;
	}
	HTTPClientSession s(uri.getHost(), uri.getPort());
	s.setTimeout(Poco::Timespan(m_timeOut,0));
	HTTPRequest request(HTTPRequest::HTTP_POST, uri.getPathAndQuery(),HTTPMessage::HTTP_1_1);
	if(m_Cookie.size()>0)
		request.setCookies(m_Cookie);
	try{
		m_tryCount++;
		Poco::Net::HTMLForm form;
		Parser parser;
		Var result;
		result = parser.parse(m_paras);
		Object::Ptr object = result.extract<Object::Ptr>();
		DynamicStruct ds = *object;
		if(ds.size()<=0){
			m_state = 3;
			return;
		}
		DynamicStruct::Iterator item = ds.begin();
		while(item!=ds.end()){
			form.add(item->first,item->second);
			item++;
		}
		form.prepareSubmit(request);
		form.write(s.sendRequest(request));
		//s.sendRequest(request);
		HTTPResponse response;
		std::istream& rs = s.receiveResponse(response);
		
		{
			int len = response.getContentLength();
			int status = response.getStatus();
			std::string contentType = response.getContentType();
			Poco::Mutex::ScopedLock autoLock(m_Mutex);
			LogTrace("content len:%d,status:%d,content type: %s \n",
				len,status,contentType.data());
			if(m_pHandler)
			{
				if(m_pHandler->on_http_success(m_url_,rs,len,contentType)!=0)
					m_state = 4;
				return;
			}
			m_state = 2;
		}

	}catch(NetException& e){
		{
			//Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 3;
		}

		return;
	}
	catch(Poco::TimeoutException&e){
		{
			//Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 4;
			LogTrace("request timeout,state:%d,url %s \n",m_state,m_url_.data());
		}
		return;
	}catch(Exception&e){
		{
			Poco::Mutex::ScopedLock autoLock(m_Mutex);
			m_state = 3;
		}
		return;
	}
}
void HttpJob::run()
{
	LogTrace("start http request,%s %s \n",m_url_.data(), m_paras.data());
	{
		Poco::Mutex::ScopedLock autoLock(m_Mutex);
		m_state = 1;
	}
	switch(m_type){
	case 0:
		runGet();
		break;
	case 1:
		runPost();
		break;
	case 2:
		runUpload();
		break;
	}
	
}
std::string HttpJob::getUrl(){
	Poco::Mutex::ScopedLock autoLock(m_Mutex);
	return m_url_;
}

std::string HttpJob::getResult(){
	Poco::Mutex::ScopedLock autoLock(m_Mutex);
	return m_strResult;
}


//得到状态
int HttpJob::getState(){
	Poco::Mutex::ScopedLock autoLock(m_Mutex);
	return m_state;
}

//是否需要重试
bool HttpJob::needTry(){
	Poco::Mutex::ScopedLock autoLock(m_Mutex);
	if(m_state == 0)
		return true;
	if(m_state <= 2)
		return false;
	if(m_tryCount<m_tryTotal)
		return true;
	return false;
}
