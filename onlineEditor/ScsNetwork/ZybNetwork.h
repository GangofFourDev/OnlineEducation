#ifndef ZYBNETWORK_H__
#define ZYBNETWORK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>

//��HttpManagerɾ�����ϲ㲻��ɾ���������
class HttpHandler
{
public:
	virtual void on_http_failed() = 0;
	//�����ȡ�����ݳ��Ⱥ�contentLen��һ�£�����-1,��������0
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
	//������
	virtual bool open(const std::string url) = 0;
	virtual void closeScsConnection() = 0;
	//�ص�����
	virtual void connect() = 0;
	virtual void release() = 0;
	virtual void reConnect() = 0;
	virtual bool isConnect() = 0;
	//��������
	virtual  void sendTextMessage(const char* message) = 0;
	//http get ����
	virtual  void httpGet(std::string url,HttpHandler*handler,int tryTotal = 1,int timeOut = 10) = 0;
	//http post ����param��json��ʽ�Ĳ���{"k1":"v1","k2":"v2"}
	virtual  void httpPost(std::string url,std::string paras,HttpHandler*handler,int tryTotal = 1,int timeOut = 5) = 0;
	//formFileName���ϴ��ӿ�Ҫ��ı����ļ��ؼ���Id��ÿ���ϴ��ӿڿ��ܲ�һ��
	virtual  void httpUpload(std::string url,std::string strFileName,HttpHandler*handler,std::string formFileName="file",
		int tryTotal = 1,int timeOut = 600) = 0;
	virtual void setCookie(std::string strCookie) = 0;
};

IScsNetwork *GetScsNetwork(ScsNetworkEvent*pEvent);

#endif // ZYBNETWORK_H__