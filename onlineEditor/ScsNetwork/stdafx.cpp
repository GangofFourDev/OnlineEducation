// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// ScsNetwork.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include <stdio.h>  
#include <wtypes.h>  
#include <stdarg.h>  
#include <tchar.h>  
#include "Poco/Mutex.h"
// TODO:  �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
static Poco::FastMutex m_Mutex;
void LogTrace(char* pszFormat, ...)  
{  
	Poco::FastMutex::ScopedLock autoLock(m_Mutex);
#ifdef _DEBUG
	va_list pArgs;  
	char szMessageBuffer[1024]={0};
	char szTimeBuffer[128] = {0};
	va_start( pArgs, pszFormat ); 
	_vsnprintf(szMessageBuffer, 1024, pszFormat, pArgs );  
	va_end( pArgs );
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	wsprintfA(szTimeBuffer,"%4d/%02d/%02d %02d:%02d:%02d.%03d ",sys.wYear,sys.wMonth,
		sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 
	::OutputDebugStringA(szTimeBuffer);
	::OutputDebugStringA(szMessageBuffer);
#endif
}