#include "UserInfo.h"


UserInfo::UserInfo(__int64 nUserId, std::string strUserName, bool bAudio, bool bVideo
	, ENUM_Role enumRole, bool bHasCam, TerminalType terminalType)
	: m_nUserId(nUserId), m_strUserName(strUserName), m_bAudio(bAudio), m_bVideo(bVideo)
	, m_enumRole(enumRole), m_bHasCam(bHasCam), m_bIsApplySpeak(false), m_enTerminalType(terminalType)
{

}


UserInfo::~UserInfo()
{

}

__int64 UserInfo::userId()
{
	return m_nUserId;
}

void UserInfo::setUserId(__int64 nUserId)
{
	m_nUserId = nUserId;
}

std::string UserInfo::userName()
{
	return m_strUserName;
}

void UserInfo::setUserName(std::string strUserName)
{
	if (strUserName.empty())
	{
		int a = 1;
		a = 7;
	}
	m_strUserName = strUserName;
}

bool UserInfo::audio()
{
	return m_bAudio;
}

void UserInfo::setAudio(bool bAudio)
{
	m_bAudio = bAudio;
}

bool UserInfo::video()
{
	return m_bVideo;

}

void UserInfo::setVideo(bool bVideo)
{
	m_bVideo = bVideo;
}

ENUM_Role UserInfo::role()
{
	return m_enumRole;
}

void UserInfo::setRole(ENUM_Role enumRole)
{
	m_enumRole = enumRole;
}

bool UserInfo::hasCam()
{
	return m_bHasCam;
}

void UserInfo::setHasCam(bool bHasCam)
{
	m_bHasCam = bHasCam;
}

bool UserInfo::applySpeak() const
{
	return m_bIsApplySpeak;
}

void UserInfo::setApplySpeak(bool isOk)
{
	m_bIsApplySpeak = isOk;
}

TerminalType UserInfo::terminalType()
{
	return m_enTerminalType;
}

void UserInfo::setTerminalType(TerminalType enumTType)
{
	m_enTerminalType = enumTType;
}
