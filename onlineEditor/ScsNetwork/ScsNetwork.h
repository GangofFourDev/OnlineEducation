#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>

//��HttpManagerɾ�����ϲ㲻��ɾ���������
class HttpHandler{
public:
	virtual void on_http_failed() = 0;
	virtual void on_http_success(std::istream& rs) = 0;
};

class ScsNetworkEvent{
public:
	virtual void on_scs_connect() = 0;
	virtual void on_scs_disconnect() = 0;
	virtual void on_scs_data(std::string strBuf) = 0;
	virtual void on_scs_timeOut() = 0;
	virtual void on_scs_net_quality(int level) = 0;
	virtual void on_scs_net_log(const char* strLog) = 0;
};

class IScsNetwork{
public:
	//������
	virtual bool open(const std::string url, const std::string cuid, const std::string urserid,
		const std::string version) = 0;
	//�ص�����
	virtual void connect() = 0;
	virtual void release() = 0;
	//��������
	virtual  void sendTextMessage(const std::string message) = 0;
	//http get ����
	virtual  void httpGet(std::string url,HttpHandler*handler,int tryTotal = 1,int timeOut = 5) = 0;
	//http post ����param��json��ʽ�Ĳ���{"k1":"v1","k2":"v2"}
	virtual  void httpPost(std::string url,std::string paras,HttpHandler*handler,int tryTotal = 1,int timeOut = 5) = 0;
	virtual void setCookie(std::string strCookie) = 0;
};

IScsNetwork *GetScsNetwork(ScsNetworkEvent*pEvent);