#ifndef USERINFO_H__
#define USERINFO_H__

#include <string>
#include "common.h"

class UserInfo
{
public:
	UserInfo(__int64 nUserId, std::string strUserName, bool bAudio, bool bVideo
		, ENUM_Role enumRole, bool bHasCam, TerminalType terminalType);
	~UserInfo();

	__int64 userId();
	void setUserId(__int64 nUserId);

	std::string userName();
	void setUserName(std::string strUserName);

	bool audio();
	void setAudio(bool bAudio);

	bool video();
	void setVideo(bool bVideo);

	ENUM_Role role();
	void setRole(ENUM_Role enumRole);

	bool hasCam();
	void setHasCam(bool bHasCam);

	bool applySpeak() const;
	void setApplySpeak(bool isOk);

	TerminalType terminalType();
	void setTerminalType(TerminalType enumTType);

private:
	__int64 m_nUserId;
	std::string m_strUserName;
	bool m_bAudio;
	bool m_bVideo;
	ENUM_Role m_enumRole;
	bool m_bHasCam;
	bool m_bIsApplySpeak;
	TerminalType m_enTerminalType;
};

#endif // USERINFO_H__