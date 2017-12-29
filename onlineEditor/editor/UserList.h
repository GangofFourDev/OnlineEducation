#ifndef USERLIST_H__
#define USERLIST_H__

#include <string>
#include <vector>

#include "UserInfo.h"
class Observer;
class UserList
{
public:
	UserList();
	~UserList();

	void addUserInfo(UserInfo* pUserInfo);
	void addUserInfo(__int64 nUserId, std::string strUserName, bool bAudio, bool bVideo, ENUM_Role enumRole, bool bHasCam, TerminalType terType);
	void removeUserInfo(__int64 nUserId);
	UserInfo* userInfoById(__int64 nUserId);
	void updateUserInfoById(__int64 nUserId, std::string strUserName, bool bAudio, bool bVideo, ENUM_Role enumRole, bool bHasCam);
	void updateUserInfo(UserInfo* pUserInfo);
	void clear();

	int count();
	UserInfo* userInfoByIndex(int nIndex);

	UserInfo* currentUserInfo();
	void setCurrentUserInfo(UserInfo* pUserInfo);

public:
	void attach(Observer* pObserver);
	void detach(Observer* pObserver);

private:
	void update(UserInfo* pUserInfo);
	void add(UserInfo* pUserInfo);
	void remove(__int64 nUserId);

private:
	std::vector<UserInfo*> m_oUserInfoList;
	std::vector<Observer*> m_oObserver;
	UserInfo* m_pCurrentUserInfo;
};

#endif // USERLIST_H__