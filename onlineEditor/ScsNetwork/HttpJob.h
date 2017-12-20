#ifndef HTTPJOB_H__
#define HTTPJOB_H__

#include "Poco/Thread.h"
#include "ZybNetwork.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"

using namespace Poco::Net;

class HttpJob : public Poco::Runnable
{
public:
	HttpJob(std::string url,HttpHandler*handler,NameValueCollection cookie,int tryTotal = 1,int timeOut = 5);
	HttpJob(std::string url,std::string paras,HttpHandler*handler,NameValueCollection cookie,int tryTotal = 1,int timeOut = 5);
	HttpJob(std::string url,std::string strFileName,HttpHandler*handler,NameValueCollection cookie,std::string formFileName,
		int tryTotal,int timeOut );

	~HttpJob();

	virtual void run();

	std::string getUrl();
	std::string getResult();
	//得到状态
	int getState();
	//是否需要重试
	bool needTry();

private:
	void runGet();
	void runPost();
	void runUpload();

private:
	Poco::Mutex m_Mutex;
	std::string m_url_;
	std::string m_paras;
	//当前这个任务的状态，没开始，已开始，已正常完成，错误，超时
	int m_state;
	// 0 ,get,1,post
	int m_type;
	//重试次数
	int m_tryTotal;
	//已重试几次
	int m_tryCount;
	//超时时间
	int m_timeOut;
	std::string  m_strResult;
	HttpHandler*m_pHandler;
	NameValueCollection m_Cookie;
	std::string m_strFileName;
	std::string m_strFormName;
};

#endif // HTTPJOB_H__