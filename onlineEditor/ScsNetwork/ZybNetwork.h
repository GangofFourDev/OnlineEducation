#ifndef ZYBNETWORK_H__
#define ZYBNETWORK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>

//由HttpManager删除，上层不能删除这个对象
class HttpHandler
{
public:
	virtual void on_http_failed() = 0;
	//如果读取的数据长度和contentLen不一致，返回-1,其它返回0
	virtual int on_http_success(std::string url,std::istream& rs,int contentLen,std::string contentType) = 0;
};

class ScsNetworkEvent
{
public:
	virtual void on_scs_connect() = 0;
	virtual void on_scs_disconnect() = 0;
	virtual void on_scs_data(const char* strBuf) = 0;
	virtual void on_scs_timeOut() = 0;
	virtual void on_scs_net_quality(int level) = 0;
	virtual void on_scs_net_log(const char* strLog) = 0;
};

class IScsNetwork
{
public:
	//打开连接
	virtual bool open(const std::string url) = 0;
	virtual void closeScsConnection() = 0;
	//关掉连接
	virtual void connect() = 0;
	virtual void release() = 0;
	virtual void reConnect() = 0;
	virtual bool isConnect() = 0;
	//发送信令
	virtual  void sendTextMessage(const char* message) = 0;
	//http get 请求
	virtual  void httpGet(std::string url,HttpHandler*handler,int tryTotal = 1,int timeOut = 10) = 0;
	//http post 请求，param是json格式的参数{"k1":"v1","k2":"v2"}
	virtual  void httpPost(std::string url,std::string paras,HttpHandler*handler,int tryTotal = 1,int timeOut = 5) = 0;
	//formFileName是上传接口要求的表单中文件控件的Id，每个上传接口可能不一样
	virtual  void httpUpload(std::string url,std::string strFileName,HttpHandler*handler,std::string formFileName="file",
		int tryTotal = 1,int timeOut = 600) = 0;
	virtual void setCookie(std::string strCookie) = 0;
};

IScsNetwork *GetScsNetwork(ScsNetworkEvent*pEvent);

#endif // ZYBNETWORK_H__